#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "dados.h"

//essa funcao retorna o numero de produtos cadastrados no arquivo produtos.dat
int numProd(FILE *arq){
    int n = 0;
    fread(&n, sizeof(int), 1, arq);
    return n;
}

//essa funcao recebe um produto e o retorna
tProduto inputProdutoTeclado(){
    tProduto prod;
    printf("%s", "Nome do produto: ");
    scanf(" %[^\n]", prod.nome_prod);
    printf("%s", "Categoria: ");
    scanf(" %[^\n]", prod.categoria);
    printf("%s", "Nome do fornecedor: ");
    scanf(" %[^\n]", prod.nome_fornec);
    printf("%s", "Quantidade em estoque: ");
    scanf("%d", &prod.qnt_estoque);
    printf("%s", "Preco: ");
    scanf("%f", &prod.preco);
    printf("%s", "ID: ");
    scanf("%d", &prod.id_prod);
    printf("%s", "Peso: ");
    scanf("%d", &prod.peso);
    return prod;
}

//essa funcao recebe o nome de um arquivo que contem n produtos
//PRECISA DE REVISAO. NAO FUNCIONA ADEQUADAMENTE.
bool inputProdutoArquivo(char* nome, int n, tProduto* prod){
    FILE *arq = fopen(nome, "r");
    if(arq == NULL){
        puts("Erro ao abrir o arquivo!");
        return false;
    }

    for(int i = 0; i < n; i++, prod++){
        fscanf(arq, "%[^\n]\n", prod->nome_prod);
        fscanf(arq, "%[^\n]\n", prod->categoria);
        fscanf(arq, "%[^\n]\n", prod->nome_fornec);
        fscanf(arq, "%d\n", &prod->qnt_estoque);
        fscanf(arq, "%f\n", &prod->preco);
        fscanf(arq, "%d\n", &prod->id_prod);
        fscanf(arq, "%d\n", &prod->peso);

    }

    fclose(arq);
    return true;
}

//essa funcao atualiza o numero de produtos em produtos.dat em +n
void atualizarNumProd(int n, FILE *arq){
    int aux;
    fread(&n, sizeof(int), 1, arq);
    aux += n;
    fseek(arq, 0, SEEK_SET);
    fwrite(&aux, sizeof(int), 1, arq);
    fflush(arq);
}

//essa funcao recebe um produto e o cadastra no arquivo produtos.dat
//existe um parametro flag que, se for 1, imprime uma mensagem de erro caso o arquivo nao seja aberto
bool cadastrarProduto(tProduto *produto, int flag, FILE *arq){
    tProduto anterior;
    int baixo = 0, alto = numProd(arq) - 1, meio, pos;
    while (baixo <= alto) {
        meio = (baixo + alto) / 2;
        fseek(arq, sizeof(int) + meio * sizeof(tProduto), SEEK_SET);
        fread(&anterior, sizeof(tProduto), 1, arq);
        if (produto->id_prod == anterior.id_prod) {
            puts("Erro: ID duplicado!");
            fclose(arq);
            return false;
        } else if (produto->id_prod < anterior.id_prod) {
            baixo = meio - 1;
        } else {
            baixo = meio + 1;
        }
    }

    pos = baixo;
    fseek(arq, sizeof(int) + pos * sizeof(tProduto), SEEK_SET);
    fread(&anterior, sizeof(tProduto), 1, arq);

    for (int i = numProd(arq); i > pos; i--) {
        fseek(arq, sizeof(int) + (i - 1) * sizeof(tProduto), SEEK_SET);
        fread(&anterior, sizeof(tProduto), 1, arq);
        fseek(arq, sizeof(int) + i * sizeof(tProduto), SEEK_SET);
        fwrite(&anterior, sizeof(tProduto), 1, arq);
    }
    
    fseek(arq, sizeof(int) + pos * sizeof(tProduto), SEEK_SET);
    fwrite(produto, sizeof(tProduto), 1, arq);
    puts("Produto cadastrado com sucesso!");
    return true;
}



//essa funcao recebe um array de id`s para serem removidos
bool removerProdutos(int *id, int n, int flag, FILE *arq){   
    FILE *arq2;
    tProduto produto;
    int i;

    arq2 = fopen("produtos2.dat", "wb");
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
    FILE* aux = arq;
    arq = arq2;
    free(arq2);
    fclose(aux);
    remove("produtos.dat");
    rename("produtos2.dat", "produtos.dat");
    return true;
}

//essa funcao recebe um id e imprime as informacoes do produto com esse id, alem de retornar a posicao dele no arquivo
//esse algoritmo foi feito a partir do algoritmo de busca binaria
int buscarProduto(int id, int flag, FILE *arq){
    tProduto produto;
    int esq = 0, dir, meio;
    rewind(arq);

    fread(&dir, sizeof(int), 1, arq);
    meio = dir / 2;

    while(esq <= dir){
        fseek(arq, sizeof(int) + (meio * sizeof(tProduto)), SEEK_SET);
        fread(&produto, sizeof(tProduto), 1, arq);
        if(produto.id_prod == id){
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
    return -1;
    
}

//essa funcao recebe o id de um produto + o produto modificado e o modifica no arquivo produtos.dat
//PRECISA DE REVISAO. FUNCIONA, MAS PODE NAO SER A MELHOR SOLUCAO PASSAR UM tProduto COMO PARAMETRO
bool modificarProduto(int id, tProduto *produto, int flag, FILE *arq){
    int pos;
    pos = buscarProduto(id, flag, arq);
    if(pos == -1){
        return false;
    }
    fseek(arq, sizeof(int) + (pos * sizeof(tProduto)), SEEK_SET);
    fwrite(produto, sizeof(tProduto), 1, arq);
    if (flag) puts("Produto modificado com sucesso!");
    return true;
}


/*
=============================================================================
A partir daqui, sao funcoes que serao chamadas pelo menu da main, diretamente
=============================================================================
*/


//essaa funcao ainda nao foi testada, mas eh a funcao de compra de produtos para o estoque
//vale ressaltar que ela considera que o estoque eh infinito e que todos os produto estao registrados
int compraProdutos(int flag, FILE *arq){
    int id, n;

    do{
        puts("Digite o ID do produto comprado (-1 para sair): ");
        scanf("%d", &id);
        

        puts("Digite a quantidade comprada: ");
        scanf("%d", &n);

        printf("%d", id);
        if(id != -1){
            tProduto produto;
            int pos = buscarProduto(id, 0, arq);
            if(pos == -1){
                if (flag) puts("Produto nao encontrado!");
                return -2;
            }
            fseek(arq, sizeof(int) + (pos * sizeof(tProduto)), SEEK_SET);
            fread(&produto, sizeof(tProduto), 1, arq);
            produto.qnt_estoque += n;
            fseek(arq, sizeof(int) + (pos * sizeof(tProduto)), SEEK_SET);
            fwrite(&produto, sizeof(tProduto), 1, arq);
            puts("Compra realizada com sucesso!");
        }
    }while(id != -1);

    return true;
}

int registroProdutos(FILE *arq){
    int aux, n;
    tProduto *produtos;
    char nome[50];

    printf("\e[1;1H\e[2J"); // Limpa o console
    puts("Quer adicionar por teclado ou arquivo?");
    puts("1. Teclado");
    puts("2. Arquivo");
    puts("Para SAIR, digite qualquer outro numero");
    printf("Input: ");
    scanf("%d", &aux);

    switch(aux){
        case 1:
            for(int i = 0; i < n; i++){
                produtos[i] = inputProdutoTeclado();
            }
            break;        
        case 2:
            puts("Quantos produtos deseja registrar? ");
            scanf("%d", &n);
            printf("%s", "Nome do arquivo: ");
            scanf(" %[^\n]", nome);
            inputProdutoArquivo(nome, n, produtos);
            break;
        default:
            return -1;
    }

    for(int i = 0; i < n; i++){
        cadastrarProduto(&produtos[i], 0, arq);
    }
    
    atualizarNumProd(n, arq);
    
    return 0;
}


//testar funcao ainda!!
bool criar_csv(FILE* arq){

    FILE *csv;
    tProduto produto;

    csv = fopen("estoque.csv", "w+");
    if(csv==NULL){
        printf("Erro na criação do arquivo csv. \n");
        return false;
    }

    fprintf(csv, "nome,categoria,fornecedor,qntd,preco,peso,id");

    fseek(arq, 0, SEEK_SET); //ponteiro do produtos.dat no inicio

    int aux;
    fread(&aux, sizeof(aux), 1, arq); //pegar a qntd de elementos no inicio do dat

    while(!feof(arq)){
        fread(&produto, sizeof(tProduto), 1, arq);  //pegar um produto em si

        fprintf(csv,"\n%s,%s,%s,%d,%f,%d,%f,%d", 
            produto.nome_prod, produto.categoria, produto.nome_fornec, 
            produto.qnt_estoque, produto.preco, produto.peso, produto.id_prod
        );

    }

    return true;
}


//funcao de caixa registradora
void caixaRegistradora(FILE *arq){
    int id, n, pos;
    FILA *carrinho;
    float total = 0;

    inicializarPilha(carrinho);
    while(id != -1){
        printf("\e[1;1H\e[2J"); // Limpa o console
        exibirCarrinho(carrinho);
        puts(" ");
        printf("Total: R$%.2f\n", total);
        puts(":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::");
        puts(":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::");
        puts("Digite o ID do produto ou -1 para fechar a compra\n");
        printf("Input: ");
        scanf("%d", &id);

        if(id == -1){
            printf("\e[1;1H\e[2J"); // Limpa o console
            printf("Valor a pagar: R$%.2f\n", total);
            puts("Compra finalizada!");
            return;
        }

        pos = buscarProduto(id, 0, arq);
        if(pos == -1){
            puts("Produto nao encontrado!");
        } else{
            tProduto produto;
            fseek(arq, sizeof(int) + (pos * sizeof(tProduto)), SEEK_SET);
            fread(&produto, sizeof(tProduto), 1, arq);
            produto.qnt_estoque--;
            fseek(arq, sizeof(int) + (pos * sizeof(tProduto)), SEEK_SET);
            fwrite(&produto, sizeof(tProduto), 1, arq);
            inserirNaFila(carrinho, produto);
            total += produto.preco * n;
            
        }
    }


}