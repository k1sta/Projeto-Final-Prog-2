#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
//#include "print_estoque.h"
#include "universal.h"

// funcao para printar o estoque em um arq bin (entrando com o arquivo ja aberto)
// FUNCIONA
void printarEstoque(FILE *arq)
{
    rewind(arq);
    int n = numProd(arq);
    tProduto produto;
    printf("\e[1;1H\e[2J"); // Limpa o console, mas nao permite ver algumas mensagens de erro
    printf("%s%-4s | %-15s | %-15s | %-15s | %-15s | %-15s | %-10s%s\n", BWHT, "ID", "Nome do produto", "Categoria", "Fornecedor", "Qtd Estoque", "Preco unitario", "Peso", SEMCOR);
    printf("-----|-----------------|-----------------|-----------------|-----------------|-----------------|------------\n");

    for (int i = 0; i < n; i++)
    {
        fread(&produto, sizeof(tProduto), 1, arq);
        printf("%-4d | %-15s | %-15s | %-15s | %-15d | %-15f | %-10d \n", produto.id_prod, produto.nome_prod, produto.categoria, produto.nome_fornec, produto.qnt_estoque, produto.preco, produto.peso);
    }
}

// FUNCIONA
bool criar_csv(FILE *arq)
{
    FILE *csv;
    tProduto produto;

    csv = fopen("estoque.csv", "w+");
    if (csv == NULL)
    {
        printf("Erro na criação do arquivo csv. \n");
        return false;
    }

    fprintf(csv, "nome,categoria,fornecedor,qntd,preco,id,peso");

    fseek(arq, 0, SEEK_SET); // ponteiro do produtos.dat no inicio

    rewind(arq);
    int aux;
    fread(&aux, sizeof(aux), 1, arq); // pegar a qntd de elementos no inicio do dat

    while (fread(&produto, sizeof(tProduto), 1, arq) == 1)
    {

        fprintf(csv, "\n%s,%s,%s,%d,%f,%d,%d",
                produto.nome_prod, produto.categoria, produto.nome_fornec,
                produto.qnt_estoque, produto.preco, produto.id_prod, produto.peso);
    }

    fclose(csv);
    puts("Arquivo criado!");
    return true;
}