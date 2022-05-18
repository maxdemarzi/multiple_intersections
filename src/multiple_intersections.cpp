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

#include "benchmark/benchmark.h"
#include <random>
#include <set>
#include <vector>
#include <algorithm>

// Generate the Data
std::unordered_map<uint64_t, std::unordered_map<uint64_t, std::vector<std::vector<uint64_t>>>> sorted_maps;
std::vector<std::vector<uint64_t>> sorted_vectors;

std::vector<uint64_t> generate_sorted_data(int count, size_t size) {
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

void DoSetupMultiSet(const benchmark::State& state) {
    std::vector<int> counts = { 2, 3, 4, 5, 6, 7 };
    std::vector<int> sizes = { 8, 64, 512, 4096, 32768, 262144 };
    for (auto count : counts) {
        for (auto size : sizes) {
            std::vector<std::vector<uint64_t>> vectors;
            for (int i = 0; i < count; i++) {
                vectors.emplace_back(generate_sorted_data(count, size));
            }
            sorted_maps[count][size] = vectors;
        }
    }

    // Setup/Teardown should never be called with any thread_idx != 0.
    assert(state.thread_index() == 0);
}

std::vector<uint64_t> using_ranges_multi_set_intersection(std::vector<std::vector<uint64_t>>& nums) {

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
    }

    return result;
}

static void BM_using_ranges_multi_set_intersection(benchmark::State &state) {
    sorted_vectors = sorted_maps[state.range(0)][state.range(1)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(using_ranges_multi_set_intersection(sorted_vectors));
    }
}



BENCHMARK(BM_using_ranges_multi_set_intersection)->ArgsProduct({
                      benchmark::CreateDenseRange(2, 7, /*step=*/ 1),
                      benchmark::CreateRange(8, 262144, /*multi=*/ 8) })
                      ->Setup(DoSetupMultiSet);

BENCHMARK_MAIN();