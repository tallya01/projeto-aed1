#include "back.h"
#include "front.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

char * genre_menu[SIZE_GENRE_MENU] = {
    "1. Romance",
    "2. Fantasia",
    "3. Clássico",
    "4. Biografia",
    "5. Computação"
};

void add_livro(WINDOW * menu){
    wclear(menu);
    box(menu,0,0);
    wrefresh(menu);
    FILE *p;
    int * genre_select;

    mvwaddstr(menu, 1,1, "Digite o nome do Livro: ");
    echo();
    curs_set(1);
    mvwscanw(menu, 2,1, "%[^\n]", livro.nome_livro);
    mvwaddstr(menu, 3,1, "Digite o nome do Autor: ");
    mvwscanw(menu, 4,1, "%[^\n]", livro.nome_autor);
    mvwaddstr(menu, 5,1, "Selecione o genero do Livro: ");
    print_menu(genre_menu, SIZE_GENRE_MENU, menu, GENRE_MENU_STARTY);
    curs_set(0);
    wrefresh(menu);
    genre_select = calloc(1, sizeof(int));
    *genre_select = getch();
verify_mouse_genre_entry:
    switch(*genre_select){
        case '1': livro.genre = 1;
            break;
        case '2': livro.genre = 2;
            break;
        case '3': livro.genre = 3;
            break;
        case '4': livro.genre = 4;
            break;
        case '5': livro.genre = 5;
            break;
        case KEY_MOUSE:
            if(getmouse(&mouse) == OK){
                if(mouse.bstate & BUTTON1_CLICKED){
                    choice = get_choice(genre_menu, SIZE_GENRE_MENU, mouse.x, mouse.y, GENRE_MENU_STARTY);
                    if(choice != -1){
                        *genre_select = choice;
                        goto verify_mouse_genre_entry;
                    } 
                }
            }
    }
    curs_set(1);
    mvwaddstr(menu, 6+SIZE_GENRE_MENU,1, "Digite o ano de publicacao: ");
    mvwscanw(menu, 7+SIZE_GENRE_MENU,1, "%d", &livro.ano_publicacao);
    mvwaddstr(menu, 8+SIZE_GENRE_MENU,1, "Digite a quantidade de exemplares: ");
    mvwscanw(menu, 9+SIZE_GENRE_MENU,1, "%d", &livro.quantidade_exemplares);

    p = fopen("books.dat", "ab+");
    if(fwrite(&livro, sizeof(livro), 1, p)){
        mvaddstr(10+SIZE_GENRE_MENU,1, "Livro salvo com sucesso");
        fclose(p);
        curs_set(0);
        wrefresh(menu);
    }else{
        mvaddstr(10+SIZE_GENRE_MENU,1, "Erro ao salvar o livro");
        curs_set(0);
        wrefresh(menu);
    }

    free(genre_select);

    mvwprintw(menu, 12+SIZE_GENRE_MENU,1, "Digite qualquer tecla para sair...");
    wclear(menu);
    getch();
}

char *get_genre(int genre){

    switch(genre){
        case 1: return "Romance";
        case 2: return "Fantasia";
        case 3: return "Clássico";
        case 4: return "Biografia";
        case 5: return "Computação";
        default: return "error: invalid genre code";
    }
}

void print_books(WINDOW * menu, int choice){

    wclear(menu);
    box(menu, 0,0);
    Livro *book = (Livro*) malloc(sizeof(Livro)), *n_genre_book;
    FILE *file = fopen("books.dat", "rb");
    int achou = 0, k = 1, i, n_genre=0, *genre_select;
    char sub_string[101];

    if(file == NULL){
        mvwprintw(menu, 1,1, "Erro ao abrir o arquivo, digite qualquer tecla para retornar ao menu");
        getch();
        wrefresh(menu);
        return;
    }

    switch(choice){

        case 1:
            curs_set(1);
            echo();
            mvwprintw(menu, 1,1, "Digite o nome do livro que deseja buscar:");
            mvwscanw(menu, 2,1, "%[^\n]", sub_string);
            noecho();

            while(fread(&livro, sizeof(Livro), 1, file)){
                if(strstr(livro.nome_livro, sub_string) != NULL){
                    memcpy(book[k-1].nome_livro, livro.nome_livro, sizeof(livro.nome_livro)+1);
                    memcpy(book[k-1].nome_autor, livro.nome_autor, sizeof(livro.nome_autor)+1);
                    book[k-1].genre = livro.genre;
                    book[k-1].ano_publicacao = livro.ano_publicacao;
                    book[k-1].quantidade_exemplares = livro.quantidade_exemplares;

                    k++;
                    n_genre_book = (Livro*) realloc(book, sizeof(Livro)*k);
                    book = n_genre_book;
                    achou = 1;
                }
            }

            if(achou == 0){
                mvwprintw(menu, 3,1, "Não possuímos livros com esse nome meu caro/a");
                fclose(file);
                free(book);
                mvwprintw(menu, 4,1, "pressione qualquer tecla para voltar");
                wrefresh(menu);
                getch();
                return;
            }

            for(i=0; i<k-1; i++){
                mvwprintw(menu, 3+(4*i),1, "Livro: %s", book[i].nome_livro);
                mvwprintw(menu, 4+(4*i),1, "Autor: %s", book[i].nome_autor);
                mvwprintw(menu, 5+(4*i),1, "Gênero: %s", get_genre(book[i].genre));
                mvwprintw(menu, 6+(4*i),1, "Ano de publicação: %d, Quant. exemplares: %d", book[i].ano_publicacao, book[i].quantidade_exemplares);
            }

            fclose(file);
            free(book);
            mvwprintw(menu, (4*i)+3,1, "pressione qualquer tecla para continuar");
            wrefresh(menu);
            getch();
            return;
        case 2:
            curs_set(1);
            echo();
            mvwprintw(menu, 1,1, "Digite o nome do autor que deseja buscar:");
            mvwscanw(menu, 2,1, "%[^\n]", sub_string);
            noecho();

            while(fread(&livro, sizeof(Livro), 1, file) == 1){
                if(strstr(livro.nome_autor, sub_string) != NULL){
                    memcpy(book[k-1].nome_livro, livro.nome_livro, sizeof(livro.nome_livro)+1);
                    memcpy(book[k-1].nome_autor, livro.nome_autor, sizeof(livro.nome_autor)+1);
                    book[k-1].genre = livro.genre;
                    book[k-1].ano_publicacao = livro.ano_publicacao;
                    book[k-1].quantidade_exemplares = livro.quantidade_exemplares;

                    k++;
                    n_genre_book = (Livro*) realloc(book, sizeof(Livro)*k);
                    book = n_genre_book;
                    achou = 1;
                }
            }

            if(achou == 0){
                mvwprintw(menu, 3,1, "Não possuímos livros com esse nome meu caro/a");
                fclose(file);
                free(book);
                mvwprintw(menu, 4,1, "pressione qualquer tecla para voltar");
                wrefresh(menu);
                getch();
                return;
            }

            for(i=0; i<k-1; i++){
                mvwprintw(menu, 3+(4*i), 1, "Nome do Livro: %s", book[i].nome_livro);
                mvwprintw(menu, 4+(4*i), 1, "Nome do Autor: %s", book[i].nome_autor);
                mvwprintw(menu, 5+(4*i), 1, "Gênero: %s", get_genre(book[i].genre));
                mvwprintw(menu, 6+(4*i), 1, "Ano de publicação: %d, exemplares: %d", book[i].ano_publicacao, book[i].quantidade_exemplares);
            }

            fclose(file);
            free(book);
            mvwprintw(menu, (4*i)+3,1, "pressione qualquer tecla para continuar");
            wrefresh(menu);
            getch();
            return;
        
        case 3:;
            curs_set(1);
            echo();
            mvwprintw(menu, 1,1, "Selecione o código do gênero que deseja buscar:");
            print_menu(genre_menu, SIZE_GENRE_MENU, menu, GENRE_MENU_STARTY-4);
            curs_set(0);
            wrefresh(menu);
            genre_select = calloc(1, sizeof(int));
            *genre_select = getch();
        verify_mouse_genre_entry:
            switch(*genre_select){
                case '1': n_genre = 1;
                    break;
                case '2': n_genre = 2;
                    break;
                case '3': n_genre = 3;
                    break;
                case '4': n_genre = 4;
                    break;
                case '5': n_genre = 5;
                    break;
                case KEY_MOUSE:
                    if(getmouse(&mouse) == OK){
                        if(mouse.bstate & BUTTON1_CLICKED){
                            choice = get_choice(genre_menu, SIZE_GENRE_MENU, mouse.x, mouse.y, GENRE_MENU_STARTY-4);
                            if(choice != -1){
                                *genre_select = choice;
                                goto verify_mouse_genre_entry;
                            } 
                        }
                    }
            }
            noecho();

            while(fread(&livro, sizeof(Livro), 1, file) == 1){
                if(livro.genre == n_genre){
                    memcpy(book[k-1].nome_livro, livro.nome_livro, sizeof(livro.nome_livro)+1);
                    memcpy(book[k-1].nome_autor, livro.nome_autor, sizeof(livro.nome_autor)+1);
                    book[k-1].genre = livro.genre;
                    book[k-1].ano_publicacao = livro.ano_publicacao;
                    book[k-1].quantidade_exemplares = livro.quantidade_exemplares;

                    k++;
                    n_genre_book = (Livro*) realloc(book, sizeof(Livro)*k);
                    book = n_genre_book;
                    achou = 1;
                }
            }

            if(achou == 0){
                mvwprintw(menu, 3+SIZE_GENRE_MENU,1, "Não possuímos livros com esse nome meu caro/a");
                fclose(file);
                free(book);
                free(genre_select);
                mvwprintw(menu, 4+SIZE_GENRE_MENU,1, "pressione qualquer tecla para voltar");
                wrefresh(menu);
                getch();
                return;
            }

            for(i=0; i<k-1; i++){
                mvwprintw(menu, 2+(4*i)+SIZE_GENRE_MENU, 1, "Nome do Livro: %s", book[i].nome_livro);
                mvwprintw(menu, 3+(4*i)+SIZE_GENRE_MENU, 1, "Nome do Autor: %s", book[i].nome_autor);
                mvwprintw(menu, 4+(4*i)+SIZE_GENRE_MENU, 1, "Gênero: %s", get_genre(book[i].genre));
                mvwprintw(menu, 5+(4*i)+SIZE_GENRE_MENU, 1, "Ano de publicação: %d, exemplares: %d", book[i].ano_publicacao, book[i].quantidade_exemplares);
            }

            fclose(file);
            free(book);
            free(genre_select);
            mvwprintw(menu, (4*i)+2+SIZE_GENRE_MENU,1, "pressione qualquer tecla para continuar");
            wrefresh(menu);
            getch();
            return;
    }
}