# Automatically enable catch2 to generate ctest targets
if(CONAN_CATCH2_ROOT_DEBUG)
    include(${CONAN_CATCH2_ROOT_DEBUG}/lib/cmake/Catch2/Catch.cmake)
else()
    include(${CONAN_CATCH2_ROOT}/lib/cmake/Catch2/Catch.cmake)
endif()

add_library(catch_main STATIC catch_main.cpp)
target_link_libraries(catch_main PUBLIC)
target_link_libraries(catch_main PRIVATE project_options)


add_executable(tests catch_main.cpp upper_and_lower_bound_tests.cpp intersection_tests.cpp)
target_link_libraries(tests PRIVATE project_warnings project_options catch_main)
#target_link_libraries(tests PRIVATE project_options catch_main)
target_include_directories(tests PRIVATE ../src)

