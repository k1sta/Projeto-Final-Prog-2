#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
//#include "caixa_reg.h"
#include "universal.h"


void notaFiscal(FILA *CARRINHO, float total){
    FILE *arquivo = fopen("nota_fiscal.txt", "w");
    if (arquivo == NULL) {
        puts("Erro ao criar o arquivo");
        return;
    }
    int pagamento, parcelas;

    printf("Insira a forma de pagamento: \n");
    printf("%s[1]%s Cartao de Credito\n", BCYN, SEMCOR);
    printf("%s[2]%s Cartao de Debito\n", BCYN, SEMCOR);
    printf("%s[3]%s PIX\n", BCYN, SEMCOR);
    printf("%s[4]%s Dinheiro\n", BCYN, SEMCOR);
    printf("\nInput: ");
    scanf("%d", &pagamento);
    getchar();

    // Eh necessario para fazer a data e hora instantanea
    time_t agora;
    struct tm *info_tempo;
    char data[11]; 
    char hora[9];
    time(&agora);
    info_tempo = localtime(&agora);
    strftime(data, sizeof(data), "%d/%m/%Y", info_tempo); // Formata a data
    strftime(hora, sizeof(hora), "%H:%M:%S", info_tempo); // Formata a hora


    fprintf(arquivo,"%-58s\n", "========================================================");
    fprintf(arquivo, "                     NOTA FISCAL\n");
    fprintf(arquivo,"%-58s\n", "========================================================");
    fprintf(arquivo, "Loja: Exemplo de Loja\n");
    fprintf(arquivo, "Endereço: Rua de Exemplo, 123 - Cidade, Estado\n");
    fprintf(arquivo, "CNPJ: 00.000.000/0000-00\n");
    fprintf(arquivo, "Telefone: (00) 0000-0000\n");
    fprintf(arquivo,"%-58s\n", "--------------------------------------------------------");
    fprintf(arquivo, "Data: %s   Hora: %s\n", data, hora);
    fprintf(arquivo,"%-58s\n", "--------------------------------------------------------");
    fprintf(arquivo,"%-23s | %-5s | %-10s ", "Descricao dos Produtos", "Quantidade", "Preco\n");
    fprintf(arquivo,"%-58s\n", "--------------------------------------------------------");

    No *end = CARRINHO->inicio;
    while (end != NULL) {
        fprintf(arquivo, "%-23s | %-5d | %-10.2f\n", end->produto.nome_prod, end->quantidade, end->quantidade * end->produto.preco);
        end = end->prox;
    }
    printf("\"\n");
    fprintf(arquivo, "--------------------------------------------------------\n");
    fprintf(arquivo, "Total a Pagar:                                 R$ %.2f\n", total);
    fprintf(arquivo, "--------------------------------------------------------\n");
    if (pagamento == 1) {
        printf("Deseja parcelar em quantas vezes? ");
        scanf("%d", &parcelas);
        fprintf(arquivo, "Forma de Pagamento: Cartao de Credito\n");
        fprintf(arquivo, "Parcelas: %dx de R$ %.2f sem juros\n", parcelas, total/parcelas);
    } else if (pagamento == 2) {
        fprintf(arquivo, "Forma de Pagamento: Cartao de Debito\n");
    } else if (pagamento == 3) {
        fprintf(arquivo, "Forma de Pagamento: PIX\n");
    } else if (pagamento == 4) {
        fprintf(arquivo, "Forma de Pagamento: Dinheiro\n");
    } 
    fprintf(arquivo, "========================================================\n");
    fprintf(arquivo, "Obrigado por comprar na nossa loja!\n");
    fprintf(arquivo, "Volte sempre!\n");
    fprintf(arquivo, "========================================================\n");

    fclose(arquivo); // Fecha o arquivo
    printf("Nota fiscal gerada com sucesso no arquivo nota_fiscal.txt!\n");



}

// funcao de caixa
// FUNCIONA
void caixaRegistradora(FILE *arq)
{
    int id = 0, pos;
    FILA *carrinho = (FILA *)malloc(sizeof(FILA));
    float total = 0;
    tProduto produto;

    if (!carrinho)
    {
        puts("Erro ao alocar memoria!");
        return;
    }
    inicializarFila(carrinho);

    while (id != -1)
    {
        printf("\e[1;1H\e[2J"); // Limpa o console
        exibirCarrinho(carrinho);
        puts(" ");
        printf("Total: R$%.2f\n", total);
        puts(":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::");
        puts(":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::");
        puts("Digite o ID do produto ou -1 para fechar a compra\n");
        printf("Input: ");
        scanf("%d", &id);

        if (id == -1)
        {
            printf("\e[1;1H\e[2J"); // Limpa o console
            printf("Valor a pagar: R$%.2f\n", total);
            puts("Compra finalizada!\n");
            delay(1000);
            notaFiscal(carrinho, total);
            return;
        }

        pos = buscarProduto(id, 0, arq);
        if (pos == -1)
        {
            puts("Produto nao encontrado!");
            delay(1000);
        }
        else
        {
            fseek(arq, sizeof(int) + (pos * sizeof(tProduto)), SEEK_SET);
            fread(&produto, sizeof(tProduto), 1, arq);
            produto.qnt_estoque--;
            fseek(arq, sizeof(int) + (pos * sizeof(tProduto)), SEEK_SET);
            fwrite(&produto, sizeof(tProduto), 1, arq);

            No *aux = carrinho->inicio;
            while (aux != NULL)
            {
                if (aux->produto.id_prod == id)
                {
                    aux->quantidade++;
                    break;
                }
                aux = aux->prox;
            }

            if (!aux)
            {
                inserirNaFila(carrinho, produto);
            }
            total += produto.preco;
        }
    }
    

}
