//
// Created by muzamil on 1/31/20.
//

#ifndef POINT_LOC_B_TREE_SUPERTREE_H
#define POINT_LOC_B_TREE_SUPERTREE_H

#include <vector>
#include "SuperNode.h"
class SuperTree
{
 private:
  std::vector< std::vector<SuperNode> > superTree;

 public:
  SuperTree(const int& height);
  void buildBottomUpBTree(const std::vector<SuperNode>& nodes);
  void fillSuperTree(std::set<LineSegment, YLeftLessThan>& lineSegments);
  const SuperNode& getRoot() const;
};

#endif  // POINT_LOC_B_TREE_SUPERTREE_H
