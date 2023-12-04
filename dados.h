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

void inicializarFila(FILA *f)
{
    f->inicio = NULL;
    f->fim = NULL;
}

void exibirCarrinho(FILA *f)
{
    No *end = f->inicio;
    printf("%-4s | %-15s | %-15s | %-5s | %-10s | %-10s\n", "ID", "Nome do produto", "Preco unitario", "Peso", "Quantidade", "Valor total");
    while (end != NULL) {
        printf("%-4d | %-15s | %-15.2f | %-5d | %-10d | %-10.2f\n", end->produto.id_prod, end->produto.nome_prod, end->produto.preco, end->produto.peso, end->quantidade, end->quantidade * end->produto.preco);
        end = end->prox;
    }
    printf("\"\n");
}

bool inserirNaFila(FILA *f, tProduto prod)
{
    No* novo = (No*)malloc(sizeof(No));
    novo->produto = prod;
    novo->prox = NULL;

    No* aux = f->inicio;
    while (aux != NULL) {
        if (strcmp(aux->produto.nome_prod, prod.nome_prod) == 0) {
            // Se o produto já está no carrinho, atualiza a quantidade
            aux->quantidade++;
            free(novo); 
            return true;
        }
        aux = aux->prox;
    }

    if (f->inicio == NULL)
        f->inicio = novo;
    else
        f->fim->prox = novo;
    novo->quantidade = 1;
    f->fim = novo;
    return true;
}