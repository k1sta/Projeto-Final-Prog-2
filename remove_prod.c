#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
//#include "remove_prod.h"
#include "universal.h"

// essa funcao recebe um array de id`s para serem removidos
// this function receives an array of id`s to be removed
bool removerProdutos(int *id, int n, FILE *arq)
{
    FILE *arq2;
    tProduto produto;
    int i, cont = 0, *id2 = (int *)calloc (n, sizeof(int));


    //abre um arquivo para escrita
    arq2 = fopen("produtos2.dat", "wb");
    if(arq2 == NULL){
        puts("Erro ao abrir o arquivo temporário!");
        return false;
    }

    //posiciona o ponteiro no inicio dos produtos
    rewind(arq);
    int tam = numProd(arq);
    fseek(arq2, sizeof(int), SEEK_SET);

    //le o produto do arquivo
    while(fread(&produto, sizeof(tProduto), 1, arq)){
        bool remover = false;
        for(int j = 0; j < tam; j++){
            //se o id do produto lido esta no array para remocao, remover vira true
            if(produto.id_prod == id[j]){
                id2[j] = 1;
                remover = true;
                break;
            }
        }

        //caso remover seja true, ele nao escreve o produto em questao no segundo arquivo
        if(!remover){
            fwrite(&produto, sizeof(tProduto), 1, arq2);
        }
    }

    //atualiza o numero de produtos no arquivo 2
    rewind(arq);
    rewind(arq2);
    fread(&i, sizeof(int), 1, arq);
    i -= n;
    fwrite(&i, sizeof(int), 1, arq2);

    //fecha os dois arquivos e renomeia o segundo para o primeiro.
    fclose(arq2);
    fclose(arq);
    rename("produtos.dat", "produtosAUX.dat");
    rename("produtos2.dat", "produtos.dat");

    //exclui o arquivo antigo
    remove("produtosAUX.dat");

    for(int j = 0; j < n; j++){
        if(id2[j] == 0){
            printf("ID %d nao encontrado!\n", id[j]);
        }
    }

    free(id2);

    printf("%d produto(s) removido(s) com sucesso!\n", n);
    delay(1000);

    //reabre o arquivo no espaço de memoria passado por referencia
    arq = fopen("produtos.dat", "rb+");
    return true;
}