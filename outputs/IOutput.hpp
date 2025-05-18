#pragma once

#include <string>

/**
 * @brief Базовый интерфейс для вывода данных.
 */
class IOutput {
public:
  virtual ~IOutput() = default;

  /**
   * @brief
   * @param data
   */
  virtual void write(const std::string &data) = 0;
};
