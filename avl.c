#include "avl.h"
#include <stdlib.h>
#include <stdio.h>

static int maior(int a, int b) {
    return (a > b) ? a : b;
}

static int obter_altura(NoAVL *no) {
    return no ? no->altura : 0;
}

static int obter_fator_balanceamento(NoAVL *no) {
    return no ? obter_altura(no->esquerda) - obter_altura(no->direita) : 0;
}

static NoAVL* criar_no(Jogador jogador) {
    NoAVL *no = (NoAVL*)malloc(sizeof(NoAVL));
    no->jogador = jogador;
    no->esquerda = no->direita = NULL;
    no->altura = 1;
    return no;
}

static NoAVL* rotacionar_direita(NoAVL *y) {
    NoAVL *x = y->esquerda;
    NoAVL *T2 = x->direita;

    x->direita = y;
    y->esquerda = T2;

    y->altura = maior(obter_altura(y->esquerda), obter_altura(y->direita)) + 1;
    x->altura = maior(obter_altura(x->esquerda), obter_altura(x->direita)) + 1;

    return x;
}

static NoAVL* rotacionar_esquerda(NoAVL *x) {
    NoAVL *y = x->direita;
    NoAVL *T2 = y->esquerda;

    y->esquerda = x;
    x->direita = T2;

    x->altura = maior(obter_altura(x->esquerda), obter_altura(x->direita)) + 1;
    y->altura = maior(obter_altura(y->esquerda), obter_altura(y->direita)) + 1;

    return y;
}

static NoAVL* inserir(NoAVL *no, Jogador jogador, long *eventos) {
    if (!no) return criar_no(jogador);

    if (jogador.pontuacao < no->jogador.pontuacao)
        no->esquerda = inserir(no->esquerda, jogador, eventos);
    else if (jogador.pontuacao > no->jogador.pontuacao)
        no->direita = inserir(no->direita, jogador, eventos);
    else
        return no;

    no->altura = 1 + maior(obter_altura(no->esquerda), obter_altura(no->direita));

    int balanceamento = obter_fator_balanceamento(no);

    // Esquerda-Esquerda
    if (balanceamento > 1 && jogador.pontuacao < no->esquerda->jogador.pontuacao) {
        (*eventos)++;
        return rotacionar_direita(no);
    }

    // Direita-Direita
    if (balanceamento < -1 && jogador.pontuacao > no->direita->jogador.pontuacao) {
        (*eventos)++;
        return rotacionar_esquerda(no);
    }

    // Esquerda-Direita
    if (balanceamento > 1 && jogador.pontuacao > no->esquerda->jogador.pontuacao) {
        (*eventos)++;
        no->esquerda = rotacionar_esquerda(no->esquerda);
        return rotacionar_direita(no);
    }

    // Direita-Esquerda
    if (balanceamento < -1 && jogador.pontuacao < no->direita->jogador.pontuacao) {
        (*eventos)++;
        no->direita = rotacionar_direita(no->direita);
        return rotacionar_esquerda(no);
    }

    return no;
}

static NoAVL* no_valor_minimo(NoAVL *no) {
    NoAVL *atual = no;
    while (atual->esquerda != NULL)
        atual = atual->esquerda;
    return atual;
}

static NoAVL* remover_no(NoAVL *raiz, int pontuacao, long *eventos) {
    if (!raiz) return raiz;

    if (pontuacao < raiz->jogador.pontuacao)
        raiz->esquerda = remover_no(raiz->esquerda, pontuacao, eventos);
    else if (pontuacao > raiz->jogador.pontuacao)
        raiz->direita = remover_no(raiz->direita, pontuacao, eventos);
    else {
        if ((raiz->esquerda == NULL) || (raiz->direita == NULL)) {
            NoAVL *temp = raiz->esquerda ? raiz->esquerda : raiz->direita;
            if (!temp) {
                temp = raiz;
                raiz = NULL;
            } else
                *raiz = *temp;
            free(temp);
        } else {
            NoAVL *temp = no_valor_minimo(raiz->direita);
            raiz->jogador = temp->jogador;
            raiz->direita = remover_no(raiz->direita, temp->jogador.pontuacao, eventos);
        }
    }

    if (!raiz) return raiz;

    raiz->altura = 1 + maior(obter_altura(raiz->esquerda), obter_altura(raiz->direita));

    int balanceamento = obter_fator_balanceamento(raiz);

    // Esquerda-Esquerda
    if (balanceamento > 1 && obter_fator_balanceamento(raiz->esquerda) >= 0) {
        (*eventos)++;
        return rotacionar_direita(raiz);
    }

    // Esquerda-Direita
    if (balanceamento > 1 && obter_fator_balanceamento(raiz->esquerda) < 0) {
        (*eventos)++;
        raiz->esquerda = rotacionar_esquerda(raiz->esquerda);
        return rotacionar_direita(raiz);
    }

    // Direita-Direita
    if (balanceamento < -1 && obter_fator_balanceamento(raiz->direita) <= 0) {
        (*eventos)++;
        return rotacionar_esquerda(raiz);
    }

    // Direita-Esquerda
    if (balanceamento < -1 && obter_fator_balanceamento(raiz->direita) > 0) {
        (*eventos)++;
        raiz->direita = rotacionar_direita(raiz->direita);
        return rotacionar_esquerda(raiz);
    }

    return raiz;
}

void avl_inicializar(ArvoreAVL *arvore) {
    arvore->raiz = NULL;
    arvore->eventos_balanceamento = 0;
}

void avl_inserir(ArvoreAVL *arvore, Jogador jogador) {
    arvore->raiz = inserir(arvore->raiz, jogador, &arvore->eventos_balanceamento);
}

void avl_remover(ArvoreAVL *arvore, int pontuacao) {
    arvore->raiz = remover_no(arvore->raiz, pontuacao, &arvore->eventos_balanceamento);
}

NoAVL* avl_buscar(ArvoreAVL *arvore, int pontuacao) {
    NoAVL *atual = arvore->raiz;
    while (atual) {
        if (pontuacao == atual->jogador.pontuacao) return atual;
        if (pontuacao < atual->jogador.pontuacao) atual = atual->esquerda;
        else atual = atual->direita;
    }
    return NULL;
}

static void em_ordem_inversa(NoAVL *no, int *contador) {
    if (!no || *contador >= 10) return;
    em_ordem_inversa(no->direita, contador);
    if (*contador < 10) {
        printf("%d. %s - %d\n", ++(*contador), no->jogador.nome, no->jogador.pontuacao);
    }
    em_ordem_inversa(no->esquerda, contador);
}

void avl_exibir_top_10(ArvoreAVL *arvore) {
    int contador = 0;
    em_ordem_inversa(arvore->raiz, &contador);
}

static void destruir(NoAVL *no) {
    if (no) {
        destruir(no->esquerda);
        destruir(no->direita);
        free(no);
    }
}

void avl_destruir(ArvoreAVL *arvore) {
    destruir(arvore->raiz);
    arvore->raiz = NULL;
}
