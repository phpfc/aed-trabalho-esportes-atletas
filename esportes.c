/*
 * 2º trabalho de implementação: esportes e atletas
 * Algoritmos e Estrutura de Dados I - UCS
 * Pedro Henrique França
 *
 * Os esportes ficam em uma lista encadeada em ordem alfabética, e cada
 * esporte aponta para a AVL com os nomes dos seus atletas.
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME 50

typedef struct atleta {
    char nome[MAX_NOME];
    int altura;
    struct atleta *esq;
    struct atleta *dir;
} TAtleta;

typedef struct esporte {
    char nome[MAX_NOME];
    TAtleta *atletas;
    int total;
    struct esporte *prox;
} TEsporte;

/* ---------- AVL de atletas ---------- */

TAtleta *criaAtleta(const char *nome) {
    TAtleta *novo = malloc(sizeof(TAtleta));
    if (novo == NULL) {
        fprintf(stderr, "Erro de alocacao\n");
        exit(1);
    }
    strcpy(novo->nome, nome);
    novo->altura = 1;
    novo->esq = NULL;
    novo->dir = NULL;
    return novo;
}

int altura(TAtleta *a) {
    if (a == NULL)
        return 0;
    return a->altura;
}

void atualizaAltura(TAtleta *a) {
    int he = altura(a->esq);
    int hd = altura(a->dir);
    a->altura = (he > hd ? he : hd) + 1;
}

/* fator de balanceamento: altura da esquerda menos altura da direita */
int fator(TAtleta *a) {
    if (a == NULL)
        return 0;
    return altura(a->esq) - altura(a->dir);
}

TAtleta *rotacaoDireita(TAtleta *a) {
    TAtleta *b = a->esq;
    a->esq = b->dir;
    b->dir = a;
    atualizaAltura(a);
    atualizaAltura(b);
    return b;
}

TAtleta *rotacaoEsquerda(TAtleta *a) {
    TAtleta *b = a->dir;
    a->dir = b->esq;
    b->esq = a;
    atualizaAltura(a);
    atualizaAltura(b);
    return b;
}

/* as rotações duplas são feitas com duas rotações simples */
TAtleta *balanceia(TAtleta *a) {
    atualizaAltura(a);
    int fb = fator(a);
    if (fb > 1) {
        if (fator(a->esq) < 0)
            a->esq = rotacaoEsquerda(a->esq);
        return rotacaoDireita(a);
    }
    if (fb < -1) {
        if (fator(a->dir) > 0)
            a->dir = rotacaoDireita(a->dir);
        return rotacaoEsquerda(a);
    }
    return a;
}

/* inseriu volta com 0 se o nome já estava na árvore */
TAtleta *insereAtleta(TAtleta *a, const char *nome, int *inseriu) {
    if (a == NULL) {
        *inseriu = 1;
        return criaAtleta(nome);
    }
    int cmp = strcmp(nome, a->nome);
    if (cmp < 0)
        a->esq = insereAtleta(a->esq, nome, inseriu);
    else if (cmp > 0)
        a->dir = insereAtleta(a->dir, nome, inseriu);
    else {
        *inseriu = 0;
        return a;
    }
    return balanceia(a);
}

void liberaAtletas(TAtleta *a) {
    if (a != NULL) {
        liberaAtletas(a->esq);
        liberaAtletas(a->dir);
        free(a);
    }
}

/* ---------- lista de esportes ---------- */

/* a lista é ordenada, então a busca pode parar no primeiro nome maior */
TEsporte *buscaEsporte(TEsporte *lista, const char *nome) {
    while (lista != NULL && strcmp(lista->nome, nome) < 0)
        lista = lista->prox;
    if (lista != NULL && strcmp(lista->nome, nome) == 0)
        return lista;
    return NULL;
}

/* insere em ordem alfabética; retorna NULL se o esporte já existe */
TEsporte *insereEsporte(TEsporte **lista, const char *nome) {
    TEsporte *ant = NULL;
    TEsporte *atual = *lista;
    while (atual != NULL && strcmp(atual->nome, nome) < 0) {
        ant = atual;
        atual = atual->prox;
    }
    if (atual != NULL && strcmp(atual->nome, nome) == 0)
        return NULL;

    TEsporte *novo = malloc(sizeof(TEsporte));
    if (novo == NULL) {
        fprintf(stderr, "Erro de alocacao\n");
        exit(1);
    }
    strcpy(novo->nome, nome);
    novo->atletas = NULL;
    novo->total = 0;
    novo->prox = atual;
    if (ant == NULL)
        *lista = novo;
    else
        ant->prox = novo;
    return novo;
}

void exibeEsportes(TEsporte *lista) {
    if (lista == NULL) {
        printf("Nenhum esporte cadastrado\n");
        return;
    }
    for (TEsporte *e = lista; e != NULL; e = e->prox)
        printf("%s (%d atleta(s))\n", e->nome, e->total);
}

void liberaTudo(TEsporte *lista) {
    while (lista != NULL) {
        TEsporte *prox = lista->prox;
        liberaAtletas(lista->atletas);
        free(lista);
        lista = prox;
    }
}

/* ---------- leitura ---------- */

void leLinha(char *destino, int tamanho) {
    if (fgets(destino, tamanho, stdin) == NULL) {
        printf("\n");
        exit(0);
    }
    destino[strcspn(destino, "\n")] = '\0';
}

/* lê um nome, tira os espaços das pontas e passa para minúsculas */
void leNome(const char *pergunta, char *destino) {
    char linha[MAX_NOME];
    printf("%s", pergunta);
    leLinha(linha, MAX_NOME);

    int ini = 0;
    while (linha[ini] == ' ')
        ini++;
    int fim = strlen(linha);
    while (fim > ini && linha[fim - 1] == ' ')
        fim--;
    int n = 0;
    for (int i = ini; i < fim; i++)
        destino[n++] = tolower((unsigned char) linha[i]);
    destino[n] = '\0';
}

int leInt(const char *pergunta) {
    char linha[MAX_NOME];
    int valor;
    while (1) {
        printf("%s", pergunta);
        leLinha(linha, MAX_NOME);
        if (sscanf(linha, "%d", &valor) == 1)
            return valor;
        printf("Digite um numero\n");
    }
}

/* ---------- operações do menu ---------- */

void opInsereEsporte(TEsporte **lista) {
    char nome[MAX_NOME];
    leNome("Nome do esporte: ", nome);
    if (nome[0] == '\0') {
        printf("Nome vazio\n");
        return;
    }
    if (insereEsporte(lista, nome) != NULL)
        printf("Esporte %s inserido\n", nome);
    else
        printf("Esporte %s ja existe\n", nome);
}

void mostraMenu(void) {
    printf("\n1 - Inserir esporte\n");
    printf("11 - Exibir esportes\n");
    printf("0 - Sair\n");
}

int main(void) {
    TEsporte *lista = NULL;

    int opcao;
    do {
        mostraMenu();
        opcao = leInt("Opcao: ");
        switch (opcao) {
        case 1:
            opInsereEsporte(&lista);
            break;
        case 11:
            exibeEsportes(lista);
            break;
        case 0:
            break;
        default:
            printf("Opcao invalida\n");
        }
    } while (opcao != 0);

    liberaTudo(lista);
    return 0;
}
