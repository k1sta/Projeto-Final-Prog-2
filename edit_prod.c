#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
//#include "edit_prod.h"
#include "universal.h"

// essa funcao cria um menu para a edicao de um produto na DB
// this function creates a menu for editing a product in the DB
void editarProduto(FILE *arq)
{
    int id;
    tProduto produto;

    printf("\nInsira o ID do produto: ");
    scanf("%d", &id);

    //retorna o tProduto baseado no id inserido
    produto = catchProduto(id, arq);

    //se o produto nao for encontrado
    if (produto.id_prod == -1)
    {
        printf("\nID invalido. \n");
        delay(1000);
        return;
    }

    //menu de edicao
    int opcao = 1, continua = 1;
    while (continua == 1)
    {
        continua = 0;
        printf("\e[1;1H\e[2J"); // Limpa o console
        printProduto(produto);
        puts("");
        printf("%sQuais informacoes deseja alterar?%s\n", BWHT, SEMCOR);
        printf("%s[1]%s Nome do produto\n", BCYN, SEMCOR);
        printf("%s[2]%s Categoria\n", BCYN, SEMCOR);
        printf("%s[3]%s Preco\n", BCYN, SEMCOR);
        printf("%s[4]%s Quantidade\n", BCYN, SEMCOR);
        printf("%s[5]%s Peso\n", BCYN, SEMCOR);
        printf("%s[6]%s Fornecedor\n", BCYN, SEMCOR);
        printf("%sPara VOLTAR, digite qualquer outro numero%s\n", BWHT, SEMCOR);
        printf("\nInput: ");

        scanf("%d", &opcao);
        getchar();

        switch (opcao)
        {
        case 1:
            printf("\nDigite o novo nome do produto: ");
            scanf("%[^\n]", produto.nome_prod);
            getchar();
            break;

        case 2:
            printf("\nDigite a nova categoria do produto: ");
            scanf("%[^\n]", produto.categoria);
            getchar();
            break;

        case 3:
            printf("\nDigite o novo preco do produto: ");
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
            scanf("%[^\n]", produto.nome_fornec);
            getchar();
            break;

        default:
            break;
        }
        
        //modifica o produto no arquivo
        if (opcao <= 6 && opcao >= 1)
        {
            puts("");
            modificarProduto(id, &produto, 1, arq);
            printf("\nDeseja modificar algum campo novamente? Digite 1 para continuar.\n");
            scanf("%d", &continua);
        }
    }
}

// essa funcao recebe o id de um produto + o produto modificado e o modifica no arquivo produtos.dat
// this function receives the id of a product + the modified product and modifies it in the file produtos.dat
bool modificarProduto(int id, tProduto *produto, int flag, FILE *arq)
{
    int pos;
    rewind(arq);
    int n = numProd(arq);
    pos = buscarProduto(id, 0, arq);
    if (pos == -2)
    {
        return false;
    }
    fseek(arq, sizeof(int) + (pos * sizeof(tProduto)), SEEK_SET);
    fwrite(produto, sizeof(tProduto), 1, arq);
    if (flag)
        puts("Produto modificado com sucesso!");
    return true;
}