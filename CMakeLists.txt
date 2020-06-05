cmake_minimum_required(VERSION 3.16)
project(point_loc_B_tree)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(point_loc_B_tree
        global_const.h
        LineSegment.cpp
        LineSegment.h
        main.cpp
        MiddleNode.cpp
        MiddleNode.h
        MiddleTree.cpp
        MiddleTree.h
        Node.cpp
        Node.h
        Point.cpp
        Point.h
        SuperNode.cpp
        SuperNode.h
        SuperTree.cpp
        SuperTree.h
        Tree.cpp
        Tree.h)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
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

# Now simply link against gtest or gtest_main as needed. Eg
add_executable(UnitTests Point.h Point.cpp LineSegment.h LineSegment.cpp Node.h Node.cpp MiddleNode.h MiddleNode.cpp SuperNode.h SuperNode.cpp Tree.h Tree.cpp MiddleTree.h MiddleTree.cpp SuperTree.h SuperTree.cpp  test.cpp)
target_link_libraries(UnitTests gtest_main)

include(GoogleTest)
gtest_discover_tests(UnitTests)
