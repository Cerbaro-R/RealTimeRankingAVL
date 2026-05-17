#ifndef RB_H
#define RB_H

#include "player.h"

typedef enum { RED, BLACK } Color;

typedef struct RBNode {
    Player player;
    Color color;
    struct RBNode *left, *right, *parent;
} RBNode;

typedef struct {
    RBNode *root;
    RBNode *TNULL;
    long balancing_events;
} RBTree;

void rb_init(RBTree *tree);
void rb_insert(RBTree *tree, Player player);
void rb_remove(RBTree *tree, int score);
RBNode* rb_search(RBTree *tree, int score);
void rb_top_10(RBTree *tree);
void rb_destroy(RBTree *tree);

#endif // RB_H
