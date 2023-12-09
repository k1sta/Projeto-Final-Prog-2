#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "universal.h"

// essa funcao recebe um id como parametro e retorna a posicao do produto no arquivo (baseado em busca binaria e inicio == 0)
int compraProdutos(FILE *arq)
{
    tProduto produto;
    int id, n;
    char aux[20];

    do
    {   
        printf("\e[1;1H\e[2J"); // Limpa o console
        printf("\n%sAdicione estoque%s", BWHT, SEMCOR);
        printf("\n%sDigite o ID do produto comprado (-1 para sair): %s", BWHT, SEMCOR);
        scanf("%s", aux);
        getchar();
        if(!testeInputInt(aux)){
            delay(1000);
            continue;
        }
        id = atoi(aux);
        if (id != -1)
        {
            puts("\nDigite a quantidade comprada: ");
            scanf("%s", aux);
            getchar();
            if(!testeInputInt(aux)){
                delay(1000);
                continue;
            }
            n = atoi(aux);
            if(n < 0){
                puts("\nQuantidade invalida!");
                delay(1000);
                continue;
            }
            //encontra a pos do produto no arquivo
            int pos = buscarProduto(id, 0, arq);

            if (pos == -1) // se o produto nao for encontrado
            {
                puts("\nProduto nao encontrado!");
                delay(1000);
                return -2;
            }

            // acha o produto na DB e aumenta a quantidade em n
            fseek(arq, sizeof(int) + (pos * sizeof(tProduto)), SEEK_SET);
            fread(&produto, sizeof(tProduto), 1, arq);
            produto.qnt_estoque += n;
            fseek(arq, sizeof(int) + (pos * sizeof(tProduto)), SEEK_SET);
            fwrite(&produto, sizeof(tProduto), 1, arq);

            printf("\n%sCompra realizada com sucesso!%s", BWHT, SEMCOR);
        }
    } while (id != -1);

    return true;
}