//
// Created by muzamil on 2/12/20.
//

#ifndef POINT_LOC_B_TREE_MIDDLETREE_H
#define POINT_LOC_B_TREE_MIDDLETREE_H

#include "MiddleNode.h"
#include "SuperNode.h"

class MiddleTree
{
 private:
  std::vector<std::vector<MiddleNode> > tree;

 public:
  MiddleTree(const int& height);
    MiddleTree(const unsigned int& height, SuperNode& superRoot,
               const std::vector<LineSegment>& lineSegments);
  void fillTree(std::vector<LineSegment>& lineSegments);
  MiddleNode* getRoot();
};

#endif  // POINT_LOC_B_TREE_MIDDLETREE_H
