#include <functools.cpp>
#include <iostream>
#include <string>

using namespace functools;

int f(int a) { return 2 * a; };

int main() {
    std::vector<int> xs{8, 4, 9, 1};
    print(map(pipeline(f, f, f, f), xs));
    std::cout << foldl(f, xs) << '\n';
    std::cout << foldl(pipeline(f, f), xs, 1) << '\n';
    std::cout << foldr(f, xs, 1) << '\n';
}
