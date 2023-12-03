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
    int peso;
} tProduto;

typedef struct aux
{
    int quantidade;
    tProduto produto;
    struct aux *prox;
} No;

typedef struct
{
    No *inicio;
    No *fim;
} FILA;

void inicializarPilha(FILA *f)
{
    f->inicio = NULL;
    f->fim = NULL;
}

void exibirCarrinho(FILA *f)
{
    No *end = f->inicio;
    puts("ID | Nome do produto | Preco unitario | Peso | Quantidade | Valor total");
    while (end != NULL)
    {
        printf("%d %s %.2f %d", end->produto.id_prod, end->produto.nome_prod, end->produto.preco, end->produto.peso, end->quantidade, end->quantidade * end->produto.preco);
        end = end->prox;
    }
    printf("\"\n");
}

bool inserirNaFila(FILA *f, tProduto prod)
{
    No* novo = (No*)malloc(sizeof(No));
    novo->produto = prod;
    novo->prox = NULL;
    if (f->inicio == NULL)
        f->inicio = novo;
    else
        f->fim->prox = novo;
    f->fim = novo;
    return true;
}
