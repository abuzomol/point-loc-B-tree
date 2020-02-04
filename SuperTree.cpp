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

//check superRoot is not null
void fillSuperTree(SuperNode& superRoot,std::set<LineSegment, YLeftLessThan>& lineSegments)
{
  vector< set<LineSegment, YLeftLessThan>* >* left = new vector< set<LineSegment, YLeftLessThan>* >(superRoot.getValSize());
  vector< set<LineSegment, YLeftLessThan>* >* right = new vector< set<LineSegment, YLeftLessThan>* >(superRoot.getValSize());
  vector< set<LineSegment, YLeftLessThan>*  >* remainingLineSegments = new vector< set<LineSegment, YLeftLessThan>* >(superRoot.getValSize()+1);

  set<LineSegment, YLeftLessThan>* middle = new set<LineSegment, YLeftLessThan>();


  //for loop that scan every line segment and produce, left, middle and right structure of nodes
  for(auto it = lineSegments.begin(); it != lineSegments.end() ; it++ )
  {
    //loop through node values
    for(int i = 0; i < superRoot.getValSize(); i++)
    {
      //if the lineSegment crosses boundary i
      if (it->getXLeft() <= superRoot.getIthVal(i) && it->getXRight() > superRoot.getIthVal(i) )
      {
        //to the left of first boundary
        if(i ==0 && it->getXLeft() < superRoot.getIthVal(i))
        {
          (*left)[i]->insert(*it);
        }
        //starts at slab i-1
        if (i > 0 && it->getXLeft()  > superRoot.getIthVal(i-1) && it->getXLeft() < superRoot.getIthVal(i) )
        {
          (*left)[i]->insert(*it);
        }
        //to the right of last boundary
        if (i == superRoot.getValSize()-1 )
        {
          (*right)[i]->insert(*it);
        }
        //ends at slab i
        if (i < superRoot.getValSize()-1 && it->getXRight()  < superRoot.getIthVal(i+1) )
        {
          (*right)[i]->insert(*it);
        }
        //case lineSegment crosses a slab
        if (i < superRoot.getValSize()-1 && it->getXRight()  >= superRoot.getIthVal(i+1) )
        {
          middle->insert(*it);
        }
      }
      //case it does not cross any boundary
      else
      {
        //case it ends before first boundary
        if(i == 0 && (*it).getXRight() < superRoot.getIthVal(0))
        {
          (*remainingLineSegments)[i]->insert(*it);
        }
        //case it ends after last boundary
        if(i == superRoot.getValSize()-1 && (*it).getXRight() > superRoot.getIthVal(i))
        {
          (*remainingLineSegments)[i+1]->insert(*it);
        }
        //case it is between two boundaries i and i +1
        if (i > 0  && (*it).getXLeft() < superRoot.getIthVal(i -1 ) &&  (*it).getXRight() < superRoot.getIthVal(i) )
        {
          (*remainingLineSegments)[i]->insert(*it);
        }

      }
    }
    //case left is not empty
    if (!left->empty())
    {

    }
  }
}

const SuperNode& SuperTree::getRoot() const
{
  return superTree[0][0];
}