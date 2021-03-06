#ifndef __LIB_H__
#define __LIB_H__
#include <ncurses.h>

#define T 100

int choice;
MEVENT mouse;

#define SIZE_MAIN_MENU 4
#define SIZE_SEARCH_MENU 4
#define SIZE_GENRE_MENU 5
#define MAIN_MENU_STARTY 4
#define SEARCH_MENU_STARTY 3
#define GENRE_MENU_STARTY 6

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
 * @brief coloca uma linha horizontal da largura da tela em y
 * 
 * @param win 
 * @param x
 */
void add_hline(WINDOW * win, int x);

/**
 * @brief Coloca borda em uma janela seguindo um par de cores
 * 
 * @param win 
 * @param pair 
 */
void border_win(WINDOW * win, int pair);

/**
 * @brief Verifica se a data passada como parâmetro está
 * no passado ou não
 * 
 * @param data 
 * @return 0 se sim, 1 se não
 */
int compare_date(Data * data);

/**
 * @brief Solicita confirmação de uma ação ao usuário
 * 
 * @param win 
 * @param mode 
 * @return int 1 para confirmar, 0 para voltar
 */
int confirm_action(WINDOW * win, int mode);

/**
 * @brief Calcula a quantidade de dias passados
 * a partir de uma data
 * 
 * @param data
 * @return Quantidade de dias 
 */
int days_late(Data * data);

/**
 * @brief Deleta o registro de um livro
 * 
 * @param book 
 * @param win 
 */
void delete_book(Livro * book, WINDOW * win);

/**
 * @brief função para retornar o valor da opção escolhida pelo usuário
 * com o mouse
 * 
 * @param menu menu no qual o usuário está fazendo a escolha
 * @param menuSize número de opções do menu
 * @param mouseX coordenada X do mouse
 * @param mouseY coordenada Y do mouse
 * @param startY linha da tela na qual o menu começa
 * @return int - número da opção escolhida no menu
 */
int get_choice(char ** menu, int menuSize, int mouseX, int mouseY, int startY);

/**
 * @brief Retorna o gênero de acordo com o código
 * 
 * @param genre 
 * @return char* 
 */
char *get_genre(int genre);

/**
 * @brief Verifica se um ano é bissexto
 * 
 * @param ano 
 * @return 1 se sim, 0 se não
 */
int leap_year(int ano);

/**
 * @brief Empresta um livro
 * 
 * @param book 
 * @param win 
 */
void lend_book(Livro * book, WINDOW * win);

/**
 * @brief Retona o número de dias de um mês
 * 
 * @param mes 
 * @param ano 
 * @return int 
 */
int number_of_days(int mes, int ano);

/**
 * @brief ordena os livros de acordo com a escolha do user
 * 
 * @param book 
 * @param tamanho 
 * @param choice 
 */

void order_books(Livro *book, int tamanho, int choice);

/**
 * @brief função para imprimir um menu na tela
 * 
 * @param menu vetor contendo as strings das opções do menu 
 * @param menuSize número de opções do menu
 * @param win janela na qual o menu deverá ser impresso
 * @param startY linha da tela na qual a função deve iniciar
 */
void print_menu(char ** menu, int menuSize, WINDOW * win, int startY);

/**
 * @brief Renova o empréstimo de um livro
 * 
 * @param lending 
 * @param win 
 */
void renew_book(Emprestimo * lending, WINDOW * win);

/**
 * @brief Registra a devolução de um livro
 * 
 * @param lending 
 * @param win 
 */
void ret_book(Emprestimo * lending, WINDOW * win);

/**
 * @brief Retorna a data de devolução de um livro
 * (7 dias a partir da data atual)
 * 
 * @param lend struct do tipo Emprestimo
 */
void ret_book_date(Emprestimo * lend);

/**
 * @brief Busca e imprime livros
 * 
 * @param menu janela
 * @param choice modo de operação da função
 */
void search_and_print_books(WINDOW * menu, int choice);

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
void swap_books(Livro *book, int i, int j);

/**
 * @brief Verifica se um livro já está cadastrado no sistema
 * 
 * @param nomeLivro 
 * @return int 1 se sim, 0 se não
 */
int verify_book(char *nomeLivro);

#endif
