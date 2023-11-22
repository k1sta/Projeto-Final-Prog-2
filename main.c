#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "dados.h"
#include "arqlib.h"

int main (void){
    tProduto produto = inputProdutoArquivo("arroz.txt");

    criarArquivoProdutos();

    printf("%d\n", numProd());
    
    cadastrarProduto(&produto, 1);

    printf("%d\n", numProd());

    buscarProduto(10, 1);

    return 0;
}