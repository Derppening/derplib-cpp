#pragma once

#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <derplib/stdext/type_traits.h>

namespace derplib {
namespace experimental {

/**
 * \brief Abstraction for interactive command-line interface.
 *
 * \tparam CharT character type for input
 * \tparam Func function type for invoking callbacks. Must be `void(const std::basic_string<CharT>&)`.
 */
template<typename CharT, typename Func = void (*)(const std::basic_string<CharT>&), typename = stdext::enable_if_invocable<
    Func>>
class basic_cli {
 public:
  /**
   * \brief `std::basic_string<CharT>`
   */
  using string_type = std::basic_string<CharT>;
  /**
   * \brief `Func`
   */
  using function_type = Func;

  /**
   * \brief Level of hints to provide to the user.
   */
  enum struct hint_level {
    /**
     * \brief Do not display any available options to the user.
     */
        none,
    /**
     * \brief Only display the primary input alias to the user.
     */
        simple,
    /**
     * \brief Display all input aliases to the user.
     */
        full
  };

  /**
   * \brief Result from parsing the command-line arguments.
   */
  enum struct parse_result {
    /**
     * \brief Command not found.
     */
        cmd_not_found = -1,
    /**
     * \brief No operation, i.e. the control is still managed by this class.
     */
        no_op,
    /**
     * \brief Exit, i.e. the control continues by the rest of the application.
     */
        exit
  };

  /**
   * \brief Representation of an entry.
   */
  struct entry {
    /**
     * \brief Description of the entry.
     */
    string_type text;
    /**
     * \brief List of all inputs that triggers this option.
     */
    std::vector<string_type> alias;
    /**
     * \brief Callback of this option.
     */
    Func fptr = nullptr;
    /**
     * \brief Whether the interactive prompt should terminate after processing the callback.
     */
    bool is_terminate = false;
  };

  /**
   * \brief Default constructor.
   *
   * Defaults to use hint level `hint_level::simple`.
   */
  basic_cli() = default;

  /**
   * \brief Constructs an instance with overridden hint level.
   *
   * \param level overridden hint level
   */
  explicit basic_cli(hint_level level) : _hint_level(level) {}

  virtual ~basic_cli() = default;

  /**
   * \brief Inserts an empty line.
   * \return `*this`.
   */
  basic_cli& div_line() {
    return header("");
  }

  /**
   * \brief Adds a header.
   *
   * \param header header text
   * \return `*this`.
   */
  basic_cli& header(const string_type& header) {
    return item(header, std::vector<string_type>());
  }

  /**
   * \brief Adds an option item.
   *
   * \param e entry to add
   * \return `*this`.
   */
  basic_cli& item(const entry& e) {
    _options.emplace_back(e);

    return *this;
  }

  /**
   * \brief Adds an option item.
   *
   * \param text description of the item
   * \param input input which triggers this option
   * \param fptr callback to execute when this option is triggered
   * \param is_term if true, leaves the interactive prompt after processing the callback
   * \return `*this`
   */
  basic_cli& item(const string_type& text,
                  const string_type& input,
                  function_type fptr = nullptr,
                  bool is_term = false) {
    return item(text, std::vector<string_type>{input}, std::move(fptr), is_term);
  }

  /**
   * \brief Adds an option item.
   *
   * \param text text description of the item
   * \param input inputs when trigger this option. If multiple are provided and hint level is set to
   * `hint_level::simple`, only the first alias will be shown
   * \param fptr callback to execute when this option is triggered
   * \param is_term if true, leaves the interactive prompt after prcessing the callback
   * \return `*this`.
   */
  basic_cli& item(const string_type& text,
                  const std::vector<string_type>& input,
                  function_type fptr = nullptr,
                  bool is_term = false) {
    entry e{text, input, std::move(fptr), is_term};
    _options.emplace_back(std::move(e));

    return *this;
  }

  virtual bool inflate() = 0;

  parse_result parse(const string_type& input) {
    // only read the first argument; put everything else in another string
    string_type cmd;
    string_type args;
    if (input.find(' ') != string_type::npos) {
      args = input.substr(input.find(' ') + 1);
      cmd = input.substr(0, input.find(' '));
    } else {
      cmd = input;
    }

    // find whether the argument corresponds to any alias
    auto search_res = std::find_if(_options.begin(), _options.end(), [&](const entry& e) -> bool {
      return std::any_of(e.alias.begin(), e.alias.end(), [&](const string_type& alias) -> bool {
        return cmd == alias;
      });
    });

    if (search_res == _options.end()) {
      return parse_result::cmd_not_found;
    }

    if (search_res->fptr) {
      search_res->fptr(args);
    }

    return search_res->is_terminate ? parse_result::exit : parse_result::no_op;
  }

 protected:
  static void screen_clear() {
#if defined(_WIN32)
    std::system("cls");
#elif defined(__unix__) || defined(__APPLE__) || defined(__linux__)
    std::system("clear");
#else
    for (int i = 0; i < 120; ++i) { std::cout << "\n\r"; }
  std::cout.flush();
#endif
  }

  hint_level hint() const { return _hint_level; }

  void output_options(std::basic_ostream<CharT>& cos) {
    for (auto it = _options.begin(); it != _options.end(); ++it) {
      if (it->text.empty()) {
        cos << '\n';
      } else if (it->alias.empty() && it == _options.begin()) {
        cos << it->text << '\n';
      } else if (it->alias.empty()) {
        cos << '\n' << it->text << '\n';
      } else {
        if (_hint_level == hint_level::simple) {
          cos << it->alias.front() << ": " << it->text << '\n';
        } else {
          std::ostringstream hints_oss;
          for (const auto& str : it->alias) {
            hints_oss << str << ", ";
          }
          string_type hints = hints_oss.str();
          hints = hints.substr(0, hints.length() - 2);

          cos << hints << ": " << it->text << '\n';
        }
      }
    }

    cos << std::endl;
  }

 private:
  hint_level _hint_level = hint_level::simple;
  std::vector<entry> _options;
};

class cli : public basic_cli<char> {
 public:
  ~cli() override;

  bool inflate() override { return inflate(false, false); }

  bool inflate(bool has_custom_text, bool terminate_anyways) {
    std::istream& cis = std::cin;
    std::ostream& cos = std::cout;

    while (true) {
      // clear screen if caller doesn't have text to display
      if (!has_custom_text) {
        basic_cli::screen_clear();
      }

      // show all available options
      if (hint() != hint_level::none) {
        basic_cli::output_options(std::cout);
      }

      // display interactive command line
      cos << "> ";
      string_type input;
      std::getline(cis, input);

      // delegate parse operation
      switch (parse(input)) {
        case parse_result::exit:
          return true;
        case parse_result::cmd_not_found:
          cos << input << ": Unknown Option\n";
          break;
        case parse_result::no_op:
          // not handled
          break;
      }

      cos << '\n';
      cos << "Press [ENTER] to continue..." << std::endl;
      std::getline(cis, input);

      // delegate back to the user if the interactive loop should be handled externally
      if (terminate_anyways) {
        return false;
      }
    }
  }
};

}  // namespace experimental
}  // namespace derplib
