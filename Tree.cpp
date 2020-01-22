//
// Created by muzamil on 1/21/20.
//

#include "Tree.h"

#include <algorithm>
using namespace std;

Tree::Tree(const int& height) { tree.resize(height); }

void Tree::buildBottomUpBTree(const vector<Node>& nodes)
{
  int height = Tree::tree.size();
  cout << "\n height: " << height;
  Tree::tree[height - 1] = nodes;

  for (int i = height - 2; i > -1; --i)
  {
    int size = Tree::tree[i + 1].size() % CHILD_SIZE == 0
                   ? Tree::tree[i + 1].size() / CHILD_SIZE
                   : (Tree::tree[i + 1].size() / CHILD_SIZE) + 1;

    cout << "\nsize: " << size;
    Tree::tree[i].resize(size);
    for (int j = 0; j < Tree::tree[i].size(); j++)
    {
      // set up the children
      for (int k = 0; k < CHILD_SIZE; k++)
      {
        if (j * CHILD_SIZE + k < Tree::tree[i + 1].size())
        {
          Tree::tree[i][j].setIthChild(Tree::tree[i + 1][j * CHILD_SIZE + k],
                                       k);
          Tree::tree[i][j].setIthMinMaxX(
              *std::min_element(Tree::tree[i + 1][j].getMinMaxX().begin(),
                       Tree::tree[i + 1][j].getMinMaxX().end()),
              k);
        }
        // set up the values (skip every node with index multiple of CHILD_SIZE)
        if (k < VAL_SIZE && j * CHILD_SIZE + k < Tree::tree[i + 1].size())
        {
          Tree::tree[i][j].setIthVal(
              Tree::tree[i + 1][j * CHILD_SIZE + k].getVal()[VAL_SIZE - 1], k);
        }
      }
    }
  }
}
const Node& Tree::getRoot() const { return Tree::tree[0][0]; }
