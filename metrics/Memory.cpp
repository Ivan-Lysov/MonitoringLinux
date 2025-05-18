#include "Memory.hpp"
#include <fstream>
#include <sstream>

Memory::Memory(const std::vector<std::string> &spec) : spec_(spec) {
  // nothing
}

std::string Memory::collect() {
  unsigned long memTotal = 0, memFree = 0, buffers = 0, cached = 0;
  std::ifstream meminfo("/proc/meminfo");
  std::string line;
  while (std::getline(meminfo, line)) {
    std::istringstream iss(line);
    std::string key;
    unsigned long value;
    std::string unit;
    iss >> key >> value >> unit;
    if (key == "MemTotal:")
      memTotal = value;
    else if (key == "MemFree:")
      memFree = value;
    else if (key == "Buffers:")
      buffers = value;
    else if (key == "Cached:")
      cached = value;
  }
  unsigned long used = memTotal - memFree - buffers - cached;

  std::ostringstream oss;
  oss << "memory: {";
  bool first = true;
  for (const auto &s : spec_) {
    if (!first)
      oss << ", ";
    if (s == "used")
      oss << "\"used\": " << used;
    else if (s == "free")
      oss << "\"free\": " << memFree;
    first = false;
  }
  oss << "}";
  return oss.str();
}
