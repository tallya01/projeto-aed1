#include <ncurses.h>
#include <string.h>
#include <locale.h>
#include "front.h"

//tentar passar isso para a biblioteca
char * main_menu[SIZE_MAIN_MENU] = {
    "1. Adicionar livro",
    "2. Buscar livro",
    "3. Ver livros emprestados",
    "4. Registrar devolução/renovação",
    "5. Deletar livro",
    "6. Sair"
};

char * search_menu[SIZE_SEARCH_MENU] = {
    "1. Por nome",
    "2. Por autor",
    "3. Por gênero",
    "4. Voltar"
};

int main(){
    int main_entry, search_entry;
    int exit_flag = 0, choice;
    WINDOW * menu;
    MEVENT mouse;

    setlocale(LC_ALL, "Portuguese");
    initscr();
    clear();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    start_color();
    curs_set(0);
    mousemask(BUTTON1_CLICKED, NULL);

    wclear(stdscr);
    refresh();
    menu = newwin(LINES, COLS, 0, 0);

start_menu:
    box(menu, 0, 0);
    mvwaddstr(menu, 1, (COLS-26)/2, "Gerenciador de Biblioteca");
    mvwaddstr(menu, 2, 1, "Clique na opção desejada ou digite o número correspondente:");
    print_menu(main_menu, SIZE_MAIN_MENU, menu, MAIN_MENU_STARTY);
    wrefresh(menu);

    while(exit_flag==0){
        main_entry = getch();

verify_mouse_main_entry:
        switch(main_entry){
            case '1':
                //opção 1
                break;
            case '2':
                wclear(menu);
                box(menu, 0, 0);
                mvwaddstr(menu, 1, (COLS-13)/2, "Buscar livro");
                print_menu(search_menu, SIZE_SEARCH_MENU, menu, SEARCH_MENU_STARTY);

                while(1){
                    choice = 0;
                    search_entry = getch();

verify_mouse_search_entry:
                    switch(search_entry){
                        case '1':
                            //função de busca
                            break;
                        case '2':
                            //função de busca
                            break;
                        case '3':
                            //função de busca
                            break;
                        case '4':
                            wclear(menu);
                            goto start_menu;
                        case KEY_MOUSE:
                            if(getmouse(&mouse) == OK){
                                if(mouse.bstate & BUTTON1_CLICKED){
                                    choice = get_choice(search_menu, SIZE_SEARCH_MENU, mouse.x, mouse.y, SEARCH_MENU_STARTY);
                                    if(choice != -1){
                                        search_entry = choice;
                                        goto verify_mouse_search_entry;
                                    } 
                                }
                            }

                    }
                }
            case '3':
                //opção 3
                break;
            case '4':
                //opção 4
                break;
            case '5':
                //opção 5
                break;
            case '6':
                exit_flag = 1;
                break;
            case KEY_MOUSE:
                if(getmouse(&mouse) == OK){
                    if(mouse.bstate & BUTTON1_CLICKED){
                        choice = get_choice(main_menu, 6, mouse.x, mouse.y, 4);
                        if(choice != -1){
                            main_entry = choice;
                            goto verify_mouse_main_entry;
                        } 
                    }
                }
        }
    }
    
    endwin();
    return 0;
}
