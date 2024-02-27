#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "universal.h"

// essa funcao recebe um id como parametro e retorna a posicao do produto no arquivo (baseado em busca binaria e inicio == 0)
// existe um parametro flag que, se for 1, imprime uma mensagem de erro
// this function receives an id as a parameter and returns the position of the product in the file (based on binary search and start == 0)
// there is a flag parameter that, if it is 1, prints an error message
int buscarProduto(int id, int flag, FILE *arq)
{
    tProduto produto;
    int esq = 0, dir = numProd(arq), meio;

    //le o numero de produtos no arquivo e calcula o meio
    rewind(arq);

    meio = dir / 2;

    //busca binaria
    while (esq <= dir)
    {
        fseek(arq, (meio * sizeof(tProduto)), SEEK_SET);
        fread(&produto, sizeof(tProduto), 1, arq);
        if (produto.id_prod == id)
        { 
            if (flag) //se o usuario "pediu" para iprimir
            {
                printf("\e[1;1H\e[2J"); // Limpa o console, mas nao permite ver algumas mensagens de erro
                printProduto(produto);
                delay(1000);
            }
            return meio;
        }
        else if (produto.id_prod > id)
        {
            dir = meio - 1;
        }
        else
        {
            esq = meio + 1;
        }
        meio = (esq + dir) / 2;
    }

    //caso nao encontre o produto
    if (flag)
        puts("Produto nao encontrado!");
    return -1;
}