#ifndef RN_H
#define RN_H

#include "estruturas.h"

// Cores da Arvore Rubro-Negra
typedef enum { VERMELHO, PRETO } Cor;

// No da Arvore Rubro-Negra
typedef struct NoRN {
    Jogador jogador;
    Cor cor;
    struct NoRN *esq, *dir, *pai;
} NoRN;

// Funcoes basicas para a Arvore Rubro-Negra
NoRN* rn_criar_no(Jogador j, NoRN *nulo);
void rn_inserir(NoRN **raiz, NoRN *nulo, Jogador j, int *ajustes);
void rn_exibir_ranking(NoRN *raiz, NoRN *nulo, int *posicao);
NoRN* rn_buscar(NoRN *raiz, NoRN *nulo, char *nome);
void rn_liberar(NoRN *raiz, NoRN *nulo);

#endif // RN_H
