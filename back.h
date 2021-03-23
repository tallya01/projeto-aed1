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
 * @brief Verifica se a data passada como parâmetro está
 * no passado ou não
 * 
 * @param data 
 * @return 0 se sim, 1 se não
 */
int compare_date(Data * data);

/**
 * @brief Retorna a data de devolução de um livro
 * (7 dias a partir da data atual)
 * 
 * @param lend struct do tipo Emprestimo
 */
void return_book_date(Emprestimo * lend);

/**
 * @brief Verifica se um ano é bissexto
 * 
 * @param ano 
 * @return 1 se sim, 0 se não
 */
int leap_year(int ano);

/**
 * @brief Retona o número de dias de um mês
 * 
 * @param mes 
 * @param ano 
 * @return int 
 */
int number_of_days(int mes, int ano);

/**
 * @brief Imprime na tela os livros que estão emprestados 
 * 
 * @param win 
 */
void see_lent_books(WINDOW * win);

/**
 * @brief inverte os valores de duas estrutras do tipo Livro
 * 
 * @param book ponteiro para a estrutura dos livros
 * @param i posição 1
 * @param j posição 2
 */

void swap_livros(Livro *book, int i, int j);

/**
 * @brief ordena os livros de acordo com a escolha do user
 * 
 * @param book 
 * @param tamanho 
 * @param choice 
 */

void ordena_books(Livro *book, int tamanho, int choice);

#endif