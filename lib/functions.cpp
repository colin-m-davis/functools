#include <utility>

namespace functools {

/*
returns a function that applies the input functions from left to right on its inputs
*/
[[nodiscard]] auto pipeline(auto&& first, auto&&... rest) {
    if constexpr (sizeof...(rest) == 0) {
        return first;
    } else {
        return
            [=](auto&&... args) {
                return pipeline(rest...)(first(std::forward<decltype(args)>(args)...));
            };
    }
}

};  // namespace functools
