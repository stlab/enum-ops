#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <stlab/enum_ops.hpp> /// [EXAMPLE_START]

#include <iostream>

enum foo { foo_4 = 1 << 2, foo_8 = 1 << 3 };
enum class views : int { none = 0, text = 1 << 0, icon = 1 << 1, preview = 1 << 2 };

auto stlab_enable_bitmask_enum(foo) -> std::true_type;
auto stlab_enable_bitmask_enum(views) -> std::true_type; /// [OMIT_START]
void run_example() {
    /// [OMIT_END] \
int main() {
    // Bitset operations on plain enums
    foo a{foo_4};
    foo b{foo_8};
    foo c{a | b};
    std::cout << a << " " << b << " " << c << "\n";

    // Bitset operations on enum class
    views x = views::text | views::icon;
    bool has_text = (x & views::text) == views::text;
    bool has_preview = (x & views::preview) == views::preview;
    std::cout << has_text << " " << has_preview << "\n";
} /// [EXAMPLE_END]

TEST_CASE("Example: common use cases") { CHECK_NOTHROW(run_example()); }
