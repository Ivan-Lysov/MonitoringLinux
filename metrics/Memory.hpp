#pragma once

#include "IMetric.hpp"
#include <string>
#include <vector>

/**
 * @brief Метрика для оперативной памяти.
 */
class Memory : public IMetric {
public:
  explicit Memory(const std::vector<std::string> &spec);

  std::string collect() override;
  std::string name() const override { return "memory"; }

private:
  std::vector<std::string> spec_;
};
