#include "Tree.h"

#include <algorithm>
#include <cmath>

using namespace std;

Tree::Tree(const int& height) { Tree::tree.resize(height); }

Tree::Tree(const int& height,
           const vector<LineSegment>& lineSegments,
           const bool& isLeftTree)
{
    Tree::tree.resize(height);

    // construct leaves first
    int noOfLeaves = lineSegments.size() % VAL_SIZE == 0
                     ? lineSegments.size() / VAL_SIZE
                     : (lineSegments.size() / VAL_SIZE) + 1;

    for (int i = 0; i < noOfLeaves; i++)
    {
        Node* node = new Node();
        for (int j = 0; j < VAL_SIZE; j++)
        {
            if (i * VAL_SIZE + j < lineSegments.size())
            {
                node->setIthVal(lineSegments[i * VAL_SIZE + j], j);
                if (isLeftTree)
                    node->setIthMinMaxX(lineSegments[i * VAL_SIZE + j].getXLeft(),
                                        j);
                else
                    node->setIthMinMaxX(lineSegments[i * VAL_SIZE + j].getXRight(),
                                        j);
            }
            else //set initial minMax values for left and right trees
            {
                if(isLeftTree)
                    node->setIthMinMaxX(INFTY,j);
                else
                    node->setIthMinMaxX(-1,j);
            }
        }
        //set the last child minMax values
        if(isLeftTree)
            node->setIthMinMaxX(INFTY, CHILD_SIZE-1);
        else
            node->setIthMinMaxX(-1, CHILD_SIZE-1);
        Tree::tree.back().push_back(*node);
        delete node;
    }

    if (Tree::tree.size() > 1)
    {
        // check if last element is underflow, if so merge it with neighbour
        /*int last = Tree::tree.back().size() - 1;
        if ((last >= 1) && Tree::tree.back().back().underflow())
        {
            int mid = ceil(Tree::tree.back()[last - 1].getValSize() / 2.0);
            auto* temp = new vector<LineSegment>();
            temp->resize(Tree::tree.back()[last - 1].getValSize() - mid
                         + Tree::tree.back()[last].getValSize());
            // copy the elements from previous node to last node
            for (int i = mid; i < Tree::tree.back()[last - 1].getValSize(); i++)
            {
                (*temp)[i - mid] = Tree::tree.back()[last - 1].getIthVal(i);
            }
            // copy all elements from last node
            for (int i = 0; i < Tree::tree[height - 1][last].getValSize(); i++)
            {
                (*temp)[i + Tree::tree[height - 1][last - 1].getValSize()
                        - mid] = Tree::tree[height - 1][last].getIthVal(i);
            }
            Tree::tree[height - 1][last - 1].setValSize(mid);
            Tree::tree[height - 1][last].setVal(*temp);
        }*/
        // go over every level in the tree
        for (int i = height - 2; i > -1; --i)
        {
            int size = Tree::tree[i + 1].size() % CHILD_SIZE == 0
                       ? Tree::tree[i + 1].size() / CHILD_SIZE
                       : (Tree::tree[i + 1].size() / CHILD_SIZE) + 1;

            Tree::tree[i].resize(size);
            // go over every node in each level
            for (int j = 0; j < Tree::tree[i].size(); j++)
            {
                // set up the children
                for (int k = 0; k < CHILD_SIZE; k++)
                {
                    if (j * CHILD_SIZE + k < Tree::tree[i + 1].size())
                    {
                        Tree::tree[i][j].setIthChild(
                            Tree::tree[i + 1][j * CHILD_SIZE + k], k);
                        if (isLeftTree)
                        {
                            double minValue = *std::min_element(
                                Tree::tree[i + 1][j].getMinMaxX().begin(),
                                Tree::tree[i + 1][j].getMinMaxX().end());
                            Tree::tree[i][j].setIthMinMaxX(minValue, k);
                        }
                        else
                        {
                            double maxValue = *std::max_element(
                                Tree::tree[i + 1][j].getMinMaxX().begin(),
                                Tree::tree[i + 1][j].getMinMaxX().end());
                            Tree::tree[i][j].setIthMinMaxX(maxValue, k);
                        }
                    }
                    else
                    {
                        if (isLeftTree)
                        {
                            Tree::tree[i][j].setIthMinMaxX(INFTY, k);
                        }
                        else
                        {
                            Tree::tree[i][j].setIthMinMaxX(-1, k);
                        }
                    }
                    // set up the values (skip every node with index multiple of
                    // CHILD_SIZE)
                    if (k < VAL_SIZE
                        && j * CHILD_SIZE + k < Tree::tree[i + 1].size())
                    {
                        Tree::tree[i][j].setIthVal(
                            Tree::tree[i + 1][j * CHILD_SIZE + k]
                                .getVal()
                                .back(),
                            k);
                    }
                }
            }
        }
    }
}

Node* Tree::getRoot() { return &(Tree::tree[0][0]); }