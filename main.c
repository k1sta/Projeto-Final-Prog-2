/*******************************************************************************************************************************************************

comando de compilacao:
gcc dados.c busca_prod.c caixa_reg.c compra_prod.c edit_prod.c func_secundarias.c print_estoque.c register_prod.c remove_prod.c main.c -o main.exe

********************************************************************************************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "universal.h"

//assinaturas de algumas funcoes implementadas aqui
void menu();
void printInicio();

int main(void)
{
    menu();
    return 0;
}

// menu principal
void menu()
{

    FILE *estoque = fopen("produtos.dat", "wb+");
    if (estoque == NULL)
    {
        puts("Erro ao abrir o arquivo\n");
        exit(1);
    }

    inicializarArquivo(estoque);

    int id;
    int qtd = 0;
    char aux[20];

    while (1)
    {
        int resposta = 0;
        printInicio();
        puts("");
        printf("%sO que gostaria de fazer? Escolha sua resposta de acordo com as instrucoes a seguir:%s\n", BWHT, SEMCOR);
        printf("%s[1]%s CADASTRAR produto\n", BCYN, SEMCOR);
        printf("%s[2]%s EDITAR produto\n", BCYN, SEMCOR);
        printf("%s[3]%s REMOVER produto\n", BCYN, SEMCOR);
        printf("%s[4]%s BUSCAR produto\n", BCYN, SEMCOR);
        printf("%s[5]%s ADICIONAR quantidade\n", BCYN, SEMCOR);
        printf("%s[6]%s VISUALIZAR estoque\n", BCYN, SEMCOR);
        printf("%s[7]%s CAIXA REGISTRADORA\n", BCYN, SEMCOR);
        printf("%sPara SAIR, digite qualquer outro numero%s\n", BWHT, SEMCOR);

        printf("\nInput: ");
        scanf("%d", &resposta);
        switch (resposta)
        {
        case 1: //cadastrar produto
            registroProdutos(estoque);
            break;
        case 2: //editar produto
            editarProduto(estoque);
            break;
        case 3: //remover produto
            do{
            printf("\nQuantos produtos deseja remover? ");
            scanf("%s", aux);
            getchar();
            if(qtd <= 0) puts("Quantidade invalida!");
            }while(qtd <= 0 || testeInputInt(aux));
            qtd = atoi(aux);

            int *apagar = (int *)malloc(sizeof(int) * qtd);
            puts("");
            for (int i = 0; i < qtd; i++)
            {
                do{
                printf("%sID %d: %s", BWHT, i+1, SEMCOR);
                scanf("%s", aux);
                getchar();
                }while(!testeInputInt(aux));
                id = atoi(aux);
                apagar[i] = id;
            }
            removerProdutos(apagar, qtd, estoque);
            free(apagar);
            break;
        case 4: //buscar produto
            do{
            printf("\nID do produto: ");
            scanf("%s", aux);
            getchar();
            }while(!testeInputInt(aux));
            id = atoi(aux);
            puts(" ");
            buscarProduto(id, 1, estoque);

            puts("\nDigite qualquer numero para voltar ao menu principal: ");
            scanf("%s", aux);
            getchar();
            break;
        case 5: //adicionar quantidade
            compraProdutos(estoque);
            break;
        case 6: //visualizar estoque
            printarEstoque(estoque);
            puts("\nO que deseja fazer?\n");
            printf("%s[1]%s Deseja criar um CSV?", BCYN, SEMCOR);
            printf("\n%s[2]%s Deseja visualizar o estoque ordenado?", BCYN, SEMCOR);
            puts("\n\nDigite qualquer outro numero para VOLTAR.");
            do{
            printf("\nInput: ");
            scanf("%s", aux);
            getchar();
            }while(!testeInputInt(aux));
            resposta = atoi(aux);
            if (resposta == 1)
            {
                criar_csv(estoque);
            }
            else if(resposta == 2){
                ordena_estoque(estoque);
            }

            break;
        case 7: //caixa registradora
            caixaRegistradora(estoque);
            break;

        default: //sair
            exit(0);
            break;
        }
        puts("");
        delay(1000);
        printf("\e[1;1H\e[2J"); // Limpa o console, mas nao permite ver algumas mensagens de erro
    }
}


//funcao para imprimir o GESTAO DE ESTOQUE.
void printInicio()
{
    puts("==================================================================================================================================================================");
    puts(" ");
    printf(BCYN);
    puts(" .d8888b.  8888888888 .d8888b.88888888888    d8888 .d88888b.    88888888b. 8888888888   88888888888 .d8888b.88888888888 .d88888b.  .d88888b.888     888888888888");
    puts("d88P  Y88b 888       d88P  Y88b   888       d88888d88P' 'Y88    8888  'Y88b888          8888       d88P  Y88b   888    d88P' 'Y88bd88P' 'Y88b88     888888       ");
    puts("888    888 88       Y88b.        888      d88P888888     888    888    888888           888       Y88b.        888    888     888888     888888     888888       ");
    puts("888        888888    'Y888b.     888     d88P 888888     888    888    8888888888       8888888    'Y888b.     888    888     888888     888888     8888888888   ");
    puts("888  88888 88           'Y88b.   888    d88P  888888     888    888    888888           888           'Y88b.   888    888     888888     888888     888888       ");
    puts("888    888 88             '888   888   d88P   888888     888    888    888888           888             '888   888    888     888888 Y8b 888888     888888       ");
    puts("Y88b  d88P 88       Y88b  d88P   888  d8888888888Y88b. .d88P    888  .d88P888           888       Y88b  d88P   888    Y88b. .d88PY88b.Y8b88PY88b. .d88P888       ");
    puts(" 'Y8888P8  888888888 'Y8888P'    888 d88P     888 'Y88888P'     8888888P' 8888888888    8888888888 'Y8888P'    888     'Y88888P'  'Y888888\\\\'Y88888P'8888888888");
    puts("");
    printf(SEMCOR);
    puts("==================================================================================================================================================================");
}
