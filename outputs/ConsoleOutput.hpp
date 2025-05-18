#pragma once

#include "IOutput.hpp"
#include <iostream>

/**
 * @brief Вывод данных в консоль.
 */
class ConsoleOutput : public IOutput {
public:
  void write(const std::string &data) override {
    std::cout << data << std::endl;
  }
};
