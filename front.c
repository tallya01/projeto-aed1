#include "front.h"
#include <string.h>

void print_menu(char ** menu, int menuSize, WINDOW * win, int startY){
    int i;

    for(i=0; i<menuSize; i++, startY++){
        mvwaddstr(win, startY, 1, menu[i]);
    }

    wrefresh(win);
}

int get_choice(char ** menu, int menuSize, int mouseX, int mouseY, int startY){
    int i;

    for(i=0;i<menuSize;i++){
        if(mouseX>=1 && mouseX<=strlen(menu[i])+1 && mouseY == startY+i) return i+49;
    }

    return -1;
}