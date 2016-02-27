/* KPCC
 * TreeNode is a 4-nary tree that represents a celluar automaton
 * Author: Safin Karim
 * Date: 2016.02.27
 */

#include <memory>
#include <unordered_map>
#include <QPainter>

#include "treenode.h"

using namespace std;

TreeNode::TreeNode()
{
    nw = ne = sw = se = nullptr;
    level = 0;
    alive = false;
    population = 0;
}

TreeNode::TreeNode(bool living)
{
    nw = ne = sw = se = nullptr;
    level = 0;
    alive = living;
    population = alive ? 1 : 0;
}

/**
 * @brief construct a node of four children
 * @param Northwestern node
 * @param Northeastern node
 * @param Southwestern node
 * @param Southeastern node
 */
TreeNode::TreeNode(shared_ptr<TreeNode> nw,
                   shared_ptr<TreeNode> ne,
                   shared_ptr<TreeNode> sw,
                   shared_ptr<TreeNode> se)
{
    this->nw = nw;
    this->ne = ne;
    this->sw = sw;
    this->se = se;
    this->level = nw->level + 1;
    population = nw->population +
                 ne->population +
                 sw->population +
                 se->population;
    alive = population > 0;
}

/**
 * @brief Sets a certain cell of a tree to 1 ( -level ^ 2 <= x, y < level ^ 2)
 * @param -level ^ 2 <= x < level ^ 2
 * @param -level ^ 2 <= y < level ^ 2
 * @return A TreeNode with a bit set
 */
shared_ptr<TreeNode> TreeNode::setBit(int x, int y)
{
    if(level == 0)
    {
        return make_shared<TreeNode>(true);
    }
    int offset = 1 << (level - 2); // pow(2, level - 2);
                                   // distance from center of this node to center of subnode is
                                   // one fourth the size of this node->
    if (x < 0)
    {
        if (y < 0)
        {
            return make_shared<TreeNode>(nw->setBit(x + offset, y + offset), ne, sw, se);
        }
        else
        {
            return make_shared<TreeNode>(nw, ne, sw->setBit(x + offset, y - offset), se);
        }
    }
    else
    {
        if (y < 0)
        {
            return make_shared<TreeNode>(nw, ne->setBit(x - offset, y + offset), sw, se);
        }
        else
        {
            return make_shared<TreeNode>(nw, ne, sw, se->setBit(x - offset, y - offset));
        }
    }
}

/**
 * @brief Sets a certain cell of a tree to 0 ( -level ^ 2 <= x, y < level ^ 2)
 * @param -level ^ 2 <= x < level ^ 2
 * @param -level ^ 2 <= y < level ^ 2
 * @return A TreeNode with a bit set
 */
shared_ptr<TreeNode> TreeNode::unsetBit(int x, int y)
{
    if(level == 0)
    {
        return make_shared<TreeNode>(false);
    }
    int offset = 1 << (level - 2); // pow(2, level - 2);
                                   // distance from center of this node to center of subnode is
                                   // one fourth the size of this node->
    if (x < 0)
    {
        if (y < 0)
        {
            return make_shared<TreeNode>(nw->unsetBit(x + offset, y + offset), ne, sw, se);
        }
        else
        {
            return make_shared<TreeNode>(nw, ne, sw->unsetBit(x + offset, y - offset), se);
        }
    }
    else
    {
        if (y < 0)
        {
            return make_shared<TreeNode>(nw, ne->unsetBit(x - offset, y + offset), sw, se);
        }
        else
        {
            return make_shared<TreeNode>(nw, ne, sw, se->unsetBit(x - offset, y - offset));
        }
    }
}

/**
 * @brief If a certain bit is alive
 * @param -level ^ 2 <= x < level ^ 2
 * @param -level ^ 2 <= y < level ^ 2
 * @return 1 if alive, 0 if dead
 */
int TreeNode::getBit(int x, int y)

{
    if (level == 0)
    {
        return alive ? 1 : 0;
    }
    int offset = 1 << (level - 2); // pow(2, level - 2);
    if (x < 0)
    {
        if (y < 0)
        {
            return nw->getBit(x + offset, y + offset);
        }
        else
        {
            return sw->getBit(x + offset, y - offset);
        }
    }
    else
    {
        if (y < 0)
        {
            return ne->getBit(x - offset, y + offset);
        }
        else
        {
            return se->getBit(x - offset, y - offset);
        }
    }
}

/**
 * @brief Builds an empty tree of a certain level
 * @param level
 * @return Empty tree of a certain level
 */
shared_ptr<TreeNode> TreeNode::emptyTree(int level)
{
    if(level == 0)
    {
        return make_shared<TreeNode>(false);
    }
    shared_ptr<TreeNode> n = emptyTree(level - 1);
    return make_shared<TreeNode>(n, n, n, n);
}

/**
 * @brief Builds a tree one level higher than this with this in the center
 * @return A tree one level higher than this with this in the center
 */
shared_ptr<TreeNode> TreeNode::expandUniverse()
{
    shared_ptr<TreeNode> border = emptyTree(level - 1);
    return make_shared<TreeNode>(make_shared<TreeNode>(border, border, border, nw),
                                 make_shared<TreeNode>(border, border, ne, border),
                                 make_shared<TreeNode>(border, sw, border, border),
                                 make_shared<TreeNode>(se, border, border, border));
}

/**
*   Given an integer with a bitmask indicating which bits are
*   set in the neighborhood, calculate whether this cell is
*   alive or dead in the next generation.  The bottom three
*   bits are the south neighbors; bits 4..6 are the current
*   row with bit 5 being the cell itself, and bits 8..10
*   are the north neighbors.
*/
shared_ptr<TreeNode> TreeNode::oneGen(int bitmask)
{
    if (bitmask == 0)
    {
        return make_shared<TreeNode>(false);
    }
    int self = (bitmask >> 5) & 1;
    bitmask &= 0x757; // mask out bits we don't care about
    int neighborCount = 0 ;
    while (bitmask != 0)
    {
        neighborCount++;
        bitmask &= bitmask - 1; // clear least significant bit
    }
    if (neighborCount == 3 || (neighborCount == 2 && self != 0))
    {
        return make_shared<TreeNode>(true);
    }
    else
    {
        return make_shared<TreeNode>(false);
    }
}

/**
*   At level 2, we can use slow simulation to compute the next
*   generation.  We use bitmask tricks.
*/
shared_ptr<TreeNode> TreeNode::slowSimulation()
{
    int allbits = 0;
    for (int y =- 2; y < 2; y++)
    {
        for (int x =- 2; x < 2; x++)
        {
            allbits = (allbits << 1) + getBit(x, y);
        }
    }
    return make_shared<TreeNode>(oneGen(allbits >> 5), oneGen(allbits >> 4),
                                 oneGen(allbits >> 1), oneGen(allbits));
}

/**
*   Return a new node one level down containing only the
*   center elements.
*/
shared_ptr<TreeNode> TreeNode::centeredSubnode()
{
    return make_shared<TreeNode>(nw->se, ne->sw, sw->ne, se->nw);
}

/**
*   Return a new node one level down from two given nodes
*   that contains the east centered two sub sub nodes from
*   the west node and the west centered two sub sub nodes
*   from the east node.
*/
shared_ptr<TreeNode> TreeNode::centeredHorizontal(shared_ptr<TreeNode> w, shared_ptr<TreeNode> e)
{
    return make_shared<TreeNode>(w->ne->se, e->nw->sw, w->se->ne, e->sw->nw);
}

/**
*   Similar, but this does it north/south instead of east/west.
*/
shared_ptr<TreeNode> TreeNode::centeredVertical(shared_ptr<TreeNode> n, shared_ptr<TreeNode> s)
{
    return make_shared<TreeNode>(n->sw->se, n->se->sw, s->nw->ne, s->ne->nw);
}

/**
*   Return a new node two levels down containing only the
*   centered elements.
*/
shared_ptr<TreeNode> TreeNode::centeredSubSubnode()
{
    return make_shared<TreeNode>(nw->se->se, ne->sw->sw, sw->ne->ne, se->nw->nw);
}


/**
*   The recursive call that computes the next generation.  It works
*   by constructing nine subnodes that are each a quarter the size
*   of the current node in each dimension, and combining these in
*   groups of four, building subnodes from these, and then
*   recursively invoking the nextGeneration function and combining
*   those final results into a single return value that is one
*   half the size of the current node and advanced one generation in
*   time.
*/
shared_ptr<TreeNode> TreeNode::nextGeneration()
{
    /*try
    {
        return make_shared<TreeNode>(hashMap.at(*make_shared<TreeNode>(this)));
    }
    catch(const out_of_range& oor)
    {*/
        // skip empty regions quickly
        if (population == 0)
        {
            return nw;
        }
        if (level == 2)
        {
            return /*make_shared<TreeNode>(hashMap[*make_shared<TreeNode>(this)] = **/slowSimulation()/*)*/;
        }
        shared_ptr<TreeNode> n00 = nw->centeredSubnode(),
        n01 = centeredHorizontal(nw, ne),
        n02 = ne->centeredSubnode(),
        n10 = centeredVertical(nw, sw),
        n11 = centeredSubSubnode(),
        n12 = centeredVertical(ne, se),
        n20 = sw->centeredSubnode(),
        n21 = centeredHorizontal(sw, se),
        n22 = se->centeredSubnode();

        return /*make_shared<TreeNode>(hashMap[*make_shared<TreeNode>(this)] = **/make_shared<TreeNode>(
                                       make_shared<TreeNode>(n00, n01, n10, n11)->nextGeneration(),
                                       make_shared<TreeNode>(n01, n02, n11, n12)->nextGeneration(),
                                       make_shared<TreeNode>(n10, n11, n20, n21)->nextGeneration(),
                                       make_shared<TreeNode>(n11, n12, n21, n22)->nextGeneration())/*)*/;
    /*}*/
}

void TreeNode::recDraw(QPainter* painter, int x0, int y0, int width)
{
    if(this->level == 1)
    {
        if(nw->population != 0)
        {
            painter->drawRect(x0 - width / 2, y0 - width / 2, width / 2, width / 2);
        }
        if(ne->population != 0)
        {
            painter->drawRect(x0,             y0 - width / 2, width / 2, width / 2);
        }
        if(sw->population != 0)
        {
            painter->drawRect(x0 - width / 2, y0,             width / 2, width / 2);
        }
        if(se->population != 0)
        {
            painter->drawRect(x0,             y0,             width / 2, width / 2);
        }
    }
    else
    {
        if(nw->population != 0)
        {
            nw->recDraw(painter, x0 - width / 4, y0 - width / 4, width / 2);
        }
        if(ne->population != 0)
        {
            ne->recDraw(painter, x0 + width / 4, y0 - width / 4, width / 2);
        }
        if(sw->population != 0)
        {
            sw->recDraw(painter, x0 - width / 4, y0 + width / 4, width / 2);
        }
        if(se->population != 0)
        {
            se->recDraw(painter, x0 + width / 4, y0 + width / 4, width / 2);
        }
    }
}

/**
 * @brief Rotates a TreeNode clockwise
 * @return this, rotated clockwise
 */
shared_ptr<TreeNode> TreeNode::rotateClockwise()
{
    if(this->level == 0) // a leaf node
    {
        return make_shared<TreeNode>(this); // Then a rotation has no effect. We usially do not rotate leaf nodes
    }
    if(this->level == 1)
    {
        return make_shared<TreeNode>(sw, nw, se, ne);
    }
    else
    {
        return make_shared<TreeNode>(sw->rotateClockwise(),
                                     nw->rotateClockwise(),
                                     se->rotateClockwise(),
                                     ne->rotateClockwise());
    }
}

/**
 * @brief Rotates a TreeNode anticlockwise
 * @return this, rotated anticlockwise
 */
shared_ptr<TreeNode> TreeNode::rotateAntiClockwise()
{
    if(this->level == 0) // a leaf node
    {
        return make_shared<TreeNode>(this); // Then a rotation has no effect. We usially do not rotate leaf nodes
    }
    if(this->level == 1)
    {
        return make_shared<TreeNode>(ne, se, nw, sw);
    }
    else
    {
        return make_shared<TreeNode>(ne->rotateAntiClockwise(),
                                     se->rotateAntiClockwise(),
                                     nw->rotateAntiClockwise(),
                                     sw->rotateAntiClockwise());
    }
}

/**
 * @brief Returns northwestern square of this tree
 * @return
 */
shared_ptr<TreeNode> TreeNode::getnw() const
{
    return nw;
}

/**
 * @brief Returns northeastern square of this tree
 * @return
 */
shared_ptr<TreeNode> TreeNode::getne() const
{
    return ne;
}

/**
 * @brief Returns southwestern square of this tree
 * @return
 */
shared_ptr<TreeNode> TreeNode::getsw() const
{
    return sw;
}

/**
 * @brief Returns southeastern square of this tree
 * @return
 */
shared_ptr<TreeNode> TreeNode::getse() const
{
    return se;
}

/**
 * @brief Returns levelof this tree
 * @return
 */
int TreeNode::getLevel() const
{
    return level;
}

/**
 * @brief Returns population of this tree
 * @return
 */
long TreeNode::getPopulation() const
{
    return population;
}

/**
 * @brief If this node is a leaf, is it alive?
 * If it is nonleaf, has it any living cells?
 * @return
 */
bool TreeNode::isAlive() const
{
    return alive;
}

size_t hash_func(const TreeNode& t)
{
    if(t.getLevel() == 0)
    {
        return t.getPopulation();
    }
    else
    {
        return hash_func(*t.getnw()) +
               11 * hash_func(*t.getne()) +
               101 * hash_func(*t.getsw()) +
               1007 * hash_func(*t.getse());
    }
}


bool equals(const TreeNode& arg1, const TreeNode& arg2)
{
    if(arg1.getLevel() != arg2.getLevel())
    {
        return false;
    }
    if(arg1.getLevel() == 0)
    {
        return arg1.isAlive() == arg2.isAlive();
    }
    return equals(*arg1.getnw(), *arg2.getnw()) &&
           equals(*arg1.getne(), *arg2.getne()) &&
           equals(*arg1.getsw(), *arg2.getsw()) &&
           equals(*arg1.getse(), *arg2.getse());

}
