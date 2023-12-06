#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "dados.h"
#include "arqlib.h"
#include <time.h>

void menu();
void printInicio();

int main(void)
{
    menu();
    /*
    tProduto produto;
    inputProdutoArquivo("arroz.txt", 0, &produto);
    criarArquivoProdutos();
    printf("%d\n", numProd());
    cadastrarProduto(&produto, 1);
    printf("%d\n", numProd());
    buscarProduto(10, 1);
    */


    return 0;
}   

void menu()
{
    
    FILE *estoque = fopen("produtos.dat", "rb+");
    if (estoque == NULL)
    {
        puts("Erro ao abrir o arquivo\n");
        exit(1);
    }

    // inicializarArquivo(estoque);

    int id;
    int qtd = 0;
    int simnao;

    puts("");
    printInicio();
    puts("");
    while (1)
    {
        int resposta = 0;
        puts("O que gostaria de fazer? Escolha sua resposta de acordo com as instrucoes a seguir:");
        puts("[1] CADASTRAR produto");
        puts("[2] EDITAR produto");
        puts("[3] REMOVER produto");
        puts("[4] BUSCAR produto");
        puts("[5] ADICIONAR quantidade");
        puts("[6] VISUALIZAR estoque");
        puts("[7] CAIXA REGISTRADORA");
        puts("Para SAIR, digite qualquer outro numero");
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
            puts("\nDeseja criar um CSV? (Digite 1 para aceitar)");
            scanf("%d", &simnao);
            if (simnao == 1)
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
        delay(10000); ///// ALTERAR ALTERAR
        printf("\e[1;1H\e[2J"); // Limpa o console, mas nao permite ver algumas mensagens de erro
    }
}

void printInicio(){
puts("===============================================================================================");
puts(" ");
puts(" _____ _____ ____  _____  ____  ____    ____  _____   _____ ____  _____  ____  ____  _     _____");
puts("/  __//  __// ___\\/__ __\\/  _ \\/  _ \\  /  _ \\/  __/  /  __// ___\\/__ __\\/  _ \\/  _ \\/ \\ /\\/  __/");
puts("| |  _|  \\  |    \\  / \\  | / \\|| / \\|  | | \\||  \\    |  \\  |    \\  / \\  | / \\|| / \\|| | |||  \\  ");
puts("| |_//|  /_ \\___ |  | |  | |-||| \\_/|  | |_/||  /_   |  /_ \\___ |  | |  | \\_/|| \\_\\|| \\_/||  /_ ");
puts("\\____\\____\\____/  \\_/  \\_/ \\|\\____/  \\____/\\____\\  \\____\\____/  \\_/  \\____/\\____\\____/\\____\\ ");                                                                                 
puts(" ");
puts("===============================================================================================");
}