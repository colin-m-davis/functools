#include <functools.cpp>
#include <iostream>
#include <string>

#include <tuple>
#include <vector>
#include <stdexcept>

using namespace functools;

int f(int x) { return x * 2; }

int main() {
    vec<int> xs{8, 4, 9, 1};
    print(map(f, xs));
}
