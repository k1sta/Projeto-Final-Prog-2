#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "dados.h"
#include "arqlib.h"

void menu();
void printInicio();

int main(void)
{
    menu();
    return 0;
}

void menu()
{

    FILE *estoque = fopen("produtos2.dat", "rb+");
    if (estoque == NULL)
    {
        puts("Erro ao abrir o arquivo\n");
        exit(1);
    }

    // inicializarArquivo(estoque);

    int id;
    int qtd = 0;

    puts("");
    printInicio();
    puts("");
    while (1)
    {
        int resposta = 0;
        printf("%sO que gostaria de fazer? Escolha sua resposta de acordo com as instrucoes a seguir:%s\n", BWHT, SEMCOR);
        printf("%s[1]%s CADASTRAR produto\n", BCYN, SEMCOR);
        printf("%s[2]%s EDITAR produto\n", BCYN, SEMCOR);
        printf("%s[3]%s REMOVER produto\n", BCYN, SEMCOR);
        printf("%s[4]%s BUSCAR produto\n", BCYN, SEMCOR);
        printf("%s[5]%s ADIOCIONAR quantidade\n", BCYN, SEMCOR);
        printf("%s[6]%s VISUALIZAR estoque\n", BCYN, SEMCOR);
        printf("%s[7]%s CAIXA REGISTRADORA\n", BCYN, SEMCOR);
        printf("%sPara SAIR, digite qualquer outro numero%s\n", BWHT, SEMCOR);
        printf("\nInput: ");
        scanf("%d", &resposta);
        switch (resposta)
        {
        case 1:
            registroProdutos(estoque);
            break;
        case 2:
            editarProduto(estoque);
            break;
        case 3:
            printf("Quantos produtos deseja remover? ");
            scanf("%d", &qtd);
            int *apagar = (int *)malloc(sizeof(int) * qtd);
            puts("");
            for (int i = 0; i < qtd; i++)
            {
                printf("ID %d:", i + 1);
                scanf("%d", &id);
                apagar[i] = id;
            }
            removerProdutos(apagar, qtd, 0, estoque);
            break;
        case 4:
            printf("ID do produto: ");
            scanf("%d", &id);
            buscarProduto(id, 1, estoque);
            break;
        case 5:
            compraProdutos(0, estoque);
            break;
        case 6:
            printarEstoque(estoque);
            puts("\nDeseja criar um CSV?");
            puts("Digite 1 para ACEITAR ou qualquer outro numero para RECUSAR");
            printf("\nInput: ");
            scanf("%d", &resposta);
            if (resposta)
            {
                criar_csv(estoque);
            }
            break;
        case 7:
            caixaRegistradora(estoque);
            break;
        default:
            exit(0);
            break;
        }
        puts("");
        delay(1000);            ///// ALTERAR ALTERAR
        printf("\e[1;1H\e[2J"); // Limpa o console, mas nao permite ver algumas mensagens de erro
    }
}

void printInicio()
{
    puts("===============================================================================================================================================================");
    puts(" ");
    printf(BCYN);
    puts(" .d8888b.  8888888888 .d8888b.88888888888    d8888 .d88888b.   88888888b. 8888888888  88888888888 .d8888b.88888888888 .d88888b.  .d88888b.888     888888888888");
    puts("d88P  Y88b 888       d88P  Y88b   888       d88888d88P' 'Y88b  8888  'Y88b888         8888       d88P  Y88b   888    d88P' 'Y88bd88P' 'Y88b88     888888       ");
    puts("888    888 88       Y88b.        888      d88P888888     888   888    888888          888       Y88b.        888    888     888888     888888     888888       ");
    puts("888        888888    'Y888b.     888     d88P 888888     888   888    8888888888      8888888    'Y888b.     888    888     888888     888888     8888888888   ");
    puts("888  88888 88           'Y88b.   888    d88P  888888     888   888    888888          888           'Y88b.   888    888     888888     888888     888888       ");
    puts("888    888 88             '888   888   d88P   888888     888   888    888888          888             '888   888    888     888888 Y8b 888888     888888       ");
    puts("Y88b  d88P 88       Y88b  d88P   888  d8888888888Y88b. .d88P   888  .d88P888          888       Y88b  d88P   888    Y88b. .d88PY88b.Y8b88PY88b. .d88P888       ");
    puts(" 'Y8888P8  888888888 'Y8888P'    888 d88P     888 'Y88888P'    8888888P' 8888888888   8888888888 'Y8888P'    888     'Y88888P'  'Y888888\\\\'Y88888P'8888888888");
    puts("");
    printf(SEMCOR);
    puts("===============================================================================================================================================================");
}
