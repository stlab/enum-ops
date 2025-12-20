# stlab::enum-ops

[![CI][ci-badge]][ci-link]
[![Documentation][docs-badge]][docs-link]
[![License][license-badge]][license-link]

Type-safe bitmask and arithmetic operators for enum types.

[ci-badge]: https://github.com/stlab/stlab-enum-ops/actions/workflows/ci.yml/badge.svg
[ci-link]: https://github.com/stlab/stlab-enum-ops/actions/workflows/ci.yml
[docs-badge]: https://img.shields.io/badge/docs-github%20pages-blue
[docs-link]: https://stlab.github.io/stlab-enum-ops/
[license-badge]: https://img.shields.io/badge/license-BSL%201.0-blue.svg
[license-link]: https://github.com/stlab/stlab-enum-ops/blob/main/LICENSE

## Overview

`stlab::enum-ops` provides opt-in operator overloads for enum types, enabling:

- Bitmask operators: `~ | & ^ |= &= ^=`
- Shift operators: `<< >> <<= >>=`
- Arithmetic operators: `+ - * / % += -= *= /=` and increment/decrement
- Comparisons with `0` and logical `!` for convenience

Opt-in is done by declaring one or both of:

```cpp
auto stlab_enable_bitmask_enum(E) -> std::true_type;
auto stlab_enable_arithmetic_enum(E) -> std::true_type;
```

## Building & Testing

```bash
cmake --preset=test
cmake --build --preset=test
ctest --preset=test
```

## Examples

Examples live under `examples/` and are built with the test preset:

```bash
cmake --build --preset=test --target enum_ops_example
./build/test/examples/enum_ops_example
```

## Using in your project (CPM)

The simplest way to use `stlab::enum-ops` in your project is to add it as a dependency via [CPM](https://github.com/cpm-cmake/CPM.cmake).

```cmake
CPMAddPackage("gh:stlab/stlab-enum-ops@1.0.0")
```

## Documentation

Generate Doxygen docs:

```bash
cmake --preset=docs
cmake --build --preset=docs --target docs
```

## License

Distributed under the Boost Software License, Version 1.0. See `LICENSE`.
