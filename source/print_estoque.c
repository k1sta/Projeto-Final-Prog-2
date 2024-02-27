#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "universal.h"

// funcao para printar o estoque de um arq bin (entrando com o arquivo ja aberto)
// function to print the stock of a bin file (entering with the file already open)
void printarEstoque(FILE *arq)
{
    rewind(arq);
    int n = numProd(arq);
    tProduto produto;
    printf("\e[1;1H\e[2J"); // Limpa o console, mas nao permite ver algumas mensagens de erro
    printf("%s%-4s | %-15s | %-15s | %-15s | %-15s | %-15s | %-10s%s\n", BWHT, "ID", "Nome do produto", "Categoria", "Fornecedor", "Qtd Estoque", "Preco unitario", "Peso", SEMCOR);
    printf("-----|-----------------|-----------------|-----------------|-----------------|-----------------|------------\n");

    for (int i = 0; i < n; i++)
    {
        fread(&produto, sizeof(tProduto), 1, arq);
        printf("%-4d | %-15s | %-15s | %-15s | %-15d | %-15.2f | %-10d \n", produto.id_prod, produto.nome_prod, produto.categoria, produto.nome_fornec, produto.qnt_estoque, produto.preco, produto.peso);
    }
}

// funcao para criar um arquivo .csv a partir do estoque em arq bin (entrando com o arquivo ja aberto)
// function to create a .csv file from the stock in bin file (entering with the file already open)
bool criar_csv(FILE *arq)
{
    FILE *csv;
    tProduto produto;

    csv = fopen("estoque.csv", "w+");
    if (csv == NULL)
    {
        printf("Erro na criação do arquivo csv. \n");
        return false;
    }

    fprintf(csv, "nome,categoria,fornecedor,qntd,preco,id,peso");

    rewind(arq);

    while (fread(&produto, sizeof(tProduto), 1, arq) == 1)
    {

        fprintf(csv, "\n%s,%s,%s,%d,%.2f,%d,%d",
                produto.nome_prod, produto.categoria, produto.nome_fornec,
                produto.qnt_estoque, produto.preco, produto.id_prod, produto.peso);
    }

    fclose(csv);
    puts("Arquivo criado!");
    return true;
}

// essa funcao ordena o estoque por preco, id ou quantidade em estoque baseado no input do usuario
// this function sorts the stock by price, id or quantity in stock based on user input
void ordena_estoque(FILE *arq)
{
    rewind(arq);
    int tam = numProd(arq);
    tProduto *prod = malloc(sizeof(tProduto) * tam);
    int resposta;
    char aux[20];

    FILE *arq2 = fopen("estoque_ordenado.dat", "wb+");
    if (arq2 == NULL)
    {
        puts("Erro ao alocar o arquivo.");
        return;
    }

    fread(prod, sizeof(tProduto), tam, arq);

    printf("\nGostaria de visualizar o estoque ordenado por:\n%s[1]%s ID\n%s[2]%s Preco\n%s[3]%s Quantidade no estoque", BCYN, SEMCOR, BCYN, SEMCOR, BCYN, SEMCOR);
    do{
    printf("\nInput: ");
    scanf("%s", aux);
    getchar();
    }while(!testeInputInt(aux));
    resposta = atoi(aux);

    switch (resposta)
    {
    case 1: // id
        for (int i = tam - 1; i > 0; i--)
        {
            for (int j = 0; j < i; j++)
            {
                if (prod[j].id_prod > prod[j + 1].id_prod)
                {

                    tProduto temp = prod[j];
                    prod[j] = prod[j + 1];
                    prod[j + 1] = temp;
                }
            }
        }
        break;
    case 2: // preco
        for (int i = tam - 1; i > 0; i--)
        {
            for (int j = 0; j < i; j++)
            {
                if (prod[j].preco > prod[j + 1].preco)
                {

                    tProduto temp = prod[j];
                    prod[j] = prod[j + 1];
                    prod[j + 1] = temp;
                }
            }
        }
        break;
    case 3: // quantidade em estoque
        for (int i = tam - 1; i > 0; i--)
        {
            for (int j = 0; j < i; j++)
            {
                if (prod[j].qnt_estoque > prod[j + 1].qnt_estoque)
                {

                    tProduto temp = prod[j];
                    prod[j] = prod[j + 1];
                    prod[j + 1] = temp;
                }
            }
        }
        break;
    }
    rewind(arq);
    fwrite(prod, sizeof(tProduto), tam, arq2);

    printarEstoque(arq2);

    printf("\nPara gerar um csv do estoque ordenado, digite %s[1]%s.", BCYN, SEMCOR);
    printf("\nCaso contrario, digite outro numero para voltar diretamente ao menu.\n");
    do{
    scanf("%s", aux);
    getchar();
    }while(!testeInputInt(aux));
    
    if(resposta==1){
        criar_csv(arq2);
    }

    fclose(arq2);
    fclose(arq);

    remove("estoque_ordenado.dat");

    free(prod);

    arq = fopen("produtos.dat", "rb+");
}