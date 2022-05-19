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
#include "std_set_intersection.h"
#include "binary_search.h"
#include "less_branching.h"
#include "galloping_search.h"

TEST_CASE("using_ranges_set_intersection is correct", "[ranges_set_intersection]") {
    std::vector<uint64_t> test = {1,3,5,7,9};
    std::vector<uint64_t> test2 = {3,6,8,9,32};
    std::vector<std::vector<uint64_t>> nums = {test, test2};
    std::vector<uint64_t> result;
    std::vector<uint64_t> expected = {3,9};
    result = using_ranges_set_intersection(nums);
    REQUIRE(result.size() == 2);
    REQUIRE(result == expected);
}

TEST_CASE("using_set_intersection_in_place in_place is correct", "[in_place]") {
    std::vector<uint64_t> test = {1,3,5,7,9};
    std::vector<uint64_t> test2 = {3,6,8,9,32};
    std::vector<std::vector<uint64_t>> nums = {test, test2};
    std::vector<uint64_t> result;
    std::vector<uint64_t> expected = {3,9};
    result = using_set_intersection_in_place(nums);
    REQUIRE(result.size() == 2);
    REQUIRE(result == expected);
}

TEST_CASE("all are correct", "[all]") {
    const std::vector<uint64_t> test = { 12635, 12921, 12921, 12921, 12921, 12923, 12924, 12926};
    const std::vector<uint64_t> test2 = { 3756, 11996, 12044, 12049, 12109, 12128,
                                          12131, 12141, 12142, 12150, 12154, 12160, 12167, 12168, 12172,
                                          12177, 12201, 12208, 12215, 12216, 12223, 12228, 12232, 12233,
                                          12234, 12235, 12236, 12240, 12241, 12242, 12243, 12254, 12255,
                                          12256, 12257, 12259, 12260, 12261, 12262, 12264, 12265, 12266,
                                          12275, 12295, 12471, 12482, 12486, 12508, 12509, 12510, 12511,
                                          12512, 12530, 12536, 12572, 12573, 12589, 12607, 12609, 12611,
                                          12630, 12631, 12632, 12633, 12634, 12635, 12636, 12653, 12655,
                                          12657, 12668, 12672, 12685, 12702, 12716, 12721, 12741, 12745,
                                          12750, 12755, 12757, 12761, 12765, 12767, 12768, 12794, 12802,
                                          12803, 12823, 12842, 12851, 12871, 12891, 12893, 12894, 12895,
                                          12896, 12897, 12915, 12917, 12918, 12919, 12920, 12921, 12922,
                                          12923, 12924, 12925, 12927, 12929, 12932, 12933, 12934, 12935,
                                          12936, 12937, 12938, 12939, 12942, 12946, 12951, 12955, 12963,
                                          12972, 13011, 13013, 13014, 13015, 13017, 13032, 13033, 13036,
                                          13042, 13050, 13051, 13052, 13057, 13058, 13060, 13090, 13120,
                                          13132, 13136, 13147, 13185, 13191, 13192, 13193, 13194, 13195,
                                          13198, 13202, 13205, 13219, 13228, 13230, 13232, 13233, 13238,
                                          13240, 13246, 13248, 13277, 13278, 13281, 13282, 13283, 13284,
                                          13291, 13320, 13338, 13346, 13347 };
    std::vector<std::vector<uint64_t>> nums = {test, test2};
    std::vector<uint64_t> result;
    std::vector<uint64_t> expected = {12635, 12921, 12923, 12924};
    result = using_ranges_set_intersection(nums);
    REQUIRE(result.size() == 4);
    REQUIRE(result == expected);
    result = using_set_intersection_in_place(nums);
    REQUIRE(result.size() == 4);
    REQUIRE(result == expected);
    result = using_binary_search(nums);
    REQUIRE(result.size() == 4);
    REQUIRE(result == expected);
    result = using_less_branching(nums);
    REQUIRE(result.size() == 4);
    REQUIRE(result == expected);
    result = using_less_branching_unrolled(nums);
    REQUIRE(result.size() == 4);
    REQUIRE(result == expected);
    result = using_galloping_search(nums);
    REQUIRE(result.size() == 4);
    REQUIRE(result == expected);
}