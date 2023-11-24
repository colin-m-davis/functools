#include <concepts>
#include <type_traits>
#include <functional>
#include <vector>
#include <tuple>
#include <algorithm>
#include <utility>
#include <iostream>
#include <iterator>

namespace functools {

/*
creates a sequence of length N, starting from the init and repeatedly applying f on the last value in the sequence
*/
[[nodiscard]] auto recursive_seq(auto&& init, auto&& f, int N) {
    std::vector<std::decay_t<decltype(init)>> result;
    result.reserve(N);
    result.push_back(std::forward<decltype(init)>(init));
    for (auto i = 1; i < N; ++i) {
        result.push_back(f(result.back()));
    }
    return result;
}

/*
helper function for zip
returns the minimum size across a sequence of containers
*/
template <typename... Vecs>
[[nodiscard]] constexpr auto min_size(const Vecs&... vecs) {
    return std::min({vecs.size()...});
}

/*
helper function for zip
returns a tuple containing the index-th element of each of the containers
*/
template <typename... Vecs>
[[nodiscard]] constexpr auto make_tuple_at_index(int index, const Vecs&... vecs) {
    return std::make_tuple(vecs[index]...);
}

/*
creates a vector of tuples from a sequence of vectors
*/
template <typename... Vecs>
[[nodiscard]] auto zip(Vecs&&... vecs) {
    std::vector<std::tuple<typename std::decay_t<Vecs>::value_type...>> result;
    const auto sz = min_size(vecs...);
    result.reserve(sz);
    for (auto i = 0; i < sz; ++i) {
        result.push_back(make_tuple_at_index(i, std::forward<Vecs>(vecs)...));
    }
    return result;
}

template <typename Vec>
[[nodiscard]] auto zip_with_indices(Vec&& vec) {
    return zip(recursive_seq(0, [](int x) { return x + 1; }, vec.size()), vec);
}

/*
left fold (left-to-right)
https://en.wikipedia.org/wiki/Fold_(higher-order_function)
*/
template <typename F, typename Vec>
[[nodiscard]] auto foldl(F&& f, const Vec& vec) {
    auto accum = *std::cbegin(vec);
    for (auto x = std::next(std::cbegin(vec)); x != std::cend(vec); ++x) {
        accum = f(std::move(accum), *x);
    }
    return accum;
}

/*
left fold with initial value
*/
template <typename F, typename Vec, typename Init>
[[nodiscard]] auto foldl(F&& f, const Vec& vec, Init&& init) {
    auto accum = init;
    for (auto x = std::cbegin(vec); x != std::cend(vec); ++x) {
        accum = f(std::move(accum), *x);
    }
    return accum;
}

/*
right fold (right-to-left)
https://en.wikipedia.org/wiki/Fold_(higher-order_function)
*/
template <typename F, typename Vec>
[[nodiscard]] auto foldr(F&& f, const Vec& vec) {
    auto accum = *std::crbegin(vec);
    for (auto x = std::next(std::crbegin(vec)); x != std::crend(vec); ++x) {
        accum = f(std::move(accum), *x);
    }
    return accum;
}

/*
right fold with initial value
*/
template <typename F, typename Vec, typename Init>
[[nodiscard]] auto foldr(F&& f, const Vec& vec, Init&& init) {
    auto accum = init;
    for (auto x = std::crbegin(vec); x != std::crend(vec); ++x) {
        accum = f(std::move(accum), *x);
    }
    return accum;
}

/*
applies f to each element of the vec and returns the results in order
*/
template <typename F, typename Vec>
[[nodiscard]] auto map(F&& f, const Vec& vec) -> Vec {
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
f takes in an argument and returns a vector
applies f to all elements of vec and merges the results into one vector
*/
[[nodiscard]] auto flat_map(auto&& f, const auto& vec) {
    using Elem = typename std::decay_t<decltype(f(*std::begin(vec)))>::value_type;
    std::vector<Elem> result;
    for (const auto& x : vec) {
        for (const auto& y : f(x)) {
            result.push_back(y);
        }
    }
    return result;
}


/*
returns a vector containing those elements x in the input vector for which pred(x) is true
*/
template <typename T>
[[nodiscard]] auto filter(auto&& pred, const std::vector<T>& vec) {
    std::vector<T> result;
    for (const auto& x : vec) {
        if (pred(x)) {
            result.push_back(x);
        }
    }
    return result;
}

/*
overload to enable move semantics
*/
template <typename T>
[[nodiscard]] auto filter(auto&& pred, std::vector<T>&& vec) {
    std::vector<T> result;
    for (auto&& x : vec) {
        if (pred(x)) {
            if constexpr (std::is_nothrow_move_constructible_v<T>) {
                result.push_back(std::move(x));
            } else {
                result.push_back(x);
            }
        }
    }
    return result;
}

/*
returns the container... but sorted!
*/
[[nodiscard]] auto sorted(const auto& xs) {
    auto new_xs = xs;
    sort(new_xs.begin(), new_xs.end());
    return new_xs;
}

/*
returns the container... but reversed!
*/
[[nodiscard]] auto reversed(const auto& xs) {
    return std::vector(xs.crbegin(), xs.crend());
}

};  // namespace functools
