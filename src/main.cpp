#include <functools.cpp>
#include <iostream>
#include <string>

#include <tuple>
#include <vector>
#include <stdexcept>


template <typename T>
void f(T) {
    const auto z = std::convertible_to<T, bool>;
    if constexpr(std::convertible_to<T, bool>) {
        std::cout << "T is convertible to a bool\n";
    } else {
        std::cout << "T is not convertible to a bool\n";
    }
}

int main() {
    vec<int> xs{8, 4, 9, 1};
    for (const auto& z : zip_with_indices(sorted(xs))) {
        print(z);
    }
}
