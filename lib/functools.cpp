#include <concepts>
#include <type_traits>
#include <functional>
#include <vector>
#include <tuple>
#include <algorithm>
#include <utility>
#include <iostream>

// namespace functools {

// source: https://stackoverflow.com/questions/6245735/pretty-print-stdtuple
template <typename TupType, size_t... I>
void print(const TupType& _tup, std::index_sequence<I...>) {
    std::cout << "(";
    (..., (std::cout << (I == 0? "" : ", ") << std::get<I>(_tup)));
    std::cout << ")\n";
}

// source: https://stackoverflow.com/questions/6245735/pretty-print-stdtuple
template <typename... T>
void print (const std::tuple<T...>& _tup) {
    print(_tup, std::make_index_sequence<sizeof...(T)>());
}

template <typename T>
using vec = std::vector<T>;

/*
creates a sequence of length N, starting from the init and repeatedly applying f on the last value in the sequence
*/
auto recursive_seq(auto&& init, auto&& f, int N) {
    std::vector<std::decay_t<decltype(init)>> result;
    result.reserve(N);
    result.push_back(std::forward<decltype(init)>(init));
    for (auto i = 1; i < N; ++i) {
        result.push_back(f(result.back()));
    }
    return result;
}

template <typename... Vecs>
constexpr auto min_size(const Vecs&... vecs) {
    return std::min({vecs.size()...});
}

template <typename... Vecs>
constexpr auto make_tuple_at_index(int index, const Vecs&... vecs) {
    return std::make_tuple(vecs[index]...);
}

template <typename... Vecs>
auto zip(Vecs&&... vecs) {
    std::vector<std::tuple<typename std::decay_t<Vecs>::value_type...>> result;
    const auto sz = min_size(vecs...);
    result.reserve(sz);
    for (auto i = 0; i < sz; ++i) {
        result.push_back(make_tuple_at_index(i, std::forward<Vecs>(vecs)...));
    }
    return result;
}

template <typename Vec>
auto zip_with_indices(Vec&& vec) {
    return zip(recursive_seq(0, [](int x) { return x + 1; }, vec.size()), vec);
}

template <typename F, typename Vec, typename Init>
auto foldl(F&& f, const Vec& vec, Init&& init) {
    auto accum = init;
    for (auto x = vec.cbegin(); x != vec.cend(); ++x) {
        accum = f(std::move(accum), *x);
    }
    return accum;
}

template <typename F, typename Vec, typename Init>
auto foldr(F&& f, const Vec& vec, Init&& init) {
    auto accum = init;
    for (auto x = vec.crbegin(); x != vec.crend(); ++x) {
        accum = f(std::move(accum), *x);
    }
    return accum;
}

template <typename F, typename Vec>
auto map(F&& f, const Vec& vec) {
    using Elem = std::invoke_result_t<F, typename Vec::value_type>;
    std::vector<Elem> result;
    const auto sz = vec.size();
    result.reserve(sz);
    for (const auto& x : vec) {
        result.push_back(f(x));
    }
    return result;
}

/*
f must take in an argument and return a vector
applies f to all elements of vec and merges the results into one vector
*/
auto flat_map(auto&& f, const auto& vec) {
    using Elem = typename std::decay_t<decltype(f(*std::begin(vec)))>::value_type;
    std::vector<Elem> result;
    for (const auto& x : vec) {
        for (const auto& y : f(x)) {
            result.push_back(y);
        }
    }
    return result;
}

template <typename T>
auto filter(auto&& f, const std::vector<T>& vec) {
    std::vector<T> result;
    for (const auto& x : vec) {
        if (f(x)) {
            result.push_back(x);
        }
    }
    return result;
}

template <typename T>
auto filter(auto&& f, std::vector<T>&& vec) {
    std::vector<T> result;
    for (auto&& x : vec) {
        if (f(x)) {
            if constexpr (std::is_nothrow_move_constructible_v<T>) {
                result.push_back(std::move(x));
            } else {
                result.push_back(x);
            }
        }
    }
    return result;
}

template <typename T, std::size_t N, std::size_t... Is>
auto deconstruct_impl(const std::vector<T>& vec, std::index_sequence<Is...>) {
    return std::make_tuple(vec[Is]..., vec[N - 1]);
}

/*
places the first N elements of a vector in an N-tuple
useful in combination with structured binding
*/
template <std::size_t N, typename T>
auto deconstruct(const std::vector<T>& vec) {
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
bool all_of(Args... args) {
    return (args && ...);
}

/*
returns true if and only if at least one of the arguments is true
*/
template <BooleanConvertible... Args>
bool one_of(Args... args) {
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
returns a function that applies the input functions from left to right on its inputs
*/
auto pipeline(auto&& first, auto&&... rest) {
    if constexpr (sizeof...(rest) == 0) {
        return first;
    } else {
        return [=](auto&&... args) {
            return pipeline(rest...)(first(std::forward<decltype(args)>(args)...));
        };
    }
}

auto sorted(const auto& xs) {
    auto new_xs = xs;
    sort(new_xs.begin(), new_xs.end());
    return new_xs;
}

template <typename T>
concept PartialEq = requires(T a, T b) {
    {a < b} -> std::convertible_to<bool>;
    {a == b} -> std::convertible_to<bool>;
};

/*
half-open interval
*/
template <PartialEq T>
auto range_filter(T&& a, T&& b) {
    return
        [a, b](T c) { 
            return (a == c || a < c) && c < b;
        };
}

// };  // namespace functools
