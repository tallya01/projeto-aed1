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

void add_book(WINDOW * menu){
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
    livro.exemplares_disponiveis = livro.quantidade_exemplares;

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

void search_and_print_books(WINDOW * menu, int choice){

    wclear(menu);
    box(menu, 0,0);
    Livro *book = (Livro*) malloc(sizeof(Livro)), *n_genre_book;
    FILE *file = fopen("books.dat", "rb");
    int achou = 0, k = 1, i, n_genre=0, *genre_select, action = 0;
    char sub_string[101];

    if(file == NULL){
        mvwprintw(menu, 1,1, "Erro ao abrir o arquivo, digite qualquer tecla para retornar ao menu");
        wrefresh(menu);
        getch();
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
                    book[k-1].exemplares_disponiveis = livro.exemplares_disponiveis;

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

            i=0;
            while(1){
                wclear(menu);
                box(menu,0,0);
                curs_set(0);
                mvwprintw(menu,1,1, "Exibindo resultado %d de %d:", i+1, k-1);
                mvwprintw(menu, 3,1, "Livro: %s", book[i].nome_livro);
                mvwprintw(menu, 4,1, "Autor: %s", book[i].nome_autor);
                mvwprintw(menu, 5,1, "Gênero: %s", get_genre(book[i].genre));
                mvwprintw(menu, 6,1, "Ano de publicação: %d", book[i].ano_publicacao);
                mvwprintw(menu, 7,1, "Total de exemplares: %d (%d disponíveis)", book[i].quantidade_exemplares, book[i].exemplares_disponiveis);
                mvwaddstr(menu, 9,1, "Setas esquerda e direita para navegar pelos resultados");
                mvwaddstr(menu, 10,1, "F1 para emprestar livro | F2 para voltar ao menu principal");
                wrefresh(menu);
                action = getch();
                switch(action){
                    case KEY_LEFT:
                        if(i==0) break;
                        else{
                            i--;
                            break;
                        }
                    case KEY_RIGHT:
                        if(i==k-2) break;
                        else{
                            i++;
                            break;
                        }
                    case KEY_F(1):
                        lend_book(&book[i], menu);
                        return;
                    case KEY_F(2):
                        fclose(file);
                        free(book);
                        return;
                }
            }
            
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
                mvwprintw(menu, 3,1, "Não possuímos livros com esse autor meu caro/a");
                fclose(file);
                free(book);
                mvwprintw(menu, 4,1, "pressione qualquer tecla para voltar");
                wrefresh(menu);
                getch();
                return;
            }

            i=0;
            while(1){
                wclear(menu);
                box(menu,0,0);
                curs_set(0);
                mvwprintw(menu,1,1, "Exibindo resultado %d de %d:", i+1, k-1);
                mvwprintw(menu, 3,1, "Livro: %s", book[i].nome_livro);
                mvwprintw(menu, 4,1, "Autor: %s", book[i].nome_autor);
                mvwprintw(menu, 5,1, "Gênero: %s", get_genre(book[i].genre));
                mvwprintw(menu, 6,1, "Ano de publicação: %d", book[i].ano_publicacao);
                mvwprintw(menu, 7,1, "Total de exemplares: %d (%d disponíveis)", book[i].quantidade_exemplares, book[i].exemplares_disponiveis);
                mvwaddstr(menu, 9,1, "Setas esquerda e direita para navegar pelos resultados");
                mvwaddstr(menu, 10,1, "F1 para emprestar livro | F2 para voltar ao menu principal");
                wrefresh(menu);
                action = getch();
                switch(action){
                    case KEY_LEFT:
                        if(i==0) break;
                        else{
                            i--;
                            break;
                        }
                    case KEY_RIGHT:
                        if(i==k-2) break;
                        else{
                            i++;
                            break;
                        }
                    case KEY_F(1):
                        lend_book(&book[i], menu);
                        return;
                    case KEY_F(2):
                        fclose(file);
                        free(book);
                        return;
                }
            }
        
        case 3:
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
                mvwprintw(menu, 3+SIZE_GENRE_MENU,1, "Não possuímos livros com esse gênero meu caro/a");
                fclose(file);
                free(book);
                free(genre_select);
                mvwprintw(menu, 4+SIZE_GENRE_MENU,1, "pressione qualquer tecla para voltar");
                wrefresh(menu);
                getch();
                return;
            }

            i=0;
            while(1){
                wclear(menu);
                box(menu,0,0);
                curs_set(0);
                mvwprintw(menu,1,1, "Exibindo resultado %d de %d:", i+1, k-1);
                mvwprintw(menu, 3,1, "Livro: %s", book[i].nome_livro);
                mvwprintw(menu, 4,1, "Autor: %s", book[i].nome_autor);
                mvwprintw(menu, 5,1, "Gênero: %s", get_genre(book[i].genre));
                mvwprintw(menu, 6,1, "Ano de publicação: %d", book[i].ano_publicacao);
                mvwprintw(menu, 7,1, "Total de exemplares: %d (%d disponíveis)", book[i].quantidade_exemplares, book[i].exemplares_disponiveis);
                mvwaddstr(menu, 9,1, "Setas esquerda e direita para navegar pelos resultados");
                mvwaddstr(menu, 10,1, "F1 para emprestar livro | F2 para voltar ao menu principal");
                wrefresh(menu);
                action = getch();
                switch(action){
                    case KEY_LEFT:
                        if(i==0) break;
                        else{
                            i--;
                            break;
                        }
                    case KEY_RIGHT:
                        if(i==k-2) break;
                        else{
                            i++;
                            break;
                        }
                    case KEY_F(1):
                        lend_book(&book[i], menu);
                        return;
                    case KEY_F(2):
                        fclose(file);
                        free(book);
                        free(genre_select);
                        return;
                }
            }
    }
}

/* falta atualizar no "books.dat" a quantidade de exemplares
disponíveis depois de emprestar um livro */
void lend_book(Livro * book, WINDOW * win){
    Emprestimo lending;
    Livro li;
    FILE * arq = fopen("lent.dat", "ab+");
    FILE * livro = fopen("books.dat", "rb+");
    time_t rawtime;
    struct tm * date;
    int i;
    time(&rawtime);
    date = localtime(&rawtime);

    wclear(win);
    box(win,0,0);

    if(book->exemplares_disponiveis==0){
        mvwaddstr(win, 1,1, "Não há exemplares disponíveis desse livro!");
        fclose(arq);
        fclose(livro);
        wrefresh(win);
        getch();
        return;
    }

    strcpy(lending.nome_livro, book->nome_livro);

    //atualiza a quantidade de exemplares disponíveis no books.dat
    while(fread(&li, sizeof(Livro), 1, livro)==1){
        if(strcmp(li.nome_livro, lending.nome_livro)==0){
            fseek(livro, (-1)*sizeof(Livro), SEEK_CUR);
            --li.exemplares_disponiveis;
            fwrite(&li, sizeof(Livro), 1, livro);
            fclose(livro);
            break;
        }
    }
    
    mvwaddstr(win, 1,1, "Digite o nome da pessoa que está retirando o livro:");
    curs_set(1);
    echo();
    wrefresh(win);
    mvwscanw(win, 2,1, "%[^\n]", &lending.nome_pessoa);
    curs_set(0);
    noecho();
    lending.data_retirada.dd = date->tm_mday;
    lending.data_retirada.mm = date->tm_mon+1;
    lending.data_retirada.yy = date->tm_year+1900;
    lending.hora_retirada.hh = date->tm_hour;
    lending.hora_retirada.mm = date->tm_min;
    return_book_date(&lending);

    if(fwrite(&lending, sizeof(Emprestimo), 1, arq)){
        mvwprintw(win, 4,1, "Empréstimo registrado com sucesso, data de devolução %d/%d/%d", lending.data_devolucao.dd, lending.data_devolucao.mm, lending.data_devolucao.yy);
        wrefresh(win);
        fclose(arq);
        getch();
    }
    else{
        mvwaddstr(win, 4,1, "Empréstimo mal sucedido, tente novamente");
        wrefresh(win);
        fclose(arq);
        getch();
    }

}

void see_lent_books(WINDOW * win){
    FILE * arq = fopen("lent.dat", "rb");
    int i = 1, k = 0, action = 0;
    Emprestimo * lent = (Emprestimo *) malloc(sizeof(Emprestimo)), * temp;

    while(fread(&emprestimo, sizeof(Emprestimo), 1, arq)){
       memcpy(lent[i-1].nome_livro, emprestimo.nome_livro, sizeof(emprestimo.nome_livro)+1);
       memcpy(lent[i-1].nome_pessoa, emprestimo.nome_pessoa, sizeof(emprestimo.nome_pessoa)+1);
       lent[i-1].hora_retirada.hh = emprestimo.hora_retirada.hh;
       lent[i-1].hora_retirada.mm = emprestimo.hora_retirada.mm;
       lent[i-1].data_retirada.dd = emprestimo.data_retirada.dd;
       lent[i-1].data_retirada.mm = emprestimo.data_retirada.mm;
       lent[i-1].data_retirada.yy = emprestimo.data_retirada.yy;
       lent[i-1].data_devolucao.dd = emprestimo.data_devolucao.dd;
       lent[i-1].data_devolucao.mm = emprestimo.data_devolucao.mm;
       lent[i-1].data_devolucao.yy = emprestimo.data_devolucao.yy;

       i++;
       temp = (Emprestimo *) realloc(lent, sizeof(Emprestimo)*i);
       lent = temp;
    }

    k=0;
    while(1){
        wclear(win);
        box(win,0,0);
        curs_set(0);
        mvwprintw(win, 1,1, "Exibindo resultado %d de %d", k+1, i-1);
        mvwprintw(win, 3,1, "Nome do livro: %s", lent[k].nome_livro);
        mvwprintw(win, 4,1, "Nome do retirante: %s", lent[k].nome_pessoa);
        mvwprintw(win, 5,1, "Data e hora de retirada: %d/%d/%d %d:%d", lent[k].data_retirada.dd, lent[k].data_retirada.mm, lent[k].data_retirada.yy, lent[k].hora_retirada.hh, lent[k].hora_retirada.mm);
        mvwprintw(win, 6,1, "Prazo para devolução: %d/%d/%d", lent[k].data_devolucao.dd, lent[k].data_devolucao.mm, lent[k].data_devolucao.yy);
        mvwaddstr(win, 8,1, "Setas esquerda e direita para navegar pelos resultados");
        mvwaddstr(win, 9,1, "F1 para voltar");
        wrefresh(win);
        action = getch();
        switch(action){
            case KEY_LEFT:
                if(i==0) break;
                else{
                    k--;
                    break;
                }
            case KEY_RIGHT:
                if(i==k-2) break;
                else{
                    k++;
                    break;
                }
            case KEY_F(1):
                free(lent);
                fclose(arq);
                return;
        }
    }

    mvwaddstr(win, i,1, "Pressione qualquer tecla para voltar");
    wrefresh(win);
    getch();
    fclose(arq);
}

int compare_date(Data * data){
    time_t rawtime;
    struct tm * date;
    time(&rawtime);
    date = localtime(&rawtime); //coleta a data e hora atuais

    if(data->yy<date->tm_year+1900) return 0;
    else if(data->yy>date->tm_year+1900) return 1;
    else if(data->mm<date->tm_mon+1) return 0;
    else if(data->mm>date->tm_mon+1) return 1;
    else if(data->dd<date->tm_mday) return 0;
    else if(data->dd>date->tm_mday) return 1;

    return 1;
}

void return_book_date(Emprestimo * lend){
    time_t rawtime;
    struct tm * date;
    time(&rawtime);
    date = localtime(&rawtime);

    if(date->tm_mday+7>number_of_days(date->tm_mon+1, date->tm_year+1900)){
        if(date->tm_mon+1>12){
            lend->data_devolucao.yy = date->tm_year+1901;
            lend->data_devolucao.mm = 1;
        } 
        else{
            lend->data_devolucao.yy = date->tm_year+1900;
            lend->data_devolucao.mm = date->tm_mon+2;
        }
        lend->data_devolucao.dd = (date->tm_mday+7)-number_of_days(date->tm_mon+1, date->tm_year+1900);
    }
    else{
        lend->data_devolucao.dd = date->tm_mday+7;
        lend->data_devolucao.mm = date->tm_mon+1;
        lend->data_devolucao.yy = date->tm_year+1900;
    }
}

int leap_year(int ano){
    if(ano % 400 == 0 || (ano % 100 != 0 && ano % 4 == 0))
    return 1; // ano bissexto
    return 0; // ano não bissexto
}

int number_of_days(int mes, int ano){
    switch(mes){
        case 1: return 31;
        case 2: if(leap_year(ano)==1) return 29;
            else return 28;
        case 3: return 31;
        case 4: return 30;
        case 5: return 31;
        case 6: return 30;
        case 7: return 31;
        case 8: return 31;
        case 9: return 30;
        case 10: return 31;
        case 11: return 30;
        case 12: return 31; //é natal é natal ...
        default: return -1; //usuário inseriu um mês inválido 
    }
}
