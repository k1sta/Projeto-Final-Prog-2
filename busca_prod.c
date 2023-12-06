#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
//#include "busca_prod.h"
#include "universal.h"

// essa funcao recebe um id e imprime as informacoes do produto com esse id, alem de retornar a posicao dele no arquivo
// esse algoritmo foi feito a partir do algoritmo de busca binaria
//  FUNCIONA
int buscarProduto(int id, int flag, FILE *arq)
{
    tProduto produto;
    int esq = 0, dir, meio;
    rewind(arq);

    fread(&dir, sizeof(int), 1, arq);
    meio = dir / 2;

    while (esq <= dir)
    {
        fseek(arq, sizeof(int) + (meio * sizeof(tProduto)), SEEK_SET);
        fread(&produto, sizeof(tProduto), 1, arq);
        if (produto.id_prod == id)
        {
            if (flag)
            {
                printf("\e[1;1H\e[2J"); // Limpa o console, mas nao permite ver algumas mensagens de erro
                printf("%s%-4s | %-15s | %-15s | %-15s | %-15s | %-15s | %-10s%s\n", BWHT, "ID", "Nome do produto", "Categoria", "Fornecedor", "Qtd Estoque", "Preco unitario", "Peso", SEMCOR);
                printf("-----|-----------------|-----------------|-----------------|-----------------|-----------------|------------\n");
                printf("%-4d | %-15s | %-15s | %-15s | %-15d | %-15f | %-10d \n", produto.id_prod, produto.nome_prod, produto.categoria, produto.nome_fornec, produto.qnt_estoque, produto.preco, produto.peso);
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
    if (flag)
        puts("Produto não encontrado!");
    return -1;
}