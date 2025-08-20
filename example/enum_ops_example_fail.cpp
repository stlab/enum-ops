#include <stlab/enum_ops.hpp>

enum foo { foo_4 = 1 << 2, foo_8 = 1 << 3 };
enum bar { bar_4 = 1 << 2, bar_8 = 1 << 3 };
enum baz { baz_4 = 1 << 2, baz_8 = 1 << 3 };

auto stlab_enable_arithmetic_enum(baz) -> std::true_type;

int main() {
    foo a{foo_4};
    bar b{bar_8};
    foo c{a + b}; // Error! a and b are different enum types, so result is integral

    baz d{baz_4};
    baz e{baz_8};
    baz f{d + e}; // Ok, arithmetic ops enabled for baz, so d + e has type baz
}
