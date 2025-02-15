#ifndef RANK_TREE_HPP
#define RANK_TREE_HPP

#include "Team.hpp"
#include <vector>

struct RBNode {
    Team* team;
    RBNode* left, *right, *parent;
    int size;
    enum COLOR { RED, BLACK };
    COLOR color;

    explicit RBNode(Team* team) :
        team(team), left(nullptr), right(nullptr),
        parent(nullptr), size(1), color(RED) {}
};

class RankTree {
private:
    RBNode* root;

    void rotate_left(RBNode* x);
    void rotate_right(RBNode* x);
    void insert_fixup(RBNode* x);
    void remove_fixup(RBNode* x);

    RBNode* uncle(RBNode* x) const;
    RBNode* successor(RBNode* x) const;
    
    void update_size(RBNode* x);
    void size_fixup(RBNode* x);

    void clear_recursive(RBNode* x);

    void print_tree_recursive(RBNode* x, int depth) const;

public:
    RankTree() : root(nullptr) {}

    RBNode* find(Team* team) const;

    void insert(RBNode* z);
    void remove(RBNode* z);

    int get_rank(Team* team) const;
    std::vector<Team*> get_ranklist() const;

    void clear();

    ~RankTree() { clear(); }

    // DEBUG
    void print_tree() const;
};

#endif // RANK_TREE_HPP