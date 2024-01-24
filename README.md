# Projeto-Final-Prog-2

ATENÇÂO: Esse readme não foi atualizado ainda! O projeto será continuado e atualizado.

Este repositório contém o trabalho final da disciplina ICP141 - Programação de Computadores II elaborado por:
- Pedro Kury Kitagawa
- Bruno Veras Schulz
- Marcelo Blak
- Gabriel Martins de Freire



## Introdução
O projeto consiste em um sistema de gestão de estoque para lojas de pequeno porte com interação através de um terminal e armazenamento em um arquivo .dat. Nele, é possível adicionar (através de uma formatação .csv descrita neste documento), modificar, buscar, comprar e vender produtos de maneira automatizada, além de poder exportar o estoque em um formato .csv para maior legibilidade.




## Arquivos do código fonte

Dentre os arquivos, temos:
- **dados.h**

    Essa header possui os TAD's (Tipos Abstratos de Dados) responsáveis pelo bom funcionamento do programa, além de funções relacionadas (Inicialização de Fila, Leitura, etc.)
- **arqlib.h**

    Essa header contém funções essenciais para a interação com o arquivo "produtos.dat" que contém as informações dos produtos.

- **main.c**

    Esse arquivo .c é o *core* do programa, contendo a função do menu e a chamada das funções.
- **produtos.dat**
  
    O arquivo *produtos.dat* é o "banco de dados" do software. Nele, todos os registros feitos, pelo nosso programa, são mantidos e salvos para que, após o encerramento do programa, os dados não se percam.


## Funções implementadas em dados.h

Algumas funções foram escritas em *dados.h*, temos, então:

- void inicializarFila(FILA* f)
  
A função *inicializarFila* requer, como parâmetro, um ponteiro do tipo FILA. Ela inicializa os ponteiros de início e fim da fila em NULL.

- void exibirCarrinho(FILA *f)

A função *exibirCarrinho* pede, ainda, como parâmetro, um ponteiro do tipo FILA. Ela faz uma leitura do arquivo "produtos.dat" e exibe, de maneira formatada, todos os dados (exceto o número de produtos cadastrados) do banco de dados.

## Funções implementadas em arqlib.h

continuar dps

