#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "universal.h"


void ordena_estoque(FILE *arq)
{
    rewind(arq);
    int tam = numProd(arq);
    tProduto *prod = malloc(sizeof(tProduto) * tam);
    int resposta;

    FILE *arq2 = fopen("estoque_ordenado.dat", "wb+");
    if (arq2 == NULL)
    {
        puts("Erro ao alocar o arquivo.");
        return;
    }

    fread(prod, sizeof(tProduto), tam, arq);

    printf("\nGostaria de visualizar o estoque ordenado por:\n%s[1]%s ID\n%s[2]%s Preco\n%s[3]%s Quantidade no estoque", BCYN, SEMCOR, BCYN, SEMCOR, BCYN, SEMCOR);

    printf("\nInput: ");
    scanf("%d", &resposta);
    switch (resposta)
    {
    case 1:
        for (int i = tam - 1; i > 0; i--)
        {
            for (int j = 0; j < i; j++)
            {
                if (prod[j].id_prod > prod[j + 1].id_prod)
                {

                    tProduto temp = prod[j];
                    prod[j] = prod[j + 1];
                    prod[j + 1] = temp;
                }
            }
        }
        break;
    case 2:
        for (int i = tam - 1; i > 0; i--)
        {
            for (int j = 0; j < i; j++)
            {
                if (prod[j].preco > prod[j + 1].preco)
                {

                    tProduto temp = prod[j];
                    prod[j] = prod[j + 1];
                    prod[j + 1] = temp;
                }
            }
        }
        break;
    case 3:
        for (int i = tam - 1; i > 0; i--)
        {
            for (int j = 0; j < i; j++)
            {
                if (prod[j].qnt_estoque > prod[j + 1].qnt_estoque)
                {

                    tProduto temp = prod[j];
                    prod[j] = prod[j + 1];
                    prod[j + 1] = temp;
                }
            }
        }
        break;
    }
    rewind(arq);
    fwrite(&tam, sizeof(int), 1, arq2);
    fwrite(prod, sizeof(tProduto), tam, arq2);

    printarEstoque(arq2);

    printf("\nPara gerar um csv do estoque ordenado, digite %s[1]%s.", BCYN, SEMCOR);
    printf("\nCaso contrario, digite outro numero para voltar diretamente ao menu.\n");
    scanf("%d", &resposta);

    if(resposta==1){
        criar_csv(arq2);
    }

    fclose(arq2);
    fclose(arq);

    remove("estoque_ordenado.dat");

    //rename("produtos.dat", "produtosAUX.dat");
    //rename("produtos2.dat", "produtos.dat");
    //remove("produtosAUX.dat");
    arq = fopen("produtos.dat", "rb+");
}