#pragma once

#ifndef UNIVERSAL_H
#define UNIVERSAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define BCYN "\e[1;36m"
#define BWHT "\e[1;37m"
#define SEMCOR "\e[0m"

typedef struct
{
    char nome_prod[50];
    char categoria[15];
    char nome_fornec[50];
    int qnt_estoque;
    float preco;
    int id_prod;
    int peso;
} tProduto;

typedef struct aux
{
    int quantidade;
    tProduto produto;
    struct aux *prox;
} No;

typedef struct
{
    No *inicio;
    No *fim;
} FILA;

void inicializarFila(FILA *f);

void exibirCarrinho(FILA *f);

bool inserirNaFila(FILA *f, tProduto prod);

//------------------------------------------------------------------

//Funções principais

int buscarProduto(int id, int flag, FILE *arq);

//

void caixaRegistradora(FILE *arq);

//

int compraProdutos(int flag, FILE *arq);

//

void editarProduto(FILE *estoque);

bool modificarProduto(int id, tProduto *produto, int flag, FILE *arq);

//

void printarEstoque (FILE *arq);

bool criar_csv(FILE* arq);

//

bool removerProdutos(int *id, int n, int flag, FILE *arq);

//

int registroProdutos(FILE *arq);

tProduto inputProdutoTeclado();

bool inputProdutoArquivo(char *nome, int n, tProduto *prod);

void lerCSV(char* nome, tProduto* prod);

bool cadastrarProduto(tProduto *produto, int flag, FILE *arq);

void ordena_estoque(FILE *arq);

//-------------------------------------------------------------------------------

//Funções secundárias

void delay(int milliseconds);

int inicializarArquivo(FILE *arq);

int numProd(FILE *arq);

int qntd_produtos_csv(char* nome);

tProduto catchProduto(int id, FILE *arq);

void printProduto(tProduto produto);

#endif
