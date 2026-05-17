# Relatório de Desempenho: AVL vs. Rubro-Negra

## 1. Introdução
Este relatório compara as estruturas de dados Árvore AVL e Árvore Rubro-Negra (RN) no contexto de um sistema de ranking de torneio em tempo real. O sistema exige atualizações frequentes (inserções, remoções e buscas) e exibição do Top-10.

## 2. Metodologia
- **Dados:** 10.000 jogadores com nomes e pontuações aleatórias.
- **Operações:** Inserção em massa de 10.000 nós, seguida de remoção em massa dos mesmos 10.000 nós.
- **Métricas:** Tempo de execução (segundos) e Número de Eventos de Balanceamento (Rotações/Ajustes de cor).
- **Indexação:** Dual indexing (Árvore por pontuação para ranking e Árvore por nome para busca).

## 3. Resultados Obtidos
Os dados abaixo foram coletados em uma execução com 10.000 operações:

| Operação | Estrutura | Tempo (s) | Eventos de Balanceamento |
| :--- | :--- | :--- | :--- |
| **Inserção** | AVL | 0.0050 | 4033 |
| **Inserção** | Rubro-Negra | 0.0020 | 3335 |
| **Remoção** | AVL | 0.0030 | 2422 |
| **Remoção** | Rubro-Negra | 0.0020 | 3663 |

## 4. Análise e Justificativa
### AVL
- **Vantagem:** É mais estritamente balanceada. Isso garante que as buscas (especialmente a travessia para o Top-10) sejam ligeiramente mais rápidas em árvores muito grandes, pois a altura é menor.
- **Desvantagem:** Exige ajustes mais frequentes durante a inserção para manter o equilíbrio rígido.

### Rubro-Negra
- **Vantagem:** O balanceamento é mais flexível. Isso resulta em inserções e remoções geralmente mais rápidas, pois exige menos ajustes estruturais profundos. No teste, a inserção foi significativamente mais rápida.
- **Desvantagem:** A árvore pode ser ligeiramente mais alta que a AVL, tornando a busca marginalmente mais lenta no pior caso.

## 5. Conclusão
Para um **sistema de ranking de torneio com atualizações a cada segundo**, a escolha ideal é a **Árvore Rubro-Negra**.

**Motivos:**
1. **Frequência de Escrita:** O sistema recebe atualizações constantes. A Rubro-Negra performou melhor em tempo de inserção e remoção no benchmark.
2. **Eficiência em Mutações:** A flexibilidade do balanceamento da Rubro-Negra permite que ela lide melhor com o fluxo contínuo de entrada e saída de jogadores.
3. **Escalabilidade:** Em sistemas de tempo real, a rapidez na atualização dos dados é crucial para manter o sistema responsivo.

A Árvore Rubro-Negra oferece o melhor equilíbrio entre performance de atualização e eficiência de consulta para este cenário de alta dinamicidade.
