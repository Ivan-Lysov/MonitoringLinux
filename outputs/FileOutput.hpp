#pragma once

#include "IOutput.hpp"
#include <fstream>
#include <string>

/**
 * @brief Вывод данных в файл.
 */
class FileOutput : public IOutput {
public:
  explicit FileOutput(const std::string &filePath);
  ~FileOutput();

  void write(const std::string &data) override;

private:
  std::ofstream out_;
};
