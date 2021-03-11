#include "back.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

void add_livro(WINDOW * menu){
    wclear(menu);
    box(menu,0,0);
    wrefresh(menu);
    FILE *p;

    mvwaddstr(menu, 1,1, "Digite o nome do Livro: ");
    echo();
    curs_set(1);
    mvwscanw(menu, 2,1, "%[^\n]", livro.nome_livro);
    mvwaddstr(menu, 3,1, "Digite o nome do Autor: ");
    mvwscanw(menu, 4,1, "%[^\n]", livro.nome_autor);
    mvwaddstr(menu, 5,1, "Digite o genero do Livro: ");
    mvwscanw(menu, 6,1, "%[^\n]", livro.genero);
    mvwaddstr(menu, 7,1, "Digite o ano de publicacao: ");
    mvwscanw(menu, 8,1, "%d", &livro.ano_publicacao);
    mvwaddstr(menu, 9,1, "Digite a quantidade de exemplares: ");
    mvwscanw(menu, 10,1, "%d", &livro.quantidade_exemplares);

    p = fopen("livros.dat", "ab+");
    if(fwrite(&livro, sizeof(livro), 1, p)){
        mvaddstr(11,1, "Livro salvo com sucesso");
        fclose(p);
        wrefresh(menu);
    }else{
        mvaddstr(11,1, "Erro ao salvar o livro");
        wrefresh(menu);
    }

    mvwprintw(menu, 13,1, "Digite qualquer tecla para sair...");
    wclear(menu);
    getch();
}