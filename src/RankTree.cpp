#include "RankTree.hpp"
#include <iostream>

#define NIL nullptr
#define R RBNode::RED
#define B RBNode::BLACK

void RankTree::rotate_left(RBNode* x) {
    RBNode* y = x->right;
    x->right = y->left;
    if (y->left != NIL) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == NIL) {
        root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
    update_size(x);
    update_size(y);
}

void RankTree::rotate_right(RBNode* x) {
    RBNode* y = x->left;
    x->left = y->right;
    if (y->right != NIL) {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == NIL) {
        root = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else {
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
    update_size(x);
    update_size(y);
}

void RankTree::update_size(RBNode* x) {
    int left_size = x->left == NIL ? 0 : x->left->size;
    int right_size = x->right == NIL ? 0 : x->right->size;
    x->size = left_size + right_size + 1;
}

RBNode* RankTree::successor(RBNode* x) const {
    if (x == NIL) {
        return NIL;
    }
    // Case 1
    if (x->right != NIL) {
        x = x->right;
        while (x->left != NIL) {
            x = x->left;
        }
        return x;
    }
    // Case 2
    RBNode* y = x->parent;
    while (y != NIL && x == y->right) {
        x = y;
        y = y->parent;
    }
    return y;
}

RBNode* RankTree::uncle(RBNode* x) const {
    if (x->parent == NIL || x->parent->parent == NIL) {
        return NIL;
    }
    if (x->parent == x->parent->parent->left) {
        return x->parent->parent->right;
    } else {
        return x->parent->parent->left;
    }
}

void RankTree::insert(RBNode* z) {
    RBNode* y = NIL;
    RBNode* x = root;
    while (x != NIL) {
        y = x;
        if (*(z->team) < *(x->team)) {
            x = x->left;
        } else {
            x = x->right;
        }

        // DEBUG
        // puts("insert:");
        // (z->team)->print_team();
        // (y->team)->print_team();
        // std::cout << bool(*(z->team) < *(y->team)) << std::endl;
    }
    z->parent = y;
    if (y == NIL) {
        root = z;
    } else if (*(z->team) < *(y->team)) {
        y->left = z;
    } else {
        y->right = z;
    }
    z->left = NIL;
    z->right = NIL;
    z->color = R;

    insert_fixup(z);
    size_fixup(z);
    
    // DEBUG
    // print_tree();
}

void RankTree::insert_fixup(RBNode* x) {
    // Case 1
    if (x->parent == NIL) {
        x->color = B;
        return;
    }
    // Case 2
    if (x->parent->color == B) {
        return;
    }
    // below: x->parent->color == R
    // Case 3
    RBNode* u = uncle(x);
    if (u != NIL && u->color == R) {
        x->parent->color = B;
        u->color = B;
        x->parent->parent->color = R;
        insert_fixup(x->parent->parent);
        return;
    }
    // below: uncle(x)->color == B
    // Case 4
    // transform to Case 5
    if (x == x->parent->right && x->parent == x->parent->parent->left) {
        rotate_left(x->parent);
        x = x->left;
    } else if (x == x->parent->left && x->parent == x->parent->parent->right) {
        rotate_right(x->parent);
        x = x->right;
    }
    // Case 5
    x->parent->color = B;
    x->parent->parent->color = R;
    if (x == x->parent->left) {
        rotate_right(x->parent->parent);
    } else {
        rotate_left(x->parent->parent);
    }
}

RBNode* RankTree::find(Team* team) const {
    RBNode* x = root;
    while (x != NIL) {
        if (*team < *(x->team)) {
            x = x->left;
        } else if (*(x->team) < *team) {
            x = x->right;
        } else {
            return x;
        }
    }
    return NIL;
}

void RankTree::remove(RBNode* z) {
    RBNode* y = z; // actual removed node
    RBNode* x;
    if (z->left != NIL && z->right != NIL) {
        y = successor(z); // y = min(z->right)
    }

    x = y->left != NIL ? y->left : y->right;
    // if y = successor(z), y->left is NIL
    if (x != NIL) {
        x->parent = y->parent;
    } else {
        x = NIL;
    }

    if (y->parent == NIL) {
        root = x;
    } else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }

    if (y != z) {
        z->team = y->team;
    }

    if (y->color == B) {
        remove_fixup(x);
    }
    size_fixup(x);

    delete y;
}

void RankTree::remove_fixup(RBNode* x) {
    while (x != root && x->color == B) {
        if (x == x->parent->left) {
            RBNode* w = x->parent->right;

            // Case 1: w is RED -> w is BLACK
            if (w->color == R) {
                w->color = B;
                x->parent->color = R;
                rotate_left(x->parent);
                w = x->parent->right;
            }

            // Case 2: w is BLACK and both children are BLACK
            if (w->left->color == B && w->right->color == B) {
                w->color = R;
                x = x->parent; // move up
            } else {
                // Case 3: left child is red -> right child is black
                if (w->right->color == B) {
                    w->left->color = B;
                    w->color = R;
                    rotate_right(w);
                    w = x->parent->right;
                }

                // Case 4: right child is red
                w->color = x->parent->color;
                x->parent->color = B;
                w->right->color = B;
                rotate_left(x->parent);
                x = root; // end
            }
        } else { // symmetric
            RBNode* w = x->parent->left;

            if (w->color == R) {
                w->color = B;
                x->parent->color = R;
                rotate_right(x->parent);
                w = x->parent->left;
            }

            if (w->right->color == B && w->left->color == B) {
                w->color = R;
                x = x->parent;
            } else {
                if (w->left->color == B) {
                    w->right->color = B;
                    w->color = R;
                    rotate_left(w);
                    w = x->parent->left;
                }

                w->color = x->parent->color;
                x->parent->color = B;
                w->left->color = B;
                rotate_right(x->parent);
                x = root;
            }
        }
    }

    x->color = B;
}

void RankTree::size_fixup(RBNode* x) {
    while (x != NIL) {
        update_size(x);
        x = x->parent;
    }
}


int RankTree::get_rank(Team* team) const {
    RBNode* x = root;
    int rank = 1;
    while (x != NIL) {
        if (&team < &x->team) {
            x = x->left;
        } else {
            rank += x->left->size + 1;
            if (&team == &x->team) {
                return rank;
            }
            x = x->right;
        }
    }
    return -1;
}

std::vector<Team*> RankTree::get_ranklist() const {
    std::vector<Team*> ranklist;
    ranklist.reserve(root->size);
    RBNode* x = root;
    while (x != NIL) {
        if (x->left != NIL) {
            ranklist.push_back(x->left->team);
        }
        ranklist.push_back(x->team);
        x = successor(x);
    }
    return ranklist;
}

void RankTree::clear_recursive(RBNode* x) {
    if (x == NIL) {
        return;
    }
    clear_recursive(x->left);
    clear_recursive(x->right);
    delete x;
}

void RankTree::clear() {
    clear_recursive(root);
    root = NIL;
}

void RankTree::print_tree_recursive(RBNode* x, int depth) const {
    if (x == NIL) {
        return;
    }
    print_tree_recursive(x->right, depth + 1);
    for (int i = 0; i < depth; ++i) {
        std::cout << "  ";
    }
    std::cout << x->team->name << " " << (x->color == R ? "RED" : "BLACK")
        << " " << x->size << std::endl;
    print_tree_recursive(x->left, depth + 1);
}

void RankTree::print_tree() const {
    puts("RankTree:");
    print_tree_recursive(root, 0);
}

#undef NIL
#undef R
#undef B