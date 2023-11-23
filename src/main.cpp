#include <functools.cpp>
#include <iostream>
#include <string>

#include <tuple>
#include <vector>
#include <stdexcept>

void printvec(auto&& vec) {
    for (const auto x : vec) {
        std::cout << x << "" << '\n';
    }
}

int a(int x) { return x > 2 && x < 5; }

vec<int> f(const int x) {
    return {x, x+1, x+2};
}

int main() {
    const auto g = range_filter(1, 4);
    vec<int> xs{2, 1, 2, 3, 4, 5};
    const auto [a, b, c] = deconstruct<3>(xs);;
    printvec(vec<int>{a, b, c});
    // std::cout << none_of(false, 0) << '\n';
    // print(f(1, 2, 3));
}
