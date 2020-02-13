//
// Created by muzamil on 2/12/20.
//

#ifndef POINT_LOC_B_TREE_MIDDLETREE_H
#define POINT_LOC_B_TREE_MIDDLETREE_H

#include <set>
#include <vector>
#include "MiddleNode.h"

class MiddleTree
{
 private:
  std::vector< std::vector<MiddleNode> > tree;

 public:
  MiddleTree(const int& height);
  void buildBottomUpBTree(const std::vector<MiddleNode>& nodes);
  void fillTree(std::set<LineSegment, YLeftLessThan>& lineSegments);
  const MiddleNode& getRoot() const;
};

#endif  // POINT_LOC_B_TREE_MIDDLETREE_H
