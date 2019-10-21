# Style Guidelines

This library uses a style based on the scheme used by libc++.

## Rules

### Syntax

- Always import `c*`compatibility headers instead of `*.h` when using C headers
- Always use west const. (`const T&`)
- Do not import `namespace std`.
- Prefer to use `using` declarations for type aliases wherever possible.

### API Naming

In this section, `private` visibility refers to both the `class` visibility and the API visibility, i.e. functions which
are not intended for external use.

1. All typedefs are in `snake_case`, regardless of visibility.
1. All `public` and `protected` visibility members are in `snake_case`, with the following exceptions:
    - Template type and non-type parameters are in `PascalCase`.
1. All `private` visibility classes and members are in leading-underscore `_snake_case`, with the following exceptions:
    - `static constexpr` variables are in `PascalCase`.
    - `private` API enumerations and enumerators which are not directly under `namespace derplib` are in `PascalCase`.
    - Template type parameters are in `PascalCase`.
    - Non-`const` member variables are in leading and trailing underscore `_snake_case_`.
1. All local variables are in `snake_case`, including argument names.

Nested classes will inherit the visibility rules of the previous-level class, unless:

- The nested class has `private` API visibility but `public` class visibility due to member sharing.
- The nested class has `public` API visibility but `private` class visibility due to proxy implementation.

### Argument Naming

- All argument name must be in `snake_case`, with no leading or trailing underscores.

## Sample

```cpp
class public_api_class {
 public:
  using public_typedef = int;
  
  static constexpr int public_constexpr_member = 0;

  enum public_enum {
    enumuator
  };

  void public_function(void* params);
  const int public_const_member;
  int public_member;

  template<typename TParams, std::size_t S>
  void public_template();

 private:
  using private_typedef = int;

  static constexpr int PrivateConstexprMember = 0;

  enum PrivateEnum {
    Enumerator
  };

  void _private_function(void* params);
  const int _private_const_member;
  int _private_member_;

  template<typename TParams, std::size_t _s>
  void _private_template();
};

class _private_api_class {
 public:
  using public_typedef = int;
  
  static constexpr int _public_constexpr_member = 0;

  enum _public_enum {
    _enumerator
  };

  void _public_function(void* params);
  const int _public_const_member;
  int _public_member_;

  template<typename TParams, std::size_t S>
  void _public_template();

 private:
  using private_typedef = int;

  static constexpr int PrivateConstexprMember = 0;

  enum PrivateEnum {
    Enumerator
  };

  void _private_api_function(void* params);
  const int _private_api_const_member;
  int _private_api_member_;

  template<typename TParams, std::size_t _s>
  void _private_api_template();
};

enum _top_level_private_api_enum {
  _enumerator
};
```
