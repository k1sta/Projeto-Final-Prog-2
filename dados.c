#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "universal.h"

//essa funcao incializa uma estrutura de fila
//this function initializes a queue structure
void inicializarFila(FILA *f)
{
    f->inicio = NULL;
    f->fim = NULL;
}

//exibe tudo da fila (como a fila eh implementada no carrinho, utilizamos para exibir o carrinho)
//displays everything in the queue (as the queue is implemented in the cart, we use it to display the cart)
void exibirCarrinho(FILA *f)
{
    No *end = f->inicio;
    printf("%s%-4s | %-15s | %-15s | %-5s | %-10s | %-10s %s\n", BWHT,"ID", "Nome do produto", "Preco unitario", "Peso", "Quantidade", "Valor total", SEMCOR);
    while (end != NULL) {
        printf("%-4d | %-15s | %-15.2f | %-5d | %-10d | %-10.2f\n", end->produto.id_prod, end->produto.nome_prod, end->produto.preco, end->produto.peso, end->quantidade, end->quantidade * end->produto.preco);
        end = end->prox;
    }
    printf("\"\n");
}

// essa funcao insere um tProduto na fila
// this function inserts a tProduct in the queue
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
    free(novo);
    return true;
}

bool excluirFila(FILA *f)
{
    No* aux = f->inicio;
    while(aux != NULL){
        No* apagar = aux;
        aux = aux->prox;
        free(apagar);
    }
    free(f);
    return;
}