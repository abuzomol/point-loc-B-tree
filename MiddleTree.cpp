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

    MiddleTree::tree.back().reserve(noOfLeaves);
    for (int i = 0; i < MiddleTree::tree.capacity() - 1; i++)
    {
        vector<LineSegment> values;
        values.reserve(VAL_SIZE);
        unsigned int spannedSlabs = 0;

        for (int j = 0; j < VAL_SIZE; j++)
        {
            values.emplace_back(lineSegments[i * VAL_SIZE + j]);
            // set up the slab flag x_start <= b_i <= b_{i+1} <= x_end
            // TODO() make this more efficient based on indices.
            if (lineSegments[i * VAL_SIZE + j].getXLeft()
                    <= superRoot.getIthVal(j)
                && superRoot.getIthVal(j + 1)
                       <= lineSegments[i * VAL_SIZE + j].getXRight())
            {
                spannedSlabs |= (1 << j);
            }
        }
        MiddleTree::tree.back().emplace_back(values, spannedSlabs);
    }

    // fill the last node
    vector<LineSegment> lastValues;
    unsigned int lastLeafSpannedSlabs;
    lastValues.reserve(VAL_SIZE);
    int t;
    for (t = 0; (noOfLeaves - 1) * VAL_SIZE + t < lineSegments.size(); t++)
    {
        lastValues.emplace_back(lineSegments[(noOfLeaves - 1) * VAL_SIZE + t]);
        if (lineSegments[(noOfLeaves - 1) * VAL_SIZE + t].getXLeft()
                <= superRoot.getIthVal(t)
            && superRoot.getIthVal(t + 1)
                   <= lineSegments[(noOfLeaves - 1) * VAL_SIZE + t].getXRight())
        {
            lastLeafSpannedSlabs |= (1 << t);
        }
    }
    // pad last node with infinities if not full
    for (t; t < VAL_SIZE; t++)
    {
        lastValues.emplace_back();
    }
    MiddleTree::tree.back().emplace_back(lastValues, lastLeafSpannedSlabs);

    if (MiddleTree::tree.size() > 1)
    {
        // go over every level in the tree, and elevate the maximum val to the
        // next level
        for (int i = height - 2; i > -1; --i)
        {
            int size = MiddleTree::tree[i + 1].size() % CHILD_SIZE == 0
                           ? MiddleTree::tree[i + 1].size() / CHILD_SIZE
                           : (MiddleTree::tree[i + 1].size() / CHILD_SIZE) + 1;

            MiddleTree::tree[i].reserve(size);
            // go over every node in each level except the last one
            for (int j = 0; j < MiddleTree::tree[i].capacity() - 1; j++)
            {
                // set up the children
                unsigned int spannedSlabs = 0;
                vector<LineSegment> parent;
                vector<MiddleNode*> child;
                parent.reserve(VAL_SIZE);
                child.reserve(CHILD_SIZE);

                for (int k = 0; k < VAL_SIZE; k++)
                {
                    parent.emplace_back(
                        MiddleTree::tree[i + 1][j * CHILD_SIZE + k]
                            .getVal()
                            .back());
                    child.emplace_back(
                        &MiddleTree::tree[i + 1][j * CHILD_SIZE + k]);
                    // Do the OR here
                    spannedSlabs |= MiddleTree::tree[i + 1][j * CHILD_SIZE + k]
                                        .getSpannedSlabs();
                }
                // add last child
                child.emplace_back(
                    &MiddleTree::tree[i + 1][j * CHILD_SIZE + VAL_SIZE]);
                MiddleTree::tree[i].emplace_back(parent, child, spannedSlabs);
            }
            // fill the last node
            vector<LineSegment> lastParent;
            vector<MiddleNode*> lastChild;
            lastParent.reserve(VAL_SIZE);
            lastChild.reserve(CHILD_SIZE);
            unsigned int lastSpannedSlabs = 0;

            int s = 0;
            for (s; (MiddleTree::tree[i].capacity() - 1) * CHILD_SIZE + s
                        < MiddleTree::tree[i + 1].size()
                    && s < VAL_SIZE;
                 s++)
            {
                if ((MiddleTree::tree[i].capacity() - 1) * CHILD_SIZE + s

                    < MiddleTree::tree[i + 1].capacity())
                {
                    lastParent.emplace_back(
                        MiddleTree::tree[i + 1]
                                        [(MiddleTree::tree[i].capacity() - 1)
                                             * CHILD_SIZE
                                         + s]
                                            .getIthVal(VAL_SIZE - 1));
                }
                else
                {
                    lastParent.emplace_back(INFTY, -1, INFTY, INFTY);
                }
                lastSpannedSlabs |=
                    MiddleTree::tree[i + 1][(MiddleTree::tree[i].capacity() - 1)
                                                * CHILD_SIZE
                                            + s]
                        .getSpannedSlabs();
                lastChild.emplace_back(
                    &MiddleTree::tree[i + 1]
                                     [(MiddleTree::tree[i].capacity() - 1)
                                          * CHILD_SIZE
                                      + s]);
            }
            if ((MiddleTree::tree[i].capacity() - 1) * CHILD_SIZE + s
                < MiddleTree::tree[i + 1].capacity())
            {
                lastChild.emplace_back(
                    &MiddleTree::tree[i + 1]
                                     [(MiddleTree::tree[i].capacity() - 1)
                                          * CHILD_SIZE
                                      + s]);
                lastSpannedSlabs =
                    MiddleTree::tree[i + 1][(MiddleTree::tree[i].capacity() - 1)
                                                * CHILD_SIZE
                                            + s]
                        .getSpannedSlabs();
            }
            else
            {
                lastChild.emplace_back(nullptr);
            }
            // pad last node with infinities if not full
            // TODO() fix the binary version. Error on setting the children.
            for (s; s < VAL_SIZE; s++)
            {
                lastParent.emplace_back(INFTY,-1, INFTY, INFTY);
                lastChild.emplace_back(nullptr);
                if (s == VAL_SIZE - 1) lastChild.emplace_back(nullptr);
            }

            MiddleTree::tree[i].emplace_back(lastParent, lastChild, lastSpannedSlabs);
        }
    }
}

MiddleNode* MiddleTree::getRoot() { return &MiddleTree::tree[0][0]; }