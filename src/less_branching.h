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

#ifndef MULTIPLE_INTERSECTIONS_LESS_BRANCHING_H
#define MULTIPLE_INTERSECTIONS_LESS_BRANCHING_H

#include <set>
#include <vector>
#include <algorithm>

/**
 * Branchless approach by N. Kurz.
 */
size_t scalar_branchless(const uint64_t *A, size_t lenA,
                         const uint64_t *B, size_t lenB,
                         uint64_t *Match) {

    const uint64_t *initMatch = Match;
    const uint64_t *endA = A + lenA;
    const uint64_t *endB = B + lenB;

    while (A < endA && B < endB) {
        int m = (*B == *A) ? 1 : 0;  // advance Match only if equal
        int a = (*B >= *A) ? 1 : 0;  // advance A if match or B ahead
        int b = (*B <= *A) ? 1 : 0;  // advance B if match or B behind

        *Match = *A;   // write the result regardless of match
        Match += m;    // but will be rewritten unless advanced
        A += a;
        B += b;
    }

    size_t count = Match - initMatch;
    return count;
}


// use in function below
#define BRANCHLESSMATCH() {                     \
        int m = (*B == *A) ? 1 : 0;             \
        int a = (*B >= *A) ? 1 : 0;             \
        int b = (*B <= *A) ? 1 : 0;             \
        *Match = *A;                            \
        Match += m;                             \
        A += a;                                 \
        B += b;                                 \
    }

/**
 * Unrolled branchless approach by N. Kurz.
 */
size_t scalar_branchless_unrolled(const uint64_t *A, size_t lenA,
                                  const uint64_t *B, size_t lenB,
                                  uint64_t *Match) {

    const size_t UNROLLED = 4;

    const uint64_t *initMatch = Match;
    const uint64_t *endA = A + lenA;
    const uint64_t *endB = B + lenB;

    if (lenA >= UNROLLED && lenB >= UNROLLED) {
        const uint64_t *stopA = endA - UNROLLED;
        const uint64_t *stopB = endB - UNROLLED;

        while (A < stopA && B < stopB) {
            BRANCHLESSMATCH();  // NOTE: number of calls must match UNROLLED
            BRANCHLESSMATCH();
            BRANCHLESSMATCH();
            BRANCHLESSMATCH();
        }
    }

    // Finish remainder without overstepping
    while (A < endA && B < endB) {
        BRANCHLESSMATCH();
    }

    size_t count = Match - initMatch;
    return count;
}

#undef BRANCHLESSMATCH

std::vector<uint64_t> using_less_branching(std::vector<std::vector<uint64_t>>& nums) {

    // 1. Check if any index is empty, if so then the intersection is empty
    for (auto& index : nums) {
        if (index.empty()) {
            return {};
        }
    }

    // 2. Sort indexes by their first value
    std::sort(nums.begin(), nums.end());

    // 3. Swap the 2nd vector for the last one to try to eliminate a bunch right away
    nums.at(1).swap(nums.at(nums.size() - 1));

    // initialize by the first vector
    std::vector<uint64_t> result(nums[0].begin(), nums[0].end());

    for (int i = 1; i < nums.size(); ++i) {
        // here we can change the intersection function to any regular scalar
        // or vector pair-set intersection algorithms.
        size_t inter_length =
                scalar_branchless(result.data(), result.size(), nums[i].data(), nums[i].size(), result.data());
        result.resize(inter_length);
    }
    return result;
}

std::vector<uint64_t> using_less_branching_unrolled(std::vector<std::vector<uint64_t>>& nums) {

    // 1. Check if any index is empty, if so then the intersection is empty
    for (auto& index : nums) {
        if (index.empty()) {
            return {};
        }
    }

    // 2. Sort indexes by their first value
    std::sort(nums.begin(), nums.end());

    // 3. Swap the 2nd vector for the last one to try to eliminate a bunch right away
    nums.at(1).swap(nums.at(nums.size() - 1));

    // initialize by the first vector
    std::vector<uint64_t> result(nums[0].begin(), nums[0].end());

    for (int i = 1; i < nums.size(); ++i) {
        // here we can change the intersection function to any regular scalar
        // or vector pair-set intersection algorithms.
        size_t inter_length =
                scalar_branchless_unrolled(result.data(), result.size(), nums[i].data(), nums[i].size(), result.data());
        result.resize(inter_length);
    }
    return result;
}

#endif //MULTIPLE_INTERSECTIONS_LESS_BRANCHING_H
