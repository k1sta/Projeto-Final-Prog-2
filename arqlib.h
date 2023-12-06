#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

void delay(int milliseconds)
{
    long pause;
    clock_t now, then;

    pause = milliseconds * (CLOCKS_PER_SEC / 1000);
    now = then = clock();
    while ((now - then) < pause)
        now = clock();
}

// FUNCIONA
int inicializarArquivo(FILE *arq){
    rewind(arq); //ponteiro do produtos.dat no inicio
    int n = 0;
    fwrite(&n, sizeof(int), 1, arq);
    return n;
}


//essa funcao retorna o numero de produtos cadastrados no arquivo produtos.dat
// FUNCIONA
int numProd(FILE *arq){
    rewind(arq);
    int n = 0;
    fread(&n, sizeof(int), 1, arq);
    return n;
}

//essa funcao recebe um produto e o retorna
// FUNCIONA
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
    printf("%s", "Peso (em gramas): ");
    scanf("%d", &prod.peso);
    return prod;
}

//essa funcao recebe o nome de um arquivo que contem n produtos
// FUNCIONA
tProduto inputProdutoArquivo(char* nome, int n){
    tProduto *prod = malloc(n * sizeof(tProduto));
    FILE *arq = fopen(nome, "r");
    if(arq == NULL){
        puts("Erro ao abrir o arquivo!");
        return *prod;
    }

    for(int i = 0; i < n; i++, prod++){
        fscanf(arq, "%[^\n]\n", prod->nome_prod);
        fscanf(arq, "%[^\n]\n", prod->categoria);
        fscanf(arq, "%[^\n]\n", prod->nome_fornec);
        fscanf(arq, "%d\n", &prod->qnt_estoque);
        fscanf(arq, "%f\n", &prod->preco);
        fscanf(arq, "%d\n", &prod->id_prod);
        fscanf(arq, "%d\n", &prod->peso);
        printf("%s, %s, %s, %d, %f, %d, %d", prod->nome_prod, prod->categoria, prod->nome_fornec,prod->qnt_estoque, prod->preco, prod->id_prod, prod->peso);

    }

    fclose(arq);
    return *prod;
}

// FUNCIONA
void lerCSV(char* nome, tProduto* prod) {
    FILE *csv = fopen(nome, "r");
    if(csv == NULL){
        puts("Erro ao abrir o arquivo!");
        return;
    }
    char linha[256]; 
    fgets(linha, sizeof(linha), csv); 
    printf("%s", linha);


    while(!feof(csv)){
        fscanf(csv, "%[^,],%[^,],%[^,],%d,%f,%d,%d", prod->nome_prod, prod->categoria, prod->nome_fornec, &prod->qnt_estoque, &prod->preco, &prod->id_prod, &prod->peso);
        prod++;
    }

    fclose(csv);
    return;
}

// FUNCIONA
int qntd_produtos_csv(char* nome){
    int prod = 0;
    FILE *csv = fopen(nome, "r");
    if(csv == NULL){
        puts("Erro ao abrir o arquivo!");
        return 0;
    }
    char linha[256]; 
    fgets(linha, sizeof(linha), csv); 

    while(!feof(csv)){
        fgets(linha, sizeof(linha), csv);
        prod++;
    }

    fclose(csv);
    return prod;

}

//essa funcao atualiza o numero de produtos em produtos.dat em +n
// NAO SEI SE FUNCIONA
void atualizarNumProd(int n, FILE *arq){
    rewind(arq);
    int aux;
    fread(&aux, sizeof(int), 1, arq);
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
    while (baixo <= alto && baixo >= 0) {
        meio = (baixo + alto) / 2;
        fseek(arq, sizeof(int) + meio * sizeof(tProduto), SEEK_SET);
        fread(&anterior, sizeof(tProduto), 1, arq);
        if (produto->id_prod == anterior.id_prod) {
            puts("Erro: ID duplicado!");
            return false;
        } else if (produto->id_prod < anterior.id_prod) {
            alto = meio - 1;
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
    delay(1000);
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
    fclose(arq2);
    fclose(arq);
    rename("produtos.dat", "produtosAUX.dat");
    rename("produtos2.dat", "produtos.dat");
    remove("produtosAUX.dat");
    arq = fopen("produtos.dat", "rb+");
    return true;
}

//essa funcao recebe um id e imprime as informacoes do produto com esse id, alem de retornar a posicao dele no arquivo
//esse algoritmo foi feito a partir do algoritmo de busca binaria
// FUNCIONA
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
    return -1;
    
}

//essa funcao recebe o id de um produto + o produto modificado e o modifica no arquivo produtos.dat
// talvez nao seja a melhor solucao enviando um tProduto como parâmetro
// FUNCIONA
bool modificarProduto(int id, tProduto *produto, int flag, FILE *arq){
    int pos;
    rewind(arq);
    int n = numProd(arq);
    pos = buscarProduto(id, flag, arq);
    if(pos == -2){
        return false;
    }
    fseek(arq, sizeof(int) + (pos * sizeof(tProduto)), SEEK_SET);
    fwrite(produto, sizeof(tProduto), 1, arq);
    if (flag) puts("Produto modificado com sucesso!");
    return true;
}


tProduto catchProduto(int id, FILE *arq) {
    tProduto produto;
    int pos = buscarProduto(id, 0, arq);
    if (pos == -1) {
        // Produto não encontrado
        produto.id_prod = -1;
    } else {
        fseek(arq, sizeof(int) + (pos * sizeof(tProduto)), SEEK_SET);
        fread(&produto, sizeof(tProduto), 1, arq);
    }
    return produto;
}

// FUNCIONA
void printProduto(tProduto produto){

    printf("%-4s | %-15s | %-15s | %-15s | %-15s | %-15s | %-10s\n", "ID", "Nome do produto", "Categoria", "Fornecedor","Qtd Estoque","Preco unitario", "Peso");
    printf("-----|-----------------|-----------------|-----------------|-----------------|-----------------|------------\n");
    printf("%-4d | %-15s | %-15s | %-15s | %-15d | %-15f | %-10d \n", produto.id_prod, produto.nome_prod, produto.categoria, produto.nome_fornec, produto.qnt_estoque, produto.preco, produto.peso);

}


/*
=============================================================================
A partir daqui, sao funcoes que serao chamadas pelo menu da main, diretamente
=============================================================================
*/


//vale ressaltar que ela considera que o estoque eh infinito e que todos os produto estao registrados
// FUNCIONA
int compraProdutos(int flag, FILE *arq){
    int id, n;

    do{
        puts("Digite o ID do produto comprado (-1 para sair): ");
        scanf("%d", &id);
        
        if (id != -1) {
            puts("Digite a quantidade comprada: ");
            scanf("%d", &n);

            printf("%d", id);

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


//ERRO. Executa para sempre quando tenta inputar 2 produtos seguidos pelo teclado.
// NAO SEI SE FUNCIONA ????
int registroProdutos(FILE *arq){
    int escolha = 1, n, escolhaAnt = 0, aux;
    tProduto *produtos;
    char nome[50];

    while(escolha == 1 || escolha == 2 || escolha == 3){
        n = 1;

        printf("\e[1;1H\e[2J"); // Limpa o console
        puts("Quer adicionar por teclado ou arquivo?");
        puts("1. Teclado");
        puts("2. Arquivo texto");
        puts("3. Arquivo csv");
        puts("Para VOLTAR, digite qualquer outro numero");
        printf("Input: ");
        scanf("%d", &aux);

        switch(aux){
            case 1:
                if(escolhaAnt == 2 && escolhaAnt == 3){ // escolhaAnt != 0 ???
                    free(produtos); 
                }
                produtos = (tProduto*)malloc(sizeof(tProduto));
                if(!produtos){
                    puts("Erro ao alocar memoria!");
                    return -1;
                }
                *produtos = inputProdutoTeclado();
                break;        
            case 2:
                puts("Quantos produtos deseja registrar? ");
                scanf("%d", &n);
                if(escolhaAnt == 1 || escolhaAnt == 2){ // escolhaAnt != 0 ???
                    free(produtos);
                }
                produtos = (tProduto*)malloc(n * sizeof(tProduto));
                if(!produtos){
                    puts("Erro ao alocar memoria!");
                    return -1;
                }
                printf("%s", "Nome do arquivo: ");
                scanf(" %[^\n]", nome);
                *produtos = inputProdutoArquivo(nome, n);
                break;
            case 3:
                printf("%s", "Nome do arquivo: ");
                scanf(" %[^\n]", nome);
                n = qntd_produtos_csv(nome);
                produtos = (tProduto*)malloc(n*sizeof(tProduto));
                if(!produtos){
                    puts("Erro ao alocar memoria!");
                    return -1;
                }
                lerCSV(nome, produtos);

                break;

            default:
                return 0;
                break;

        }

        for(int i = 0; i < n; i++){
            aux = cadastrarProduto(&produtos[i], 0, arq);
            if(!aux){
                puts("ID Duplicado!");
                break;
            }
        }

        if(aux) atualizarNumProd(n, arq);
        escolhaAnt = escolha;
        //printf("\e[1;1H\e[2J"); // Limpa o console, mas nao permite ver algumas mensagens de erro

    }

    return -2;
}

void editarProduto(FILE *estoque)
{
    int id;
    printf("Insira o ID do produto: ");
    scanf("%d", &id);

    tProduto produto = catchProduto(id, estoque);

    if (produto.id_prod == -1)
    {
        printf("ID inválido. \n");
        return -1;
    }

    printProduto(produto);

    int opcao = 1, continua = 1;
    while (continua == 1)
    {
        continua = 0;

        printf("\nQual das informações deseja alterar?\n");

        puts("[1] Nome do produto");
        puts("[2] Categoria");
        puts("[3] Preço");
        puts("[4] Quantidade");
        puts("[5] Peso");
        puts("[6] Fornecedor");
        puts("Para SAIR, digite qualquer outro numero");
        printf("\nInput: ");

        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            printf("\nDigite o novo nome do produto: ");
            scanf("%s", produto.nome_prod);
            break;

        case 2:
            printf("\nDigite a nova categoria do produto: ");
            scanf("%s", produto.categoria);
            break;

        case 3:
            printf("\nDigite o novo preço do produto: ");
            scanf("%f", &produto.preco);
            break;

        case 4:
            printf("\nDigite a nova quantidade do produto: ");
            scanf("%d", &produto.qnt_estoque);
            break;

        case 5:
            printf("\nDigite o novo peso do produto: ");
            scanf("%d", &produto.peso);
            break;

        case 6:
            printf("\nDigite o novo fornecedor do produto: ");
            scanf("%s", produto.nome_fornec);
            break;

        default:
            break;
        }

        if (opcao <= 6 && opcao >= 1)
        {
            modificarProduto(id, &produto, 0, estoque);
            printf("\nAlteração realizada com sucesso! \n");
            printf("\nDeseja modificar algum campo novamente? Digite 1 para aceitar.\n");
            scanf("%d", &continua);
        }
    }
}


// FUNCIONA
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

    rewind(arq);
    int aux;
    fread(&aux, sizeof(aux), 1, arq); //pegar a qntd de elementos no inicio do dat

    while(fread(&produto, sizeof(tProduto), 1, arq) == 1){

        fprintf(csv,"\n%s,%s,%s,%d,%f,%d,%d", 
            produto.nome_prod, produto.categoria, produto.nome_fornec, 
            produto.qnt_estoque, produto.preco, produto.id_prod, produto.peso
        );

    }
    
    fclose(csv);
    return true;
}

//funcao de caixa registradora
// FUNCIONA
void caixaRegistradora(FILE *arq){
    int id = 0, pos;
    FILA *carrinho = (FILA*)malloc(sizeof(FILA));
    float total = 0;
    tProduto produto;

    if(!carrinho){
        puts("Erro ao alocar memoria!");
        return;
    }
    inicializarFila(carrinho);

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
            fseek(arq, sizeof(int) + (pos * sizeof(tProduto)), SEEK_SET);
            fread(&produto, sizeof(tProduto), 1, arq);
            produto.qnt_estoque--;
            fseek(arq, sizeof(int) + (pos * sizeof(tProduto)), SEEK_SET);
            fwrite(&produto, sizeof(tProduto), 1, arq);

            No* aux = carrinho->inicio;
            while(aux != NULL){
                if(aux->produto.id_prod == id){
                    aux->quantidade++;
                    break;
                }
                aux = aux->prox;
            }

            if(!aux){
                inserirNaFila(carrinho, produto);
                
            }
            total += produto.preco;
            
        }
    }


}

// funcao para printar o estoque em um arq bin (entrando com o arquivo ja aberto)
// FUNCIONA
void printarEstoque (FILE *arq) {
    rewind(arq);
    int n = numProd(arq); tProduto produto;

    printf("%-4s | %-15s | %-15s | %-15s | %-15s | %-15s | %-10s\n", "ID", "Nome do produto", "Categoria", "Fornecedor","Qtd Estoque","Preco unitario", "Peso");
    printf("-----|-----------------|-----------------|-----------------|-----------------|-----------------|------------\n");
   

    for (int i = 0 ; i < n ; i++) {
        fread(&produto, sizeof(tProduto), 1, arq);
        printf("%-4d | %-15s | %-15s | %-15s | %-15d | %-15f | %-10d \n", produto.id_prod, produto.nome_prod, produto.categoria, produto.nome_fornec, produto.qnt_estoque, produto.preco, produto.peso);
        
    }
}