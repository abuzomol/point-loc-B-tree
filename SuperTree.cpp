//
// Created by muzamil on 1/31/20.
//

#include "SuperTree.h"

#include <cmath>

#include "MiddleTree.h"
#include "Tree.h"

using namespace std;

SuperTree::SuperTree(const int& height) { superTree.resize(height); }

SuperTree::SuperTree(const int& height, const std::vector<SuperNode*>& nodes)
{
  superTree.resize(height);
  for (auto& node : nodes)
  {
    SuperTree::superTree[height - 1].push_back(*node);
  }
  /*//check if last element is underflow
  int last = nodes.size()-1;
  if(Tree::tree[height-1].size() > 1  && Tree::tree[height -
  1][last].underflow() )
  {
    if(last >= 1 )
    {
      int mid =
          (Tree::tree[height - 1][nodes.size() - 2].getValSize() +
  Tree::tree[height - 1][last].getValSize() )/ 2;
    }
  }*/
  // go over every level in the tree
  for (int i = height - 2; i > -1; --i)
  {
    int size = SuperTree::superTree[i + 1].size() % CHILD_SIZE == 0
                   ? SuperTree::superTree[i + 1].size() / CHILD_SIZE
                   : (SuperTree::superTree[i + 1].size() / CHILD_SIZE) + 1;

    SuperTree::superTree[i].resize(size);
    // go over every node in each level
    for (int j = 0; j < SuperTree::superTree[i].size(); j++)
    {
      // set up the children
      for (int k = 0; k < CHILD_SIZE; k++)
      {
        if (j * CHILD_SIZE + k < SuperTree::superTree[i + 1].size())
        {
          SuperTree::superTree[i][j].setIthChild(
              SuperTree::superTree[i + 1][j * CHILD_SIZE + k], k);
        }
        // set up the values (skip every node with index multiple of CHILD_SIZE)
        if (k < VAL_SIZE
            && j * CHILD_SIZE + k < SuperTree::superTree[i + 1].size())
        {
          SuperTree::superTree[i][j].setIthVal(
              SuperTree::superTree[i + 1][j * CHILD_SIZE + k]
                  .getVal()[VAL_SIZE - 1],
              k);
        }
      }
    }
  }
}

// check superRoot is not null

void partitionLineSegments(SuperNode& superRoot,
                           vector<LineSegment*>& lineSegments,
                           vector<vector<LineSegment>*>* left,
                           vector<vector<LineSegment>*>* right,
                           vector<LineSegment*>* middle,
                           vector<vector<LineSegment*>*>* remainingLineSegments)
{
  for (int i = 0; i < superRoot.getValSize(); i++)
  {
    // for loop that scan every line segment
    for (auto& lineSegment : lineSegments)
    {
      // if the lineSegment crosses or touches boundary i
      if (lineSegment->getXLeft() <= superRoot.getIthVal(i)
          && lineSegment->getXRight() >= superRoot.getIthVal(i))
      {
        // to the left of first boundary
        if (i == 0 && lineSegment->getXLeft() < superRoot.getIthVal(i))
        {
          (*left)[i]->push_back(*lineSegment);
        }
        // starts at slab i-1
        if (i > 0 && superRoot.getIthVal(i - 1) < lineSegment->getXLeft()
            && lineSegment->getXLeft() < superRoot.getIthVal(i))
        {
          (*left)[i]->push_back(*lineSegment);
        }
        // to the right of last boundary

        if (i == superRoot.getValSize() - 1
            && lineSegment->getXRight() > superRoot.getIthVal(i))
        {
          (*right)[i]->push_back(*lineSegment);
        }

        // ends at slab i
        if (i < superRoot.getValSize() - 1
            && lineSegment->getXRight() < superRoot.getIthVal(i + 1)
            && lineSegment->getXRight() > superRoot.getIthVal(i))
        {
          (*right)[i]->push_back(*lineSegment);
        }
        // case lineSegment crosses slab i
        if (i < superRoot.getValSize() - 1
            && lineSegment->getXRight() >= superRoot.getIthVal(i + 1))
        {
          // check this for pointers
          if (find(middle->begin(), middle->end(), lineSegment)
              == middle->end())
          {
            middle->push_back(lineSegment);
          }
        }
      }
      // case it does not cross any boundary
      else
      {
        // case it ends before first boundary
        if (i == 0 && lineSegment->getXRight() < superRoot.getIthVal(i))
        {
          (*remainingLineSegments)[i]->push_back(lineSegment);
        }
        // case it starts after last boundary
        if (i == superRoot.getValSize() - 1
            && lineSegment->getXLeft() > superRoot.getIthVal(i))
        {
          (*remainingLineSegments)[i + 1]->push_back(lineSegment);
        }
        // case it is between two boundaries i and i +1 i.e. b_i < left x <
        // b_i+1

        if (i > 0 && lineSegment->getXLeft() > superRoot.getIthVal(i - 1)
            && lineSegment->getXRight() < superRoot.getIthVal(i))
        {
          (*remainingLineSegments)[i]->push_back(lineSegment);
        }
      }
    }
  }
}

void constructLeftTrees(SuperNode& superRoot,
                        vector<vector<LineSegment>*>* left)
{
  for (int k = 0; k < superRoot.getValSize(); k++)
  {
    // case there are some lineSegments in the set left[k]
    if (!(*left)[k]->empty())
    {
      // create the leaves nodes first
      unsigned int nodesTotal = ceil((*left)[k]->size() * 1.0 / VAL_SIZE);
      auto* nodes = new vector<Node*>(nodesTotal);

      for (int i = 0; i < nodesTotal; i++)
      {
        (*nodes)[i] = new Node();
        for (int j = 0; j < VAL_SIZE; j++)
        {
          if (i * VAL_SIZE + j < (*left)[k]->size())
          {
            (*nodes)[i]->setIthVal(*((*left)[k]->begin() + i * VAL_SIZE + j),
                                   j);
            (*nodes)[i]->setIthMinMaxX(
                ((*left)[k]->begin() + i * VAL_SIZE + j)->getXLeft(), j);
          }
        }
      }

      unsigned int height = ceil(log2(nodes->size()) / log2(CHILD_SIZE)) + 1;

      Tree* tree = new Tree(height, *nodes, true);
      // tree->Tree(*nodes, true);
      Node* root = tree->getRoot();

      superRoot.setIthLeftSemiLines(*root, k);
    }
  }
}

void constructRightTrees(SuperNode& superRoot,
                         vector<vector<LineSegment>*>* right)
{
  for (int k = 0; k < superRoot.getValSize(); k++)
  {
    // case there are some lineSegments in the set right[k]
    if (!(*right)[k]->empty())
    {
      // create the leaves nodes first
      unsigned int nodesTotal = ceil((*right)[k]->size() * 1.0 / VAL_SIZE);
      auto* nodes = new vector<Node*>(nodesTotal);

      for (int i = 0; i < nodesTotal; i++)
      {
        (*nodes)[i] = new Node();
        for (int j = 0; j < VAL_SIZE; j++)
        {
          if (i * VAL_SIZE + j < (*right)[k]->size())
          {
            (*nodes)[i]->setIthVal(*((*right)[k]->begin() + i * VAL_SIZE + j),
                                   j);
            (*nodes)[i]->setIthMinMaxX(
                ((*right)[k]->begin() + i * VAL_SIZE + j)->getXRight(), j);
          }
        }
      }
      unsigned int height = ceil(log2(nodes->size()) / log2(CHILD_SIZE)) + 1;
      Tree* tree = new Tree(height, *nodes, false);
      // tree->Tree(*nodes, true);
      Node* root = tree->getRoot();

      superRoot.setIthRightSemiLines(*root, k);
    }
  }
}

void constructMiddleTree(SuperNode& superRoot, vector<LineSegment*>* middle)
{
  unsigned int middleNodesTotal = ceil(middle->size() * 1.0 / VAL_SIZE);
  auto* middleNodes = new vector<MiddleNode*>(middleNodesTotal);

  // fill in the leaves first
  // loop every middleNode
  for (int i = 0; i < middleNodesTotal; i++)
  {
    (*middleNodes)[i] = new MiddleNode();
    unsigned int spannedSlabs = 0;
    // loop over the middleNode val
    for (int j = 0; j < VAL_SIZE; j++)
    {
      if (i * VAL_SIZE + j < middle->size())
      {
        (*middleNodes)[i]->setIthVal(*(*middle)[i * VAL_SIZE + j], j);
        // or the spannedSlabs if lineSegment crosses slab j
        if (j < VAL_SIZE - 1
            && (*middle)[i * VAL_SIZE + j]->getXLeft() <= superRoot.getIthVal(j)
            && superRoot.getIthVal(j + 1)
                   <= (*middle)[i * VAL_SIZE + j]->getXLeft())
        {
          spannedSlabs |= (1 << j);
        }
      }
    }
    (*middleNodes)[i]->setSpannedSlabs(spannedSlabs);
  }

  unsigned int height = ceil(log2(middleNodes->size()) / log2(CHILD_SIZE)) + 1;
  MiddleTree* middleTree = new MiddleTree(height, *middleNodes);
  MiddleNode* middleRoot = middleTree->getRoot();
  superRoot.setMiddle(middleRoot);
}

void fillSuperTree(SuperNode& superRoot, vector<LineSegment*>& lineSegments)
{
  auto* left = new vector<vector<LineSegment>*>(superRoot.getValSize());
  auto* right = new vector<vector<LineSegment>*>(superRoot.getValSize());
  // initialize the middle in the heap. Middle is connected to SuperNode parent.
  auto* middle = new vector<LineSegment*>();
  // initialize the remainingLineSegments that left out from left, right, and
  // middle
  auto* remainingLineSegments =
      new vector<vector<LineSegment*>*>(superRoot.getValSize() + 1);

  for (int i = 0; i < superRoot.getValSize(); i++)
  {
    (*left)[i] = new vector<LineSegment>();
    (*right)[i] = new vector<LineSegment>();
    (*remainingLineSegments)[i] = new vector<LineSegment*>();
  }

  // initialize the last child to an empty vector
  (*remainingLineSegments)[superRoot.getValSize()] = new vector<LineSegment*>();

  // loop through node values and produce, left, middle and right and
  // remainingLineSegments
  partitionLineSegments(
      superRoot, lineSegments, left, right, middle, remainingLineSegments);

  // construct the left B-trees for each value of superRoot
  constructLeftTrees(superRoot, left);
  // construct the right B-trees for each value of superRoot
  constructRightTrees(superRoot, right);
  // construct the middle B-trees
  constructMiddleTree(superRoot, middle);
  // recursive call for fill in the tree with remaining lineSegments
  for (int i = 0; i < remainingLineSegments->size(); i++)
  {
    if (!(*remainingLineSegments)[i]->empty()
        && superRoot.getIthChild(i) != nullptr)
    {
      fillSuperTree(*superRoot.getIthChild(i), *((*remainingLineSegments)[i]));
    }
  }
}
const int SuperTree::size() const { return SuperTree::superTree.size(); }
const SuperNode& SuperTree::getRoot() const { return superTree[0][0]; }

const LineSegment* pointLocationQuery(SuperNode& superRoot, Point& point)
{
  LineSegment* ans = new LineSegment();

  const LineSegment* middleSegment;
  if (superRoot.getMiddle() != nullptr)
  {
    middleSegment = pointLocationMiddle(superRoot.getMiddle(), point);
  }

  auto low = lower_bound(
      superRoot.getVal().begin(), superRoot.getVal().end(), point.getX());
  int index = low - superRoot.getVal().begin();
  cout << "\nlower bound: " << *low << " at position: " << index;
  // case at the beginning when index = 0
  if (index == 0 && superRoot.getIthLeftSemiLines(index) != nullptr)
  {
    const LineSegment* leftSegment =
        pointLocationLeft(superRoot.getIthLeftSemiLines(index), point);
    const LineSegment* segment =
        pointLocationQuery(*superRoot.getIthChild(index), point);
    if (middleSegment != nullptr)
      return &max(*leftSegment,
                  max(*middleSegment, *segment, YLeftLessThan()),
                  YLeftLessThan());
  }
  // case at the end when index == root.val
  else if (index == superRoot.getVal().size())
  {
    const LineSegment* rightSegment =
        pointLocationLeft(superRoot.getIthRightSemiLines(index), point);
    const LineSegment* segment =
        pointLocationQuery(*superRoot.getIthChild(index + 1), point);
    if (middleSegment != nullptr)
      return &max(*rightSegment,
                  max(*middleSegment, *segment, YLeftLessThan()),
                  YLeftLessThan());
  }
  // case index in the middles
  else
  {
    const LineSegment* leftSegment =
        pointLocationLeft(superRoot.getIthLeftSemiLines(index), point);
    const LineSegment* rightSegment =
        pointLocationLeft(superRoot.getIthRightSemiLines(index), point);
    const LineSegment* segment =
        pointLocationQuery(*superRoot.getIthChild(index), point);
    return &max(max(*leftSegment, *rightSegment, YLeftLessThan()),
                max(*middleSegment, *segment, YLeftLessThan()),
                YLeftLessThan());
  }
  return ans;
}
const LineSegment* pointLocationLeft(const Node* root, Point& point)
{
  return nullptr;
}
const LineSegment* pointLocationMiddle(MiddleNode* middleRoot, Point& point)
{
  return nullptr;
}
