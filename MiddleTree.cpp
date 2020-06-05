//
// Created by muzamil on 2/12/20.
//
#include "MiddleTree.h"

#include <algorithm>
#include <cmath>

using namespace std;

MiddleTree::MiddleTree(const int& height) { tree.resize(height); }

/* Constructor that build a tree bottom up of line segments that crosses one or
 more slabs. A slab s_i is defined as real line interval [b_i, b_{i+1}] for i =
 1, ... , ValSize - 1. The middleNode maintain a bit vector of size ValSize - 1
 to indicate if there is a line segment that crosses slab s_i
 */
MiddleTree::MiddleTree(const unsigned int& height,
                       SuperNode& superRoot,
                       const std::vector<LineSegment>& lineSegments)
{
    MiddleTree::tree.resize(height);
    // construct leaves first
    int noOfLeaves = lineSegments.size() % VAL_SIZE == 0
                     ? lineSegments.size() / VAL_SIZE
                     : (lineSegments.size() / VAL_SIZE) + 1;
    for (int i = 0; i < noOfLeaves; i++)
    {
        MiddleNode* node = new MiddleNode();
        unsigned int spannedSlabs = 0;

        for (int j = 0; j < VAL_SIZE; j++)
        {
            if (i * VAL_SIZE + j < lineSegments.size())
            {
                node->setIthVal(lineSegments[i * VAL_SIZE + j], j);
                //set up the slab flag x_start <= b_i <= b_{i+1} <= x_end
                if (j < VAL_SIZE - 1
                    && lineSegments[i * VAL_SIZE + j].getXLeft()
                           <= superRoot.getIthVal(j)
                    && superRoot.getIthVal(j + 1)
                           <= lineSegments[i * VAL_SIZE + j].getXRight())
                {
                    spannedSlabs |= (1 << j);
                }
                node->setSpannedSlabs(spannedSlabs);
            }
        }
        MiddleTree::tree.back().push_back(*node);
    }
    //TODO() change height of tree
    if (MiddleTree::tree.size() > 1)
    {
        // check if last element is underflow. If so then merge it with its
        // neighbour
        /*int last = MiddleTree::tree[height - 1].size() - 1;
        if ((last >= 1) && MiddleTree::tree[height - 1][last].underflow())
        {
            int mid =
                ceil(MiddleTree::tree[height - 1][last - 1].getValSize() / 2.0);
            auto* temp = new vector<LineSegment>();
            temp->resize(MiddleTree::tree[height - 1][last - 1].getValSize()
                         - mid
                         + MiddleTree::tree[height - 1][last].getValSize());
            // copy the elements from previous node to last node
            for (int i = mid;
                 i < MiddleTree::tree[height - 1][last - 1].getValSize();
                 i++)
            {
                (*temp)[i - mid] =
                    MiddleTree::tree[height - 1][last - 1].getIthVal(i);
            }
            // copy all elements from last node
            for (int i = 0; i < MiddleTree::tree[height - 1][last].getValSize();
                 i++)
            {
                (*temp)[i + MiddleTree::tree[height - 1][last - 1].getValSize()
                        - mid] =
                    MiddleTree::tree[height - 1][last].getIthVal(i);
            }
            MiddleTree::tree[height - 1][last - 1].setValSize(mid);
            MiddleTree::tree[height - 1][last].setVal(*temp);
        }*/
        // go over every level in the tree, and elevate the maximum val to the
        // next level
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
                unsigned int spannedslabs = 0;

                for (int k = 0; k < CHILD_SIZE; k++)
                {
                    if (j * CHILD_SIZE + k < MiddleTree::tree[i + 1].size())
                    {
                        MiddleTree::tree[i][j].setIthChild(
                            MiddleTree::tree[i + 1][j * CHILD_SIZE + k], k);
                        // Do the OR here
                        spannedslabs |=
                            MiddleTree::tree[i + 1][j * CHILD_SIZE + k]
                                .getSpannedSlabs();
                    }
                    // set up the values (skip every node with index multiple of
                    // CHILD_SIZE)
                    if (k < VAL_SIZE
                        && j * CHILD_SIZE + k < MiddleTree::tree[i + 1].size())
                    {
                        MiddleTree::tree[i][j].setIthVal(
                            MiddleTree::tree[i + 1][j * CHILD_SIZE + k]
                                .getVal().back(),
                            k);
                    }
                }
                MiddleTree::tree[i][j].setSpannedSlabs(spannedslabs);
            }
        }
    }
}

MiddleNode* MiddleTree::getRoot() { return &MiddleTree::tree[0][0]; }