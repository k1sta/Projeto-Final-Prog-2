#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool criarArquivoProdutos(){
    FILE *arq;
    int n = 0;
    arq = fopen("produtos.dat", "ab");
    if(arq == NULL){
        puts("Erro ao criar o arquivo!");
        return false;
    }
    fwrite(&n, sizeof(int), 1, arq);
    fclose(arq);
    puts("Arquivo criado com sucesso!");
    return true;
}

int numProd(){
    FILE *arq;
    int n = 0;
    arq = fopen("produtos.dat", "rb");
    if(arq == NULL){
        puts("Erro ao abrir o arquivo!");
        return -1;
    }
    fread(&n, sizeof(int), 1, arq);
    fclose(arq);
    return n;
}

tProduto inputProdutoTeclado(){
    tProduto prod;
    printf("Nome do produto: ");
    scanf(" %[^\n]", prod.nome_prod);
    printf("Categoria: ");
    scanf(" %[^\n]", prod.categoria);
    printf("Nome do fornecedor: ");
    scanf(" %[^\n]", prod.nome_fornec);
    printf("Quantidade em estoque: ");
    scanf("%d", &prod.qnt_estoque);
    printf("Preco: ");
    scanf("%f", &prod.preco);
    printf("ID: ");
    scanf("%d", &prod.id_prod);
    printf("Peso: ");
    scanf("%d", &prod.peso);
    return prod;
}

tProduto inputProdutoArquivo(char* nome){
    tProduto prod;
    FILE *arq = fopen(nome, "r");
    if(arq == NULL){
        puts("Erro ao abrir o arquivo!");
        return prod;
    }
    fgets(prod.nome_prod, sizeof(prod.nome_prod), arq);
    fgets(prod.categoria, sizeof(prod.categoria), arq);
    fgets(prod.nome_fornec, sizeof(prod.nome_fornec), arq);
    fscanf(arq, "%d", &prod.qnt_estoque);
    fscanf(arq, "%f", &prod.preco);
    fscanf(arq, "%d", &prod.id_prod);
    fscanf(arq, "%d", &prod.peso);
    fclose(arq);
    return prod;
}

void atualizarNumProd(FILE *arq){
    int n;
    fread(&n, sizeof(int), 1, arq);
    n++;
    fseek(arq, 0, SEEK_SET);
    fwrite(&n, sizeof(int), 1, arq);
    fflush(arq);
}

bool cadastrarProduto(tProduto *produto, int flag){
    FILE *arq;
    arq = fopen("produtos.dat", "rb+");
    if(arq == NULL){
        if (flag) puts("Erro ao abrir o arquivo!");
        return false;
    }
    fseek(arq, 0, SEEK_END);
    fwrite(produto, sizeof(tProduto), 1, arq);
    atualizarNumProd(arq);
    fclose(arq);
    return true;
}

bool removerProdutos(PILHA *p, int flag){   
    FILE *arq;
    FILE *arq2;
    tProduto produto;
    int n = tamanhoPilha(p), *id = (int *) malloc(sizeof(int) * n), i;

    for(int i = 0; i < n; i++){
        ELEMENTO temp;
        popPilha(p, &temp.reg);
        id[i] = temp.reg.id;
    }

    arq = fopen("produtos.dat", "rb");
    arq2 = fopen("produtos2.dat", "wb");
    if(arq == NULL){
        if (flag) puts("Erro ao abrir 'produtos.dat'!");
        return false;
    }
    if(arq2 == NULL){
        if (flag) puts("Erro ao abrir o arquivo temporário!");
        return false;
    }  

    fread(&i, sizeof(int), 1, arq);
    i -= n;
    fwrite(&i, sizeof(int), 1, arq2);

    while(fread(&produto, sizeof(tProduto), 1, arq)){
        bool remover = false;
        for(int i = 0; i < n; i++){
            if(produto.id_prod == id[i]){
                remover = true;
                break;
            }
        }
        if(!remover){
            fwrite(&produto, sizeof(tProduto), 1, arq2);
        }
    }
    free(id);
    fclose(arq);
    fclose(arq2);
    remove("produtos.dat");
    rename("produtos2.dat", "produtos.dat");
    return true;
}

int buscarProduto(int id, int flag){
    FILE *arq;
    tProduto produto;
    int esq = 0, dir, meio;
    arq = fopen("produtos.dat", "rb");
    if(arq == NULL){
        if (flag) puts("Erro ao abrir o arquivo!");
        return -1;
    }

    fread(&dir, sizeof(int), 1, arq);
    meio = dir / 2;

    while(esq <= dir){
        fseek(arq, sizeof(int) + (meio * sizeof(tProduto)), SEEK_SET);
        fread(&produto, sizeof(tProduto), 1, arq);
        if(produto.id_prod == id){
            fclose(arq);
            if(flag){
                printf("%s | ", produto.nome_prod);
                printf("Categoria: %s | ", produto.categoria);
                printf("Fornecedor: %s | ", produto.nome_fornec);
                printf("Estoque: %d | ", produto.qnt_estoque);
                printf("Preco %.2f | ", produto.preco);
                printf("ID: %d | ", produto.id_prod);
                printf("Peso: %dg\n", produto.peso);
            }
            return meio;
        }
        else if(produto.id_prod > id){
            dir = meio - 1;
        }
        else{
            esq = meio + 1;
        }
        meio = (esq + dir) / 2;
    }
    if(flag) puts("Produto não encontrado!");
    fclose(arq);
    return -2;
    
}

bool modificarProduto(int id, tProduto *produto, int flag){
    FILE *arq;
    int pos;
    arq = fopen("produtos.dat", "r+b");
    if(arq == NULL){
        if(flag) puts("Erro ao abrir o arquivo!");
        return false;
    }
    pos = buscarProduto(id, flag);
    if(pos == -1){
        fclose(arq);
        return false;
    }
    fseek(arq, sizeof(int) + (pos * sizeof(tProduto)), SEEK_SET);
    fwrite(produto, sizeof(tProduto), 1, arq);
    fclose(arq);
    if (flag) puts("Produto modificado com sucesso!");
    return true;
}
