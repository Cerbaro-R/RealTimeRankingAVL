#ifndef RN_H
#define RN_H

#include "jogador.h"

typedef enum { VERMELHO, PRETO } Cor;

typedef struct NoRN {
    Jogador jogador;
    Cor cor;
    struct NoRN *esquerda, *direita, *pai;
} NoRN;

typedef struct {
    NoRN *raiz;
    NoRN *NULO;
    long eventos_balanceamento;
} ArvoreRN;

void rn_inicializar(ArvoreRN *arvore);
void rn_inserir(ArvoreRN *arvore, Jogador jogador);
void rn_remover(ArvoreRN *arvore, int pontuacao);
NoRN* rn_buscar(ArvoreRN *arvore, int pontuacao);
void rn_exibir_top_10(ArvoreRN *arvore);
void rn_destruir(ArvoreRN *arvore);

#endif // RN_H
