#include<ncurses.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

int delay = 100000;
int made_or_load_user();
void pair_colors();
int main(){
    initscr();
    start_color();
    pair_colors();
    keypad(stdscr , TRUE);
    mvprintw(15 ,40, "press any key to continue");
    getch();
    made_or_load_user();
    endwin();
    return 0;
}

void pair_colors(){
    init_pair(1,COLOR_BLACK,COLOR_WHITE);
}
int made_or_load_user(){
    clear();
    noecho();
    int which = 0;
    int key;
    move(6,1);
    mvprintw(6,1,"create new user");
    mvprintw(7,1,"login");
    mvprintw(8,1,"game menu");
    mvprintw(9,1,"profile menu");
    mvprintw(10,1,"Hall of Heroes");
    while (1)
    {
        key = getch();
        if (key == KEY_UP && which > 0)
        {
            which--;
        }else if (key == KEY_DOWN &&  which < 4)
        {
            which ++;
        }
        clear();
        if (which == 0)
        {
            attron(COLOR_PAIR(1));
            mvprintw(6,1,"create new user");
            attroff(COLOR_PAIR(1));
            mvprintw(7,1,"login");
            mvprintw(8,1,"game menu");
            mvprintw(9,1,"profile menu");
            mvprintw(10,1,"Hall of Heroes");
        }else if (which == 1)
        {
            mvprintw(6,1,"create new user");
            attron(COLOR_PAIR(1));
            mvprintw(7,1,"login");
            attroff(COLOR_PAIR(1));
            mvprintw(8,1,"game menu");
            mvprintw(9,1,"profile menu");
            mvprintw(10,1,"Hall of Heroes");
        }else if (which == 2)
        {
            mvprintw(6,1,"create new user");
            mvprintw(7,1,"login");
            attron(COLOR_PAIR(1));
            mvprintw(8,1,"game menu");
            attroff(COLOR_PAIR(1));
            mvprintw(9,1,"profile menu");
            mvprintw(10,1,"Hall of Heroes");
        }else if (which == 3)
        {
            mvprintw(6,1,"create new user");
            mvprintw(7,1,"login");
            mvprintw(8,1,"game menu");
            attron(COLOR_PAIR(1));
            mvprintw(9,1,"profile menu");
            attroff(COLOR_PAIR(1));
            mvprintw(10,1,"Hall of Heroes");
        }else if (which == 4)
        {
            mvprintw(6,1,"create new user");
            mvprintw(7,1,"login");
            mvprintw(8,1,"game menu");
            mvprintw(9,1,"profile menu");
            attron(COLOR_PAIR(1));
            mvprintw(10,1,"Hall of Heroes");
            attroff(COLOR_PAIR(1));
        }
        
        
        if (key == '\n')
        {
            break;
        }
    }
    return which;
}