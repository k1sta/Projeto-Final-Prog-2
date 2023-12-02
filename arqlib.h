#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


//essa funcao cria o arquivo produtos.dat, que armazena os produtos cadastrados
//vale ressaltar que o arquivo eh criado com um inteiro que armazena o numero de produtos cadastrados no inicio
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

//essa funcao retorna o numero de produtos cadastrados no arquivo produtos.dat
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
    for(int i = 0; i < n; i++){
        fgets(prod->nome_prod, sizeof(prod->nome_prod), arq);
        fgets(prod->categoria, sizeof(prod->categoria), arq);
        fgets(prod->nome_fornec, sizeof(prod->nome_fornec), arq);
        fscanf(arq, "%d", &prod->qnt_estoque);
        fscanf(arq, "%f", &prod->preco);
        fscanf(arq, "%d", &prod->id_prod);
        fscanf(arq, "%d", &prod->peso);
    }


    fclose(arq);
    return true;
}

//essa funcao atualiza o numero de produtos em produtos.dat em +n
void atualizarNumProd(FILE *arq, int n){
    int aux;
    fread(&n, sizeof(int), 1, arq);
    aux += n;
    fseek(arq, 0, SEEK_SET);
    fwrite(&aux, sizeof(int), 1, arq);
    fflush(arq);
}

//essa funcao recebe um produto e o cadastra no arquivo produtos.dat
//existe um parametro flag que, se for 1, imprime uma mensagem de erro caso o arquivo nao seja aberto
//PROBLEMA: essa funcao quebra a ordenacao dos id's. reimplementar com insercao inteligente.
bool cadastrarProduto(tProduto *produto, int flag){
    FILE *arq;

    arq = fopen("produtos.dat", "rb+");
    if(arq == NULL){
        if (flag) puts("Erro ao abrir o arquivo!");
        return false;
    }
    fseek(arq, 0, SEEK_END);
    fwrite(produto, sizeof(tProduto), 1, arq);
    atualizarNumProd(arq, 1);
    fclose(arq);
    puts("Produto cadastrado com sucesso!");
    return true;
}

//essa funcao recebe uma pilha de id`s para serem removidos
//PRECISA DE REVISAO. FUNCIONA, MAS PODE NAO SER A MELHOR SOLUCAO USAR PILHA
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

//essa funcao recebe um id e imprime as informacoes do produto com esse id, alem de retornar a posicao dele no arquivo
//esse algoritmo foi feito a partir do algoritmo de busca binaria
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

//essa funcao recebe o id de um produto + o produto modificado e o modifica no arquivo produtos.dat
//PRECISA DE REVISAO. FUNCIONA, MAS PODE NAO SER A MELHOR SOLUCAO PASSAR UM tProduto COMO PARAMETRO
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


/*
A partir daqui, sao funcoes que serao chamadas pelo menu da main, diretamente
*/



//essaa funcao ainda nao foi testada, mas eh a funcao de compra de produtos para o estoque
//vale ressaltar que ela considera que o estoque eh infinito e que todos os produto estao registrados
int compraProdutos(int flag){
    int id, n;

    do{
        puts("Digite o ID do produto comprado (-1 para sair): ");
        scanf("%d", &id);

        puts("Digite a quantidade comprada: ");
        scanf("%d", &n);

        if(id != -1){
            tProduto produto;
            int pos = buscarProduto(id, 0);
            if(pos == -1){
                if (flag) puts("Erro ao abrir o arquivo!");
                return -1;
            }
            else if(pos == -2){
                if (flag) puts("Produto não encontrado!");
                return -2;
            }
            FILE *arq = fopen("produtos.dat", "r+b");
            if(arq == NULL){
                if (flag) puts("Erro ao abrir o arquivo!");
                return -3;
            }
            fseek(arq, sizeof(int) + (pos * sizeof(tProduto)), SEEK_SET);
            fread(&produto, sizeof(tProduto), 1, arq);
            produto.qnt_estoque += n;
            fseek(arq, sizeof(int) + (pos * sizeof(tProduto)), SEEK_SET);
            fwrite(&produto, sizeof(tProduto), 1, arq);
            fclose(arq);
            puts("Compra realizada com sucesso!");
        }
    }while(id != -1);

    return true;
}

int registroProdutos(){
    int aux, n;
    tProduto *produtos;

    puts("Registro escrito ou via arquivo (1 / 2 / -1 para sair)?");
    scanf("%d", &aux);
    puts("Quantos produtos deseja registrar? ");
    scanf("%d", &n);

    switch(aux){
        case 1:
            for(int i = 0; i < n; i++){
                produtos[i] = inputProdutoTeclado();
            }
            for(int i = 0; i < n; i++){
                cadastrarProduto(&produtos[i], 0);
            }
            break;        
        case 2:
            char nome[50];
            printf("%s", "Nome do arquivo: ");
            scanf(" %[^\n]", nome);
            inputProdutoArquivo(nome, n, produtos);
            for(int i = 0; i < n; i++){
                cadastrarProduto(&produtos[i], 0);
            }
            break;
        default:
            return -1;
    }
    
    return 0;
}