#ifndef AVL_H
#define AVL_H

#include "jogador.h"

typedef struct NoAVL {
    Jogador jogador;
    struct NoAVL *esquerda;
    struct NoAVL *direita;
    int altura;
} NoAVL;

typedef struct {
    NoAVL *raiz;
    long eventos_balanceamento;
} ArvoreAVL;

void avl_inicializar(ArvoreAVL *arvore);
void avl_inserir(ArvoreAVL *arvore, Jogador jogador);
void avl_remover(ArvoreAVL *arvore, int pontuacao);
NoAVL* avl_buscar(ArvoreAVL *arvore, int pontuacao);
void avl_exibir_top_10(ArvoreAVL *arvore);
void avl_destruir(ArvoreAVL *arvore);

#endif // AVL_H
