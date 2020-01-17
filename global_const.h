//
// Created by muzamil on 1/14/20.
//

#ifndef POINT_LOC_B_TREE_GLOBAL_CONST_H
#define POINT_LOC_B_TREE_GLOBAL_CONST_H

#define MAX_ELTS 5000000
#define DOUBLE_PRECISION 25
#define ELEMENT_TYPE double

// Grid is [0..10000, 0..10000]
#define EPS 1e-9
#define GRID_SIZE 10000.0
#define GRID_LEFT_BOUNDARY 0
#define GRID_RIGHT_BOUNDARY GRID_SIZE
#define Y_INFTY GRID_SIZE
#define X_INFTY GRID_SIZE
#define MINUS_X_INFTY 0
#define MINUS_Y_INFTY 0

// Took the numbers from /sys/devices/system/cpu/cpu*/cache
// assuming 1 KB = 1000B

#define OBJ_SIZE 32          // in bytes (4 words for now)
#define RAM_SIZE 8137360000  // in bytes (over 7.5GB)
#define L3_SIZE 6144000      // in bytes (6MB, L3 shared among all 4 cores)
#define L2_SIZE 256000       // in bytes (L2 private to each of 4 cores)
#define CACHE_LINE_SIZE 64   // in bytes (L1 through L3 on my office machine)
#define OBJ_IN_L3 L3_SIZE / OBJ_SIZE              // 192,000
#define OBJ_IN_L2 L2_SIZE / OBJ_SIZE              // 8,000
#define L3_CACHE_LINES L3_SIZE / CACHE_LINE_SIZE  // (96,000)
#define L2_CACHE_LINES L2_SIZE / CACHE_LINE_SIZE  // (4,000)
#define WORDS_IN_CACHE_LINE CACHE_LINE_SIZE / 8   // for 64-bit words (8 words)

/*
#define INSERT 0
#define DELETE 1
#define QUERY 2
*/

#define B_SIZE 4
#define Val_SIZE 3
#define A_SIZE 2

#endif // POINT_LOC_B_TREE_GLOBAL_CONST_H
