#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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

// essa funcao retorna o numero de produtos cadastrados no arquivo produtos.dat
int numProd(FILE *arq)
{
    fseek(arq, 0L, SEEK_END);
    int sz = ftell(arq);
    rewind(arq);
    return sz/sizeof(tProduto);
}

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
        fseek(arq, (pos * sizeof(tProduto)), SEEK_SET);
        fread(&produto, sizeof(tProduto), 1, arq);
    }
    return produto;
}

void printProduto(tProduto produto)
{
    puts("");
    printf("%s%-4s | %-15s | %-15s | %-15s | %-15s | %-15s | %-10s%s\n", BWHT, "ID", "Nome do produto", "Categoria", "Fornecedor", "Qtd Estoque", "Preco unitario", "Peso", SEMCOR);
    printf("-----|-----------------|-----------------|-----------------|-----------------|-----------------|------------\n");
    printf("%-4d | %-15s | %-15s | %-15s | %-15d | %-15.2f | %-10d \n", produto.id_prod, produto.nome_prod, produto.categoria, produto.nome_fornec, produto.qnt_estoque, produto.preco, produto.peso);
}

bool testeInputInt(char* str) {
    int n = strlen(str);

    if (str[0] == '-') {
        if (n == 1) {
            printf("Entrada invalida!\n");
            return false;
        }

        for (int i = 1; i < n; i++) {
            if (str[i] < '0' || str[i] > '9') {
                printf("Entrada invalida!\n");
                return false;
            }
        }
    } else {
        for (int i = 0; i < n; i++) {
            if (str[i] < '0' || str[i] > '9') {
                printf("Entrada invalida!\n");
                return false;
            }
        }
    }

    return true;
}

bool testeInputFloat(char* str) {
    int n = strlen(str);

    if (str[0] == '-') {
        if (n == 1) {
            printf("Entrada invalida!\n");
            return false;
        }

        for (int i = 1; i < n; i++) {
            if ((str[i] < '0' || str[i] > '9') && str[i] != '.') {
                printf("Entrada invalida!\n");
                return false;
            }
        }
    } else {
        for (int i = 0; i < n; i++) {
            if ((str[i] < '0' || str[i] > '9') && str[i] != '.') {
                printf("Entrada invalida!\n");
                return false;
            }
        }
    }

    return true;
}