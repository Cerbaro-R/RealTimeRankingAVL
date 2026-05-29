#include "avl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Funcao auxiliar para retornar o maior entre dois numeros
int maximo(int a, int b) {
    return (a > b) ? a : b;
}

// Funcao que devolve a altura de um no (trata o caso de no nulo)
int altura_no(NoAVL *n) {
    if (n == NULL) return 0;
    return n->altura;
}

// Funcao para criar um novo no na memoria
NoAVL* avl_criar_no(Jogador j) {
    NoAVL *novo = (NoAVL*)malloc(sizeof(NoAVL));
    if (novo) {
        novo->jogador = j;
        novo->esq = NULL;
        novo->dir = NULL;
        novo->altura = 1; // Todo no novo comeca com altura 1
    }
    return novo;
}

// Rotacao para a direita (usada quando o desbalanceamento eh a esquerda)
NoAVL* rotar_direita(NoAVL *y, int *rotacoes) {
    NoAVL *x = y->esq;
    NoAVL *T2 = x->dir;

    // Realiza a rotacao
    x->dir = y;
    y->esq = T2;

    // Atualiza as alturas
    y->altura = maximo(altura_no(y->esq), altura_no(y->dir)) + 1;
    x->altura = maximo(altura_no(x->esq), altura_no(x->dir)) + 1;

    (*rotacoes)++; // Conta a rotacao para o relatorio
    return x;
}

// Rotacao para a esquerda (usada quando o desbalanceamento eh a direita)
NoAVL* rotar_esquerda(NoAVL *x, int *rotacoes) {
    NoAVL *y = x->dir;
    NoAVL *T2 = y->esq;

    // Realiza a rotacao
    y->esq = x;
    x->dir = T2;

    // Atualiza as alturas
    x->altura = maximo(altura_no(x->esq), altura_no(x->dir)) + 1;
    y->altura = maximo(altura_no(y->esq), altura_no(y->dir)) + 1;

    (*rotacoes)++;
    return y;
}

// Calcula o fator de balanceamento (FB = altura_esq - altura_dir)
int obter_fb(NoAVL *n) {
    if (n == NULL) return 0;
    return altura_no(n->esq) - altura_no(n->dir);
}

// Insere um jogador na Arvore AVL de forma recursiva
NoAVL* avl_inserir(NoAVL *no, Jogador j, int *rotacoes) {
    // 1. Insercao normal de Arvore Binaria de Busca
    if (no == NULL) return avl_criar_no(j);

    // No ranking, quem tem mais pontos vai para a direita
    if (j.pontuacao < no->jogador.pontuacao)
        no->esq = avl_inserir(no->esq, j, rotacoes);
    else if (j.pontuacao > no->jogador.pontuacao)
        no->dir = avl_inserir(no->dir, j, rotacoes);
    else
        return no; // Pontuacoes iguais (nao lida aqui para simplificar)

    // 2. Atualiza a altura deste no ancestral
    no->altura = 1 + maximo(altura_no(no->esq), altura_no(no->dir));

    // 3. Verifica o fator de balanceamento para ver se precisa de rotacao
    int fb = obter_fb(no);

    // Caso Esquerda-Esquerda
    if (fb > 1 && j.pontuacao < no->esq->jogador.pontuacao)
        return rotar_direita(no, rotacoes);

    // Caso Direita-Direita
    if (fb < -1 && j.pontuacao > no->dir->jogador.pontuacao)
        return rotar_esquerda(no, rotacoes);

    // Caso Esquerda-Direita
    if (fb > 1 && j.pontuacao > no->esq->jogador.pontuacao) {
        no->esq = rotar_esquerda(no->esq, rotacoes);
        return rotar_direita(no, rotacoes);
    }

    // Caso Direita-Esquerda
    if (fb < -1 && j.pontuacao < no->dir->jogador.pontuacao) {
        no->dir = rotar_direita(no->dir, rotacoes);
        return rotar_esquerda(no, rotacoes);
    }

    return no;
}

// Encontra o no com o menor valor (usado na remocao)
NoAVL* menor_no(NoAVL* no) {
    NoAVL* atual = no;
    while (atual->esq != NULL)
        atual = atual->esq;
    return atual;
}

// Remove um jogador da Arvore AVL
NoAVL* avl_remover(NoAVL* raiz, int pontuacao, int *rotacoes) {
    if (raiz == NULL) return raiz;

    if (pontuacao < raiz->jogador.pontuacao)
        raiz->esq = avl_remover(raiz->esq, pontuacao, rotacoes);
    else if (pontuacao > raiz->jogador.pontuacao)
        raiz->dir = avl_remover(raiz->dir, pontuacao, rotacoes);
    else {
        // No encontrado!
        if ((raiz->esq == NULL) || (raiz->dir == NULL)) {
            NoAVL *temp = raiz->esq ? raiz->esq : raiz->dir;

            if (temp == NULL) {
                temp = raiz;
                raiz = NULL;
            } else
                *raiz = *temp;
            free(temp);
        } else {
            NoAVL* temp = menor_no(raiz->dir);
            raiz->jogador = temp->jogador;
            raiz->dir = avl_remover(raiz->dir, temp->jogador.pontuacao, rotacoes);
        }
    }

    if (raiz == NULL) return raiz;

    raiz->altura = 1 + maximo(altura_no(raiz->esq), altura_no(raiz->dir));

    int fb = obter_fb(raiz);

    if (fb > 1 && obter_fb(raiz->esq) >= 0)
        return rotar_direita(raiz, rotacoes);

    if (fb > 1 && obter_fb(raiz->esq) < 0) {
        raiz->esq = rotar_esquerda(raiz->esq, rotacoes);
        return rotar_direita(raiz, rotacoes);
    }

    if (fb < -1 && obter_fb(raiz->dir) <= 0)
        return rotar_esquerda(raiz, rotacoes);

    if (fb < -1 && obter_fb(raiz->dir) > 0) {
        raiz->dir = rotar_direita(raiz->dir, rotacoes);
        return rotar_esquerda(raiz, rotacoes);
    }

    return raiz;
}

// Busca por nome (precisa percorrer a arvore toda pois esta ordenada por pontos)
NoAVL* avl_buscar(NoAVL *raiz, char *nome) {
    if (raiz == NULL) return NULL;
    if (strcmp(raiz->jogador.nome, nome) == 0) return raiz;

    NoAVL *res = avl_buscar(raiz->esq, nome);
    if (res) return res;
    return avl_buscar(raiz->dir, nome);
}

// Exibe os jogadores em ordem decrescente de pontos (In-ordem inversa)
void avl_exibir_ranking(NoAVL *raiz, int *posicao) {
    if (raiz != NULL && *posicao < 10) {
        avl_exibir_ranking(raiz->dir, posicao);
        if (*posicao < 10) {
            (*posicao)++;
            printf("%d lugar: %s - %d pontos\n", *posicao, raiz->jogador.nome, raiz->jogador.pontuacao);
        }
        avl_exibir_ranking(raiz->esq, posicao);
    }
}

// Limpa a arvore da memoria
void avl_liberar(NoAVL *raiz) {
    if (raiz != NULL) {
        avl_liberar(raiz->esq);
        avl_liberar(raiz->dir);
        free(raiz);
    }
}
