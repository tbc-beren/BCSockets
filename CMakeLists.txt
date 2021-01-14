#
# === Coverage ===============================================================
# cmake .. -DBCSOCKETS_TESTCOVERAGE=1
# rm *.info
# rm -rf cov-results
# lcov -t "result" -o coverage_01.info -c -d .
# lcov --remove coverage_01.info -o coverage.info \
#     '/usr/include/*' \
#     '*/googletest-src/*'
# genhtml -o cov-results coverage.info
#
#
# === Build Debug ============================================================
# cmake .. -DCMAKE_BUILD_TYPE=Debug
#
cmake_minimum_required (VERSION 3.8)

set (CMAKE_CXX_STANDARD 11)

project (BCSockets CXX)
add_library(BCSockets INTERFACE)
target_include_directories(BCSockets INTERFACE include/)






# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.googletest googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()


# Set coverage options when needed
if (DEFINED BCSOCKETS_TESTCOVERAGE)
set(COVERAGE_OPTIONS "--coverage")
endif (DEFINED BCSOCKETS_TESTCOVERAGE)

enable_testing()
add_executable(
    BCSocketsTest

    # The tests
    test/test.cpp
    test/testStream.cpp
    )
target_link_libraries(BCSocketsTest gtest_main)
target_link_libraries(BCSocketsTest BCSockets ${COVERAGE_OPTIONS})
if(MSVC)
  target_compile_options(BCSocketsTest PRIVATE /W4 /WX)
else()
  target_compile_options(BCSocketsTest PRIVATE -Wall -Wextra -pedantic -Werror ${COVERAGE_OPTIONS})
endif()
add_test(NAME BCSocketsTestName COMMAND BCSocketsTest)