#include "sistema_ranking.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// --- Implementação do Índice de Nomes (AVL interna por nome) ---
static int maior(int a, int b) { return (a > b) ? a : b; }
static int obter_altura(NoNome *n) { return n ? n->altura : 0; }
static int obter_fator(NoNome *n) { return n ? obter_altura(n->esquerda) - obter_altura(n->direita) : 0; }

static NoNome* rotacionar_direita(NoNome *y) {
    NoNome *x = y->esquerda;
    NoNome *T2 = x->direita;
    x->direita = y;
    y->esquerda = T2;
    y->altura = maior(obter_altura(y->esquerda), obter_altura(y->direita)) + 1;
    x->altura = maior(obter_altura(x->esquerda), obter_altura(x->direita)) + 1;
    return x;
}

static NoNome* rotacionar_esquerda(NoNome *x) {
    NoNome *y = x->direita;
    NoNome *T2 = y->esquerda;
    y->esquerda = x;
    x->direita = T2;
    x->altura = maior(obter_altura(x->esquerda), obter_altura(x->direita)) + 1;
    y->altura = maior(obter_altura(y->esquerda), obter_altura(y->direita)) + 1;
    return y;
}

static NoNome* nome_inserir(NoNome *no, Jogador jogador) {
    if (!no) {
        NoNome *n = (NoNome*)malloc(sizeof(NoNome));
        n->jogador = jogador;
        n->esquerda = n->direita = NULL;
        n->altura = 1;
        return n;
    }
    int cmp = strcmp(jogador.nome, no->jogador.nome);
    if (cmp < 0) no->esquerda = nome_inserir(no->esquerda, jogador);
    else if (cmp > 0) no->direita = nome_inserir(no->direita, jogador);
    else { no->jogador = jogador; return no; }

    no->altura = 1 + maior(obter_altura(no->esquerda), obter_altura(no->direita));
    int balanceamento = obter_fator(no);
    if (balanceamento > 1 && strcmp(jogador.nome, no->esquerda->jogador.nome) < 0) return rotacionar_direita(no);
    if (balanceamento < -1 && strcmp(jogador.nome, no->direita->jogador.nome) > 0) return rotacionar_esquerda(no);
    if (balanceamento > 1 && strcmp(jogador.nome, no->esquerda->jogador.nome) > 0) {
        no->esquerda = rotacionar_esquerda(no->esquerda);
        return rotacionar_direita(no);
    }
    if (balanceamento < -1 && strcmp(jogador.nome, no->direita->jogador.nome) < 0) {
        no->direita = rotacionar_direita(no->direita);
        return rotacionar_esquerda(no);
    }
    return no;
}

static NoNome* no_minimo(NoNome *n) {
    while (n->esquerda) n = n->esquerda;
    return n;
}

static NoNome* nome_remover(NoNome *raiz, char *nome) {
    if (!raiz) return raiz;
    int cmp = strcmp(nome, raiz->jogador.nome);
    if (cmp < 0) raiz->esquerda = nome_remover(raiz->esquerda, nome);
    else if (cmp > 0) raiz->direita = nome_remover(raiz->direita, nome);
    else {
        if (!raiz->esquerda || !raiz->direita) {
            NoNome *temp = raiz->esquerda ? raiz->esquerda : raiz->direita;
            if (!temp) { temp = raiz; raiz = NULL; }
            else *raiz = *temp;
            free(temp);
        } else {
            NoNome *temp = no_minimo(raiz->direita);
            raiz->jogador = temp->jogador;
            raiz->direita = nome_remover(raiz->direita, temp->jogador.nome);
        }
    }
    if (!raiz) return raiz;
    raiz->altura = 1 + maior(obter_altura(raiz->esquerda), obter_altura(raiz->direita));
    int balanceamento = obter_fator(raiz);
    if (balanceamento > 1 && obter_fator(raiz->esquerda) >= 0) return rotacionar_direita(raiz);
    if (balanceamento > 1 && obter_fator(raiz->esquerda) < 0) {
        raiz->esquerda = rotacionar_esquerda(raiz->esquerda);
        return rotacionar_direita(raiz);
    }
    if (balanceamento < -1 && obter_fator(raiz->direita) <= 0) return rotacionar_esquerda(raiz);
    if (balanceamento < -1 && obter_fator(raiz->direita) > 0) {
        raiz->direita = rotacionar_direita(raiz->direita);
        return rotacionar_esquerda(raiz);
    }
    return raiz;
}

static NoNome* nome_buscar(NoNome *n, char *nome) {
    if (!n) return NULL;
    int cmp = strcmp(nome, n->jogador.nome);
    if (cmp == 0) return n;
    if (cmp < 0) return nome_buscar(n->esquerda, nome);
    return nome_buscar(n->direita, nome);
}

static void nome_destruir(NoNome *n) {
    if (n) { nome_destruir(n->esquerda); nome_destruir(n->direita); free(n); }
}

// --- API Pública do Sistema de Ranking ---

void ranking_inicializar(SistemaRanking *sr) {
    avl_inicializar(&sr->arvore_pontuacao_avl);
    rn_inicializar(&sr->arvore_pontuacao_rn);
    sr->indice_nome.raiz = NULL;
}

void ranking_atualizar(SistemaRanking *sr, Jogador jogador) {
    NoNome *encontrado = nome_buscar(sr->indice_nome.raiz, jogador.nome);
    if (encontrado) {
        avl_remover(&sr->arvore_pontuacao_avl, encontrado->jogador.pontuacao);
        rn_remover(&sr->arvore_pontuacao_rn, encontrado->jogador.pontuacao);
    }
    
    sr->indice_nome.raiz = nome_inserir(sr->indice_nome.raiz, jogador);
    
    avl_inserir(&sr->arvore_pontuacao_avl, jogador);
    rn_inserir(&sr->arvore_pontuacao_rn, jogador);
}

void ranking_remover(SistemaRanking *sr, char *nome) {
    NoNome *encontrado = nome_buscar(sr->indice_nome.raiz, nome);
    if (encontrado) {
        avl_remover(&sr->arvore_pontuacao_avl, encontrado->jogador.pontuacao);
        rn_remover(&sr->arvore_pontuacao_rn, encontrado->jogador.pontuacao);
        sr->indice_nome.raiz = nome_remover(sr->indice_nome.raiz, nome);
    }
}

Jogador* ranking_buscar_por_nome(SistemaRanking *sr, char *nome) {
    NoNome *encontrado = nome_buscar(sr->indice_nome.raiz, nome);
    return encontrado ? &encontrado->jogador : NULL;
}

void ranking_exibir_top_10(SistemaRanking *sr, int usar_rn) {
    printf("\n--- RANKING TOP 10 (%s) ---\n", usar_rn ? "Rubro-Negra" : "AVL");
    if (usar_rn) rn_exibir_top_10(&sr->arvore_pontuacao_rn);
    else avl_exibir_top_10(&sr->arvore_pontuacao_avl);
    printf("---------------------------\n");
}

void ranking_destruir(SistemaRanking *sr) {
    avl_destruir(&sr->arvore_pontuacao_avl);
    rn_destruir(&sr->arvore_pontuacao_rn);
    nome_destruir(sr->indice_nome.raiz);
}
