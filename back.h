#ifndef __BACK_H__
#define __BACK_H__
#include <ncurses.h>

#define T 100

int choice;
MEVENT mouse;

//estrutura correspondente aos dados do livro
typedef struct{
    char nome_livro[T];
    char nome_autor[T];
    int genre;
    int ano_publicacao;
    int quantidade_exemplares;
    int exemplares_disponiveis;
} Livro;

//estrutura para data
typedef struct{
    int dd;
    int mm;
    int yy;
} Data;

//estrutura para hora
typedef struct{
    int hh; //hora
    int mm; // minuto
} Hora;

typedef struct{
    char nome_livro[100];
    char nome_pessoa[100];
    Hora hora_retirada;
    Data data_retirada;
    Data data_devolucao;
} Emprestimo;

//declaração das estruturas
Livro livro;
Data data;
Hora hora;
Emprestimo emprestimo;

/**
 * @brief adiciona um novo livro
 * 
 * @param win janela da qual será coletada a entrada do usuário e 
 * na qual serão impressas as mensagens necessárias
 */
void add_book(WINDOW * menu);

/**
 * @brief Busca e imprime livros
 * 
 * @param menu janela
 * @param choice modo de operação da função
 */
void search_and_print_books(WINDOW * menu, int choice);

void lend_book(Livro * book, WINDOW * win);

/**
 * @brief Compara se a data e hora inseridas pelo usuário com as atuais
 * 
 * @param dd dia
 * @param mm mês
 * @param yy ano
 * @return int 0 se estiver no passado, 1 se não
 */
int compare_date(int dd, int mm, int yy); //adaptar parâmetros

void return_book_date(Emprestimo * lend);

int leap_year(int ano);

int number_of_days(int mes, int ano);

void see_lent_books(WINDOW * win);

#endif
