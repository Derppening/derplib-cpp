//
// Created by david on 9/7/2017.
//

#ifndef DERPLIB_CONFIG_FILE_H_
#define DERPLIB_CONFIG_FILE_H_

#if __cplusplus > 201402L

#include <map>
#include <memory>
#include <string>

namespace derplib {

/**
 * Class to manage application configurations.
 *
 * Configuration can be used within the application, as well as export/import to a savefile.
 */
class config_file {
  /**
   * Type definition for the container storing key-value pairs of one section.
   */
  using section_type = std::map<std::string, std::string>;
  /**
   * Type definition for the container storing the whole configuration map.
   */
  using container_type = std::map<std::string, section_type>;

 public:
  config_file() = default;

  /**
   * Move constructor.
   *
   * \\param c Existing ConfigFile class
   */
  config_file(config_file&& c) noexcept = default;
  /**
   * Copy constructor.
   *
   * \\param c Existing ConfigFile class
   */
  config_file(const config_file& c) = default;

  ~config_file() = default;

  /**
   * Move assignment operator.
   *
   * \\return Newly created object from existing \\c ConfigFile.
   */
  config_file& operator=(config_file&&) noexcept = default;
  /**
   * Copy assignment operator.
   *
   * \\return Newly created object from existing \\c ConfigFile.
   */
  config_file& operator=(const config_file&) = default;

  std::optional<std::reference_wrapper<std::string>> field(const std::string& section_name, const std::string& field_name) noexcept;
  const std::string& field(const std::string& section, const std::string& field) const;

  section_type& section(const std::string& section_name);
  const section_type& section(const std::string& section_name) const;

  bool contains(const std::string& section_name) const;
  bool contains(const std::string& section_name, const std::string& field_name) const;

  container_type& get();
  const container_type& get() const;

  /**
   * \\brief Adds a field and its data to this configuration.
   *
   * \\param section Section of the field to add into.
   * \\param field Field name to add.
   * \\param data Data to attach to the field.
   */
  void put(const std::string& section, const std::string& field, const std::string& data);
  /**
   * \\brief Erases a field from this configuration.
   *
   * \\param section Section of the field to erase.
   * \\param field Field to erase.
   */
  void erase(const std::string& section, const std::string& field);

  /**
   * \\brief Serializes and exports this configuration into a file.
   *
   * \\param filename Filename to export to.
   * \\param skip_empty Whether to skip empty sections.
   */
  void to_file(const std::string& filename, bool skip_empty = true) const;

  /**
   * \\brief Parses configuration from a file.
   *
   * \\param filename Filename of the configuration file.
   * \\param dispose_empty Whether to dispose sections without a section name.
   * \\return A `config_file` representing the configuration as read from the file.
   */
  static config_file from_file(const std::string& filename, bool dispose_empty = true);

  /**
   * Remove all fields and sections from the current configuration.
   */
  void clear();
  /**
   * Removes all sections which do not have any fields associated with.
   */
  void garbage_collect();

 private:
  container_type _config;
};

}  // namespace derplib

#else
#error "This library requires C++17-supported compiler"
#endif  // __cplusplus > 201402L

#endif  // DERPLIB_CONFIG_FILE_H_
