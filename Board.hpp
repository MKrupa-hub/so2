#pragma once

class Board
{
public:
    Board(){
    
    init();
    }

    void addBorder(){
        box(board_win,0,0);
    }

    void addAt(int x,int y, int id){
        
        mvwaddch(board_win,y,x,65+id);
        refresh();

    }

    void addStrAt(int x,const char *str){
        
        mvwaddstr(board_win,14,0+x,str);
        refresh();

    }

    void clearAt(int x,int y){
        
        mvwaddch(board_win,y,x,32);
        refresh();
    }

    void refresh(){
        wrefresh(board_win);
    }

    chtype getInput(){

        return wgetch(board_win);

    }

    chtype getCharAt(int x, int y){

        return mvwinch(board_win,y,x);

    }

    void init(){
        board_win=newwin(15,40,5,10);
        board_win->_clear;
        keypad(board_win,true);
        addBorder();
        refresh();
    }

private:
WINDOW *board_win;

};