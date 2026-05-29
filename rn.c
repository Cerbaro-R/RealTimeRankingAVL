#include "rn.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Funcao para criar um no (usa o no Nulo como sentinela)
NoRN* rn_criar_no(Jogador j, NoRN *nulo) {
    NoRN *novo = (NoRN*)malloc(sizeof(NoRN));
    if (novo) {
        novo->jogador = j;
        novo->cor = VERMELHO; // Todo no novo nasce vermelho
        novo->esq = nulo;
        novo->dir = nulo;
        novo->pai = nulo;
    }
    return novo;
}

// Rotacao para a esquerda (padrao Cormen)
void rotar_esq_rn(NoRN **raiz, NoRN *nulo, NoRN *x, int *ajustes) {
    NoRN *y = x->dir;
    x->dir = y->esq;
    if (y->esq != nulo) y->esq->pai = x;
    y->pai = x->pai;
    if (x->pai == nulo) *raiz = y;
    else if (x == x->pai->esq) x->pai->esq = y;
    else x->pai->dir = y;
    y->esq = x;
    x->pai = y;
    (*ajustes)++;
}

// Rotacao para a direita (padrao Cormen)
void rotar_dir_rn(NoRN **raiz, NoRN *nulo, NoRN *y, int *ajustes) {
    NoRN *x = y->esq;
    y->esq = x->dir;
    if (x->dir != nulo) x->dir->pai = y;
    x->pai = y->pai;
    if (y->pai == nulo) *raiz = x;
    else if (y == y->pai->esq) y->pai->esq = x;
    else y->pai->dir = x;
    x->dir = y;
    y->pai = x;
    (*ajustes)++;
}

// Funcao para consertar o balanceamento apos insercao
void consertar_insercao(NoRN **raiz, NoRN *nulo, NoRN *z, int *ajustes) {
    while (z->pai->cor == VERMELHO) {
        if (z->pai == z->pai->pai->esq) {
            NoRN *y = z->pai->pai->dir;
            if (y->cor == VERMELHO) { // Caso 1
                z->pai->cor = PRETO;
                y->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            } else {
                if (z == z->pai->dir) { // Caso 2
                    z = z->pai;
                    rotar_esq_rn(raiz, nulo, z, ajustes);
                }
                // Caso 3
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotar_dir_rn(raiz, nulo, z->pai->pai, ajustes);
            }
        } else {
            NoRN *y = z->pai->pai->esq;
            if (y->cor == VERMELHO) {
                z->pai->cor = PRETO;
                y->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            } else {
                if (z == z->pai->esq) {
                    z = z->pai;
                    rotar_dir_rn(raiz, nulo, z, ajustes);
                }
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotar_esq_rn(raiz, nulo, z->pai->pai, ajustes);
            }
        }
    }
    (*raiz)->cor = PRETO;
}

// Insere um jogador na Rubro-Negra
void rn_inserir(NoRN **raiz, NoRN *nulo, Jogador j, int *ajustes) {
    NoRN *z = rn_criar_no(j, nulo);
    NoRN *y = nulo;
    NoRN *x = *raiz;

    while (x != nulo) {
        y = x;
        if (z->jogador.pontuacao < x->jogador.pontuacao)
            x = x->esq;
        else
            x = x->dir;
    }

    z->pai = y;
    if (y == nulo) *raiz = z;
    else if (z->jogador.pontuacao < y->jogador.pontuacao)
        y->esq = z;
    else
        y->dir = z;

    consertar_insercao(raiz, nulo, z, ajustes);
}

// Exibe ranking decrescente
void rn_exibir_ranking(NoRN *raiz, NoRN *nulo, int *posicao) {
    if (raiz != nulo && *posicao < 10) {
        rn_exibir_ranking(raiz->dir, nulo, posicao);
        if (*posicao < 10) {
            (*posicao)++;
            printf("%d lugar: %s - %d pontos (%s)\n",
                   *posicao, raiz->jogador.nome, raiz->jogador.pontuacao,
                   raiz->cor == VERMELHO ? "V" : "P");
        }
        rn_exibir_ranking(raiz->esq, nulo, posicao);
    }
}

// Busca por nome
NoRN* rn_buscar(NoRN *raiz, NoRN *nulo, char *nome) {
    if (raiz == nulo) return NULL;
    if (strcmp(raiz->jogador.nome, nome) == 0) return raiz;

    NoRN *res = rn_buscar(raiz->esq, nulo, nome);
    if (res) return res;
    return rn_buscar(raiz->dir, nulo, nome);
}

// Libera memoria
void rn_liberar(NoRN *raiz, NoRN *nulo) {
    if (raiz != nulo) {
        rn_liberar(raiz->esq, nulo);
        rn_liberar(raiz->dir, nulo);
        free(raiz);
    }
}
