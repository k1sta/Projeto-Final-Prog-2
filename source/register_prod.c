#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "universal.h"

// Essa funcao e responsavel por criar um menu para a insercao de produtos no estoque
// Ela utiliza de outras funcoes de mais "baixo nivel"
// This function is responsible for creating a menu for inserting products into the stock
// It uses other functions of a lower level
int registroProdutos(FILE *arq)
{
    int n, qnt, aux, cont = 0;
    tProduto *produtos;
    char nome[50], inputAux[20];

    while (1)
    {
        //users choice
        printf("\e[1;1H\e[2J"); // Limpa o console
        printf("%sQuer adicionar por teclado ou arquivo?%s\n", BWHT, SEMCOR);
        printf("%s[1]%s Teclado\n", BCYN, SEMCOR);
        printf("%s[2]%s Arquivo TXT\n", BCYN, SEMCOR);
        printf("%s[3]%s Arquivo CSV\n", BCYN, SEMCOR);
        printf("%sPara VOLTAR, digite qualquer outro numero%s\n", BWHT, SEMCOR);
        do{
        printf("%s", "Input: ");
        scanf("%s", inputAux);
        getchar();
        }while(!testeInputInt(inputAux));
        aux = atoi(inputAux);

        switch (aux)
        {
        case 1: //keyboard
            n = 1;
            produtos = (tProduto *)malloc(sizeof(tProduto));
            if (!produtos)
            {
                puts("\nErro ao alocar memoria!");
                return -1;
            }
            *produtos = inputProdutoTeclado();
            break;
        case 2: //txt file (need to check the format in documentation)
            do{
            puts("\nQuantos produtos deseja registrar?");
            printf("%s", "Input: ");
            scanf("%s", inputAux);
            getchar();
            n = atoi(inputAux);
            if (n <= 0)
                puts("Quantidade invalida!");
            }while(n <= 0 || !testeInputInt(inputAux));
            produtos = (tProduto *)malloc(n * sizeof(tProduto));
            if (!produtos)
            {
                puts("\nErro ao alocar memoria!");
                return -1;
            }
            do
            {
                printf("%s", "\nNome do arquivo: ");
                scanf(" %[^\n]", nome);
                getchar();
                qnt = strlen(nome);
                if (nome[qnt - 1] == 't' && nome[qnt - 2] == 'x' && nome[qnt - 3] == 't' && nome[qnt - 4] == '.')
                    break;
                else
                    puts("Formato invalido!");
            } while (1);
            inputProdutoArquivo(nome, n, produtos);
            break;
        case 3: //csv file (need to check the format in documentation)
            do
            {
                printf("%s", "\nNome do arquivo: ");
                scanf(" %[^\n]", nome);
                getchar();
                n = strlen(nome);
                if (nome[n - 1] == 'v' && nome[n - 2] == 's' && nome[n - 3] == 'c' && nome[n - 4] == '.')
                    break;
                else
                    puts("Formato invalido!");
            } while (1);

            n = qntd_produtos_csv(nome);
            produtos = (tProduto *)calloc(n, sizeof(tProduto));
            if (!produtos)
            {
                puts("\nErro ao alocar memoria!");
                return -1;
            }

            lerCSV(nome, produtos);

            break;

        default: //return
            return 0;
            break;
        }

        for (int i = 0; i < n; i++)
        {
            aux = cadastrarProduto(&produtos[i], 0, arq);
            if (!aux) //aux == 0 means that the product was not registered because of an error (id already exists)
            {   
                puts("ID Duplicado!");
                cont++;
                continue;
            }
            int aux2 = numProd(arq);
            aux2++;
            fseek(arq, 0, SEEK_SET);
            fwrite(&aux2, sizeof(int), 1, arq);
        }

        free(produtos); //free the memory allocated for the array of products 

        printf("\n%d produto(s) cadastrado(s) com sucesso!\n", n - cont);
        delay(2000);
    }

    return -2;
}

// essa funcao recebe um produto e o cadastra no arquivo produtos.dat atraves de uma busca binária
// existe um parametro flag que, se for 1, imprime uma mensagem de erro
// this function receives a product and registers it in the file produtos.dat through a binary search
// there is a flag parameter that, if it is 1, prints an error message
bool cadastrarProduto(tProduto *produto, int flag, FILE *arq)
{
    tProduto anterior; //produto anterior ao que esta sendo inserido

    //busca binaria, com alterações, para encontrar a posicao correta do produto a fim de manter o BD ordenado
    int baixo = 0, alto = numProd(arq) - 1, meio, pos;
    while (baixo <= alto && baixo >= 0)
    {
        meio = (baixo + alto) / 2;
        fseek(arq, sizeof(int) + meio * sizeof(tProduto), SEEK_SET);
        fread(&anterior, sizeof(tProduto), 1, arq);
        if (produto->id_prod == anterior.id_prod)
        {
            if (flag)puts("Erro: ID duplicado!");
            return false;
        }
        else if (produto->id_prod < anterior.id_prod)
        {
            alto = meio - 1;
        }
        else
        {
            baixo = meio + 1;
        }
    }

    pos = baixo; //posicao correta do produto

    //"desloca os produtos para a direita" para inserir o novo produto
    fseek(arq, sizeof(int) + pos * sizeof(tProduto), SEEK_SET);
    fread(&anterior, sizeof(tProduto), 1, arq);

    for (int i = numProd(arq); i > pos; i--)
    {
        fseek(arq, sizeof(int) + (i - 1) * sizeof(tProduto), SEEK_SET);
        fread(&anterior, sizeof(tProduto), 1, arq);
        fseek(arq, sizeof(int) + i * sizeof(tProduto), SEEK_SET);
        fwrite(&anterior, sizeof(tProduto), 1, arq);
    }

    fseek(arq, sizeof(int) + pos * sizeof(tProduto), SEEK_SET);
    fwrite(produto, sizeof(tProduto), 1, arq);

    if(flag) puts("Produto cadastrado com sucesso!");

    return true;
}

// essa funcao executa um menu para a criacao de um produto e retorna ele proprio
// this function executes a menu for the creation of a product and returns it
tProduto inputProdutoTeclado()
{   
    char aux[100];
    int n;
    tProduto prod;
    do
    {
        printf("%s", "\nNome do produto: ");
        scanf(" %[^\n]", aux);
        getchar();
        n = strlen(aux);
        if (n >= 50)
            puts("Nome muito grande!");
        else
            strcpy(prod.nome_prod, aux);
    } while (n >= 50);
    do
    {
        printf("%s", "\nCategoria: ");
        scanf(" %[^\n]", aux);
        getchar();
        n = strlen(aux);
        if (n >= 25)
            puts("Nome muito grande!");
        else
            strcpy(prod.categoria, aux);
    } while (n >= 50);
    do
    {
        printf("%s", "\nNome do fornecedor: ");
        scanf(" %[^\n]", aux);
        getchar();
        n = strlen(aux);
        if (n >= 50)
            puts("Nome muito grande!");
        else
            strcpy(prod.nome_fornec, aux);
    } while (n >= 50);
    do
    {
        printf("%s", "\nQuantidade em estoque: ");
        scanf("%s", aux);
        getchar();
        prod.qnt_estoque = atoi(aux);
        if (prod.qnt_estoque < 0)
            puts("Quantidade invalida!");
    } while (prod.qnt_estoque < 0 || !testeInputInt(aux));
    do
    {
        printf("%s", "\nPreco: ");
        scanf("%s", aux);
        getchar();
        prod.preco = atof(aux);
        if (prod.preco <= 0)
            puts("Valor invalido!");
    } while (prod.preco <= 0 || !testeInputFloat(aux));
    do
    {
        printf("%s", "\nID: ");
        scanf("%s", aux);
        getchar();
        prod.id_prod = atoi(aux);
        if (prod.id_prod <= 0)
            puts("ID invalido!");
    } while (prod.id_prod <= 0 || !testeInputInt(aux));
    do
    {
        printf("%s", "\nPeso (em gramas): ");
        scanf("%s", aux);
        getchar();
        prod.peso = atoi(aux);
        if (prod.peso <= 0)
            puts("Peso invalido!");
    } while (prod.peso <= 0 || !testeInputInt(aux));

    return prod;
}

// essa funcao realiza a leitura de um arquivo txt contendo informacoes para a criacao de n produtos
// ela retorna um array de produtos
// this function reads a txt file containing information for the creation of n products
// it returns an array of products
bool inputProdutoArquivo(char *nome, int n, tProduto *prod)
{
    FILE *arq = fopen(nome, "r");
    if (arq == NULL)
    {
        puts("Erro ao abrir o arquivo!");
        return false;
    }

    for (int i = 0; i < n; i++, prod++)
    {
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

// essa funcao le um csv em um formato especifico para a criacao de novos tProdutos
// ela retorna um array de tProdutos
// this function reads a csv in a specific format for the creation of new tProdutos
// it returns an array of tProdutos
void lerCSV(char *nome, tProduto *prod)
{
    FILE *csv = fopen(nome, "r");
    int n = 0;

    if (csv == NULL)
    {
        puts("Erro ao abrir o arquivo!");
        return;
    }
    char linha[256];
    fgets(linha, sizeof(linha), csv);

    while (!feof(csv))
    {
        fscanf(csv, "%[^,],%[^,],%[^,],%d,%f,%d,%d\n", prod[n].nome_prod, prod[n].categoria, prod[n].nome_fornec, &prod[n].qnt_estoque, &prod[n].preco, &prod[n].id_prod, &prod[n].peso);
        n++;
    }

    fclose(csv);
    return;
}