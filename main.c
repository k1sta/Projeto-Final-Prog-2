#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "dados.h"
#include "arqlib.h"

void menu(char *arqDB)
{
    FILE *estoque = fopen(arqDB, "wb+");
    if (estoque == NULL)
    {
        puts("Erro ao abrir o arquivo\n");
        exit(1);
    }

    inicializarArquivo(estoque);

    int id;
    int qtd = 0;

    puts("");
    printf(":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
    printf("::::::::::::::::: Seja bem-vindo ao sistema de gerencia de loja! ::::::::::::::::::::::\n");
    printf(":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
    puts("");
    while (1)
    {
        int resposta = 0;
        puts("O que gostaria de fazer? Escolha sua resposta de acordo com as instrucoes a seguir:");
        puts("1. CADASTRAR produto");
        puts("2. EDITAR produto");
        puts("3. REMOVER produto");
        puts("4. BUSCAR produto");
        puts("5. VISUALIZAR estoque");
        puts("6. CAIXA REGISTRADORA");
        puts("Para SAIR, digite qualquer outro numero");
        printf("\nInput: ");
        scanf("%d", &resposta);
        switch (resposta)
        {
        case 1:
            registroProdutos(estoque);
            break;
        case 2:
            /*
            A função modificarProduto deve ter uma comunicação com o usuário para o entendimento de quais campos ele quer modificar de um produto específico.
            O que acho que pode ser feito é que o usuário digite o ID do produto que ele quer modificar e depois o programa mostre os campos que podem ser modificados.
            */
            puts("NÃO FUNCIONA DIREITO AINDA");
            printf("ID do produto: ");
            scanf("%d", &id);
            // modificarProduto(id, &produto, 1, estoque);
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
            puts("talvez funciona?");
            printarEstoque(estoque);
            //printf("numProd: %d\n", numProd(estoque)); //debug
            char simnao;
            puts("\nDeseja criar um CSV? (S/N)");
            scanf("%c", &simnao);
            getchar();
            if (simnao == 's' || simnao - 32 == 's')
            {
                criar_csv(estoque);
            }
            break;
        default:
            exit(0);
            break;
        }
        puts("");
        // printf("\e[1;1H\e[2J"); // Limpa o console, mas nao permite ver algumas mensagens de erro
    }
}

int main(void)
{
    menu("produtos.dat");
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