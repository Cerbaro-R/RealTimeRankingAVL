# Sistema de Ranking em Tempo Real

Este projeto implementa um sistema de ranking para torneios esportivos de alta performance, utilizando estruturas de dados balanceadas para garantir eficiência em inserções, remoções e buscas.

## 🚀 Objetivo

O sistema foi desenvolvido para processar atualizações de pontuação a cada segundo, permitindo:
1.  Exibir o **Top-10** jogadores sempre atualizado.
2.  Realizar **buscas por nome** em tempo real ($O(\log N)$).
3.  Comparar o desempenho entre **Árvores AVL** e **Árvores Rubro-Negras (RN)**.

## 🛠️ Tecnologias e Estruturas

O projeto foi desenvolvido em **C11** e utiliza as seguintes técnicas:

*   **Árvore AVL:** Garante um balanceamento rigoroso, otimizando o tempo de busca.
*   **Árvore Rubro-Negra (RN):** Oferece um balanceamento mais flexível, otimizando inserções e remoções frequentes.
*   **Dual Indexing (Indexação Dupla):**
    *   Um índice por **Pontuação** (Árvore) para gerar o ranking.
    *   Um índice por **Nome** (Árvore) para buscas instantâneas por jogador.

## 📂 Estrutura do Projeto

*   `main.c`: Demonstração do sistema e execução dos testes.
*   `avl.c/h`: Implementação da Árvore AVL.
*   `rn.c/h`: Implementação da Árvore Rubro-Negra.
*   `sistema_ranking.c/h`: Orquestrador que gerencia os múltiplos índices.
*   `desempenho.c/h`: Módulo de benchmark para medir tempo e eventos de balanceamento.
*   `jogador.h`: Definição da estrutura de dados do competidor.
*   `RELATORIO.md`: Análise técnica detalhada comparando as duas estruturas.

## 🔨 Compilação e Execução

Para compilar o projeto utilizando o GCC:

```bash
gcc -O3 main.c avl.c rn.c sistema_ranking.c desempenho.c -o RealTimeRankingAVL
```

Para executar:

```bash
./RealTimeRankingAVL
```

## 📊 Teste de Desempenho

O sistema inclui um teste automático com **10.000 operações** aleatórias de inserção e remoção. Ao final da execução, são exibidos os tempos de CPU e a quantidade de ajustes (rotações/trocas de cor) realizados por cada estrutura.

## 📝 Conclusão Técnica

Com base nos testes realizados, a **Árvore Rubro-Negra** mostrou-se superior para o cenário de "tempo real" com muitas atualizações, devido ao seu menor custo computacional para reequilíbrio durante inserções e remoções em massa.

---
Desenvolvido para a Avaliação 2 de Estrutura de Dados.
