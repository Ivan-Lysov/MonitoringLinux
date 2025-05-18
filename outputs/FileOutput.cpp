#include "FileOutput.hpp"

FileOutput::FileOutput(const std::string &filePath)
    : out_(filePath, std::ios::app) {}

FileOutput::~FileOutput() {
  if (out_.is_open()) {
    out_.close();
  }
}

void FileOutput::write(const std::string &data) {
  if (out_.is_open()) {
    out_ << data << std::endl;
  }
}
