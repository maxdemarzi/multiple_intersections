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

#ifndef MULTIPLE_INTERSECTIONS_GENERATE_DATA_H
#define MULTIPLE_INTERSECTIONS_GENERATE_DATA_H

#include <random>
#include <set>
#include <vector>
#include <algorithm>

// Generate the Data
std::unordered_map<uint64_t, std::unordered_map<uint64_t, std::vector<std::vector<uint64_t>>>> sorted_maps;
std::vector<std::vector<uint64_t>> sorted_vectors;

std::vector<uint64_t> generate_sorted_data(uint64_t count, uint64_t size) {
    auto randomNumberBetween = [](uint64_t low, uint64_t high) {
        auto randomFunc = [distribution_ = std::uniform_int_distribution<uint64_t>(low, high),
                random_engine_ = std::mt19937{ std::random_device{}() }]() mutable {
            return distribution_(random_engine_);
        };
        return randomFunc;
    };

    std::vector<uint64_t> data;
    std::generate_n(std::back_inserter(data), size, randomNumberBetween(1, size * 20 / count));

    std::ranges::sort(data);
    return data;
}

void load_data(const benchmark::State& state) {
    auto count = state.range(0);
    auto size = state.range(1);

    std::vector<std::vector<uint64_t>> vectors;
    for (auto i = 0; i < count; i++) {
        vectors.emplace_back(generate_sorted_data(count, size));
    }
    sorted_maps[count][size] = vectors;

    // Setup/Teardown should never be called with any thread_idx != 0.
    assert(state.thread_index() == 0);
}

#endif //MULTIPLE_INTERSECTIONS_GENERATE_DATA_H
