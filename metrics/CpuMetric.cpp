#include "CpuMetric.hpp"
#include <chrono>
#include <fstream>
#include <sstream>
#include <thread>
#include <unistd.h> // для sysconf

namespace {
/**
 * @brief Получить usage для всех ядер
 * @return Вектор пар (user+nice+system, total) для каждого ядра
 */
std::vector<std::pair<unsigned long long, unsigned long long>> getCpuStats() {
  std::ifstream stat("/proc/stat");
  std::vector<std::pair<unsigned long long, unsigned long long>> result;
  std::string line;
  while (std::getline(stat, line)) {
    if (line.rfind("cpu", 0) == 0 && line.size() > 3 && isdigit(line[3])) {
      std::istringstream iss(line);
      std::string label;
      unsigned long long user, nice, system, idle, iowait, irq, softirq, steal,
          guest, guest_nice;
      iss >> label >> user >> nice >> system >> idle >> iowait >> irq >>
          softirq >> steal >> guest >> guest_nice;
      unsigned long long active = user + nice + system + irq + softirq + steal;
      unsigned long long total = active + idle + iowait;
      result.emplace_back(active, total);
    }
  }
  return result;
}
} // namespace

CpuMetric::CpuMetric(const std::vector<int> &coreIds) : coreIds_(coreIds) {
  // nothing
}

std::string CpuMetric::collect() {
  static std::vector<std::pair<unsigned long long, unsigned long long>>
      prevStats = getCpuStats();
  std::this_thread::sleep_for(std::chrono::milliseconds(
      100)); 
  auto curStats = getCpuStats();

  std::ostringstream oss;
  oss << "cpu: [";
  for (size_t i = 0; i < coreIds_.size(); ++i) {
    int id = coreIds_[i];
    if (id >= 0 && id < (int)curStats.size() && id < (int)prevStats.size()) {
      auto [prevActive, prevTotal] = prevStats[id];
      auto [curActive, curTotal] = curStats[id];
      double usage =
          (curTotal == prevTotal)
              ? 0.0
              : 100.0 * (curActive - prevActive) / (curTotal - prevTotal);
      if (i > 0)
        oss << ", ";
      oss << usage;
    }
  }
  oss << "]";
  prevStats = curStats;
  return oss.str();
}
