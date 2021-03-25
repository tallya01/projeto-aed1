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

char * ordenacao_menu[3] = {
    "1. Nome do Livro",
    "2. Nome do Autor",
    "3. Ano de Publicação",
};

void add_book(WINDOW * menu){
    wclear(menu);
    box(menu,0,0);
    wrefresh(menu);
    FILE *p;
    int * genre_select, var = 0;

    mvwaddstr(menu, 1,1, "Digite o nome do Livro: ");
    echo();
    curs_set(1);
    mvwscanw(menu, 2,1, "%[^\n]", livro.nome_livro);
    var = verify_book(livro.nome_livro);
    if(var == 1){
        mvwaddstr(menu, 3,1, "Livro já cadastrado");
        mvwaddstr(menu, 4,1, "Digite qualquer tecla para sair...");
        wrefresh(menu);
        wclear(menu);
        getch();
        return;
    }

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
        mvaddstr(11+SIZE_GENRE_MENU,1, "Livro salvo com sucesso");
        fclose(p);
        curs_set(0);
    }else{
        mvaddstr(11+SIZE_GENRE_MENU,1, "Erro ao salvar o livro");
        curs_set(0);
    }

    free(genre_select);
    
    mvwprintw(menu, 12+SIZE_GENRE_MENU,1, "Digite qualquer tecla para sair...");
    wrefresh(menu);
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

int verify_book(char *nomeLivro){
    FILE *file = fopen("books.dat", "rb");
    if(file == NULL) return 0;
    int count = 0;
    Livro temp;

    while(fread(&temp, sizeof(Livro), 1, file)==1){
        if(strcmp(temp.nome_livro, nomeLivro) == 0) count++;
    }
    fclose(file);
    if(count>0) return 1;
    else return 0;
}

void swap_livros(Livro *book, int i, int j){
    Livro *temp = (Livro*) malloc(sizeof(Livro));
    
    temp[0] = book[i];
    book[i] = book[j];
    book[j] = temp[0];

    free(temp);
}

void order_books(Livro *book, int tamanho, int choice){
    /*
        "1. Nome do Livro",
        "2. Nome do Autor",
        "3. Ano de Publicação",
    */
    int x = 0, menor = 0, i, j;

    switch(choice){
        case 1:
            for(i=0; i<tamanho-1; i++){
            x = 0;
            menor = i;
            for(j=i+1; j<tamanho-1; j++){
                if(book[i].nome_livro[0] > book[j].nome_livro[0]) swap_livros(book, i, j);
            }
        }
        break;
        case 2:
            for(i=0; i<tamanho-1; i++){
            x = 0;
            menor = i;
            for(j=i+1; j<tamanho-1; j++){
                x = 0;
                while(book[menor].nome_autor[x] == book[j].nome_autor[x]){
                    x++;
                }
                if(book[menor].nome_autor[x] > book[j].nome_autor[x]){
                    menor  = j;
                }
                if(menor != i){
                    swap_livros(book, menor, i);
                }
            }
        }
        break;
        case 3:
            for(i=0; i<tamanho-1; i++){
                for(j = i+1; j<tamanho-1; j++){
                    if(book[i].ano_publicacao > book[j].ano_publicacao){
                        swap_livros(book, i, j);
                    }
                }
            }
        break;
    }
}

void search_and_print_books(WINDOW * menu, int choice){

    wclear(menu);
    box(menu, 0,0);
    Livro *book = (Livro*) malloc(sizeof(Livro)), *n_genre_book;
    FILE *file = fopen("books.dat", "rb");
    int achou = 0, k = 1, i, j, n_genre=0, *genre_select, action = 0, x = 0, menor;
    char sub_string[101];
    int *ordenacao_choice, ordenacao = 0;

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
            mvwprintw(menu, 3,1, "Como deseja ordenar os resultados?");
            print_menu(ordenacao_menu, 3, menu, 4);
            noecho();
            curs_set(0);
            wrefresh(menu);
            ordenacao_choice = calloc(1, sizeof(int));
            *ordenacao_choice = getch();
verify_mouse_ordenacao_entry1:
            switch(*ordenacao_choice){
                case '1': ordenacao = 1;
                    break;
                case '2': ordenacao = 2;
                    break;
                case '3': ordenacao = 3;
                    break;
                case KEY_MOUSE:
                    if(getmouse(&mouse) == OK){
                        if(mouse.bstate & BUTTON1_CLICKED){
                            choice = get_choice(genre_menu, 3, mouse.x, mouse.y, 4);
                            if(choice != -1){
                                *ordenacao_choice = choice;
                                goto verify_mouse_ordenacao_entry1;
                            } 
                        }
                    }
            }
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
                mvwprintw(menu, 8,1, "Não possuímos livros com esse nome meu caro/a");
                fclose(file);
                free(book);
                mvwprintw(menu, 9,1, "pressione qualquer tecla para voltar");
                wrefresh(menu);
                getch();
                return;
            }

            order_books(book, k, ordenacao);

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
                mvwaddstr(menu, 11,1, "F3 para deletar livro");
                mvwprintw(menu, 12,1, "ordenacao %d", ordenacao);
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
                    case KEY_F(3):
                        delete_book(&book[i], menu);
                        return;
                }
            }
            
        case 2:
            curs_set(1);
            echo();
            mvwprintw(menu, 1,1, "Digite o nome do autor que deseja buscar:");
            mvwscanw(menu, 2,1, "%[^\n]", sub_string);
            mvwprintw(menu, 3,1, "Como deseja ordenar os resultados?");
            print_menu(ordenacao_menu, 3, menu, 4);
            noecho();
            curs_set(0);
            wrefresh(menu);
            ordenacao_choice = calloc(1, sizeof(int));
            *ordenacao_choice = getch();
verify_mouse_ordenacao_entry2:
            switch(*ordenacao_choice){
                case '1': ordenacao = 1;
                    break;
                case '2': ordenacao = 2;
                    break;
                case '3': ordenacao = 3;
                    break;
                case KEY_MOUSE:
                    if(getmouse(&mouse) == OK){
                        if(mouse.bstate & BUTTON1_CLICKED){
                            choice = get_choice(genre_menu, 3, mouse.x, mouse.y, 4);
                            if(choice != -1){
                                *ordenacao_choice = choice;
                                goto verify_mouse_ordenacao_entry2;
                            } 
                        }
                    }
            }
            while(fread(&livro, sizeof(Livro), 1, file) == 1){
                if(strstr(livro.nome_autor, sub_string) != NULL){
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
                mvwprintw(menu, 8,1, "Não possuímos livros com esse autor meu caro/a");
                fclose(file);
                free(book);
                mvwprintw(menu, 9,1, "pressione qualquer tecla para voltar");
                wrefresh(menu);
                getch();
                return;
            }
            
            order_books(book, k, ordenacao);

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
                mvwaddstr(menu, 11,1, "F3 para deletar livro");
                mvwprintw(menu, 12,1, "ordenacao %d", ordenacao);
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
                    case KEY_F(3):
                        delete_book(&book[i], menu);
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
verify_mouse_genre_entry3:
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
                                goto verify_mouse_genre_entry3;
                            } 
                        }
                    }
            }
            noecho();
            mvwprintw(menu, 2+SIZE_GENRE_MENU,1, "Como deseja ordenar os resultados?");
            print_menu(ordenacao_menu, 3, menu, 3+SIZE_GENRE_MENU);
            noecho();
            curs_set(0);
            wrefresh(menu);
            ordenacao_choice = calloc(1, sizeof(int));
            *ordenacao_choice = getch();
verify_mouse_ordenacao_entry:
            switch(*ordenacao_choice){
                case '1': ordenacao = 1;
                    break;
                case '2': ordenacao = 2;
                    break;
                case '3': ordenacao = 3;
                    break;
                case KEY_MOUSE:
                    if(getmouse(&mouse) == OK){
                        if(mouse.bstate & BUTTON1_CLICKED){
                            choice = get_choice(genre_menu, 3, mouse.x, mouse.y, 3+SIZE_GENRE_MENU);
                            if(choice != -1){
                                *ordenacao_choice = choice;
                                goto verify_mouse_ordenacao_entry;
                            } 
                        }
                    }
            }

            while(fread(&livro, sizeof(Livro), 1, file) == 1){
                if(livro.genre == n_genre){
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
                mvwprintw(menu, 8+SIZE_GENRE_MENU,1, "Não possuímos livros com esse gênero meu caro/a");
                fclose(file);
                free(book);
                free(genre_select);
                mvwprintw(menu, 9+SIZE_GENRE_MENU,1, "pressione qualquer tecla para voltar");
                wrefresh(menu);
                getch();
                return;
            }

            order_books(book, k, ordenacao);

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
                mvwaddstr(menu, 11,1, "F3 para deletar livro");
                mvwprintw(menu, 12,1, "ordenacao %d", ordenacao);
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
                    case KEY_F(3):
                        delete_book(&book[i], menu);
                        return;
                }
            }
    }
}

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
    ret_book_date(&lending);

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
       lent[i-1].hora_retirada = emprestimo.hora_retirada;
       lent[i-1].data_retirada = emprestimo.data_retirada;
       lent[i-1].data_devolucao = emprestimo.data_devolucao;

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
        mvwaddstr(win, 9,1, "F1 para voltar | F2 para renovar livro | F3 devolver livro");
        wrefresh(win);
        action = getch();
        switch(action){
            case KEY_LEFT:
                if(k==0) break;
                else{
                    k--;
                    break;
                }
            case KEY_RIGHT:
                if(k==i-2) break;
                else{
                    k++;
                    break;
                }
            case KEY_F(1):
                free(lent);
                fclose(arq);
                return;
            case KEY_F(2):
                renew_book(&lent[k], win);
                return;
            case KEY_F(3):
                ret_book(&lent[k], win);
                return;
        }
    }

    mvwaddstr(win, i,1, "Pressione qualquer tecla para voltar");
    wrefresh(win);
    getch();
    fclose(arq);
}

void renew_book(Emprestimo * lending, WINDOW * win){
    FILE * arq = fopen("lent.dat", "rb+");
    int i = 5;

    wclear(win);
    box(win,0,0);

    while(fread(&emprestimo, sizeof(Emprestimo), 1, arq)){
        if(strcmp(emprestimo.nome_livro,lending->nome_livro)==0 && strcmp(emprestimo.nome_pessoa,lending->nome_pessoa)==0){
            ret_book_date(&emprestimo);
            fseek(arq, (-1)*sizeof(Emprestimo), SEEK_CUR);
            fwrite(&emprestimo, sizeof(Emprestimo), 1, arq);
            fclose(arq);
            mvwprintw(win, 1,1,"Renovação registrada! Nova data de devolução: %d/%d/%d", emprestimo.data_devolucao.dd, emprestimo.data_devolucao.mm, emprestimo.data_devolucao.yy);
            mvwaddstr(win, 3,1, "Pressione qualquer tecla para voltar.");
            wrefresh(win);
            getch();
            return;
        }
    }

    fclose(arq);
    mvwaddstr(win, 1,1,"Não foi possível registrar a devolução! Tente novamente.");
    mvwaddstr(win, 3,1, "Pressione qualquer tecla para voltar.");
    wrefresh(win);
    getch();
    return;
}

void ret_book(Emprestimo * lending, WINDOW * win){
    FILE * li = fopen("books.dat", "rb+"), * lend = fopen("lent.dat", "rb"), * temp = fopen("temp.dat", "ab");
    Livro bk;
    int i=0;

    wclear(win);
    box(win,0,0);

    //atualiza a quantidade de exemplares disponíveis
    while(fread(&bk, sizeof(Livro), 1, li)){
        if(strcmp(bk.nome_livro, lending->nome_livro)==0){
            ++bk.exemplares_disponiveis;
            fseek(li, (-1)*sizeof(Livro), SEEK_CUR);
            fwrite(&bk, sizeof(Livro), 1, li);
            fclose(li);
            break;
        }
    }

    //deleta o livro devolvido do lent.dat
    while(fread(&emprestimo, sizeof(Emprestimo), 1, lend)){
        if(strcmp(emprestimo.nome_pessoa, lending->nome_pessoa)==0 && strcmp(emprestimo.nome_livro, lending->nome_livro)==0){
            mvwaddstr(win, 1,1, "Devolução registrada!");
            mvwaddstr(win, 3,1, "Pressione qualquer tecla para voltar");
            wrefresh(win);
        }
        else{
            fwrite(&emprestimo, sizeof(Emprestimo), 1, temp);
        }
    }

    fclose(temp);
    fclose(lend);
    remove("lent.dat");
    rename("temp.dat", "lent.dat");

    getch();
    return;
}

void delete_book(Livro * book, WINDOW * win){
    FILE * arq = fopen("books.dat", "rb"), * temp = fopen("temp.dat", "ab");
    Livro bk;

    wclear(win);
    box(win,0,0);

    while(fread(&bk, sizeof(Livro), 1, arq)){
        if(strcmp(bk.nome_livro, book->nome_livro)==0){
            mvwaddstr(win,1,1,"Livro deletado com sucesso!");
            mvwaddstr(win,3,1,"Pressione qualquer tecla para voltar");
        }
        else fwrite(&bk, sizeof(Livro), 1, temp);
    }

    fclose(arq);
    fclose(temp);
    remove("books.dat");
    rename("temp.dat", "books.dat");
    wrefresh(win);
    getch();
    return;
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

void ret_book_date(Emprestimo * lend){
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
        case 12: return 31; //é natal é natal ... PEGA NO MEU PÉ
        default: return -1; //usuário inseriu um mês inválido 
    }
}
