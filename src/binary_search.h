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

#ifndef MULTIPLE_INTERSECTIONS_BINARY_SEARCH_H
#define MULTIPLE_INTERSECTIONS_BINARY_SEARCH_H

/**
 * This is pure binary search
 * Used by BSintersectioncardinality below
 * @param array
 * @param pos
 * @param min
 * @return
 */
static size_t bs_advance_until(const uint64_t * array, const size_t pos,
                               const size_t length, const size_t min) {
    size_t lower = pos + 1;
    if (lower == length || array[lower] >= min) {
        return lower;
    }
    // can safely assume that length>0
    size_t upper = length - 1;
    if (array[upper] < min) {
        return length;
    }
    size_t mid;
    while (lower < upper) {
        mid = (lower + upper) / 2;
        if (array[mid] == min) {
            return mid;
        }

        if (array[mid] < min) {
            lower = mid + 1;
        } else {
            upper = mid;
        }
    }
    return upper;
}

/**
 * Based on binary search.
 */
size_t binary_search_intersection(const uint64_t * set1, const size_t length1,
                      const uint64_t * set2, const size_t length2, uint64_t *out) {
    if ((0 == length1) or (0 == length2))
        return 0;
    size_t answer = 0;
    size_t k1 = 0, k2 = 0;
    while (true) {
        if (set1[k1] < set2[k2]) {
            k1 = bs_advance_until(set1, k1, length1, set2[k2]);
            if (k1 == length1)
                return answer;
        }
        if (set2[k2] < set1[k1]) {
            k2 = bs_advance_until(set2, k2, length2, set1[k1]);
            if (k2 == length2)
                return answer;
        } else {
            out[answer++] = set1[k1];
            ++k1;
            if (k1 == length1)
                break;
            ++k2;
            if (k2 == length2)
                break;
        }
    }
    return answer;
}

std::vector<uint64_t> using_binary_search(std::vector<std::vector<uint64_t>>& nums) {

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
                binary_search_intersection(result.data(), result.size(), nums[i].data(), nums[i].size(), result.data());
        result.resize(inter_length);
    }
    return result;
}

#endif //MULTIPLE_INTERSECTIONS_BINARY_SEARCH_H
