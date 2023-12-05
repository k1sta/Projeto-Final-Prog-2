#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "dados.h"
#include "arqlib.h"

void menu()
{
    char arqDB[20] = "produtos2.dat";
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
            
            printf("Insira o ID do produto: ");
            scanf("%d", &id);

            tProduto produto = catchProduto(id, estoque);

            if(produto.id_prod==-1){
                printf("ID inválido. \n");
                break;
            }

            printProduto(produto);

            int opcao = 1, continua = 1;
            while(continua==1){
                continua = 0;

                printf("\nQual das informações deseja alterar?\n");

                puts("1. Nome do produto");
                puts("2. Categoria");
                puts("3. Preço");
                puts("4. Quantidade");
                puts("5. Peso");
                puts("6. Fornecedor");
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
                    scanf("%f", produto.preco);
                    break;

                case 4:
                    printf("\nDigite a nova quantidade do produto: ");
                    scanf("%d", produto.qnt_estoque);
                    break;

                case 5:
                    printf("\nDigite o novo peso do produto: ");
                    scanf("%d", produto.peso);
                    break;

                case 6:
                    printf("\nDigite o novo fornecedor do produto: ");
                    scanf("%s", produto.nome_fornec);
                    break;

                default:
                    break;
                }

                if(opcao<=6 && opcao>=1){
                    modificarProduto(id, &produto, 1, estoque);
                    printf("\nAlteração realizada com sucesso! \n");
                    printf("\nDeseja modificar algum campo novamente? Digite 1 para continuar.\n");
                    scanf("%d", &continua);
                }
            }

            
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