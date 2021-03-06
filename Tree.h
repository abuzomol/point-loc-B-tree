//
// Created by muzamil on 1/21/20.
//

#ifndef POINT_LOC_B_TREE_TREE_H
#define POINT_LOC_B_TREE_TREE_H

#include "Node.h"

class Tree
{
 private:
  std::vector<std::vector<Node> > tree;

 public:
  Tree(const int& height);
  Tree(const int& height, const std::vector<LineSegment>& lineSegments, const bool& isLeftTree);
  void fillTree(const std::vector<LineSegment>& lineSegments);
  Node* getRoot();
};

#endif  // POINT_LOC_B_TREE_TREE_H
