
#include <iostream>
#include <stdlib.h>
#include <unistd.h>  //Header file for sleep(). man 3 sleep for details.
#include <pthread.h>
#include <vector>
#include <ncurses.h>
#include "Board.hpp"
using namespace std;

Board board;
int PlayerX ;
int PlayerY ;
int PlayerChar = -1; //^
pthread_mutex_t mutex;

//generuje losowa liczba ktora sluzy do okreslenia poczatkowego polozenia, 
//korzysta z id wątku w celu otrzymania losowej wartosci 
int getRandom(int max,int pthreadId){
    srand(time(NULL)*abs(pthreadId/100));
    return (rand() % max)+2;
}

//generuje losowa liczba ktora sluzy do okreslenia kierunku poruszenia sie, 
//korzysta z id wątku w celu otrzymania losowej wartosci
int getRandomMove(int max,int pthreadId){
    srand(time(NULL)*abs(pthreadId/100));
    return ((rand() % max)+1);
}


void moveInDirection(int &x, int &y, long index){
    pthread_mutex_lock(&mutex);
    board.clearAt(x,y);
    int random = getRandomMove(4,(int)pthread_self());
    string st = "Ruszyl sie watek: ";
    st.push_back(65 + index);

    switch(random){
        
        case 1:
        if(x==38){
            x=x-2;
        }
        x++;
        break;

        case 2:
        if(y==13){
            y=y-2;
        }
        y++;
        break;

        case 3:
        if(x==1){
            x=x+2;
        }
        x--;
        break;

        case 4:
        if(y==1){
            y=y+2;
        }
        y--;
        break;

    }

    
    board.addAt(x,y,index);   
    //przekazujemy string w formie tablicy znakow by poprawnie wyswietlic komunikat
    board.addStrAt(0,st.c_str());
    pthread_mutex_unlock(&mutex);
    


}

void myThreadInit(int x, int y, long index){
    pthread_mutex_lock(&mutex);

    board.addAt(x,y,index);

    pthread_mutex_unlock(&mutex);
}

void *myThreadFun(void *index){

    int x=getRandom(30,(int)pthread_self());
    int y=getRandom(10,(int)pthread_self());
    myThreadInit(x,y,(long)index);
    for(int j =0;j<1000;j++){
    sleep(getRandom(5,(int)pthread_self()));
    moveInDirection(x,y,(long)index);
    }
    pthread_exit(NULL);
}

void checkMove(int x,int y){

    
    if(board.getCharAt(x,y) !=' '){
        board.addStrAt(19,"      Koniec gry !!!!");
        pthread_exit(NULL);
    }
    
}

void *proces(void *dummy){

    PlayerX = 19;
    PlayerY = 13;
    sleep(1);
    board.addAt(PlayerX,PlayerY,PlayerChar);

    while(true){
    chtype input = board.getInput();
    switch (input)
    {
    case KEY_RIGHT:
    case 'd':
        board.clearAt(PlayerX,PlayerY);
        if(PlayerX==38){
        PlayerX = PlayerX - 1;
        }
        PlayerX++;
        checkMove(PlayerX,PlayerY);
        board.addAt(PlayerX,PlayerY,PlayerChar);
        break;
    case KEY_DOWN:
    case 's':
        board.clearAt(PlayerX,PlayerY);
        if(PlayerY==13){
        PlayerY = PlayerY - 1;
        }
        PlayerY++;
        checkMove(PlayerX,PlayerY);
        board.addAt(PlayerX,PlayerY,PlayerChar);
    break;
    case KEY_LEFT:
    case 'a':
        board.clearAt(PlayerX,PlayerY);
        if(PlayerX==1){
        PlayerX = PlayerX + 1;
        }
        PlayerX--;
        checkMove(PlayerX,PlayerY);
        board.addAt(PlayerX,PlayerY,PlayerChar);
    break;
    case KEY_UP:
    case 'w':
        board.clearAt(PlayerX,PlayerY);
        if(PlayerY==1){
        PlayerY = PlayerY + 1;
        }
        PlayerY--;
        checkMove(PlayerX,PlayerY);
        board.addAt(PlayerX,PlayerY,PlayerChar);
    break;

    default:
    break;
    }

    }
    pthread_exit(NULL);
}

void createThreads(pthread_t thread_id[],int howMany){
       
    for(long i=0;i<howMany;i++){

        pthread_create(&thread_id[i], NULL, myThreadFun, (void*) i);
        
    }
    pthread_t user;
    pthread_create(&user, NULL, proces, NULL);
    for(int i=0;i<howMany;i++){
        
        pthread_join(thread_id[i], NULL);
        
        
    }
    pthread_join(user,NULL);
}   


int main(int argc, char **argv){

    
    initscr();
    refresh();

    int howMany = atoll(argv[1]);
    pthread_t thread_id[howMany];
    cout <<"Threads: "<<howMany<<endl;

    board.init();
    pthread_mutex_init(&mutex, NULL);
    createThreads(thread_id,howMany);
    
    

    getch();
    endwin();
    pthread_mutex_destroy(&mutex);
    exit(0);
}
