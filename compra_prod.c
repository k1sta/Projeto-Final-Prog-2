#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
//#include "compra_prod.h"
#include "universal.h"

// vale ressaltar que ela considera que o estoque eh infinito e que todos os produto estao registrados
//  FUNCIONA
int compraProdutos(int flag, FILE *arq)
{
    int id, n;

    do
    {
        puts("Digite o ID do produto comprado (-1 para sair): ");
        scanf("%d", &id);
        if (id != -1)
        {
            puts("Digite a quantidade comprada: ");
            scanf("%d", &n);
            tProduto produto;
            int pos = buscarProduto(id, 0, arq);
            if (pos == -1)
            {
                if (flag)
                    puts("Produto nao encontrado!");
                return -2;
            }
            fseek(arq, sizeof(int) + (pos * sizeof(tProduto)), SEEK_SET);
            fread(&produto, sizeof(tProduto), 1, arq);
            produto.qnt_estoque += n;
            fseek(arq, sizeof(int) + (pos * sizeof(tProduto)), SEEK_SET);
            fwrite(&produto, sizeof(tProduto), 1, arq);
            puts("Compra realizada com sucesso!");
        }
    } while (id != -1);

    return true;
}