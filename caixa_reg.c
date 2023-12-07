#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
//#include "caixa_reg.h"
#include "universal.h"

// funcao de caixa registradora
//  FUNCIONA
void caixaRegistradora(FILE *arq)
{
    int id = 0, pos;
    FILA *carrinho = (FILA *)malloc(sizeof(FILA));
    float total = 0;
    tProduto produto;

    if (!carrinho)
    {
        puts("Erro ao alocar memoria!");
        return;
    }
    inicializarFila(carrinho);

    while (id != -1)
    {
        printf("\e[1;1H\e[2J"); // Limpa o console
        exibirCarrinho(carrinho);
        puts(" ");
        printf("Total: R$%.2f\n", total);
        puts(":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::");
        puts(":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::");
        puts("Digite o ID do produto ou -1 para fechar a compra\n");
        printf("Input: ");
        scanf("%d", &id);

        if (id == -1)
        {
            printf("\e[1;1H\e[2J"); // Limpa o console
            printf("Valor a pagar: R$%.2f\n", total);
            puts("Compra finalizada!");
            return;
        }

        pos = buscarProduto(id, 0, arq);
        if (pos == -1)
        {
            puts("\nProduto nao encontrado!");
            delay(1000);
        }
        else
        {
            fseek(arq, sizeof(int) + (pos * sizeof(tProduto)), SEEK_SET);
            fread(&produto, sizeof(tProduto), 1, arq);
            produto.qnt_estoque--;
            fseek(arq, sizeof(int) + (pos * sizeof(tProduto)), SEEK_SET);
            fwrite(&produto, sizeof(tProduto), 1, arq);

            No *aux = carrinho->inicio;
            while (aux != NULL)
            {
                if (aux->produto.id_prod == id)
                {
                    aux->quantidade++;
                    break;
                }
                aux = aux->prox;
            }

            if (!aux)
            {
                inserirNaFila(carrinho, produto);
            }
            total += produto.preco;
        }
    }
}