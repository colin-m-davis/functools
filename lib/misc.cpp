#include <vector>
#include <iterator>
#include <tuple>

namespace functools {

/*
helper function for deconstruct
need separate impl function to take in index sequence as parameter pack
*/
template <typename T, std::size_t N, std::size_t... Is>
[[nodiscard]] auto deconstruct_impl(const std::vector<T>& vec, std::index_sequence<Is...>) {
    return std::make_tuple(vec[Is]..., vec[N - 1]);
}

/*
places the first N elements of a vector in an N-tuple
useful in combination with structured binding
*/
template <std::size_t N, typename T>
[[nodiscard]] auto deconstruct(const std::vector<T>& vec) {
    if (vec.size() < N) {
        throw std::invalid_argument("Vector size is less than N");
    }
    return deconstruct_impl<T, N>(vec, std::make_index_sequence<N - 1>{});
}

template <typename T>
concept BooleanConvertible = std::convertible_to<T, bool>;

/*
returns true if and only if all of the arguments are true
*/
template <BooleanConvertible... Args>
[[nodiscard]] bool all_of(Args... args) {
    return (args && ...);
}

/*
returns true if and only if at least one of the arguments is true
*/
template <BooleanConvertible... Args>
[[nodiscard]] bool one_of(Args... args) {
    return (args || ...);
}

/*
returns true if and only if all of the arguments are false
*/
template <BooleanConvertible... Args>
bool none_of(Args... args) {
    return (!args && ...);
}

/*
https://en.wikipedia.org/wiki/Total_order
*/
template <typename T>
concept Ordered = requires(T a, T b) {
    {a < b} -> std::convertible_to<bool>;
    {a == b} -> std::convertible_to<bool>;
};

/*
returns a function that returns whether an elemet lies in the half-open interval [a, b)
*/
template <Ordered T>
[[nodiscard]] auto range_filter(T&& a, T&& b) {
    return
        [a, b](T c) { 
            return (a == c || a < c) && c < b;
        };
}

template <typename T>
[[nodiscard]] std::pair<T, T> divmod(T x, T d) {
    return {x / d, x % d};
}

/*
Iterables can be used in a range-based for loop
*/
template <typename T>
concept Iterable = requires(T x) {
    { std::begin(x) } -> std::forward_iterator;
    { std::end(x) } -> std::forward_iterator;
};

void print(const Iterable auto& iterable) {
    std::cout << "[";
    std::cout << *std::begin(iterable);
    for (auto it = std::next(std::begin(iterable)); it != std::end(iterable); ++it) {
        std::cout << ", " << *it;
    }
    std::cout << "]\n";
}

/*
helper function for pretty-printing tuples
source: https://stackoverflow.com/questions/6245735/pretty-print-stdtuple
*/
template <typename TupType, size_t... I>
void print(const TupType& tup, std::index_sequence<I...>) {
    std::cout << "(";
    (..., (std::cout << (I == 0? "" : ", ") << std::get<I>(tup)));
    std::cout << ")\n";
}

/*
pretty-prints a tuple to cout
source: https://stackoverflow.com/questions/6245735/pretty-print-stdtuple
*/
template <typename... T>
void print(const std::tuple<T...>& tup) {
    print(tup, std::make_index_sequence<sizeof...(T)>());
}

};  // namespace functools
