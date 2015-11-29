#ifndef TREENODE_H
#define TREENODE_H

#include <unordered_map>
#include <QPainter>

#include <iostream>
using namespace std;

class TreeNode;

static size_t hash_func(TreeNode* t);

static bool equals(TreeNode* arg1, TreeNode* arg2);

//stores nextGeneration values
static unordered_map<TreeNode*, TreeNode*, decltype(&hash_func), decltype(&equals)> hashMap(100, hash_func, equals);

class TreeNode
{
public:

    /**
     * @brief construct a leaf cell
     * @param is it alive
     */
    TreeNode(bool living)
    {
        nw = ne = sw = se = nullptr;
        level = 0;
        alive = living;
        population = alive ? 1 : 0;
        bitView = 0;
    }

    /**
     * @brief construct a node of four children
     * @param Northwestern node
     * @param Northeastern node
     * @param Southwestern node
     * @param Southeastern node
     */
    TreeNode(TreeNode *nw, TreeNode *ne, TreeNode *sw, TreeNode *se)
    {
        this->bitView = 0;
        this->nw = nw;
        this->ne = ne;
        this->sw = sw;
        this->se = se;
        this->level = nw->level + 1 ;
        if(this->level == 2) //then we set a bitview
        {
            // First row
            this->bitView |= nw->nw->population << 0;
            this->bitView |= nw->ne->population << 1;
            this->bitView |= ne->nw->population << 2;
            this->bitView |= ne->ne->population << 3;

            // Second row
            this->bitView |= nw->sw->population << 4;
            this->bitView |= nw->se->population << 5;
            this->bitView |= ne->sw->population << 6;
            this->bitView |= ne->se->population << 7;

            // Third row
            this->bitView |= sw->nw->population << 8;
            this->bitView |= sw->ne->population << 9;
            this->bitView |= se->nw->population << 10;
            this->bitView |= se->ne->population << 11;

            // Fourth row
            this->bitView |= sw->sw->population << 12;
            this->bitView |= sw->se->population << 13;
            this->bitView |= se->sw->population << 14;
            this->bitView |= se->se->population << 15;
        }
        population = nw->population +
                     ne->population +
                     sw->population +
                     se->population;
        alive = population > 0;
    }

    /**
     * @brief construct a node of four children
     * @param Northwestern node
     * @param Northeastern node
     * @param Southwestern node
     * @param Southeastern node
     */
    TreeNode(TreeNode *nw, TreeNode *ne, TreeNode *sw, TreeNode *se, unsigned short bitView)
    {
        this->bitView = 0;
        this->nw = nw;
        this->ne = ne;
        this->sw = sw;
        this->se = se;
        this->level = nw->level + 1;
        this->bitView = bitView;
        population = nw->population +
                     ne->population +
                     sw->population +
                     se->population;
        alive = population > 0;
    }

    /**
    * Factory methods to allow us to "override" the constructors.
    * These two calls are the only places that the constructors
    * should ever be called.  The first two are nonstatic member
    * functions only so they can be overriden; they do not actually use
    * the self class at all.  The third provides a mechanism for us
    * to bootstrap the root.
    */
    TreeNode* create(bool living)
    {
        return new TreeNode(living);
    }

    TreeNode* create(TreeNode *nw, TreeNode *ne, TreeNode *sw, TreeNode *se)
    {
        return new TreeNode(nw, ne, sw, se);
    }

    TreeNode* create(TreeNode *nw, TreeNode *ne, TreeNode *sw, TreeNode *se, unsigned short bitView)
    {
        return new TreeNode(nw, ne, sw, se, bitView);
    }

    static TreeNode* create()
    {
        return new TreeNode(false);
    }

    /**
     * @brief Sets a certain cell of a tree to 1 ( -level ^ 2 <= x, y < level ^ 2)
     * @param -level ^ 2 <= x < level ^ 2
     * @param -level ^ 2 <= y < level ^ 2
     * @return A TreeNode with a bit set
     */
    TreeNode* setBit(int x, int y)
    {
        if(level == 0)
        {
            return new TreeNode(true);
        }
        int offset = 1 << (level - 2); // pow(2, level - 2);
                                       // distance from center of this node to center of subnode is
                                       // one fourth the size of this node->
        if (x < 0)
        {
            if (y < 0)
            {
                return create(nw->setBit(x + offset, y + offset), ne, sw, se);
            }
            else
            {
                return create(nw, ne, sw->setBit(x + offset, y - offset), se);
            }
        }
        else
        {
            if (y < 0)
            {
                return create(nw, ne->setBit(x - offset, y + offset), sw, se);
            }
            else
            {
                return create(nw, ne, sw, se->setBit(x - offset, y - offset));
            }
        }
    }

    /**
     * @brief Sets a certain cell of a tree to 0 ( -level ^ 2 <= x, y < level ^ 2)
     * @param -level ^ 2 <= x < level ^ 2
     * @param -level ^ 2 <= y < level ^ 2
     * @return A TreeNode with a bit set
     */
    TreeNode* unsetBit(int x, int y)
    {
        if(level == 0)
        {
            return new TreeNode(false);
        }
        int offset = 1 << (level - 2); // pow(2, level - 2);
                                       // distance from center of this node to center of subnode is
                                       // one fourth the size of this node->
        if (x < 0)
        {
            if (y < 0)
            {
                return create(nw->unsetBit(x + offset, y + offset), ne, sw, se);
            }
            else
            {
                return create(nw, ne, sw->unsetBit(x + offset, y - offset), se);
            }
        }
        else
        {
            if (y < 0)
            {
                return create(nw, ne->unsetBit(x - offset, y + offset), sw, se);
            }
            else
            {
                return create(nw, ne, sw, se->unsetBit(x - offset, y - offset));
            }
        }
    }

    /**
     * @brief If a certain bit is alive
     * @param -level ^ 2 <= x < level ^ 2
     * @param -level ^ 2 <= y < level ^ 2
     * @return 1 if alive, 0 if dead
     */
    int getBit(int x, int y)

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
    TreeNode* emptyTree(int level)
    {
        if(level == 0)
        {
            return create(false);
        }
        TreeNode* n = emptyTree(level - 1);
        return create(n, n, n, n);
    }
    /**
     * @brief Builds a tree one level higher than this with this in the center
     * @return A tree one level higher than this with this in the center
     */
    TreeNode* expandUniverse()
    {
        TreeNode* border = emptyTree(level - 1);
        return create(create(border, border, border, nw),
                      create(border, border, ne, border),
                      create(border, sw, border, border),
                      create(se, border, border, border));
    }

    /**
    *   Given an integer with a bitmask indicating which bits are
    *   set in the neighborhood, calculate whether this cell is
    *   alive or dead in the next generation.  The bottom three
    *   bits are the south neighbors; bits 4..6 are the current
    *   row with bit 5 being the cell itself, and bits 8..10
    *   are the north neighbors.
    */
    TreeNode* oneGen(int bitmask)
    {
        if (bitmask == 0)
        {
            return create(false);
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
            return create(true);
        }
        else
        {
            return create(false);
        }
    }

    /**
    *   At level 2, we can use slow simulation to compute the next
    *   generation.  We use bitmask tricks.
    */
    TreeNode* slowSimulation()
    {
        int allbits = 0;
        for (int y =- 2; y < 2; y++)
        {
            for (int x =- 2; x < 2; x++)
            {
                allbits = (allbits << 1) + getBit(x, y);
            }
        }
        return create(oneGen(allbits >> 5), oneGen(allbits >> 4),
                      oneGen(allbits >> 1), oneGen(allbits));
    }

    /**
    *   Return a new node one level down containing only the
    *   center elements.
    */
    TreeNode* centeredSubnode()
    {
        return create(nw->se, ne->sw, sw->ne, se->nw);
    }

    /**
    *   Return a new node one level down from two given nodes
    *   that contains the east centered two sub sub nodes from
    *   the west node and the west centered two sub sub nodes
    *   from the east node.
    */
    TreeNode* centeredHorizontal(TreeNode *w, TreeNode *e)
    {
        return create(w->ne->se, e->nw->sw, w->se->ne, e->sw->nw);
    }

    /**
    *   Similar, but this does it north/south instead of east/west.
    */
    TreeNode* centeredVertical(TreeNode *n, TreeNode *s)
    {
        return create(n->sw->se, n->se->sw, s->nw->ne, s->ne->nw);
    }

    /**
    *   Return a new node two levels down containing only the
    *   centered elements.
    */
    TreeNode* centeredSubSubnode()
    {
        return create(nw->se->se, ne->sw->sw, sw->ne->ne, se->nw->nw);
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
    TreeNode* nextGeneration()
    {
        /*try
        {
            return hashMap.at(this);
        }
        catch(const out_of_range& oor)
        {*/
            // skip empty regions quickly
            if (population == 0)
            {
                return nw; // bitView is already 0
            }
            if (level == 2)
            {
                // slowSimulation returns a treenode level 1, therefore no bitview
                return /*hashMap[this] = */slowSimulation();
            }
            TreeNode *n00 = nw->centeredSubnode(),
            *n01 = centeredHorizontal(nw, ne),
            *n02 = ne->centeredSubnode(),
            *n10 = centeredVertical(nw, sw),
            *n11 = centeredSubSubnode(),
            *n12 = centeredVertical(ne, se),
            *n20 = sw->centeredSubnode(),
            *n21 = centeredHorizontal(sw, se),
            *n22 = se->centeredSubnode();

            return /*hashMap[this] = */create(create(n00, n01, n10, n11)->nextGeneration(),
                                          create(n01, n02, n11, n12)->nextGeneration(),
                                          create(n10, n11, n20, n21)->nextGeneration(),
                                          create(n11, n12, n21, n22)->nextGeneration())->setBitMask();
        /*}*/
    }

    void recDraw(QPainter* painter, int x0, int y0, int width)
    {
        if(this->level <= 2)
        {
            unsigned short bv = this->bitView;
            for(int i = 0; i < 16 && bv != 0; i++)
            {
                if(bv % 2 == 1)
                {
                    painter->drawRect(x0 + (i % 4) * width / 4, y0 + (i / 4) * width / 4, width / 4, width / 4);
                }
                bv /= 2;
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
    TreeNode* rotateClockwise()
    {
        if(this->level == 0) // a leaf node
        {
            return this; // Then a rotation has no effect. We usially do not rotate leaf nodes
        }
        if(this->level == 1)
        {
            return create(sw, nw, se, ne);
        }
        else
        {
            return create(sw->rotateClockwise(),
                          nw->rotateClockwise(),
                          se->rotateClockwise(),
                          ne->rotateClockwise());
        }
    }

    /**
     * @brief Rotates a TreeNode anticlockwise
     * @return this, rotated anticlockwise
     */
    TreeNode* rotateAntiClockwise()
    {
        if(this->level == 0) // a leaf node
        {
            return this; // Then a rotation has no effect. We usially do not rotate leaf nodes
        }
        if(this->level == 1)
        {
            return create(ne, se, nw, sw);
        }
        else
        {
            return create(ne->rotateAntiClockwise(),
                          se->rotateAntiClockwise(),
                          nw->rotateAntiClockwise(),
                          sw->rotateAntiClockwise());
        }
    }

    /**
     * @brief Returns northwestern square of this tree
     * @return
     */
    TreeNode* getnw() const
    {
        return nw;
    }

    /**
     * @brief Returns northeastern square of this tree
     * @return
     */
    TreeNode* getne() const
    {
        return ne;
    }

    /**
     * @brief Returns southwestern square of this tree
     * @return
     */
    TreeNode* getsw() const
    {
        return sw;
    }

    /**
     * @brief Returns southeastern square of this tree
     * @return
     */
    TreeNode* getse() const
    {
        return se;
    }

    /**
     * @brief Returns levelof this tree
     * @return
     */
    int getLevel() const
    {
        return level;
    }

    /**
     * @brief Returns population of this tree
     * @return
     */
    long getPopulation() const
    {
        return population;
    }

    /**
     * @brief If this node is a leaf, is it alive?
     * If it is nonleaf, has it any living cells?
     * @return
     */
    bool isAlive() const
    {
        return alive;
    }

    unsigned short getBitView() const
    {
        return bitView;
    }

private:

    TreeNode *nw, *ne, *sw, *se; //children
    int level; //distance to the root
    long population; //number of living cells
    unsigned short bitView; //exactly 16 bits to represent a tree of level 3
    bool alive; //if this is a leaf node, is it alive?
                //if nonleaf, does it have any living cells?

    // If this tree node has level 2, then it has bitmask and we calculate it. If not level 2, we do not care
    TreeNode* setBitMask()
    {
        if(this->level == 2)
        {
            unsigned short bv = 0;
            // First row
            bv |= nw->nw->population << 0;
            bv |= nw->ne->population << 1;
            bv |= ne->nw->population << 2;
            bv |= ne->ne->population << 3;

            // Second row
            bv |= nw->sw->population << 4;
            bv |= nw->se->population << 5;
            bv |= ne->sw->population << 6;
            bv |= ne->se->population << 7;

            // Third row
            bv |= sw->nw->population << 8;
            bv |= sw->ne->population << 9;
            bv |= se->nw->population << 10;
            bv |= se->ne->population << 11;

            // Fourth row
            bv |= sw->sw->population << 12;
            bv |= sw->se->population << 13;
            bv |= se->sw->population << 14;
            bv |= se->se->population << 15;

            return create(nw, ne, sw, se, bv);
        }
        else
        {
            return this;
        }
    }
};


static size_t hash_func(TreeNode* t)
{
    if(t->getLevel() == 0)
    {
        return t->getPopulation();
    }
    else
    {
        return hash_func(t->getnw()) +
               11 * hash_func(t->getne()) +
               101 * hash_func(t->getsw()) +
               1007 * hash_func(t->getse());
    }
}


bool equals(TreeNode* arg1, TreeNode* arg2)
{
    if(arg1->getLevel() != arg2->getLevel())
    {
        return false;
    }
    if(arg1->getLevel() == 0)
    {
        return arg1->isAlive() == arg2->isAlive();
    }
    return equals(arg1->getnw(), arg2->getnw()) &&
           equals(arg1->getne(), arg2->getne()) &&
           equals(arg1->getsw(), arg2->getsw()) &&
           equals(arg1->getse(), arg2->getse());

}

#endif // TREENODE_H
