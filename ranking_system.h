#ifndef RANKING_SYSTEM_H
#define RANKING_SYSTEM_H

#include "avl.h"
#include "rb.h"

// Secondary index for name lookup
typedef struct NameNode {
    Player player;
    struct NameNode *left, *right;
    int height;
} NameNode;

typedef struct {
    NameNode *root;
} NameIndex;

typedef struct {
    AVLTree score_tree_avl;
    RBTree score_tree_rb;
    NameIndex name_index;
} RankingSystem;

void ranking_init(RankingSystem *rs);
void ranking_update(RankingSystem *rs, Player player);
void ranking_remove(RankingSystem *rs, char *name);
Player* ranking_search_by_name(RankingSystem *rs, char *name);
void ranking_display_top_10(RankingSystem *rs, int use_rb);
void ranking_destroy(RankingSystem *rs);

#endif // RANKING_SYSTEM_H
