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
#include "generate_data.h"
#include "std_set_intersection.h"
#include "galloping_search.h"
#include "binary_search.h"
#include "less_branching.h"

static void BM_using_ranges_set_intersection(benchmark::State &state) {
    sorted_vectors = sorted_maps[state.range(0)][state.range(1)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(using_ranges_set_intersection(sorted_vectors));
    }
}

static void BM_using_set_intersection_in_place(benchmark::State &state) {
    sorted_vectors = sorted_maps[state.range(0)][state.range(1)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(using_set_intersection_in_place(sorted_vectors));
    }
}

static void BM_using_galloping_search(benchmark::State &state) {
    sorted_vectors = sorted_maps[state.range(0)][state.range(1)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(using_galloping_search(sorted_vectors));
    }
}

static void BM_using_binary_search(benchmark::State &state) {
    sorted_vectors = sorted_maps[state.range(0)][state.range(1)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(using_binary_search(sorted_vectors));
    }
}

static void BM_using_less_branching(benchmark::State &state) {
    sorted_vectors = sorted_maps[state.range(0)][state.range(1)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(using_less_branching(sorted_vectors));
    }
}

static void BM_using_less_branching_unrolled(benchmark::State &state) {
    sorted_vectors = sorted_maps[state.range(0)][state.range(1)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(using_less_branching_unrolled(sorted_vectors));
    }
}

BENCHMARK(BM_using_ranges_set_intersection)
    ->ArgsProduct({benchmark::CreateDenseRange(2, 7, /*step=*/ 1),
                   benchmark::CreateRange(8, 262144, /*multi=*/ 8)})
        ->Setup(load_data);

BENCHMARK(BM_using_set_intersection_in_place)
        ->ArgsProduct({benchmark::CreateDenseRange(2, 7, /*step=*/ 1),
                       benchmark::CreateRange(8, 262144, /*multi=*/ 8)})
        ->Setup(load_data);

BENCHMARK(BM_using_galloping_search)
        ->ArgsProduct({benchmark::CreateDenseRange(2, 7, /*step=*/ 1),
                       benchmark::CreateRange(8, 262144, /*multi=*/ 8)})
        ->Setup(load_data);

BENCHMARK(BM_using_binary_search)
        ->ArgsProduct({benchmark::CreateDenseRange(2, 7, /*step=*/ 1),
                       benchmark::CreateRange(8, 262144, /*multi=*/ 8)})
        ->Setup(load_data);

BENCHMARK(BM_using_less_branching)
        ->ArgsProduct({benchmark::CreateDenseRange(2, 7, /*step=*/ 1),
                       benchmark::CreateRange(8, 262144, /*multi=*/ 8)})
        ->Setup(load_data);

BENCHMARK(BM_using_less_branching_unrolled)
        ->ArgsProduct({benchmark::CreateDenseRange(2, 7, /*step=*/ 1),
                       benchmark::CreateRange(8, 262144, /*multi=*/ 8)})
        ->Setup(load_data);

BENCHMARK_MAIN();