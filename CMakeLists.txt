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

#configure_file(/Users/muzamil/Dropbox/2020-classes/fall19/algoritm/code/pt-set-gen/objects.1K.1583107550.long objects.1k.1583107550.long   COPYONLY)