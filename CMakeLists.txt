cmake_minimum_required(VERSION 3.15)
project(multiple_intersections VERSION 1.0.0 LANGUAGES CXX)
include(cmake/StandardProjectSettings.cmake)
include(cmake/PreventInSourceBuilds.cmake)

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options INTERFACE)
set(CMAKE_CXX_STANDARD 20)
target_compile_features(project_options INTERFACE cxx_std_20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
if(COMPILER_SUPPORTS_MARCH_NATIVE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
endif()

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)

# enable cache system
include(cmake/Cache.cmake)

# standard compiler warning
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# Set up some extra Conan dependencies based on our needs before loading Conan
set(CONAN_EXTRA_REQUIRES "")
set(CONAN_EXTRA_OPTIONS "")
set(ENV{CONAN_REVISIONS_ENABLED} 1)

# Run conan install automatically, or display warning if conan is not installed
include(cmake/Conan.cmake)

file(GLOB_RECURSE ALL_BENCH_CPP src/*.cpp)

foreach(ONE_BENCH_CPP ${ALL_BENCH_CPP})

    get_filename_component(ONE_BENCH_EXEC ${ONE_BENCH_CPP} NAME_WE)
    set(TARGET_NAME Benchmark_${ONE_BENCH_EXEC})

    add_executable(${TARGET_NAME} ${ONE_BENCH_CPP} src/multiple_intersections.cpp src/generate_data.h src/std_set_intersection.h src/galloping_search.h src/binary_search.h src/less_branching.h)
    set_target_properties(${TARGET_NAME} PROPERTIES OUTPUT_NAME ${ONE_BENCH_EXEC})
    target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/src)
    target_link_libraries(
            ${TARGET_NAME}
            PUBLIC
            benchmark
            ${CMAKE_THREAD_LIBS_INIT}
    )

endforeach()

option(ENABLE_TESTING "Enable Test Builds" ON)

if(ENABLE_TESTING)
    enable_testing()
    message("Building Tests. Be sure to check out test/constexpr_tests for constexpr testing")
    add_subdirectory(test)
endif()