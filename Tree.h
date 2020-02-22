//
// Created by muzamil on 1/21/20.
//

#ifndef POINT_LOC_B_TREE_TREE_H
#define POINT_LOC_B_TREE_TREE_H

#include <vector>
#include <set>
#include <iostream>
#include "Node.h"

class Tree
{
 private:
  std::vector< std::vector<Node> > tree;

 public:
  Tree(const int& height);
  Tree(const int& height ,const std::vector<Node>& nodes, const bool& direction);
  void fillTree(std::set<LineSegment, YLeftLessThan>& lineSegments);
  const Node& getRoot() const;
};

#endif  // POINT_LOC_B_TREE_TREE_H
