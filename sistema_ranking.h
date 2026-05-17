#ifndef SISTEMA_RANKING_H
#define SISTEMA_RANKING_H

#include "avl.h"
#include "rn.h"

// Índice secundário para busca por nome
typedef struct NoNome {
    Jogador jogador;
    struct NoNome *esquerda, *direita;
    int altura;
} NoNome;

typedef struct {
    NoNome *raiz;
} IndiceNome;

typedef struct {
    ArvoreAVL arvore_pontuacao_avl;
    ArvoreRN arvore_pontuacao_rn;
    IndiceNome indice_nome;
} SistemaRanking;

void ranking_inicializar(SistemaRanking *sr);
void ranking_atualizar(SistemaRanking *sr, Jogador jogador);
void ranking_remover(SistemaRanking *sr, char *nome);
Jogador* ranking_buscar_por_nome(SistemaRanking *sr, char *nome);
void ranking_exibir_top_10(SistemaRanking *sr, int usar_rn);
void ranking_destruir(SistemaRanking *sr);

#endif // SISTEMA_RANKING_H
