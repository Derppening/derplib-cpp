#if __cplusplus > 201402L

#include "derplib/experimental/config_file.h"

#include <algorithm>
#include <fstream>
#include <map>
#include <memory>
#include <stdexcept>

namespace derplib {

std::optional<std::reference_wrapper<std::string>> config_file::field(const std::string& section_name,
                                                                      const std::string& field_name) noexcept {
  if (this->_config.find(section_name) != this->_config.end()) {
    auto& s = this->section(section_name);
    if (s.find(field_name) != s.end()) {
      return std::optional(std::ref(s.at(field_name)));
    }
  }

  return std::optional<std::reference_wrapper<std::string>>();
}

auto config_file::field(const std::string& section_name, const std::string& field_name) const -> const std::string& {
  return section(section_name).at(field_name);
}

void config_file::put(const std::string& section, const std::string& field, const std::string& data) {
  // find the section
  auto config_it = this->_config.find(section);
  if (config_it == this->_config.end()) {
    auto res = this->_config.emplace(section, std::map<std::string, std::string>());
    config_it = res.first;
  }

  // find the field
  auto& section_ref = config_it->second;
  auto field_it = section_ref.find(field);

  // create the field if it's not found
  if (field_it == section_ref.end()) {
    section_ref.emplace(field, data);
  } else {
    field_it->second = data;
  }
}

void config_file::erase(const std::string& section, const std::string& field) {
  // find the section
  auto config_it = this->_config.find(section);
  if (config_it == this->_config.end()) {
    return;
  }

  // find the field
  section_type& section_ref = config_it->second;
  auto field_it = section_ref.find(field);

  // erase the field if it's found
  if (field_it != section_ref.end()) {
    section_ref.erase(field_it);
  }
}

config_file::section_type& config_file::section(const std::string& section_name) {
  return this->_config.at(section_name);
}

const config_file::section_type& config_file::section(const std::string& section_name) const {
  return this->_config.at(section_name);
}

bool config_file::contains(const std::string& section_name) const {
  return this->_config.find(section_name) != this->_config.end();
}

bool config_file::contains(const std::string& section_name, const std::string& field_name) const {
  if (!contains(section_name)) {
    return false;
  }

  const auto& s = this->section(section_name);
  return s.find(field_name) != s.end();
}

config_file::container_type& config_file::get() {
  return this->_config;
}

const config_file::container_type& config_file::get() const {
  return this->_config;
}

void config_file::to_file(const std::string& filename, const bool skip_empty) const {
  // open file for write
  auto file_ostr = std::ofstream();
  file_ostr.exceptions(std::ofstream::badbit | std::ofstream::failbit);
  file_ostr.open(filename);

  // loop through the section headers
  for (auto&& section : this->_config) {
    // skip empty headers
    if (skip_empty && section.second.empty()) {
      continue;
    }

    // dump section header
    if (section.first.length() != 0) {
      file_ostr << "[" << section.first << "]" << '\n';
    }

    // dump config contents to file
    for (const auto& field : section.second) {
      file_ostr << field.first << "=" << field.second << '\n';
    }
    file_ostr << std::endl;
  }
}

config_file config_file::from_file(const std::string& filename, bool dispose_empty) {
  // open the config file
  auto file_istr = std::ifstream();
  file_istr.exceptions(std::ofstream::badbit | std::ofstream::failbit);
  file_istr.open(filename);
  file_istr.exceptions(std::ofstream::badbit);

  config_file cf;

  // temporary variables
  std::string line;
  std::string section;

  // loop through all lines
  while (std::getline(file_istr, line)) {
    // get the position of '='
    auto equal_pos = line.find('=');

    if (line.length() == 0) {
      // empty line
      continue;
    }
    if (line.front() == '[' && line.back() == ']') {
      // section
      section = line.substr(1, line.length() - 2);
      continue;
    }
    if (equal_pos == std::string::npos) {
      // line does not have format. not parsing
      continue;
    }

    if (section.empty() && !dispose_empty) {
      // force parsing lines without a section header
      cf.put("", line.substr(0, equal_pos), line.substr(equal_pos + 1));
    } else if (!section.empty()) {
      // parse lines with a section header
      cf.put(section, line.substr(0, equal_pos), line.substr(equal_pos + 1));
    }
  }

  return cf;
}

void config_file::garbage_collect() {
  for (auto it = this->_config.begin(); it != this->_config.end();) {
    if (it->second.empty()) {
      it = this->_config.erase(it);
    } else {
      ++it;
    }
  }
}

void config_file::clear() {
  this->_config.clear();
}

}  // namespace derplib

#endif  // __cplusplus > 201402L
