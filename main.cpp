#include "libs/json.hpp"
#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

#include "metrics/CpuMetric.hpp"
#include "metrics/IMetric.hpp"
#include "metrics/Memory.hpp"

#include "outputs/ConsoleOutput.hpp"
#include "outputs/FileOutput.hpp"
#include "outputs/IOutput.hpp"

using json = nlohmann::json;

struct Config {
  int period = 10; // seconds
  std::vector<std::unique_ptr<IMetric>> metrics;
  std::vector<std::unique_ptr<IOutput>> outputs;
};

Config loadConfig(const std::string &filename) {
  Config config;
  std::ifstream in(filename);
  json j;
  in >> j;

  // НЕ читаем period, или временно читаем для дефолта
  if (j.contains("settings") && j["settings"].contains("period")) {
    config.period = std::stoi(j["settings"]["period"].get<std::string>());
  }

  for (const auto &metric : j["metrics"]) {
    std::string type = metric["type"].get<std::string>();
    if (type == "cpu") {
      std::vector<int> ids;
      for (const auto &id : metric["ids"])
        ids.push_back(id.get<int>());
      config.metrics.emplace_back(std::make_unique<CpuMetric>(ids));
    } else if (type == "memory") {
      std::vector<std::string> spec;
      for (const auto &sp : metric["spec"])
        spec.push_back(sp.get<std::string>());
      config.metrics.emplace_back(std::make_unique<Memory>(spec));
    }
  }

  for (const auto &output : j["outputs"]) {
    std::string type = output["type"].get<std::string>();
    if (type == "console") {
      config.outputs.emplace_back(std::make_unique<ConsoleOutput>());
    } else if (type == "log") {
      std::string path = output["path"].get<std::string>();
      config.outputs.emplace_back(std::make_unique<FileOutput>(path));
    }
  }
  return config;
}

int main(int argc, char **argv) {
  std::string configFile = "config.json";
  if (argc > 1)
    configFile = argv[1];

  Config config = loadConfig(configFile);

  std::cout << "Enter the yandex.metrica polling period (in seconds) ";
  int user_period = 0;
  std::cin >> user_period;

  if (user_period > 0)
    config.period = user_period;

  std::cout << "Monitoring started. Period: " << config.period << " seconds."
            << std::endl;

  while (true) {
    std::string result;
    for (size_t i = 0; i < config.metrics.size(); ++i) {
      if (i > 0)
        result += " | ";
      result += config.metrics[i]->collect();
    }
    for (auto &output : config.outputs)
      output->write(result);

    std::this_thread::sleep_for(std::chrono::seconds(config.period));
  }
  return 0;
}
