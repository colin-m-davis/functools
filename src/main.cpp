#include <functools.cpp>
#include <iostream>
#include <string>

void printvec(auto&& vec) {
    for (const auto x : vec) {
        std::cout << x << "" << '\n';
    }
}

int a(int x) { return x * x + 2; }

vec<int> f(const int x) {
    return {x, x+1, x+2};
}

int main() {
    vec<int> xs{1, 2, 3, 4};
    printvec(flat_map(f, xs));
    // std::cout << none_of(false, 0) << '\n';
    // print(f(1, 2, 3));
}
