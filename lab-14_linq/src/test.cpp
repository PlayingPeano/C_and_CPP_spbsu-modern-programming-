#include "linq.hpp"
#include <cassert>
#include <vector>

using linq::from;

void from_select() {
    const std::size_t xs[] = { 1, 2, 3, 4, 5 };
    std::vector<int> res = from(xs, xs + 3).select<int>([](int x) { return x - 5; }).to_vector();
    std::vector<int> expected = { -4, -3, -2 };
    assert(res == expected);
}

int main(int argc, char* argv[])
{
    from_select();
    return EXIT_SUCCESS;
}