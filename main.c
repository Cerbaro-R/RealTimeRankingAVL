#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "avl.h"
#include "rn.h"

// Funcao para limpar o buffer do teclado
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Funcao para gerar nomes aleatorios para o teste de desempenho
void gerar_nome_aleatorio(char *nome) {
    char *prefixos[] = {"Player", "Gamer", "Sniper", "Knight", "Mage", "Dragon", "Shadow"};
    sprintf(nome, "%s_%d", prefixos[rand() % 7], rand() % 10000);
}

void menu() {
    printf("\n--- SISTEMA DE RANKING ---\n");
    printf("1. Inserir novo jogador manualmente\n");
    printf("2. Ver Top 10 (Ranking AVL)\n");
    printf("3. Ver Top 10 (Ranking Rubro-Negra)\n");
    printf("4. Buscar jogador por nome\n");
    printf("5. Executar Teste de Desempenho (10.000 nodes)\n");
    printf("0. Sair\n");
    printf("Escolha uma opcao: ");
}

int main() {
    srand(time(NULL));
    
    // Inicializacao das arvores
    NoAVL *raiz_avl = NULL;
    int rotacoes_avl = 0;

    // Inicializacao da Rubro-Negra com o no Nulo (sentinela)
    NoRN *nulo = (NoRN*)malloc(sizeof(NoRN));
    nulo->cor = PRETO;
    nulo->esq = nulo->dir = nulo->pai = nulo;
    NoRN *raiz_rn = nulo;
    int ajustes_rn = 0;

    int opcao;
    char busca[50];
    Jogador temp;

    do {
        menu();
        if (scanf("%d", &opcao) != 1) {
            limpar_buffer();
            continue;
        }

        switch(opcao) {
            case 1:
                printf("Nome do jogador: ");
                scanf("%s", temp.nome);
                printf("Pontuacao: ");
                scanf("%d", &temp.pontuacao);
                
                raiz_avl = avl_inserir(raiz_avl, temp, &rotacoes_avl);
                rn_inserir(&raiz_rn, nulo, temp, &ajustes_rn);
                printf("Jogador inserido com sucesso!\n");
                break;

            case 2:
                printf("\n--- TOP 10 RANKING (AVL) ---\n");
                int pos_avl = 0;
                avl_exibir_ranking(raiz_avl, &pos_avl);
                if (pos_avl == 0) printf("Ranking vazio.\n");
                break;

            case 3:
                printf("\n--- TOP 10 RANKING (RUBRO-NEGRA) ---\n");
                int pos_rn = 0;
                rn_exibir_ranking(raiz_rn, nulo, &pos_rn);
                if (pos_rn == 0) printf("Ranking vazio.\n");
                break;

            case 4:
                printf("Digite o nome para buscar: ");
                scanf("%s", busca);
                NoAVL *encontrado = avl_buscar(raiz_avl, busca);
                if (encontrado)
                    printf("Encontrado! %s possui %d pontos.\n", encontrado->jogador.nome, encontrado->jogador.pontuacao);
                else
                    printf("Jogador nao encontrado.\n");
                break;

            case 5:
                printf("\nExecutando teste com 10.000 jogadores aleatorios...\n");
                
                // Limpa as arvores atuais para o teste
                avl_liberar(raiz_avl);
                rn_liberar(raiz_rn, nulo);
                raiz_avl = NULL;
                raiz_rn = nulo;
                rotacoes_avl = 0;
                ajustes_rn = 0;

                clock_t inicio, fim;
                
                // Teste AVL
                inicio = clock();
                for(int i = 0; i < 10000; i++) {
                    gerar_nome_aleatorio(temp.nome);
                    temp.pontuacao = rand() % 100000;
                    raiz_avl = avl_inserir(raiz_avl, temp, &rotacoes_avl);
                }
                fim = clock();
                double tempo_avl = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

                // Teste RN
                inicio = clock();
                for(int i = 0; i < 10000; i++) {
                    // Usar os mesmos dados exigiria armazenar, mas aqui geramos novos para simplicidade
                    gerar_nome_aleatorio(temp.nome);
                    temp.pontuacao = rand() % 100000;
                    rn_inserir(&raiz_rn, nulo, temp, &ajustes_rn);
                }
                fim = clock();
                double tempo_rn = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

                printf("\n--- RESULTADOS DO TESTE ---\n");
                printf("AVL: %.4f segundos, %d rotacoes.\n", tempo_avl, rotacoes_avl);
                printf("RN : %.4f segundos, %d ajustes de balanceamento.\n", tempo_rn, ajustes_rn);
                break;

            case 0:
                printf("Saindo...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }
    } while(opcao != 0);

    // Limpeza final
    avl_liberar(raiz_avl);
    rn_liberar(raiz_rn, nulo);
    free(nulo);

    return 0;
}
