#ifndef __BACK_H__
#define __BACK_H__
#include <ncurses.h>

#define T 31

//estrutura correspondente aos dados do livro
typedef struct{
    char nome_livro[T];
    char nome_autor[T];
    char genero[T];
    int ano_publicacao;
    int quantidade_exemplares;
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

//declaração das estruturas
Livro livro;
Data data;
Hora hora;

/**
 * @brief adiciona um novo livro
 * 
 * @param win janela da qual será coletada a entrada do usuário e 
 * na qual serão impressas as mensagens necessárias
 */
void add_livro(WINDOW * menu);

#endif