#ifndef JUMBLE_NPUZZLE_HPP_INCLUDED
#define JUMBLE_NPUZZLE_HPP_INCLUDED

#include "jumble/hash_table.hpp"
#include "jumble/binary_heap.hpp"
#include "jumble/util/random.hpp"
#include <vector>
#include <sstream>
#include <string>
#include <list>
#include <utility>

JUMBLE_NAMESPACE_BEGIN

enum Direc {
    NONE,
    LEFT,
    UP,
    RIGHT,
    DOWN
};

class NPuzzle;

class PuzzleNode {
    friend class NPuzzle;

public:
    using Container = std::vector<int>;
    using SizeType = Container::size_type;

    /*
    Initialize the node. The node value is stored in an one-dimension array.
    For a 3*3 puzzle, the contents of the array must be like this:
    {1, 2, 3, 4, 5, 6, 7, 8, 0}, where number '0' indicates the the empty grid
    and number '1-8' denote other eight grids. The configuration is similar when
    the puzzle's size is 4*4, 5*5, etc.

    @param val_ The node value
    @param row_ The row amount
    @param col_ The column amount
    */
    PuzzleNode(const Container& val_, const SizeType row_, const SizeType col_)
        : val(val_), row(row_), col(col_) {
        for (SizeType i = 0; i < val.size(); ++i) {
            if (val[i] == 0) {
                emptyPos = i;
            }
        }
    }

    // Return the amount of rows
    SizeType getRowCount() const noexcept {
        return row;
    }

    // Return the amount of columns
    SizeType getColCount() const noexcept {
        return col;
    }

    // Return true if the empty grid can move one step along a given direction
    bool canMove(const Direc d) const noexcept {
        switch (d) {
            case LEFT:
                return getCol(emptyPos) != 0;
            case UP:
                return getRow(emptyPos) != 0;
            case RIGHT:
                return getCol(emptyPos) != col - 1;
            case DOWN:
                return getRow(emptyPos) != row - 1;
            case NONE:
                return true;
            default:
                return false;
        }
    }

    // Move the empty grid along a given direction
    void move(const Direc d) {
        SizeType goalPos = emptyPos;
        switch (d) {
            case LEFT:
                goalPos -= 1;
                break;
            case UP:
                goalPos -= col;
                break;
            case RIGHT:
                goalPos += 1;
                break;
            case DOWN:
                goalPos += col;
                break;
            default:
                break;
        }
        if (emptyPos != goalPos) {
            std::swap(val[emptyPos], val[goalPos]);
            emptyPos = goalPos;
        }
    }

    // Randomly rearrange the node value (ensuring a solution exists)
    void shuffle() {
        Random *random = Random::getInstance();
        for (SizeType i = 0; i < SHUFFLE_TIMES; ++i) {
            Direc d = Direc(random->nextInt(1, 4));
            if (canMove(d)) {
                move(d);
            }
        }
    }

    /*
    Return a string description of the node value.
    */
    std::string toString() const {
        std::ostringstream oss;
        oss << "{";
        for (SizeType i = 0; i < val.size(); ++i) {
            if (i) oss << ", ";
            oss << val[i];
        }
        oss << "}";
        return oss.str();
    }

    /*
    Return the direction of this node relelative to a given adjacent node.

    @param n The given adjacent node
    */
    Direc getDirectionTo(const PuzzleNode *const n) const noexcept {
        if (n) {
            int offset = (int)emptyPos - (int)n->emptyPos;
            if (offset == -1) {
                return LEFT;
            } else if (offset == 1) {
                return RIGHT;
            } else if (offset == (int)col) {
                return DOWN;
            } else if (offset == -(int)col) {
                return UP;
            }
        }
        return NONE;
    }

    bool operator==(const PuzzleNode& n) const noexcept {
        return getVal() == n.getVal();
    }

    bool operator<(const PuzzleNode& n) const noexcept {
        return getF() < n.getF();
    }

private:
    // Return the node value
    const Container& getVal() const noexcept {
        return val;
    }

    // Return the hash value of the node
    SizeType hash() const {
        return std::hash<std::string>()(toString());
    }

    /*
    Compute the row number according to a given index of one-dimension array.

    @param i The given index
    @return  The row number in interval [0, row - 1]
    */
    SizeType getRow(const SizeType i) const noexcept {
        return i / col;
    }

    /*
    Compute the column number according to a given index of one-dimension array.

    @param i The given index
    @return  The row number in interval [0, col - 1]
    */
    SizeType getCol(const SizeType i) const noexcept {
        return i % col;
    }

    /*
    Return the adjacent node at a given direction.
    Precondition: the current node can move along the given direction

    @param d The given direction
    */
    PuzzleNode* getNeighbor(const Direc d) const {
        PuzzleNode* n = new PuzzleNode(*this);
        n->move(d);
        return n;
    }

    /*
    Return the heuristic value to the destination node.

    @param des   The destination node
    @param index The index of each number in the destination node value array
    */
    SizeType heuristic(const PuzzleNode *const des, const std::vector<SizeType>& index) const {
        SizeType size = val.size();

        // Number of nodes which are in a wrong position
        SizeType wrong = 0;
        for (SizeType i = 0; i < size; ++i) {
            if (val[i] != des->val[i]) {
                ++wrong;
            }
        }

        // Distance to the destination
        SizeType manhatten = 0, geometric = 0;
        for (SizeType i = 0; i < size; ++i) {
            if (val[i]) {  // Escape value 0
                SizeType curR = getRow(i);
                SizeType curC = getCol(i);
                SizeType desR = getRow(index[val[i]]);
                SizeType desC = getCol(index[val[i]]);
                SizeType dR = curR > desR ? curR - desR : desR - curR;
                SizeType dC = curC > desC ? curC - desC : desC - curC;
                manhatten += dR + dC;
                geometric += (SizeType)(sqrt(dR * dR + dC * dC));
            }
        }

        return 5 * (1 * wrong + 2 * manhatten + 1 * geometric);
    }

    void setG(const SizeType g_) noexcept {
        g = g_;
    }

    void setH(const SizeType h_) noexcept {
        h = h_;
    }

    void setParent(PuzzleNode* p) noexcept {
        parent = p;
    }

    SizeType getG() const noexcept {
        return g;
    }

    SizeType getH() const noexcept {
        return h;
    }

    SizeType getF() const noexcept {
        return getG() + getH();
    }

    const PuzzleNode* getParent() const {
        return parent;
    }

private:
    static const SizeType SHUFFLE_TIMES = 1000;

    Container val;

    SizeType emptyPos = 0;
    SizeType row = 0;
    SizeType col = 0;

    SizeType g = 0;
    SizeType h = 0;
    PuzzleNode *parent = nullptr;
};


class NPuzzle {
public:
    using Node = PuzzleNode;
    using SizeType = Node::SizeType;

    NPuzzle(const Node& beg_, const Node& end_)
        : beg(beg_), end(end_), closeList(1000000) {}

    void solve() {
        std::vector<SizeType> index = buildIndex();
        searchCnt = 0;
        openList.push(&beg);
        while (!openList.isEmpty()) {
            Node *cur = nullptr;
            do {
                // Loop until the open list is empty or a node
                // that is not in the close list is found.
                cur = openList.top();
                openList.pop();
            } while (!openList.isEmpty() && closeList.has(cur));
            if (openList.isEmpty() && closeList.has(cur)) {
                // If all the nodes in the open list is in the
                // close list, then there is no available path
                // between the two nodes.
                return;
            }
            ++searchCnt;
            closeList.insert(cur);
            if (*cur == end) {
                buildPath(cur);
                freeResources();
                return;
            }
            for (int i = 1; i < 5; ++i) {
                Direc d = Direc(i);
                if (cur->canMove(d)) {
                    Node *adj = cur->getNeighbor(d);
                    alloc.push_back(adj);
                    if (!closeList.has(adj)) {
                        adj->setParent(cur);
                        adj->setG(cur->getG() + 1);
                        adj->setH(adj->heuristic(&end, index));
                        openList.push(adj);
                    }
                }
            }
        }
    }

    const std::list<Direc>& getPath() const {
        return path;
    }

    SizeType getSearchCount() const {
        return searchCnt;
    }

    const Node& shuffleBeg() {
        beg.shuffle();
        return beg;
    }

    const Node& shuffleEnd() {
        end.shuffle();
        return end;
    }

    void setBeg(const Node& beg_) {
        beg = beg_;
    }

    void setEnd(const Node& end_) {
        end = end_;
    }

    const Node& getBeg() const {
        return beg;
    }

    const Node& getEnd() const {
        return end;
    }

private:
    // Return the index of the grid numbers in the ending node value.
    std::vector<SizeType> buildIndex() const {
        const auto &endVal = end.getVal();
        std::vector<SizeType> index(endVal.size());
        for (SizeType i = 0; i < endVal.size(); ++i) {
            index[endVal[i]] = i;
        }
        return index;
    }

    // Build a path from the beginning node to the destination node
    void buildPath(const Node* des) {
        path.clear();
        while (des) {
            auto parent = des->getParent();
            Direc d = des->getDirectionTo(parent);
            if (d != Direc::NONE) {
                path.push_front(d);
            }
            des = parent;
        }
    }

    void freeResources() {
        for (const auto &n : alloc) {
            delete n;
        }
        alloc.clear();
        openList.clear();
        closeList.clear();
    }

private:
    struct Equal {
        bool operator()(const Node *const &n1,
                        const Node *const &n2) const noexcept {
            return *n1 == *n2;
        }
    };

    struct Less {
        bool operator()(const Node *const &n1,
                        const Node *const &n2) const noexcept {
            return *n1 < *n2;
        }
    };

    struct Hash {
        SizeType operator()(const Node *const &n) const noexcept {
            return n->hash();
        }
    };

    Node beg;
    Node end;

    BinaryHeap<Node*, Less> openList;
    HashTable<Node*, Equal, Hash> closeList;

    std::list<Direc> path;
    std::list<Node*> alloc;

    SizeType searchCnt = 0;
};

JUMBLE_NAMESPACE_END

#endif
