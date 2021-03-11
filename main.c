#include <ncurses.h>
#include <string.h>
#include <locale.h>
#include "front.h"

//tentar passar isso para a biblioteca
char * main_menu[6] = {
    "1. Adicionar livro",
    "2. Buscar livro",
    "3. Ver livros emprestados",
    "4. Registrar devolução/renovação",
    "5. Deletar livro",
    "6. Sair",
};

int main(){
    int entry, exit_flag=0, choice;
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
    box(menu, 0, 0);
    mvwaddstr(menu, 1, (COLS-26)/2, "Gerenciador de Biblioteca");
    mvwaddstr(menu, 2, 1, "Clique na opção desejada ou digite o número correspondente:");
    print_menu(main_menu, 6, menu, 4);
    wrefresh(menu);

    while(exit_flag==0){
        entry = getch();
verify_mouse_entry:
        switch(entry){
            case '1':
                mvwaddstr(menu, LINES-2, 1, "Opção 1");
                wrefresh(menu);
                break;
            case '2':
                mvwaddstr(menu, LINES-2, 1, "Opção 2");
                wrefresh(menu);
                break;
            case '3':
                mvwaddstr(menu, LINES-2, 1, "Opção 3");
                wrefresh(menu);
                break;
            case '4':
                mvwaddstr(menu, LINES-2, 1, "Opção 4");
                wrefresh(menu);
                break;
            case '5':
                mvwaddstr(menu, LINES-2, 1, "Opção 5");
                wrefresh(menu);
                break;
            case '6':
                exit_flag = 1;
                break;
            case KEY_MOUSE:
                if(getmouse(&mouse) == OK){
                    if(mouse.bstate & BUTTON1_CLICKED){
                        choice = get_choice(main_menu, 6, mouse.x, mouse.y, 4);
                        if(choice != -1){
                            entry = choice;
                            goto verify_mouse_entry;
                        } 
                    }
                }
        }
    }
    
    endwin();
    return 0;
}