#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct
{
    char nome_prod[50];
    char categoria[15];
    char nome_fornec[50];
    int qnt_estoque;
    float preco;
    int id_prod;
    int cod_barras;
    int peso;
} tProduto;

typedef struct
{
    int id;
} REGISTRO;

typedef struct aux
{
    REGISTRO reg;
    struct aux *prox;
} ELEMENTO;

typedef struct
{
    ELEMENTO* topo;
} PILHA;


void inicializarPilha(PILHA* p)
{
    p->topo = NULL;
}

int tamanhoPilha(PILHA *p)
{
    ELEMENTO *end = p->topo;
    int tam = 0;
    while (end != NULL)
    {
        tam++;
        end = end->prox;
    }
    return tam;
}

bool popPilha(PILHA *p, REGISTRO *reg)
{
    if (p->topo == NULL)
        return false;
    *reg = p->topo->reg;
    ELEMENTO *apagar = p->topo;
    p->topo = p->topo->prox;
    free(apagar);
    return true;
}

bool pushPilha(PILHA *p, REGISTRO reg)
{
    ELEMENTO* novo = (ELEMENTO* ) malloc(sizeof(ELEMENTO));
    novo->reg = reg;
    novo->prox = p->topo;
    p->topo = novo;
    return true;
}