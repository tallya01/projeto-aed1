#include <ncurses.h>
#include <string.h>
#include <locale.h>
#include "lib.h"

//tentar passar isso para a biblioteca
char * main_menu[SIZE_MAIN_MENU] = {
    "1. Adicionar livro",
    "2. Buscar livro (+ emprestar/deletar)",
    "3. Ver livros emprestados (+ renovar/devolver)",
    "4. Sair"
};

char * search_menu[SIZE_SEARCH_MENU] = {
    "1. Por nome",
    "2. Por autor",
    "3. Por gênero",
    "4. Voltar"
};

int main(){
    int main_entry, search_entry;
    int exit_flag = 0;
    WINDOW * menu;

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
    keypad(menu, TRUE);
    init_color(COLOR_MAGENTA, 988,59,753);
    init_pair(1,COLOR_MAGENTA, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_MAGENTA);

start_menu:
    border_win(menu, 1);
    curs_set(0);
    mvwaddstr(menu, 1, (COLS-26)/2, "Gerenciador de Biblioteca");
    wattron(menu, COLOR_PAIR(2));
    mvwaddstr(menu, 2, 1, "Clique na opção desejada ou digite o número correspondente:");
    wattroff(menu, COLOR_PAIR(2));
    print_menu(main_menu, SIZE_MAIN_MENU, menu, MAIN_MENU_STARTY);
    wrefresh(menu);

    //menu principal
    while(exit_flag==0){
        main_entry = getch();

verify_mouse_main_entry:
        switch(main_entry){
            case '1':
                add_book(menu);
                wclear(menu);
                goto start_menu;
                break;
            case '2':
                wclear(menu);
                border_win(menu, 1);
                mvwaddstr(menu, 1, (COLS-13)/2, "Buscar livro");
                print_menu(search_menu, SIZE_SEARCH_MENU, menu, SEARCH_MENU_STARTY);

                while(1){
                    choice = 0;
                    search_entry = getch();

verify_mouse_search_entry:
                    //menu de busca
                    switch(search_entry){
                        case '1':
                            search_and_print_books(menu, 1);
                            wclear(menu);
                            goto start_menu;
                        case '2':
                            search_and_print_books(menu, 2);
                            wclear(menu);
                            goto start_menu;
                        case '3':
                            search_and_print_books(menu, 3);
                            wclear(menu);
                            goto start_menu;
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
                see_lent_books(menu);
                wclear(menu);
                goto start_menu;
            case '4':
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
