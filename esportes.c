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

TAtleta *menorAtleta(TAtleta *a) {
    while (a->esq != NULL)
        a = a->esq;
    return a;
}

/* nodo com dois filhos é substituído pelo menor da subárvore direita */
TAtleta *removeAtleta(TAtleta *a, const char *nome, int *removeu) {
    if (a == NULL) {
        *removeu = 0;
        return NULL;
    }
    int cmp = strcmp(nome, a->nome);
    if (cmp < 0) {
        a->esq = removeAtleta(a->esq, nome, removeu);
    } else if (cmp > 0) {
        a->dir = removeAtleta(a->dir, nome, removeu);
    } else {
        *removeu = 1;
        if (a->esq == NULL || a->dir == NULL) {
            TAtleta *filho = a->esq != NULL ? a->esq : a->dir;
            free(a);
            return filho;
        }
        TAtleta *sucessor = menorAtleta(a->dir);
        strcpy(a->nome, sucessor->nome);
        int aux;
        a->dir = removeAtleta(a->dir, sucessor->nome, &aux);
    }
    return balanceia(a);
}

/* percurso em ordem: esquerda, raiz, direita */
void exibeAZ(TAtleta *a) {
    if (a != NULL) {
        exibeAZ(a->esq);
        printf("%s\n", a->nome);
        exibeAZ(a->dir);
    }
}

/* percurso em ordem invertido: direita, raiz, esquerda */
void exibeZA(TAtleta *a) {
    if (a != NULL) {
        exibeZA(a->dir);
        printf("%s\n", a->nome);
        exibeZA(a->esq);
    }
}

/* percurso prefixado: raiz, esquerda, direita */
void exibePrefixado(TAtleta *a) {
    if (a != NULL) {
        printf("%s\n", a->nome);
        exibePrefixado(a->esq);
        exibePrefixado(a->dir);
    }
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

/* tira o esporte da lista e libera a árvore de atletas dele */
int removeEsporte(TEsporte **lista, const char *nome) {
    TEsporte *ant = NULL;
    TEsporte *atual = *lista;
    while (atual != NULL && strcmp(atual->nome, nome) < 0) {
        ant = atual;
        atual = atual->prox;
    }
    if (atual == NULL || strcmp(atual->nome, nome) != 0)
        return 0;
    if (ant == NULL)
        *lista = atual->prox;
    else
        ant->prox = atual->prox;
    liberaAtletas(atual->atletas);
    free(atual);
    return 1;
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

TEsporte *escolheEsporte(TEsporte *lista) {
    char nome[MAX_NOME];
    leNome("Nome do esporte: ", nome);
    TEsporte *e = buscaEsporte(lista, nome);
    if (e == NULL)
        printf("Esporte %s nao existe\n", nome);
    return e;
}

/* retorna 1 para ordem de A a Z e 0 para ordem de Z a A */
int leOrdem(void) {
    while (1) {
        int ordem = leInt("Ordem (1 = A a Z, 2 = Z a A): ");
        if (ordem == 1 || ordem == 2)
            return ordem == 1;
        printf("Digite 1 ou 2\n");
    }
}

void opInsereAtleta(TEsporte **lista) {
    TEsporte *e = escolheEsporte(*lista);
    if (e == NULL)
        return;
    char nome[MAX_NOME];
    leNome("Nome do atleta: ", nome);
    if (nome[0] == '\0') {
        printf("Nome vazio\n");
        return;
    }
    int inseriu;
    e->atletas = insereAtleta(e->atletas, nome, &inseriu);
    if (inseriu) {
        e->total++;
        printf("Atleta %s inserido em %s\n", nome, e->nome);
    } else {
        printf("Atleta %s ja esta em %s\n", nome, e->nome);
    }
}

void opExibeAtletasEsporte(TEsporte **lista) {
    TEsporte *e = escolheEsporte(*lista);
    if (e == NULL)
        return;
    if (e->total == 0) {
        printf("%s nao tem atletas\n", e->nome);
        return;
    }
    if (leOrdem())
        exibeAZ(e->atletas);
    else
        exibeZA(e->atletas);
}

void opConsultaEsporte(TEsporte **lista) {
    TEsporte *e = escolheEsporte(*lista);
    if (e != NULL)
        printf("Esporte %s: %d atleta(s), arvore com altura %d\n", e->nome, e->total, altura(e->atletas));
}

void opRemoveAtleta(TEsporte **lista) {
    TEsporte *e = escolheEsporte(*lista);
    if (e == NULL)
        return;
    char nome[MAX_NOME];
    leNome("Nome do atleta: ", nome);
    int removeu;
    e->atletas = removeAtleta(e->atletas, nome, &removeu);
    if (removeu) {
        e->total--;
        printf("Atleta %s removido de %s\n", nome, e->nome);
    } else {
        printf("Atleta %s nao esta em %s\n", nome, e->nome);
    }
}

void opRemoveEsporte(TEsporte **lista) {
    char nome[MAX_NOME];
    leNome("Nome do esporte: ", nome);
    if (removeEsporte(lista, nome))
        printf("Esporte %s removido com todos os seus atletas\n", nome);
    else
        printf("Esporte %s nao existe\n", nome);
}

int totalAtletas(TEsporte *lista) {
    int total = 0;
    for (TEsporte *e = lista; e != NULL; e = e->prox)
        total += e->total;
    return total;
}

void opTotalAtletas(TEsporte **lista) {
    printf("Total de atletas: %d\n", totalAtletas(*lista));
}

/*
 * Copia os ponteiros dos esportes para um vetor auxiliar e ordena o vetor
 * por inserção direta, do maior total para o menor. A lista não é alterada,
 * e em caso de empate os esportes continuam em ordem alfabética.
 */
void opTotalOrdenado(TEsporte **lista) {
    int n = 0;
    for (TEsporte *e = *lista; e != NULL; e = e->prox)
        n++;
    if (n == 0) {
        printf("Nenhum esporte cadastrado\n");
        return;
    }

    TEsporte **vetor = malloc(n * sizeof(TEsporte *));
    if (vetor == NULL) {
        fprintf(stderr, "Erro de alocacao\n");
        exit(1);
    }
    int i = 0;
    for (TEsporte *e = *lista; e != NULL; e = e->prox)
        vetor[i++] = e;

    for (i = 1; i < n; i++) {
        TEsporte *chave = vetor[i];
        int j = i - 1;
        while (j >= 0 && vetor[j]->total < chave->total) {
            vetor[j + 1] = vetor[j];
            j--;
        }
        vetor[j + 1] = chave;
    }

    for (i = 0; i < n; i++)
        printf("%s: %d atleta(s)\n", vetor[i]->nome, vetor[i]->total);
    free(vetor);
}

/* insere em destino todos os nomes da árvore origem */
TAtleta *copiaAtletas(TAtleta *destino, TAtleta *origem) {
    if (origem == NULL)
        return destino;
    int inseriu;
    destino = insereAtleta(destino, origem->nome, &inseriu);
    destino = copiaAtletas(destino, origem->esq);
    return copiaAtletas(destino, origem->dir);
}

/*
 * Junta os atletas de todos os esportes em uma AVL auxiliar, exibe na ordem
 * pedida e libera a auxiliar. Um atleta que está em mais de um esporte
 * aparece uma vez só.
 */
void opExibeAtletasTotal(TEsporte **lista) {
    TAtleta *todos = NULL;
    for (TEsporte *e = *lista; e != NULL; e = e->prox)
        todos = copiaAtletas(todos, e->atletas);
    if (todos == NULL) {
        printf("Nenhum atleta cadastrado\n");
        return;
    }
    if (leOrdem())
        exibeAZ(todos);
    else
        exibeZA(todos);
    liberaAtletas(todos);
}

void opPrefixado(TEsporte **lista) {
    TEsporte *e = escolheEsporte(*lista);
    if (e == NULL)
        return;
    if (e->total == 0)
        printf("%s nao tem atletas\n", e->nome);
    exibePrefixado(e->atletas);
}

void mostraMenu(void) {
    printf("\n1 - Inserir esporte\n");
    printf("2 - Inserir atleta em um esporte\n");
    printf("3 - Consultar esporte\n");
    printf("4 - Remover esporte\n");
    printf("5 - Remover atleta de um esporte\n");
    printf("6 - Total de atletas\n");
    printf("7 - Total de atletas por esporte (ordem decrescente)\n");
    printf("8 - Exibir todos os atletas\n");
    printf("9 - Exibir atletas de um esporte\n");
    printf("10 - Percurso prefixado dos atletas de um esporte\n");
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
        case 2:
            opInsereAtleta(&lista);
            break;
        case 3:
            opConsultaEsporte(&lista);
            break;
        case 4:
            opRemoveEsporte(&lista);
            break;
        case 5:
            opRemoveAtleta(&lista);
            break;
        case 6:
            opTotalAtletas(&lista);
            break;
        case 7:
            opTotalOrdenado(&lista);
            break;
        case 8:
            opExibeAtletasTotal(&lista);
            break;
        case 9:
            opExibeAtletasEsporte(&lista);
            break;
        case 10:
            opPrefixado(&lista);
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
