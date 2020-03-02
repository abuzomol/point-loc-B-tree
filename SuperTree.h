//
// Created by muzamil on 1/31/20.
//

#ifndef POINT_LOC_B_TREE_SUPERTREE_H
#define POINT_LOC_B_TREE_SUPERTREE_H

#include "SuperNode.h"

class SuperTree
{
 private:
  std::vector<std::vector<SuperNode> > superTree;

 public:
  SuperTree(const int& height);
  SuperTree(const int& height, const std::vector<SuperNode>& nodes);
  const SuperNode& getRoot() const;
  const int size() const;
};

void fillSuperTree(SuperNode& superRoot,
                   std::vector<LineSegment>& lineSegments);

#endif  // POINT_LOC_B_TREE_SUPERTREE_H
