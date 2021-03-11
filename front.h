#ifndef __FRONT_H__
#define __FRONT_H__

#include <ncurses.h>

#define SIZE_MAIN_MENU 6
#define SIZE_SEARCH_MENU 4
#define MAIN_MENU_STARTY 4
#define SEARCH_MENU_STARTY 3

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

#endif
