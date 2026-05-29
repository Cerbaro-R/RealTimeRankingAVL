# Sistema de Ranking - AVL vs Rubro-Negra

Este projeto foi desenvolvido para a disciplina de **Estrutura de Dados II**. O objetivo é comparar o desempenho de duas árvores binárias balanceadas (AVL e Rubro-Negra) em um sistema de ranking de jogadores.

## 🚀 Funcionalidades
*   **Cadastro Manual:** Insira o nome e a pontuação de um jogador.
*   **Ranking Top 10:** Exibe os 10 melhores jogadores usando a lógica de cada árvore.
*   **Busca:** Encontre a pontuação de um jogador pelo nome.
*   **Teste de Estresse:** Gera 10.000 jogadores aleatórios para comparar o tempo de inserção e a quantidade de balanceamentos de cada estrutura.

## 🛠️ Como Compilar e Rodar
Certifique-se de ter o `gcc` instalado em sua máquina.

1. Abra o terminal na pasta do projeto.
2. Compile com o seguinte comando:
   ```bash
   gcc main.c avl.c rn.c -o ranking
   ```
3. Execute o programa:
   ```bash
   ./ranking
   ```

## 📂 Estrutura do Projeto
*   `main.c`: Menu interativo e lógica principal.
*   `avl.c/h`: Implementação da Árvore AVL.
*   `rn.c/h`: Implementação da Árvore Rubro-Negra.
*   `estruturas.h`: Definição da struct de Jogador.
*   `RELATORIO.md`: Análise comparativa dos resultados.

---
*Dica: Para o teste de desempenho, o programa limpa as árvores atuais e gera dados aleatórios novos.*
