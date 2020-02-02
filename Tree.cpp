//
// Created by muzamil on 1/21/20.
//

#include "Tree.h"

#include <algorithm>
#include <cmath>

using namespace std;

Tree::Tree(const int& height) { tree.resize(height); }

void Tree::buildBottomUpBTree(const vector<Node>& nodes, const bool &direction)
{
  int height = Tree::tree.size();
  cout << "\n height: " << height;
  Tree::tree[height - 1] = nodes;

  //check if last element is underflow
  int last = Tree::tree[height-1].size()-1;
  if( (last >= 1)  && Tree::tree[height-1][last].underflow())
  {
    int mid = ceil(Tree::tree[height - 1][last-1].getValSize() / 2.0);
    vector<LineSegment> *temp = new vector<LineSegment>() ;
    temp->resize(Tree::tree[height - 1][last-1].getValSize() - mid + Tree::tree[height - 1][last].getValSize());
    //copy the elements from previous node to last node
    for(int i = mid; i < Tree::tree[height - 1][last-1].getValSize() ; i++ )
    {
      (*temp)[i-mid] =  Tree::tree[height - 1][last-1].getIthVal(i);
    }
    //copy all elements from last node
    for(int i = 0; i < Tree::tree[height - 1][last].getValSize() ; i++ )
    {
      (*temp)[i+ Tree::tree[height - 1][last-1].getValSize() - mid ] =  Tree::tree[height - 1][last].getIthVal(i);
    }
    Tree::tree[height-1][last-1].setValSize(mid);
    Tree::tree[height-1][last].setVal(*temp);
  }
  // go over every level in the tree
  for (int i = height - 2; i > -1; --i)
  {
    int size = Tree::tree[i + 1].size() % CHILD_SIZE == 0
                   ? Tree::tree[i + 1].size() / CHILD_SIZE
                   : (Tree::tree[i + 1].size() / CHILD_SIZE) + 1;

    cout << "\nsize: " << size;
    Tree::tree[i].resize(size);
    //go over every node in each level
    for (int j = 0; j < Tree::tree[i].size(); j++)
    {
      // set up the children
      for (int k = 0; k < CHILD_SIZE; k++)
      {
        if (j * CHILD_SIZE + k < Tree::tree[i + 1].size())
        {
          Tree::tree[i][j].setIthChild(Tree::tree[i + 1][j * CHILD_SIZE + k],
                                       k);
          if (direction)
          {
            double minValue = *std::min_element(Tree::tree[i + 1][j].getMinMaxX().begin(),
                              Tree::tree[i + 1][j].getMinMaxX().end());
            Tree::tree[i][j].setIthMinMaxX(minValue, k);
          }
          else
          {
            Tree::tree[i][j].setIthMinMaxX(
                *std::max_element(Tree::tree[i + 1][j].getMinMaxX().begin(),
                                  Tree::tree[i + 1][j].getMinMaxX().end()),
                k);
          }
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
