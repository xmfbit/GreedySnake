#include <stdlib.h>
#include "snake.h"
psnakeNode InitSnake(psnakeHead head)   //Initial the snake
{
	
           psnakeNode p=(psnakeNode)
			malloc(sizeof(snakeNode)*MAX_LENGTH);
	enum direction dd=head->headdirection;
    	
	
	p->curdirection=dd;
	p->backdirection=dd;
	//find the position of the body 
	switch(dd)
	{
		case DOWN:
			p->row=head->row-1;
			p->col=head->col;
			break;
		case UP:
			p->row=head->row+1;
			p->col=head->col;
			break;
		case LEFT:
			p->row=head->row;
			p->col=head->col+1;
			break;
		case RIGHT:
			p->row=head->row;
			p->col=head->col-1;
			break;
		default:
			break;
	}
	head->length++;
	head->pbody=p;
	return p;
}

int IsEnoughLong(psnakeHead head)
{
	return (head->length>MAX_LENGTH) ? 1:0;
}

void Grow(psnakeHead head)
{
	if(IsEnoughLong(head))	return ;
	snakeNode newbody;
	enum direction dd;
	int length=head->length;

	psnakeNode pfirst=head->pbody;
	psnakeNode plast=pfirst+length-1;

	//change the direction of the new body
	dd=plast->backdirection;
	newbody.curdirection=dd;
	newbody.backdirection=dd;

	//set the coordinatation of the new body
	switch(dd)
	{
		case DOWN:
			newbody.row=plast->row-1;
			newbody.col=plast->col;
			break;
		case UP:
			newbody.row=plast->row+1;
			newbody.col=plast->col;
			break;
		case LEFT:
			newbody.row=plast->row;
			newbody.col=plast->col+1;
			break;
		case RIGHT:
			newbody.row=plast->row;
			newbody.col=plast->col-1;
			break;
		default:
			break;
	}

	pfirst[length]=newbody;
	// length++
	head->length++;
}

void DisplaySnake(psnakeHead head) //Show the snake on  the stdscr
{
	int length=head->length;
	int i;
	psnakeNode pfirst=head->pbody;
	psnakeNode p;
	mvaddch(head->row,head->col,SNAKE_HEAD);
	for(i=0;i<length;i++)
	{
		p=pfirst+i;
		mvaddch(p->row,p->col,SNAKE_BODY);
	}
	refresh();
}

void FindBodyNewPosition(psnakeNode p)
{
	enum direction dd=p->curdirection;
	switch(dd)
	{
		case UP:
			p->row -= 1;
			break;
		case DOWN:
			p->row +=1;
			break;
		case LEFT:
			p->col -=1;
			break;
		case RIGHT:
			p->col +=1;
			break;
		default:
			break;
	}
}

void FindHeadNewPosition(psnakeHead p)
{
	enum direction dd=p->headdirection;
	switch(dd)
	{
		case UP:
			p->row -= 1;
			break;
		case DOWN:
			p->row +=1;
			break;
		case LEFT:
			p->col -=1;
			break;
		case RIGHT:
			p->col +=1;
			break;
		default:
			break;
	}
}

void MoveSnake(psnakeHead head)   //move the snake in the next step
{
	int length=head->length;
	int i,j;
	psnakeNode pfirst=head->pbody;
	psnakeNode p;
	FindHeadNewPosition(head);
	FindBodyNewPosition(pfirst);
	pfirst->backdirection=pfirst->curdirection;
	pfirst->curdirection=head->headdirection;
	for(i=1;i<length;i++)
	{
		p=pfirst+i;
		FindBodyNewPosition(p);
		
		p->backdirection=p->curdirection;
		p->curdirection=(p-1)->backdirection;	
	}
}

void ClearSnake(psnakeHead head)   //Clear the snake
{
	int length=head->length;
	int i;
	psnakeNode pfirst=head->pbody;
	mvaddch(head->row,head->col,' ');
	for (i=0;i<length;i++)
	{
		mvaddch((pfirst+i)->row,(pfirst+i)->col,' ');
	}
	refresh();
}

// Cotrol the snake's motion according to the key the user pressed
void ControlSnake(int key,psnakeHead head)    //Control the snake using keyboard
{
	enum direction dhead=head->headdirection;
	enum direction dd=dhead;
	switch (key)
	{
		case KEY_LEFT:
            		if(dhead!=RIGHT) {dd=LEFT;}
            		break;
        		case KEY_UP:
            		if(dhead!=DOWN) {dd=UP;}
            		break;
        		case KEY_DOWN:
            		if(dhead!=UP)      {dd=DOWN;}
           			break;
        		case KEY_RIGHT:
            		if(dhead!=LEFT)   {dd=RIGHT;}
           			break;
        		default:
           			 break; 
	}
	head->headdirection=dd;
}

//Display the food in the stdscr
position DisplayFood(psnakeHead head)
{
	position p;
	int foodrow,foodcol;
	//Generate random number
    	srand(time(0));  
    	do
    	{
    		foodcol = rand() % (COLS - 2) + 1;  
    		foodrow = rand() % (LINES - TOP_ROW - 2) + TOP_ROW + 1;  
    	}while( IsInSnake (head,foodrow,foodcol));    // the food must not in the snake
    	
    	mvaddch(foodrow, foodcol, FOOD);/* display the food */  
    	refresh();     
    	p.row=foodrow;
    	p.col=foodcol;    //return the position where the food is
    	return p;  
}

//If the position (row,col) is in the snake's body
int IsInSnake(psnakeHead head,int row,int col)
{
	psnakeNode pfirst=head->pbody;
	int length=head->length;
	int i=0;
	for(;i<length;i++)
	{
		if( (pfirst+i)->row == row && (pfirst+i)->col==col )
		{
			break;
		}
	}
	if( i == length)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

//If the game is failed
int IsFail(psnakeHead head)
{
	int row=head->row;
	int col=head->col;
	// hit the wall 
	if( row>=BUT_ROW
	   || row<=TOP_ROW
	   || col>=RIGHT_EDGE
	   || col<=LEFT_EDGE)
	{return 1;}
	
	//hit itself body
	int i=0;
	int length=head->length;
	psnakeNode pfirst=head->pbody;
	for(;i<length;i++)
	{
		if( (pfirst+i)->row==row && (pfirst+i)->col==col )
		{
			return 1;
		}
	}
	
	return 0;
}
