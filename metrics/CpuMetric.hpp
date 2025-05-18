#pragma once

#include "IMetric.hpp"
#include <vector>

/**
 * @brief Метрика для загрузки процессора по ядрам.
 */
class CpuMetric : public IMetric {
public:
  explicit CpuMetric(const std::vector<int> &coreIds);

  std::string collect() override;
  std::string name() const override { return "cpu"; }

private:
  std::vector<int> coreIds_;
};
