#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "dados.h"
#include "arqlib.h"

void menu(char *produtos)
{
    tProduto produto;
    int id;

    puts("");
    printf(":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
    printf("::::::::::::::::: Seja bem-vindo ao sistema de gerencia de loja! ::::::::::::::::::::::\n");
    printf(":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
    puts("");
    while (1)
    {
        int resposta = 0;
        puts("O que gostaria de fazer? Escolha sua resposta de acordo com as instrucoes a seguir:");
        puts("1. CADASTRAR produto");
        puts("2. EDITAR produto");
        puts("3. REMOVER produto");
        puts("4. BUSCAR produto");
        puts("5. VIZUALIZAR estoque");
        puts("Para SAIR, digite qualquer outro numero");
        printf("\nInput: ");
        scanf("%d", &resposta);
        switch (resposta)
        {
        case 1:
            printf("\e[1;1H\e[2J"); // Limpa o console
            puts("Quer adicionar por teclado ou arquivo?");
            puts("1. Teclado");
            puts("2. Arquivo");
            puts("Para SAIR, digite qualquer outro numero");
            printf("Input: ");
            scanf("%d", &resposta);
            switch (resposta)
            {
            case 1:
                produto = inputProdutoTeclado();
                break;
            case 2:
                inputProdutoArquivo("arroz.txt", 0, &produto);
                break;
            default:
                return;
            }
            FILE *estoque = fopen(produtos, "r+");
            cadastrarProduto(&produto, 1, estoque);
            break;
        case 2:
            printf("ID do produto: ");
            scanf("%d", &id);
            modificarProduto(id, &produto, 1, estoque);
            break;
        case 3:
            //removerProdutos();
            break;
        case 4:
            printf("ID do produto: ");
            scanf("%d", &id);
            buscarProduto(id, 1, estoque);
            break;
        case 5:

            break;
        default:
            return;
            break;
        }
        printf("\e[1;1H\e[2J"); // Limpa o console
    }
}

int main(void)
{
    menu("produtos.dat");
    /*
    tProduto produto;
    inputProdutoArquivo("arroz.txt", 0, &produto);
    criarArquivoProdutos();
    printf("%d\n", numProd());
    cadastrarProduto(&produto, 1);
    printf("%d\n", numProd());
    buscarProduto(10, 1);
    */

    return 0;
}