//
// Created by muzamil on 1/31/20.
//

#include "SuperTree.h"
using namespace std;

SuperTree::SuperTree(const int& height)
{
  superTree.resize(height);
}
void SuperTree::buildBottomUpBTree(const std::vector<SuperNode>& nodes)
{
  int height = SuperTree::superTree.size();
  cout << "\n height: " << height;
  SuperTree::superTree[height - 1] = nodes;

  /*//check if last element is underflow
  int last = nodes.size()-1;
  if(Tree::tree[height-1].size() > 1  && Tree::tree[height - 1][last].underflow() )
  {
    if(last >= 1 )
    {
      int mid =
          (Tree::tree[height - 1][nodes.size() - 2].getValSize() + Tree::tree[height - 1][last].getValSize() )/ 2;
    }
  }*/
  // go over every level in the tree
  for (int i = height - 2; i > -1; --i)
  {
    int size = SuperTree::superTree[i + 1].size() % CHILD_SIZE == 0
               ? SuperTree::superTree[i + 1].size() / CHILD_SIZE
               : (SuperTree::superTree[i + 1].size() / CHILD_SIZE) + 1;

    cout << "\nsize: " << size;
    SuperTree::superTree[i].resize(size);
    //go over every node in each level
    for (int j = 0; j < SuperTree::superTree[i].size(); j++)
    {
      // set up the children
      for (int k = 0; k < CHILD_SIZE; k++)
      {
        if (j * CHILD_SIZE + k < SuperTree::superTree[i + 1].size())
        {
          SuperTree::superTree[i][j].setIthChild(SuperTree::superTree[i + 1][j * CHILD_SIZE + k],
                                   k);

        }
        // set up the values (skip every node with index multiple of CHILD_SIZE)
        if (k < VAL_SIZE && j * CHILD_SIZE + k < SuperTree::superTree[i + 1].size())
        {
          SuperTree::superTree[i][j].setIthVal(
              SuperTree::superTree[i + 1][j * CHILD_SIZE + k].getVal()[VAL_SIZE - 1], k);
        }
      }
    }
  }
}

void fillSuperTree(std::set<LineSegment, YLeftLessThan>& lineSegments)
{

}

const SuperNode& SuperTree::getRoot() const
{
  return superTree[0][0];
}