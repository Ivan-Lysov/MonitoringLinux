#pragma once

#include <string>

/**
 * @brief
 */
class IMetric {
public:
  virtual ~IMetric() = default;

  /**
   * @brief Собрать данные метрики в строковом виде (для лога/консоли).
   * @return std::string – строковое представление метрики.
   */
  virtual std::string collect() = 0;

  /**
   * @brief Получить имя метрики (например, "cpu" или "memory").
   */
  virtual std::string name() const = 0;
};
