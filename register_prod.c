#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
//#include "register_prod.h"

#include "universal.h"

// Essa funcao e responsavel por criar um menu para a insercao de produtos no estoque
// Ela utiliza de outras funcoes de mais "baixo nivel"
// This function is responsible for creating a menu for inserting products into the stock
// It uses other functions of a lower level
int registroProdutos(FILE *arq)
{
    int n, aux, cont = 0;
    tProduto *produtos;
    char nome[50];

    while (1)
    {
        //users choice
        printf("\e[1;1H\e[2J"); // Limpa o console
        printf("%sQuer adicionar por teclado ou arquivo?%s\n", BWHT, SEMCOR);
        printf("%s[1]%s Teclado\n", BCYN, SEMCOR);
        printf("%s[2]%s Arquivo TXT\n", BCYN, SEMCOR);
        printf("%s[3]%s Arquivo CSV\n", BCYN, SEMCOR);
        printf("%sPara VOLTAR, digite qualquer outro numero%s\n", BWHT, SEMCOR);
        printf("%s", "Input: ");
        scanf("%d", &aux);

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
            puts("\nQuantos produtos deseja registrar?");
            printf("%s", "Input: ");
            scanf("%d", &n);
            produtos = (tProduto *)malloc(n * sizeof(tProduto));
            if (!produtos)
            {
                puts("\nErro ao alocar memoria!");
                return -1;
            }
            printf("%s", "\nNome do arquivo: ");
            scanf(" %[^\n]", nome);
            getchar();
            inputProdutoArquivo(nome, n, produtos);
            break;
        case 3: //csv file (need to check the format in documentation)
            printf("%s", "\nNome do arquivo: ");
            scanf(" %[^\n]", nome);
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
    tProduto prod;
    printf("%s", "\nNome do produto: ");
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