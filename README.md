# 2º trabalho de implementação: esportes e atletas

Trabalho de árvores da disciplina FBI4005 - Algoritmos e Estrutura de Dados I (UCS, 2026/2). A descrição do problema está em [DESCRICAO.txt](DESCRICAO.txt) e as orientações de entrega em [ENUNCIADO.txt](ENUNCIADO.txt). Apresentação em 04/12/2026.

## Estruturas

- **Lista de esportes**: lista encadeada simples, mantida em ordem alfabética. Cada esporte guarda o nome, o total de atletas e a raiz da árvore dos seus atletas.
- **Árvore de atletas**: uma AVL por esporte, ordenada pelo nome do atleta. Cada nodo guarda o nome, a altura e os filhos da esquerda e da direita.

Nenhuma estrutura pronta da linguagem é usada: todas as referências entre nodos são manipuladas pelo programa.

## Operações

| Opção | Operação |
|---|---|
| 1 | INSERE_ESPORTE |
| 2 | INSERE_ATLETA |
| 3 | CONSULTA_ESPORTE |
| 4 | REMOVE_ESPORTE |
| 5 | REMOVE_ATLETA |
| 6 | TOTAL_ATLETAS |
| 7 | TOTAL_ATLETAS_ORDENADO |
| 8 | EXIBE_ATLETAS_TOTAL (A a Z ou Z a A) |
| 9 | EXIBE_ATLETAS_ESPORTE (A a Z ou Z a A) |
| 10 | PERCURSO_PREFIXADO |
| 11 | Extra: exibir os esportes |
| 12 | Extra: buscar um atleta em todos os esportes |
| 13 | Extra: exibir a árvore de um esporte com os fatores de balanceamento |

Algumas decisões:

- Os nomes são convertidos para minúsculas, então `Joao` e `joao` são o mesmo atleta.
- Um atleta não se repete dentro de um esporte, mas pode estar em mais de um esporte.
- A operação 7 ordena um vetor auxiliar de ponteiros por inserção direta; a lista de esportes não é alterada.
- A operação 8 junta os atletas de todos os esportes em uma AVL auxiliar, percorre e libera. Um nome que está em mais de um esporte aparece uma vez.
- Na remoção de um nodo com dois filhos, ele é substituído pelo menor da subárvore direita.

## Carga inicial

O programa começa com os cinco esportes do exemplo da descrição (atletismo, basquete, ginástica, iatismo e voleibol) e usa cada palavra do texto do enunciado como nome de atleta, distribuindo as palavras em rodízio entre os esportes. Das 70 palavras do texto, 64 são inseridas e 6 são descartadas por já estarem no mesmo esporte.

## Como compilar e executar

```sh
gcc -Wall -Wextra -std=c99 -o esportes esportes.c
./esportes
```

## Uso de IA

A declaração de uso de IA pedida no enunciado está no comentário do início de [esportes.c](esportes.c).
