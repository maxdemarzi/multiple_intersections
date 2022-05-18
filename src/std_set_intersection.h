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

#ifndef MULTIPLE_INTERSECTIONS_STD_SET_INTERSECTION_H
#define MULTIPLE_INTERSECTIONS_STD_SET_INTERSECTION_H

#include <set>
#include <vector>
#include <algorithm>

std::vector<uint64_t> using_ranges_set_intersection(std::vector<std::vector<uint64_t>>& nums) {

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
        std::vector<uint64_t> intersection;
        std::ranges::set_intersection(nums[i], result, back_inserter(intersection));
        result = intersection;
        if (result.empty()) return result;
    }

    return result;
}

std::vector<uint64_t> using_set_intersection_in_place(std::vector<std::vector<uint64_t>>& nums) {

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

    // https://stackoverflow.com/questions/1773526/in-place-c-set-intersection
    for (int i = 1; i < nums.size(); ++i) {
        auto end = std::set_intersection(
                result.begin(), result.end(),
                nums[i].begin(), nums[i].end(),
                result.begin() // intersection is written in results
        );
        result.erase(end, result.end()); // erase redundant elements
        if (result.empty()) return result;
    }

    return result;
}

#endif //MULTIPLE_INTERSECTIONS_STD_SET_INTERSECTION_H
