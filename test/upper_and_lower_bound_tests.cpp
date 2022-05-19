/*
 * Copyright Max De Marzi. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <catch2/catch.hpp>
#include <set>
#include <vector>
#include <algorithm>

#if defined(__GNUC__) || defined(__HP_aCC) || defined(__clang__)
#define INLINE inline __attribute__((always_inline))
#else
#define INLINE __forceinline
#endif


template< class T>
T branchless_lower_bound(std::vector<T>& vec, T value) {
    auto len = vec.size();
    int *base = vec.data();
      while (len > 1) {
        auto half = len / 2;
        base = (base[half] < value ? &base[half] : base);
        len -= half;
    }
    return *(base + (*base < value));
}

template< class T>
T branch_less_upper_bound(std::vector<T>& vec, T value) {
    auto len = vec.size();
    int *base = vec.data();
    while (len > 1) {
        auto half = len / 2;
        base = (value < base[half] ? base : &base[half]);
        len -= half;
    }

    return *(base + (*base <= value));
}

template<class RandomAccessIterator, class T>
RandomAccessIterator branch_less_upper_bound_iterators(RandomAccessIterator first, RandomAccessIterator last, T value) {
    auto len = std::distance(first, last);
    while (len > 1) {
        auto half = len / 2;
        std::ranges::advance(first, half);
        std::ranges::advance(first, (value < *first ? -1 * half : 0));
        len -= half;
    }
    std::ranges::advance(first, *first <= value);
    return first;
}

TEST_CASE("Lower Bound is correct", "[lower_bound]") {
    std::vector<int> test = {1,3,5,7,9};
    REQUIRE(branchless_lower_bound<int>(test, 4) == std::lower_bound(test.begin(), test.end(), 4).operator*());
    REQUIRE(branchless_lower_bound<int>(test, 5) == std::lower_bound(test.begin(), test.end(), 5).operator*());
    REQUIRE(branchless_lower_bound<int>(test, 11) == std::lower_bound(test.begin(), test.end(), 11).operator*());
}

TEST_CASE("Upper Bound is correct", "[upper_bound]") {
    std::vector<int> test = {1,3,5,7,9};
    REQUIRE(branch_less_upper_bound<int>(test, 4) == std::upper_bound(test.begin(), test.end(), 4).operator*());
    REQUIRE(branch_less_upper_bound<int>(test, 5) == std::upper_bound(test.begin(), test.end(), 5).operator*());
    REQUIRE(branch_less_upper_bound<int>(test, 11) == std::upper_bound(test.begin(), test.end(), 11).operator*());

    REQUIRE(*branch_less_upper_bound_iterators(test.begin(), test.end(), 5) == 7);
    REQUIRE(*branch_less_upper_bound_iterators(test.begin(), test.end(), 4) == 5);
    REQUIRE(branch_less_upper_bound_iterators(test.begin(), test.end(), 11) == test.end());

    REQUIRE(branch_less_upper_bound_iterators(test.begin(), test.end(), 4) == std::upper_bound(test.begin(), test.end(), 4));
    REQUIRE(branch_less_upper_bound_iterators(test.begin(), test.end(), 5) == std::upper_bound(test.begin(), test.end(), 5));
    REQUIRE(branch_less_upper_bound_iterators(test.begin(), test.end(), 11) == std::upper_bound(test.begin(), test.end(), 11));
}
