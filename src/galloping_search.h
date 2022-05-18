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

#ifndef MULTIPLE_INTERSECTIONS_GALLOPING_SEARCH_H
#define MULTIPLE_INTERSECTIONS_GALLOPING_SEARCH_H

/**
 * This is often called galloping or exponential search.
 *
 * Used by frogintersectioncardinality below
 *
 * Based on binary search...
 * Find the smallest integer larger than pos such
 * that array[pos]>= min.
 * If none can be found, return array.length.
 * From code by O. Kaser.
 */
static size_t frog_advance_until(const uint64_t * array, const size_t pos,
                                 const size_t length, const size_t min) {
    size_t lower = pos + 1;

    // special handling for a possibly common sequential case
    if ((lower >= length) or (array[lower] >= min)) {
        return lower;
    }

    size_t spansize = 1; // could set larger
    // bootstrap an upper limit

    while ((lower + spansize < length) and (array[lower + spansize] < min))
        spansize *= 2;
    size_t upper = (lower + spansize < length) ? lower + spansize : length - 1;

    // maybe we are lucky (could be common case when the seek ahead expected to be small and sequential will otherwise make us look bad)
    //if (array[upper] == min) {
    //    return upper;
    //}

    if (array[upper] < min) {// means array has no item >= min
        return length;
    }

    // we know that the next-smallest span was too small
    lower += (spansize / 2);

    // else begin binary search
    size_t mid = 0;
    while (lower + 1 != upper) {
        mid = (lower + upper) / 2;
        if (array[mid] == min) {
            return mid;
        } else if (array[mid] < min)
            lower = mid;
        else
            upper = mid;
    }
    return upper;

}

size_t onesided_galloping_intersection(const uint64_t * smallset,
                                     const size_t smalllength, const uint64_t * largeset,
                                     const size_t largelength, uint64_t * out) {
    if(largelength < smalllength) return onesided_galloping_intersection(largeset, largelength, smallset, smalllength, out);
    if (0 == smalllength)
        return 0;
    const uint64_t * const initout(out);
    size_t k1 = 0, k2 = 0;
    while (true) {
        if (largeset[k1] < smallset[k2]) {
            k1 = frog_advance_until(largeset, k1, largelength, smallset[k2]);
            if (k1 == largelength)
                break;
        }
        midpoint: if (smallset[k2] < largeset[k1]) {
        ++k2;
        if (k2 == smalllength)
            break;
    } else {
        *out++ = smallset[k2];
        ++k2;
        if (k2 == smalllength)
            break;
        k1 = frog_advance_until(largeset, k1, largelength, smallset[k2]);
        if (k1 == largelength)
            break;
        goto midpoint;
    }
    }
    return out - initout;

}

std::vector<uint64_t> using_galloping_search(std::vector<std::vector<uint64_t>>& nums) {

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
                onesided_galloping_intersection(result.data(), result.size(), nums[i].data(), nums[i].size(), result.data());
        result.resize(inter_length);
    }
    return result;
}

#endif //MULTIPLE_INTERSECTIONS_GALLOPING_SEARCH_H
