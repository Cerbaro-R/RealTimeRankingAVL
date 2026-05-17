#include "performance.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

void run_performance_test(int num_operations) {
    AVLTree avl;
    RBTree rb;
    avl_init(&avl);
    rb_init(&rb);

    Player *test_players = (Player*)malloc(num_operations * sizeof(Player));
    for (int i = 0; i < num_operations; i++) {
        sprintf(test_players[i].name, "Player%d", i);
        test_players[i].score = rand() % (num_operations * 10);
    }

    printf("\n=== PERFORMANCE TEST (%d operations) ===\n", num_operations);

    // --- AVL Insertion ---
    clock_t start = clock();
    for (int i = 0; i < num_operations; i++) {
        avl_insert(&avl, test_players[i]);
    }
    clock_t end = clock();
    double avl_ins_time = (double)(end - start) / CLOCKS_PER_SEC;

    // --- RB Insertion ---
    start = clock();
    for (int i = 0; i < num_operations; i++) {
        rb_insert(&rb, test_players[i]);
    }
    end = clock();
    double rb_ins_time = (double)(end - start) / CLOCKS_PER_SEC;

    printf("AVL Insertion: %.4fs, Events: %ld\n", avl_ins_time, avl.rotation_events);
    printf("RB Insertion:  %.4fs, Events: %ld\n", rb_ins_time, rb.balancing_events);

    long avl_ins_events = avl.rotation_events;
    long rb_ins_events = rb.balancing_events;

    // Reset counts for removal
    avl.rotation_events = 0;
    rb.balancing_events = 0;

    // --- AVL Removal ---
    start = clock();
    for (int i = 0; i < num_operations; i++) {
        avl_remove(&avl, test_players[i].score);
    }
    end = clock();
    double avl_rem_time = (double)(end - start) / CLOCKS_PER_SEC;

    // --- RB Removal ---
    start = clock();
    for (int i = 0; i < num_operations; i++) {
        rb_remove(&rb, test_players[i].score);
    }
    end = clock();
    double rb_rem_time = (double)(end - start) / CLOCKS_PER_SEC;

    printf("AVL Removal:   %.4fs, Events: %ld\n", avl_rem_time, avl.rotation_events);
    printf("RB Removal:    %.4fs, Events: %ld\n", rb_rem_time, rb.balancing_events);

    printf("========================================\n");

    // Write to a temporary result file for the report
    FILE *f = fopen("results.txt", "w");
    if (f) {
        fprintf(f, "AVL_INS_TIME: %.4f\n", avl_ins_time);
        fprintf(f, "RB_INS_TIME: %.4f\n", rb_ins_time);
        fprintf(f, "AVL_INS_EVENTS: %ld\n", avl_ins_events);
        fprintf(f, "RB_INS_EVENTS: %ld\n", rb_ins_events);
        fprintf(f, "AVL_REM_TIME: %.4f\n", avl_rem_time);
        fprintf(f, "RB_REM_TIME: %.4f\n", rb_rem_time);
        fprintf(f, "AVL_REM_EVENTS: %ld\n", avl.rotation_events);
        fprintf(f, "RB_REM_EVENTS: %ld\n", rb.balancing_events);
        fclose(f);
    }

    avl_destroy(&avl);
    rb_destroy(&rb);
    free(test_players);
}
