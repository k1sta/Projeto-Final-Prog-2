#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
//#include "remove_prod.h"
#include "universal.h"

// essa funcao recebe um array de id`s para serem removidos
bool removerProdutos(int *id, int n, int flag, FILE *arq)
{
    FILE *arq2;
    tProduto produto;
    int i;
    arq2 = fopen("produtos2.dat", "wb");
    if(arq2 == NULL){
        if (flag) puts("Erro ao abrir o arquivo temporário!");
        return false;
    }
    rewind(arq);
    fread(&i, sizeof(int), 1, arq);
    i -= n;
    fwrite(&i, sizeof(int), 1, arq2);
    while(fread(&produto, sizeof(tProduto), 1, arq)){
        bool remover = false;
        for(int j = 0; j < n; j++){
            if(produto.id_prod == id[j]){
                remover = true;
                break;
            }
        }
        if(!remover){
            fwrite(&produto, sizeof(tProduto), 1, arq2);
        }
    }

    fclose(arq2);
    fclose(arq);
    rename("produtos.dat", "produtosAUX.dat");
    rename("produtos2.dat", "produtos.dat");
    remove("produtosAUX.dat");
    arq = fopen("produtos.dat", "rb+");
    return true;
}