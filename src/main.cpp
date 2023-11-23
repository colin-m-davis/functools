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

int h(int x) { return x + 100; }
int f(int x) { return 2 * x; }

int main() {
    const auto g = pipeline(f, h);
    std::cout << pipeline(f, h)(1) << ' ' << pipeline(h, f)(1) << '\n';
    // std::cout << g(3) << '\n';
    // std::cout << none_of(false, 0) << '\n';
    // print(f(1, 2, 3));
}
