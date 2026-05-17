#include "rn.h"
#include <stdlib.h>
#include <stdio.h>

static NoRN* criar_no(ArvoreRN *arvore, Jogador jogador) {
    NoRN *no = (NoRN*)malloc(sizeof(NoRN));
    no->jogador = jogador;
    no->pai = no->esquerda = no->direita = arvore->NULO;
    no->cor = VERMELHO;
    return no;
}

static void rotacionar_esquerda(ArvoreRN *arvore, NoRN *x) {
    NoRN *y = x->direita;
    x->direita = y->esquerda;
    if (y->esquerda != arvore->NULO) {
        y->esquerda->pai = x;
    }
    y->pai = x->pai;
    if (x->pai == NULL) {
        arvore->raiz = y;
    } else if (x == x->pai->esquerda) {
        x->pai->esquerda = y;
    } else {
        x->pai->direita = y;
    }
    y->esquerda = x;
    x->pai = y;
}

static void rotacionar_direita(ArvoreRN *arvore, NoRN *y) {
    NoRN *x = y->esquerda;
    y->esquerda = x->direita;
    if (x->direita != arvore->NULO) {
        x->direita->pai = y;
    }
    x->pai = y->pai;
    if (y->pai == NULL) {
        arvore->raiz = x;
    } else if (y == y->pai->esquerda) {
        y->pai->esquerda = x;
    } else {
        y->pai->direita = x;
    }
    x->direita = y;
    y->pai = x;
}

static void corrigir_insercao(ArvoreRN *arvore, NoRN *k) {
    NoRN *u;
    while (k->pai != NULL && k->pai->cor == VERMELHO) {
        if (k->pai == k->pai->pai->direita) {
            u = k->pai->pai->esquerda;
            if (u->cor == VERMELHO) {
                u->cor = PRETO;
                k->pai->cor = PRETO;
                k->pai->pai->cor = VERMELHO;
                k = k->pai->pai;
            } else {
                if (k == k->pai->esquerda) {
                    k = k->pai;
                    rotacionar_direita(arvore, k);
                }
                k->pai->cor = PRETO;
                k->pai->pai->cor = VERMELHO;
                rotacionar_esquerda(arvore, k->pai->pai);
                arvore->eventos_balanceamento++;
            }
        } else {
            u = k->pai->pai->direita;
            if (u->cor == VERMELHO) {
                u->cor = PRETO;
                k->pai->cor = PRETO;
                k->pai->pai->cor = VERMELHO;
                k = k->pai->pai;
            } else {
                if (k == k->pai->direita) {
                    k = k->pai;
                    rotacionar_esquerda(arvore, k);
                }
                k->pai->cor = PRETO;
                k->pai->pai->cor = VERMELHO;
                rotacionar_direita(arvore, k->pai->pai);
                arvore->eventos_balanceamento++;
            }
        }
        if (k == arvore->raiz) break;
    }
    arvore->raiz->cor = PRETO;
}

void rn_inicializar(ArvoreRN *arvore) {
    arvore->NULO = (NoRN*)malloc(sizeof(NoRN));
    arvore->NULO->cor = PRETO;
    arvore->NULO->esquerda = arvore->NULO->direita = arvore->NULO->pai = NULL;
    arvore->raiz = arvore->NULO;
    arvore->eventos_balanceamento = 0;
}

void rn_inserir(ArvoreRN *arvore, Jogador jogador) {
    NoRN *no = criar_no(arvore, jogador);

    NoRN *y = NULL;
    NoRN *x = arvore->raiz;

    while (x != arvore->NULO) {
        y = x;
        if (no->jogador.pontuacao < x->jogador.pontuacao) {
            x = x->esquerda;
        } else if (no->jogador.pontuacao > x->jogador.pontuacao) {
            x = x->direita;
        } else {
            free(no);
            return;
        }
    }

    no->pai = y;
    if (y == NULL) {
        arvore->raiz = no;
    } else if (no->jogador.pontuacao < y->jogador.pontuacao) {
        y->esquerda = no;
    } else {
        y->direita = no;
    }

    if (no->pai == NULL) {
        no->cor = PRETO;
        return;
    }

    if (no->pai->pai == NULL) {
        return;
    }

    corrigir_insercao(arvore, no);
}

static void rn_transplante(ArvoreRN *arvore, NoRN *u, NoRN *v) {
    if (u->pai == NULL) {
        arvore->raiz = v;
    } else if (u == u->pai->esquerda) {
        u->pai->esquerda = v;
    } else {
        u->pai->direita = v;
    }
    v->pai = u->pai;
}

static void corrigir_remocao(ArvoreRN *arvore, NoRN *x) {
    NoRN *s;
    while (x != arvore->raiz && x->cor == PRETO) {
        if (x == x->pai->esquerda) {
            s = x->pai->direita;
            if (s->cor == VERMELHO) {
                s->cor = PRETO;
                x->pai->cor = VERMELHO;
                rotacionar_esquerda(arvore, x->pai);
                arvore->eventos_balanceamento++;
                s = x->pai->direita;
            }

            if (s->esquerda->cor == PRETO && s->direita->cor == PRETO) {
                s->cor = VERMELHO;
                x = x->pai;
            } else {
                if (s->direita->cor == PRETO) {
                    s->esquerda->cor = PRETO;
                    s->cor = VERMELHO;
                    rotacionar_direita(arvore, s);
                    arvore->eventos_balanceamento++;
                    s = x->pai->direita;
                }

                s->cor = x->pai->cor;
                x->pai->cor = PRETO;
                s->direita->cor = PRETO;
                rotacionar_esquerda(arvore, x->pai);
                arvore->eventos_balanceamento++;
                x = arvore->raiz;
            }
        } else {
            s = x->pai->esquerda;
            if (s->cor == VERMELHO) {
                s->cor = PRETO;
                x->pai->cor = VERMELHO;
                rotacionar_direita(arvore, x->pai);
                arvore->eventos_balanceamento++;
                s = x->pai->esquerda;
            }

            if (s->direita->cor == PRETO && s->esquerda->cor == PRETO) {
                s->cor = VERMELHO;
                x = x->pai;
            } else {
                if (s->esquerda->cor == PRETO) {
                    s->direita->cor = PRETO;
                    s->cor = VERMELHO;
                    rotacionar_esquerda(arvore, s);
                    arvore->eventos_balanceamento++;
                    s = x->pai->esquerda;
                }

                s->cor = x->pai->cor;
                x->pai->cor = PRETO;
                s->esquerda->cor = PRETO;
                rotacionar_direita(arvore, x->pai);
                arvore->eventos_balanceamento++;
                x = arvore->raiz;
            }
        }
    }
    x->cor = PRETO;
}

static NoRN* minimo(ArvoreRN *arvore, NoRN *no) {
    while (no->esquerda != arvore->NULO) {
        no = no->esquerda;
    }
    return no;
}

void rn_remover(ArvoreRN *arvore, int pontuacao) {
    NoRN *z = arvore->NULO;
    NoRN *x, *y;
    NoRN *no = arvore->raiz;

    while (no != arvore->NULO) {
        if (no->jogador.pontuacao == pontuacao) {
            z = no;
            break;
        }

        if (no->jogador.pontuacao <= pontuacao) {
            no = no->direita;
        } else {
            no = no->esquerda;
        }
    }

    if (z == arvore->NULO) return;

    y = z;
    Cor cor_original_y = y->cor;
    if (z->esquerda == arvore->NULO) {
        x = z->direita;
        rn_transplante(arvore, z, z->direita);
    } else if (z->direita == arvore->NULO) {
        x = z->esquerda;
        rn_transplante(arvore, z, z->esquerda);
    } else {
        y = minimo(arvore, z->direita);
        cor_original_y = y->cor;
        x = y->direita;
        if (y->pai == z) {
            x->pai = y;
        } else {
            rn_transplante(arvore, y, y->direita);
            y->direita = z->direita;
            y->direita->pai = y;
        }

        rn_transplante(arvore, z, y);
        y->esquerda = z->esquerda;
        y->esquerda->pai = y;
        y->cor = z->cor;
    }
    free(z);
    if (cor_original_y == PRETO) {
        corrigir_remocao(arvore, x);
    }
}

NoRN* rn_buscar(ArvoreRN *arvore, int pontuacao) {
    NoRN *atual = arvore->raiz;
    while (atual != arvore->NULO) {
        if (pontuacao == atual->jogador.pontuacao) return atual;
        if (pontuacao < atual->jogador.pontuacao) atual = atual->esquerda;
        else atual = atual->direita;
    }
    return NULL;
}

static void em_ordem_inversa(ArvoreRN *arvore, NoRN *no, int *contador) {
    if (no == arvore->NULO || *contador >= 10) return;
    em_ordem_inversa(arvore, no->direita, contador);
    if (*contador < 10) {
        printf("%d. %s - %d\n", ++(*contador), no->jogador.nome, no->jogador.pontuacao);
    }
    em_ordem_inversa(arvore, no->esquerda, contador);
}

void rn_exibir_top_10(ArvoreRN *arvore) {
    int contador = 0;
    em_ordem_inversa(arvore, arvore->raiz, &contador);
}

static void destruir(ArvoreRN *arvore, NoRN *no) {
    if (no != arvore->NULO) {
        destruir(arvore, no->esquerda);
        destruir(arvore, no->direita);
        free(no);
    }
}

void rn_destruir(ArvoreRN *arvore) {
    destruir(arvore, arvore->raiz);
    free(arvore->NULO);
    arvore->raiz = NULL;
}
