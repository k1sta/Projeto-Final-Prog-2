#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
//#include "register_prod.h"

#include "universal.h"

// ERRO. Executa para sempre quando tenta inputar 2 produtos seguidos pelo teclado.
//  NAO SEI SE FUNCIONA ????
int registroProdutos(FILE *arq)
{
    int escolha = 1, n, escolhaAnt = 0, aux;
    tProduto *produtos;
    char nome[50];

    while (escolha == 1 || escolha == 2 || escolha == 3)
    {
        n = 1;

        printf("\e[1;1H\e[2J"); // Limpa o console
        printf("%sQuer adicionar por teclado ou arquivo?%s\n", BWHT, SEMCOR);
        printf("%s[1]%s Teclado\n", BCYN, SEMCOR);
        printf("%s[2]%s Arquivo TXT\n", BCYN, SEMCOR);
        printf("%s[3]%s Arquivo CSV\n", BCYN, SEMCOR);
        printf("%sPara VOLTAR, digite qualquer outro numero%s\n", BWHT, SEMCOR);
        printf("Input: ");
        scanf("%d", &aux);

        switch (aux)
        {
        case 1:
            produtos = (tProduto *)malloc(sizeof(tProduto));
            if (!produtos)
            {
                puts("Erro ao alocar memoria!");
                return -1;
            }
            *produtos = inputProdutoTeclado();
            break;
        case 2:
            puts("Quantos produtos deseja registrar? ");
            scanf("%d", &n);
            produtos = (tProduto *)malloc(n * sizeof(tProduto));
            if (!produtos)
            {
                puts("Erro ao alocar memoria!");
                return -1;
            }
            printf("%s", "Nome do arquivo: ");
            scanf(" %[^\n]", nome);
            getchar();
            inputProdutoArquivo(nome, n, produtos);
            break;
        case 3:
            printf("%s", "Nome do arquivo: ");
            scanf(" %[^\n]", nome);
            n = qntd_produtos_csv(nome);
            produtos = (tProduto *)calloc(n, sizeof(tProduto));
            if (!produtos)
            {
                puts("Erro ao alocar memoria!");
                return -1;
            }
            lerCSV(nome, produtos);

            break;

        default:
            return 0;
            break;
        }

        for (int i = 0; i < n; i++)
        {
            aux = cadastrarProduto(&produtos[i], 0, arq);
            if (!aux)
            {
                puts("ID Duplicado!");
                break;
            }
            int aux2 = numProd(arq);
            aux2++;
            fseek(arq, 0, SEEK_SET);
            fwrite(&aux2, sizeof(int), 1, arq);
        }
        puts("Produto cadastrado com sucesso!");

        free(produtos);

        escolhaAnt = escolha;
        delay(1000);
        printf("\e[1;1H\e[2J"); // Limpa o console, mas nao permite ver algumas mensagens de erro
    }

    return -2;
}

// essa funcao recebe um produto e o cadastra no arquivo produtos.dat
// existe um parametro flag que, se for 1, imprime uma mensagem de erro caso o arquivo nao seja aberto
bool cadastrarProduto(tProduto *produto, int flag, FILE *arq)
{
    tProduto anterior;
    int baixo = 0, alto = numProd(arq) - 1, meio, pos;
    while (baixo <= alto && baixo >= 0)
    {
        meio = (baixo + alto) / 2;
        fseek(arq, sizeof(int) + meio * sizeof(tProduto), SEEK_SET);
        fread(&anterior, sizeof(tProduto), 1, arq);
        if (produto->id_prod == anterior.id_prod)
        {
            puts("Erro: ID duplicado!");
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

    pos = baixo;
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

    delay(1000);
    return true;
}

// essa funcao recebe um produto e o retorna
//  FUNCIONA
tProduto inputProdutoTeclado()
{
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

// essa funcao recebe o nome de um arquivo que contem n produtos
//  FUNCIONA
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

// FUNCIONA
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