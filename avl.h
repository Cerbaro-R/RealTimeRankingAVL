#ifndef AVL_H
#define AVL_H

#include "estruturas.h"

// No da Arvore AVL
typedef struct NoAVL {
    Jogador jogador;
    struct NoAVL *esq;
    struct NoAVL *dir;
    int altura;
} NoAVL;

// Funcoes basicas para a Arvore AVL
NoAVL* avl_criar_no(Jogador j);
NoAVL* avl_inserir(NoAVL *raiz, Jogador j, int *rotacoes);
NoAVL* avl_remover(NoAVL *raiz, int pontuacao, int *rotacoes);
NoAVL* avl_buscar(NoAVL *raiz, char *nome);
void avl_exibir_ranking(NoAVL *raiz, int *posicao);
void avl_liberar(NoAVL *raiz);

#endif // AVL_H
