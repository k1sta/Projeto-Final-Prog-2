#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
//#include "compra_prod.h"
#include "universal.h"

// essa funcao recebe um id como parametro e retorna a posicao do produto no arquivo (baseado em busca binaria e inicio == 0)
int compraProdutos(int flag, FILE *arq)
{
    tProduto produto;
    int id, n;

    do
    {   
        printf("\e[1;1H\e[2J"); // Limpa o console
        printf("\n%sAdicione estoque%s", BWHT, SEMCOR);
        printf("\n%sDigite o ID do produto comprado (-1 para sair): %s", BWHT, SEMCOR);
        scanf("%d", &id);
        if (id != -1)
        {
            puts("\nDigite a quantidade comprada: ");
            scanf("%d", &n);

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