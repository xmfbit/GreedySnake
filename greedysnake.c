#include <ncurses.h>
#include <unistd.h>
#include <signal.h>
#include "snake.h"
#include <stdlib.h>
#include <sys/time.h>  

int DFL_SPEED=60;      /* snake default speed */  
int score=0;               //score the player has gotten
char scorestr[5];

psnakeHead head;   // the head node of the snake
position foodpos;   // the position of the food
int isfailed=0;       //if the player has failed the game 
int isrestart=0;     //if the game restarts
int chinput='w';

void Init_Disp()  
{  
    char wall = ' ';  
    int i, j;  
    initscr();  
    raw();                    /* close row buffer */
    cbreak();               /* put termial to CBREAK mode */  
    noecho();  
    curs_set(0);            /* set cursor invisible */  
    keypad(stdscr,TRUE);               /* recognise user's function key,F1,F2,...
    /* display some message about title and wall */  
    attrset(A_NORMAL);      /* set NORMAL first */  
    attron(A_REVERSE);      /* turn on REVERSE to display the wall */  
    for(i = 0; i < LINES; i++)  
    {  
        mvaddch(i, LEFT_EDGE, wall);  
        mvaddch(i, RIGHT_EDGE, wall);  
    }  
    for(j = 0; j < COLS; j++)  
    {  
        mvaddch(0, j, '=');  
        mvaddch(TOP_ROW, j, wall);  
        mvaddch(BUT_ROW, j, wall);  
    }  
    attroff(A_REVERSE);     /* turn off REVERSE */  
    sprintf(scorestr,"%d",score);
    mvaddstr(1, 2, "Game: snake  version: 1.0   Score :");  
    mvaddstr(1,38,scorestr);
    mvaddstr(2, 2, "Press  'A' to increse the lengh.Press 'S' to slow and 'F' to accelerate." );  
    mvaddstr(3, 2, "When the game is over,press 'q' twice to quit,press 'r' twice to restart!");  
    mvaddstr(4, 2, "       Nagivation key controls snake how to move.");  
    refresh();  
}  

void gameover()
{
    char ch;
    mvaddstr(1, 2, "Game  Over!                        ");  
    refresh();
    while(ch=getch() ,ch!= 'q'  && ch !='r')
    {;}
    if(ch=='r')
    {
        isrestart=1;
    }

}

    /* Function: set_ticker(number_of_milliseconds) 
     * Usage: arrange for interval timer to issue SIGALRM's at regular intervals 
     * Return: -1 on error, 0 for ok 
     * arg in milliseconds, converted into whole seconds and microseconds 
     * note: set_ticker(0) turns off ticker 
     */  
    int set_ticker(int n_msecs)  
    {  
        struct itimerval new_timeset;  
        long n_sec, n_usecs;  
      
        n_sec = n_msecs / 1000;                 /* int second part */  
        n_usecs = (n_msecs % 1000) * 1000L;     /* microsecond part */  
      
        new_timeset.it_interval.tv_sec = n_sec; /* set reload */  
        new_timeset.it_interval.tv_usec = n_usecs;  
      
        new_timeset.it_value.tv_sec = n_sec;    /* set new ticker value */  
        new_timeset.it_value.tv_usec = n_usecs;  
      
        return setitimer(ITIMER_REAL, &new_timeset, NULL);  
    }  

int ii=0;
void StartAlarm()
{
    signal(SIGALRM, SIG_IGN);  
     if(ii%2==0)
    {
        MoveSnake(head);
        DisplaySnake(head);
    }
    else
        ClearSnake(head);

    if(head->row==foodpos.row && head->col==foodpos.col)
    {
            //EatFood(foodpos);
        foodpos=DisplayFood(head);
        Grow(head);
        score += 10;
        sprintf(scorestr,"%d",score);
        mvaddstr(1,38,scorestr);
    }
    if(IsFail(head))    //fail
    {
        isfailed=1;
        gameover();
    }
    else if(chinput!='q')
    {
            ii++;
            signal(SIGALRM, StartAlarm);  
    }
}

void KeyDownProcess(int keyDown,psnakeHead head)
{
    if(keyDown=='a')   Grow(head);
        else if(keyDown=='f' && DFL_SPEED>=30)  {DFL_SPEED -= 10; set_ticker(DFL_SPEED); } 
            else if(keyDown=='s' && DFL_SPEED<=90)  {DFL_SPEED += 10; set_ticker(DFL_SPEED); }
    ControlSnake(keyDown,head);
}

//clear the screen in the rectangular (left,right,top,buttom)
void ClearScr(int left,int right,int top,int buttom)
{
    int i,
         j;
    for(i=top;i<=buttom;i++)
    {
        for(j=left;j<=right;j++)
        {
            mvaddch(i,j,' ');
        }
    }
}

int main()
{
 START:   head=(psnakeHead)
                            malloc(sizeof(snakeHead));
                            head->length=0;
    head->row=14;
    head->col=14;
    head->pbody=NULL;
    head->headdirection=RIGHT;
    InitSnake(head);

    Init_Disp();
    signal(SIGALRM, StartAlarm);  
    set_ticker(DFL_SPEED);  
    //StartAlarm();
    foodpos=DisplayFood(head);
    while(!isfailed && chinput !='q')
    {
        chinput=getch();
        KeyDownProcess(chinput,head);
    }
    if(isfailed)
    {
            ClearScr(LEFT_EDGE+1,RIGHT_EDGE-1,TOP_ROW+1,BUT_ROW-1);  //mark
            free (head->pbody);
            free (head);
            head=NULL;
    }
     if(isrestart)
    {
        isfailed=0;
        isrestart=0;     // clear the flag
        goto START;    // go to the START statement
    }
    endwin();
    return 0;
}
