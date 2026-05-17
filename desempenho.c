#include "desempenho.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

void executar_teste_desempenho(int num_operacoes) {
    ArvoreAVL avl;
    ArvoreRN rn;
    avl_inicializar(&avl);
    rn_inicializar(&rn);

    Jogador *jogadores_teste = (Jogador*)malloc(num_operacoes * sizeof(Jogador));
    for (int i = 0; i < num_operacoes; i++) {
        sprintf(jogadores_teste[i].nome, "Jogador%d", i);
        jogadores_teste[i].pontuacao = rand() % (num_operacoes * 10);
    }

    printf("\n=== TESTE DE DESEMPENHO (%d operacoes) ===\n", num_operacoes);

    // --- Inserção AVL ---
    clock_t inicio = clock();
    for (int i = 0; i < num_operacoes; i++) {
        avl_inserir(&avl, jogadores_teste[i]);
    }
    clock_t fim = clock();
    double tempo_ins_avl = (double)(fim - inicio) / CLOCKS_PER_SEC;

    // --- Inserção RN ---
    inicio = clock();
    for (int i = 0; i < num_operacoes; i++) {
        rn_inserir(&rn, jogadores_teste[i]);
    }
    fim = clock();
    double tempo_ins_rn = (double)(fim - inicio) / CLOCKS_PER_SEC;

    printf("Insercao AVL: %.4fs, Eventos: %ld\n", tempo_ins_avl, avl.eventos_balanceamento);
    printf("Insercao RN:  %.4fs, Eventos: %ld\n", tempo_ins_rn, rn.eventos_balanceamento);

    // Resetar contagem para remoção
    avl.eventos_balanceamento = 0;
    rn.eventos_balanceamento = 0;

    // --- Remoção AVL ---
    inicio = clock();
    for (int i = 0; i < num_operacoes; i++) {
        avl_remover(&avl, jogadores_teste[i].pontuacao);
    }
    fim = clock();
    double tempo_rem_avl = (double)(fim - inicio) / CLOCKS_PER_SEC;

    // --- Remoção RN ---
    inicio = clock();
    for (int i = 0; i < num_operacoes; i++) {
        rn_remover(&rn, jogadores_teste[i].pontuacao);
    }
    fim = clock();
    double tempo_rem_rn = (double)(fim - inicio) / CLOCKS_PER_SEC;

    printf("Remocao AVL:   %.4fs, Eventos: %ld\n", tempo_rem_avl, avl.eventos_balanceamento);
    printf("Remocao RN:    %.4fs, Eventos: %ld\n", tempo_rem_rn, rn.eventos_balanceamento);

    printf("==========================================\n");

    avl_destruir(&avl);
    rn_destruir(&rn);
    free(jogadores_teste);
}
