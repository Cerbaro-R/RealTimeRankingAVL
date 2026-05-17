#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sistema_ranking.h"
#include "desempenho.h"

int main(void) {
    srand(time(NULL));
    SistemaRanking sr;
    ranking_inicializar(&sr);

    printf("--- Demo do Sistema de Ranking em Tempo Real ---\n");

    // Dados de exemplo
    Jogador j1 = {"Alice", 500};
    Jogador j2 = {"Bob", 750};
    Jogador j3 = {"Charlie", 600};
    Jogador j4 = {"David", 900};
    Jogador j5 = {"Eve", 450};
    Jogador j6 = {"Frank", 800};
    Jogador j7 = {"Grace", 950};
    Jogador j8 = {"Heidi", 700};
    Jogador j9 = {"Ivan", 300};
    Jogador j10 = {"Judy", 850};
    Jogador j11 = {"Mallory", 550};

    ranking_atualizar(&sr, j1);
    ranking_atualizar(&sr, j2);
    ranking_atualizar(&sr, j3);
    ranking_atualizar(&sr, j4);
    ranking_atualizar(&sr, j5);
    ranking_atualizar(&sr, j6);
    ranking_atualizar(&sr, j7);
    ranking_atualizar(&sr, j8);
    ranking_atualizar(&sr, j9);
    ranking_atualizar(&sr, j10);
    ranking_atualizar(&sr, j11);

    ranking_exibir_top_10(&sr, 0); // Exibir usando AVL

    printf("\nBuscando por 'Charlie'...\n");
    Jogador *encontrado = ranking_buscar_por_nome(&sr, "Charlie");
    if (encontrado) printf("Encontrado: %s com pontuacao %d\n", encontrado->nome, encontrado->pontuacao);

    printf("\nAtualizando pontuacao de 'Alice' para 1000...\n");
    j1.pontuacao = 1000;
    ranking_atualizar(&sr, j1);

    ranking_exibir_top_10(&sr, 1); // Exibir usando RN

    printf("\nRemovendo 'Grace'...\n");
    ranking_remover(&sr, "Grace");
    
    ranking_exibir_top_10(&sr, 0);

    ranking_destruir(&sr);

    // Executar benchmark de desempenho
    executar_teste_desempenho(10000);

    return 0;
}
