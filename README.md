# Multiple Intersections
Google Benchmark code to see which algorithm we should use to
get the intersection of multiple vectors.

Imagine a group of people, and they all have a list of friends by some integer id. 
We want to quickly find out which friends, if any they all share one in common.

We will also need a simple one list against many lists but individually. 
So we'll test intersecting just 2 lists as well.

Ideas and code from: 

[Daniel Lemire, Nathan Kurz, Owen Kaser](https://github.com/lemire/SIMDIntersections)
[Sergey Slotin](https://en.algorithmica.org/hpc/data-structures/binary-search/)
[Paul-Virak Khuong, Pat Morin](https://arxiv.org/pdf/1509.05053.pdf)



## Installation:

Install Additional Dependencies:

    sudo apt-get install -y ccache python3-pip

Install conan:

    pip install --user conan
    sudo ln -s ~/.local/bin/conan /usr/bin/conan

Running it:

./cmake-build-release/bin/multiple_intersections --benchmark_out "./results/2022-05-19.csv" --benchmark_out_format=csv

## Comparing:

    git clone https://github.com/google/benchmark.git
    cd ./benchmark/tools
    pip3 install -r requirements.txt

If the requirements install fails, try:

    pip3 install numpy
    pip3 install scipy
    pip3 install pandas

Once the requirements are met try:

    ./compare.py filters ../../multiple_intersections/cmake-build-release/bin/multiple_intersections BM_using_ranges_set_intersection BM_using_set_intersection_in_place

Winner BM_using_set_intersection_in_place by 0.1820

    ./compare.py filters ../../multiple_intersections/cmake-build-release/bin/multiple_intersections BM_using_set_intersection_in_place BM_using_galloping_search

Winner BM_using_set_intersection_in_place by 0.1647

    ./compare.py filters ../../multiple_intersections/cmake-build-release/bin/multiple_intersections BM_using_set_intersection_in_place BM_using_binary_search

Winner BM_using_set_intersection_in_place by 0.5332

    ./compare.py filters ../../multiple_intersections/cmake-build-release/bin/multiple_intersections BM_using_set_intersection_in_place BM_using_less_branching

Winner BM_using_set_intersection_in_place by 0.2770

    ./compare.py filters ../../multiple_intersections/cmake-build-release/bin/multiple_intersections BM_using_set_intersection_in_place BM_using_less_branching_unrolled
 
Winner BM_using_set_intersection_in_place by 0.2554

## Important Note:

As stated in google benchmark documentation, to disable CPU scaling use cpupower tool.

Install the linux tools generic package:

    sudo apt-get install linux-tools-generic

Check the cpupower frequency information:

    cpupower frequency-info -o proc

If you see this something like this with "powersave" as the governor:

              minimum CPU frequency  -  maximum CPU frequency  -  governor
    CPU  0       800000 kHz ( 15 %)  -    5100000 kHz (100 %)  -  powersave
    CPU  1       800000 kHz ( 15 %)  -    5100000 kHz (100 %)  -  powersave
    CPU  2       800000 kHz ( 15 %)  -    5100000 kHz (100 %)  -  powersave
    CPU  3       800000 kHz ( 15 %)  -    5100000 kHz (100 %)  -  powersave
    CPU  4       800000 kHz ( 15 %)  -    5100000 kHz (100 %)  -  powersave
    CPU  5       800000 kHz ( 15 %)  -    5100000 kHz (100 %)  -  powersave
    CPU  6       800000 kHz ( 15 %)  -    5100000 kHz (100 %)  -  powersave
    CPU  7       800000 kHz ( 15 %)  -    5100000 kHz (100 %)  -  powersave

Run this command to use the performance governor instead:

    sudo cpupower frequency-set --governor performance

Check to make sure it took effect:

    cpupower frequency-info -o proc

          minimum CPU frequency  -  maximum CPU frequency  -  governor
    CPU  0       800000 kHz ( 15 %)  -    5100000 kHz (100 %)  -  performance
    CPU  1       800000 kHz ( 15 %)  -    5100000 kHz (100 %)  -  performance
    CPU  2       800000 kHz ( 15 %)  -    5100000 kHz (100 %)  -  performance
    CPU  3       800000 kHz ( 15 %)  -    5100000 kHz (100 %)  -  performance
    CPU  4       800000 kHz ( 15 %)  -    5100000 kHz (100 %)  -  performance
    CPU  5       800000 kHz ( 15 %)  -    5100000 kHz (100 %)  -  performance
    CPU  6       800000 kHz ( 15 %)  -    5100000 kHz (100 %)  -  performance
    CPU  7       800000 kHz ( 15 %)  -    5100000 kHz (100 %)  -  performance

Now you can run the benchmarks. Once you are done, reset it back to powersave

    sudo cpupower frequency-set --governor powersave
    