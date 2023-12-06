#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
//#include "func_secundarias.h"
#include <time.h>
#include "universal.h"

void delay(int milliseconds)
{
    long pause;
    clock_t now, then;

    pause = milliseconds * (CLOCKS_PER_SEC / 1000);
    now = then = clock();
    while ((now - then) < pause)
        now = clock();
}

// FUNCIONA
int inicializarArquivo(FILE *arq)
{
    rewind(arq); // ponteiro do produtos.dat no inicio
    int n = 0;
    fwrite(&n, sizeof(int), 1, arq);
    return n;
}

// essa funcao retorna o numero de produtos cadastrados no arquivo produtos.dat
//  FUNCIONA
int numProd(FILE *arq)
{
    rewind(arq);
    int n = 0;
    fread(&n, sizeof(int), 1, arq);
    return n;
}

// FUNCIONA
int qntd_produtos_csv(char *nome)
{
    int prod = 0;
    FILE *csv = fopen(nome, "r");
    if (csv == NULL)
    {
        puts("Erro ao abrir o arquivo!");
        return 0;
    }
    char linha[256];
    fgets(linha, sizeof(linha), csv);

    while (!feof(csv))
    {
        fgets(linha, sizeof(linha), csv);
        prod++;
    }

    fclose(csv);
    return prod;
}

tProduto catchProduto(int id, FILE *arq)
{
    tProduto produto;
    int pos = buscarProduto(id, 0, arq);
    if (pos == -1)
    {
        // Produto não encontrado
        produto.id_prod = -1;
    }
    else
    {
        fseek(arq, sizeof(int) + (pos * sizeof(tProduto)), SEEK_SET);
        fread(&produto, sizeof(tProduto), 1, arq);
    }
    return produto;
}

// FUNCIONA
void printProduto(tProduto produto)
{
    puts("");
    printf("%s%-4s | %-15s | %-15s | %-15s | %-15s | %-15s | %-10s%s\n", BWHT, "ID", "Nome do produto", "Categoria", "Fornecedor", "Qtd Estoque", "Preco unitario", "Peso", SEMCOR);
    printf("-----|-----------------|-----------------|-----------------|-----------------|-----------------|------------\n");
    printf("%-4d | %-15s | %-15s | %-15s | %-15d | %-15f | %-10d \n", produto.id_prod, produto.nome_prod, produto.categoria, produto.nome_fornec, produto.qnt_estoque, produto.preco, produto.peso);
}