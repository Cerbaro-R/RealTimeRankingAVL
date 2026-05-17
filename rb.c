#include "rb.h"
#include <stdlib.h>
#include <stdio.h>

static RBNode* create_node(RBTree *tree, Player player) {
    RBNode *node = (RBNode*)malloc(sizeof(RBNode));
    node->player = player;
    node->parent = node->left = node->right = tree->TNULL;
    node->color = RED;
    return node;
}

static void left_rotate(RBTree *tree, RBNode *x) {
    RBNode *y = x->right;
    x->right = y->left;
    if (y->left != tree->TNULL) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == NULL) {
        tree->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

static void right_rotate(RBTree *tree, RBNode *y) {
    RBNode *x = y->left;
    y->left = x->right;
    if (x->right != tree->TNULL) {
        x->right->parent = y;
    }
    x->parent = y->parent;
    if (y->parent == NULL) {
        tree->root = x;
    } else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }
    x->right = y;
    y->parent = x;
}

static void fix_insert(RBTree *tree, RBNode *k) {
    RBNode *u;
    while (k->parent != NULL && k->parent->color == RED) {
        if (k->parent == k->parent->parent->right) {
            u = k->parent->parent->left;
            if (u->color == RED) {
                u->color = BLACK;
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                k = k->parent->parent;
            } else {
                if (k == k->parent->left) {
                    k = k->parent;
                    right_rotate(tree, k);
                }
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                left_rotate(tree, k->parent->parent);
                tree->balancing_events++;
            }
        } else {
            u = k->parent->parent->right;
            if (u->color == RED) {
                u->color = BLACK;
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                k = k->parent->parent;
            } else {
                if (k == k->parent->right) {
                    k = k->parent;
                    left_rotate(tree, k);
                }
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                right_rotate(tree, k->parent->parent);
                tree->balancing_events++;
            }
        }
        if (k == tree->root) break;
    }
    tree->root->color = BLACK;
}

void rb_init(RBTree *tree) {
    tree->TNULL = (RBNode*)malloc(sizeof(RBNode));
    tree->TNULL->color = BLACK;
    tree->TNULL->left = tree->TNULL->right = tree->TNULL->parent = NULL;
    tree->root = tree->TNULL;
    tree->balancing_events = 0;
}

void rb_insert(RBTree *tree, Player player) {
    RBNode *node = create_node(tree, player);

    RBNode *y = NULL;
    RBNode *x = tree->root;

    while (x != tree->TNULL) {
        y = x;
        if (node->player.score < x->player.score) {
            x = x->left;
        } else if (node->player.score > x->player.score) {
            x = x->right;
        } else {
            free(node);
            return;
        }
    }

    node->parent = y;
    if (y == NULL) {
        tree->root = node;
    } else if (node->player.score < y->player.score) {
        y->left = node;
    } else {
        y->right = node;
    }

    if (node->parent == NULL) {
        node->color = BLACK;
        return;
    }

    if (node->parent->parent == NULL) {
        return;
    }

    fix_insert(tree, node);
}

static void rb_transplant(RBTree *tree, RBNode *u, RBNode *v) {
    if (u->parent == NULL) {
        tree->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

static void fix_delete(RBTree *tree, RBNode *x) {
    RBNode *s;
    while (x != tree->root && x->color == BLACK) {
        if (x == x->parent->left) {
            s = x->parent->right;
            if (s->color == RED) {
                s->color = BLACK;
                x->parent->color = RED;
                left_rotate(tree, x->parent);
                tree->balancing_events++;
                s = x->parent->right;
            }

            if (s->left->color == BLACK && s->right->color == BLACK) {
                s->color = RED;
                x = x->parent;
            } else {
                if (s->right->color == BLACK) {
                    s->left->color = BLACK;
                    s->color = RED;
                    right_rotate(tree, s);
                    tree->balancing_events++;
                    s = x->parent->right;
                }

                s->color = x->parent->color;
                x->parent->color = BLACK;
                s->right->color = BLACK;
                left_rotate(tree, x->parent);
                tree->balancing_events++;
                x = tree->root;
            }
        } else {
            s = x->parent->left;
            if (s->color == RED) {
                s->color = BLACK;
                x->parent->color = RED;
                right_rotate(tree, x->parent);
                tree->balancing_events++;
                s = x->parent->left;
            }

            if (s->right->color == BLACK && s->left->color == BLACK) {
                s->color = RED;
                x = x->parent;
            } else {
                if (s->left->color == BLACK) {
                    s->right->color = BLACK;
                    s->color = RED;
                    left_rotate(tree, s);
                    tree->balancing_events++;
                    s = x->parent->left;
                }

                s->color = x->parent->color;
                x->parent->color = BLACK;
                s->left->color = BLACK;
                right_rotate(tree, x->parent);
                tree->balancing_events++;
                x = tree->root;
            }
        }
    }
    x->color = BLACK;
}

static RBNode* minimum(RBTree *tree, RBNode *node) {
    while (node->left != tree->TNULL) {
        node = node->left;
    }
    return node;
}

void rb_remove(RBTree *tree, int score) {
    RBNode *z = tree->TNULL;
    RBNode *x, *y;
    RBNode *node = tree->root;

    while (node != tree->TNULL) {
        if (node->player.score == score) {
            z = node;
            break;
        }

        if (node->player.score <= score) {
            node = node->right;
        } else {
            node = node->left;
        }
    }

    if (z == tree->TNULL) return;

    y = z;
    Color y_original_color = y->color;
    if (z->left == tree->TNULL) {
        x = z->right;
        rb_transplant(tree, z, z->right);
    } else if (z->right == tree->TNULL) {
        x = z->left;
        rb_transplant(tree, z, z->left);
    } else {
        y = minimum(tree, z->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        } else {
            rb_transplant(tree, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        rb_transplant(tree, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    free(z);
    if (y_original_color == BLACK) {
        fix_delete(tree, x);
    }
}

RBNode* rb_search(RBTree *tree, int score) {
    RBNode *current = tree->root;
    while (current != tree->TNULL) {
        if (score == current->player.score) return current;
        if (score < current->player.score) current = current->left;
        else current = current->right;
    }
    return NULL;
}

static void reverse_inorder(RBTree *tree, RBNode *node, int *count) {
    if (node == tree->TNULL || *count >= 10) return;
    reverse_inorder(tree, node->right, count);
    if (*count < 10) {
        printf("%d. %s - %d\n", ++(*count), node->player.name, node->player.score);
    }
    reverse_inorder(tree, node->left, count);
}

void rb_top_10(RBTree *tree) {
    int count = 0;
    reverse_inorder(tree, tree->root, &count);
}

static void destroy(RBTree *tree, RBNode *node) {
    if (node != tree->TNULL) {
        destroy(tree, node->left);
        destroy(tree, node->right);
        free(node);
    }
}

void rb_destroy(RBTree *tree) {
    destroy(tree, tree->root);
    free(tree->TNULL);
    tree->root = NULL;
}
