#include "ranking_system.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// --- Name Index Implementation (Internal AVL keyed by name) ---
static int max(int a, int b) { return (a > b) ? a : b; }
static int get_height(NameNode *n) { return n ? n->height : 0; }
static int get_balance(NameNode *n) { return n ? get_height(n->left) - get_height(n->right) : 0; }

static NameNode* rotate_right(NameNode *y) {
    NameNode *x = y->left;
    NameNode *T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(get_height(y->left), get_height(y->right)) + 1;
    x->height = max(get_height(x->left), get_height(x->right)) + 1;
    return x;
}

static NameNode* rotate_left(NameNode *x) {
    NameNode *y = x->right;
    NameNode *T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(get_height(x->left), get_height(x->right)) + 1;
    y->height = max(get_height(y->left), get_height(y->right)) + 1;
    return y;
}

static NameNode* name_insert(NameNode *node, Player player) {
    if (!node) {
        NameNode *n = (NameNode*)malloc(sizeof(NameNode));
        n->player = player;
        n->left = n->right = NULL;
        n->height = 1;
        return n;
    }
    int cmp = strcmp(player.name, node->player.name);
    if (cmp < 0) node->left = name_insert(node->left, player);
    else if (cmp > 0) node->right = name_insert(node->right, player);
    else { node->player = player; return node; }

    node->height = 1 + max(get_height(node->left), get_height(node->right));
    int balance = get_balance(node);
    if (balance > 1 && strcmp(player.name, node->left->player.name) < 0) return rotate_right(node);
    if (balance < -1 && strcmp(player.name, node->right->player.name) > 0) return rotate_left(node);
    if (balance > 1 && strcmp(player.name, node->left->player.name) > 0) {
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }
    if (balance < -1 && strcmp(player.name, node->right->player.name) < 0) {
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }
    return node;
}

static NameNode* min_node(NameNode *n) {
    while (n->left) n = n->left;
    return n;
}

static NameNode* name_remove(NameNode *root, char *name) {
    if (!root) return root;
    int cmp = strcmp(name, root->player.name);
    if (cmp < 0) root->left = name_remove(root->left, name);
    else if (cmp > 0) root->right = name_remove(root->right, name);
    else {
        if (!root->left || !root->right) {
            NameNode *temp = root->left ? root->left : root->right;
            if (!temp) { temp = root; root = NULL; }
            else *root = *temp;
            free(temp);
        } else {
            NameNode *temp = min_node(root->right);
            root->player = temp->player;
            root->right = name_remove(root->right, temp->player.name);
        }
    }
    if (!root) return root;
    root->height = 1 + max(get_height(root->left), get_height(root->right));
    int balance = get_balance(root);
    if (balance > 1 && get_balance(root->left) >= 0) return rotate_right(root);
    if (balance > 1 && get_balance(root->left) < 0) {
        root->left = rotate_left(root->left);
        return rotate_right(root);
    }
    if (balance < -1 && get_balance(root->right) <= 0) return rotate_left(root);
    if (balance < -1 && get_balance(root->right) > 0) {
        root->right = rotate_right(root->right);
        return rotate_left(root);
    }
    return root;
}

static NameNode* name_search(NameNode *n, char *name) {
    if (!n) return NULL;
    int cmp = strcmp(name, n->player.name);
    if (cmp == 0) return n;
    if (cmp < 0) return name_search(n->left, name);
    return name_search(n->right, name);
}

static void name_destroy(NameNode *n) {
    if (n) { name_destroy(n->left); name_destroy(n->right); free(n); }
}

// --- Ranking System Public API ---

void ranking_init(RankingSystem *rs) {
    avl_init(&rs->score_tree_avl);
    rb_init(&rs->score_tree_rb);
    rs->name_index.root = NULL;
}

void ranking_update(RankingSystem *rs, Player player) {
    // If player exists, remove old score from score trees
    NameNode *found = name_search(rs->name_index.root, player.name);
    if (found) {
        avl_remove(&rs->score_tree_avl, found->player.score);
        rb_remove(&rs->score_tree_rb, found->player.score);
    }
    
    // Insert/Update in name index
    rs->name_index.root = name_insert(rs->name_index.root, player);
    
    // Insert into score trees
    avl_insert(&rs->score_tree_avl, player);
    rb_insert(&rs->score_tree_rb, player);
}

void ranking_remove(RankingSystem *rs, char *name) {
    NameNode *found = name_search(rs->name_index.root, name);
    if (found) {
        avl_remove(&rs->score_tree_avl, found->player.score);
        rb_remove(&rs->score_tree_rb, found->player.score);
        rs->name_index.root = name_remove(rs->name_index.root, name);
    }
}

Player* ranking_search_by_name(RankingSystem *rs, char *name) {
    NameNode *found = name_search(rs->name_index.root, name);
    return found ? &found->player : NULL;
}

void ranking_display_top_10(RankingSystem *rs, int use_rb) {
    printf("\n--- TOP 10 RANKING (%s) ---\n", use_rb ? "Red-Black" : "AVL");
    if (use_rb) rb_top_10(&rs->score_tree_rb);
    else avl_top_10(&rs->score_tree_avl);
    printf("---------------------------\n");
}

void ranking_destroy(RankingSystem *rs) {
    avl_destroy(&rs->score_tree_avl);
    rb_destroy(&rs->score_tree_rb);
    name_destroy(rs->name_index.root);
}
