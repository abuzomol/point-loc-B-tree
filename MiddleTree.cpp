//
// Created by muzamil on 2/12/20.
//

#include "MiddleTree.h"

#include <algorithm>
#include <cmath>

using namespace std;

MiddleTree::MiddleTree(const int& height) { tree.resize(height); }

MiddleTree::MiddleTree(const int& height,
                       const vector<int>& val,
                       const vector<MiddleNode>& nodes)
{
  MiddleTree::tree[height - 1] = nodes;

  if (MiddleTree::tree.size() > 1)
  {
    // check if last element is underflow
    int last = MiddleTree::tree[height - 1].size() - 1;
    if ((last >= 1) && MiddleTree::tree[height - 1][last].underflow())
    {
      int mid = ceil(MiddleTree::tree[height - 1][last - 1].getValSize() / 2.0);
      auto* temp = new vector<LineSegment>();
      temp->resize(MiddleTree::tree[height - 1][last - 1].getValSize() - mid
                   + MiddleTree::tree[height - 1][last].getValSize());
      // copy the elements from previous node to last node
      for (int i = mid; i < MiddleTree::tree[height - 1][last - 1].getValSize();
           i++)
      {
        (*temp)[i - mid] = MiddleTree::tree[height - 1][last - 1].getIthVal(i);
      }
      // copy all elements from last node
      for (int i = 0; i < MiddleTree::tree[height - 1][last].getValSize(); i++)
      {
        (*temp)[i + MiddleTree::tree[height - 1][last - 1].getValSize() - mid] =
            MiddleTree::tree[height - 1][last].getIthVal(i);
      }
      MiddleTree::tree[height - 1][last - 1].setValSize(mid);
      MiddleTree::tree[height - 1][last].setVal(*temp);
    }
    // go over every level in the tree
    for (int i = height - 2; i > -1; --i)
    {
      int size = MiddleTree::tree[i + 1].size() % CHILD_SIZE == 0
                     ? MiddleTree::tree[i + 1].size() / CHILD_SIZE
                     : (MiddleTree::tree[i + 1].size() / CHILD_SIZE) + 1;

      MiddleTree::tree[i].resize(size);
      // go over every node in each level
      for (int j = 0; j < MiddleTree::tree[i].size(); j++)
      {
        // set up the children
        for (int k = 0; k < CHILD_SIZE; k++)
        {
          if (j * CHILD_SIZE + k < MiddleTree::tree[i + 1].size())
          {
            MiddleTree::tree[i][j].setIthChild(
                MiddleTree::tree[i + 1][j * CHILD_SIZE + k], k);
            // DO XOR HERE
          }
          // set up the values (skip every node with index multiple of
          // CHILD_SIZE)
          if (k < VAL_SIZE
              && j * CHILD_SIZE + k < MiddleTree::tree[i + 1].size())
          {
            MiddleTree::tree[i][j].setIthVal(
                MiddleTree::tree[i + 1][j * CHILD_SIZE + k]
                    .getVal()[VAL_SIZE - 1],
                k);
          }
        }
      }
    }
  }
}

const MiddleNode& MiddleTree::getRoot() const { return MiddleTree::tree[0][0]; }