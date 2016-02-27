/* KPCC
 * TreeNode is a 4-nary tree that represents a celluar automaton
 * Author: Safin Karim
 * Date: 2015.10.01
 */

#ifndef TREENODE_H
#define TREENODE_H

#include <memory>
#include <unordered_map>
#include <QPainter>

using namespace std;

class TreeNode
{
public:
    /**
     * @brief construct a dead leaf cell
     */
    TreeNode();

    /**
     * @brief construct a leaf cell
     * @param is it alive
     */
    TreeNode(bool living);

    /**
     * @brief construct a node of four children
     * @param Northwestern node
     * @param Northeastern node
     * @param Southwestern node
     * @param Southeastern node
     */
    TreeNode(shared_ptr<TreeNode> nw,
             shared_ptr<TreeNode> ne,
             shared_ptr<TreeNode> sw,
             shared_ptr<TreeNode> se);

    /**
     * @brief Sets a certain cell of a tree to 1 ( -level ^ 2 <= x, y < level ^ 2)
     * @param -level ^ 2 <= x < level ^ 2
     * @param -level ^ 2 <= y < level ^ 2
     * @return A TreeNode with a bit set
     */
    shared_ptr<TreeNode> setBit(int x, int y);

    /**
     * @brief Sets a certain cell of a tree to 0 ( -level ^ 2 <= x, y < level ^ 2)
     * @param -level ^ 2 <= x < level ^ 2
     * @param -level ^ 2 <= y < level ^ 2
     * @return A TreeNode with a bit set
     */
    shared_ptr<TreeNode> unsetBit(int x, int y);

    /**
     * @brief If a certain bit is alive
     * @param -level ^ 2 <= x < level ^ 2
     * @param -level ^ 2 <= y < level ^ 2
     * @return 1 if alive, 0 if dead
     */
    int getBit(int x, int y);

    /**
     * @brief Builds an empty tree of a certain level
     * @param level
     * @return Empty tree of a certain level
     */
    shared_ptr<TreeNode> emptyTree(int level);

    /**
     * @brief Builds a tree one level higher than this with this in the center
     * @return A tree one level higher than this with this in the center
     */
    shared_ptr<TreeNode> expandUniverse();

    /**
    *   Given an integer with a bitmask indicating which bits are
    *   set in the neighborhood, calculate whether this cell is
    *   alive or dead in the next generation.  The bottom three
    *   bits are the south neighbors; bits 4..6 are the current
    *   row with bit 5 being the cell itself, and bits 8..10
    *   are the north neighbors.
    */
    shared_ptr<TreeNode> oneGen(int bitmask);

    /**
    *   At level 2, we can use slow simulation to compute the next
    *   generation.  We use bitmask tricks.
    */
    shared_ptr<TreeNode> slowSimulation();

    /**
    *   Return a new node one level down containing only the
    *   center elements.
    */
    shared_ptr<TreeNode> centeredSubnode();

    /**
    *   Return a new node one level down from two given nodes
    *   that contains the east centered two sub sub nodes from
    *   the west node and the west centered two sub sub nodes
    *   from the east node.
    */
    shared_ptr<TreeNode> centeredHorizontal(shared_ptr<TreeNode> w, shared_ptr<TreeNode> e);

    /**
    *   Similar, but this does it north/south instead of east/west.
    */
    shared_ptr<TreeNode> centeredVertical(shared_ptr<TreeNode> n, shared_ptr<TreeNode> s);

    /**
    *   Return a new node two levels down containing only the
    *   centered elements.
    */
    shared_ptr<TreeNode> centeredSubSubnode();


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
    shared_ptr<TreeNode> nextGeneration();

    void recDraw(QPainter* painter, int x0, int y0, int width);

    /**
     * @brief Rotates a TreeNode clockwise
     * @return this, rotated clockwise
     */
    shared_ptr<TreeNode> rotateClockwise();

    /**
     * @brief Rotates a TreeNode anticlockwise
     * @return this, rotated anticlockwise
     */
    shared_ptr<TreeNode> rotateAntiClockwise();

    /**
     * @brief Returns northwestern square of this tree
     * @return
     */
    shared_ptr<TreeNode> getnw() const;

    /**
     * @brief Returns northeastern square of this tree
     * @return
     */
    shared_ptr<TreeNode> getne() const;

    /**
     * @brief Returns southwestern square of this tree
     * @return
     */
    shared_ptr<TreeNode> getsw() const;

    /**
     * @brief Returns southeastern square of this tree
     * @return
     */
    shared_ptr<TreeNode> getse() const;

    /**
     * @brief Returns the level of this tree
     */
    int getLevel() const;

    /**
     * @brief Returns population of this tree
     * @return
     */
    long getPopulation() const;

    /**
     * @brief If this node is a leaf, is it alive?
     * If it is nonleaf, has it any living cells?
     */
    bool isAlive() const;

private:

    shared_ptr<TreeNode> nw, ne, sw, se; //children
    int level; //distance to the root
    long population; //number of living cells
    bool alive; //if this is a leaf node, is it alive?
                //if nonleaf, does it have any living cells?
};


// Hash function for TreeNode
size_t hash_func(const TreeNode& t);

// Function that compares two TreeNodes by their content
bool equals(const TreeNode &arg1, const TreeNode& arg2);

//stores nextGeneration values
static unordered_map<TreeNode, TreeNode, decltype(&hash_func), decltype(&equals)> hashMap(100, hash_func, equals);

#endif // TREENODE_H
