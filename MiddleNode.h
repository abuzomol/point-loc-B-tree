//
// Created by muzamil on 1/17/20.
//

#ifndef POINT_LOC_B_TREE_MIDDLENODE_H
#define POINT_LOC_B_TREE_MIDDLENODE_H

#include <ostream>
#include <vector>
#include "LineSegment.h"
#include "global_const.h"

// Val can store a double for y-value, but the total ordering of lineSegments
// make it easier to use the lineSegment itself.
// Hence, we can save the size of double for each node.

// MiddleNode class stores the lineSegments that crosses multiple slabs, and
// stored at the parent node
// Data it holds, val[1..B^epsilon], children[1..B^epsilon+1], as well as set
// of Boleans that can tell whether there is line segment that spans a slab
// within the children nodes

class MiddleNode
{
 private:
  std::vector<LineSegment> val;
  std::vector<MiddleNode*> child;
  // for prototype we use unsigned int to store sqrt B bits
  unsigned int spannedSlabs;

 public:
  unsigned int getSpannedSlabs() const;
  void setSpannedSlabs(unsigned int spannedSlabs);
  MiddleNode();
  MiddleNode(const std::vector<LineSegment>& val);
  MiddleNode(const std::vector<LineSegment>& val,const unsigned int& spannedSlabs);
  MiddleNode(const std::vector<LineSegment>& val,
             const std::vector<MiddleNode*>& child,
             const unsigned int& spannedSlabs);
  const std::vector<LineSegment>& getVal() const;
  void setVal(const std::vector<LineSegment>& val);
  const std::vector<MiddleNode*>& getChild() const;
  void setChild(const std::vector<MiddleNode*>& child);

  const LineSegment getIthVal() const;
  void setIthVal(const LineSegment& lineSegment, const int& i);
  const MiddleNode* getIthChild(int& i) const;
  void setIthChild(MiddleNode& node, int& i);
  const bool underflow() const;
  const unsigned int getValSize() const;
  const LineSegment& getIthVal(const int& i) const;
  void setValSize(const unsigned int& valSize) ;
  friend std::ostream& operator<<(std::ostream& os, const MiddleNode& node);
};

std::ostream& operator<<(std::ostream& os, const MiddleNode& node);

#endif  // POINT_LOC_B_TREE_MIDDLENODE_H
