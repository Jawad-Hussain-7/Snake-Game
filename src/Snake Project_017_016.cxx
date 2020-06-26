/* Name: Jawad Hussain & Muhammad Ali Raza
   Roll No.: 017 & 016
   Submitted to: Mr. Danish Khattak
   Final Project   */



#include "graphics.h"
#include <iostream>
#include <math.h>
#include <stdlib.h>                   
#include <ctype.h>  
#include <time.h>
#include<fstream>
#include<string.h>
#include<cstring>
using namespace std;

enum keys { Up = 'H', Down = 'P', Left = 'K', Right = 'M' };

struct CurrentPoint
{
	int x;
	int y;
};
struct snakebody
{
	int snake[2][500];
};
struct playerinfo
{
	char name[20];
	int score;
};

void starting();  // Prints opening 
char mainMenu(char);  // Controls the main menu of the game
void initializeSnake(snakebody &, int &);  // Puts snake in the middle of the map
void enterInfo(playerinfo &);  // Inputs player name
void printSnake(snakebody &, int &);  // Displays snake
void displayPlayerInfo(playerinfo &); // Displays players name and score at the bottom
void generateBoundry(); // Prints boundries
void generateObstacles(); // Prints obstacles
void generateFood(CurrentPoint &, snakebody &, int &); // Generates food on map
void printFood(CurrentPoint &); // Displays fruit
void eatFood(snakebody &, playerinfo &, CurrentPoint &, int &); // if snakes eats the food it increases the snake lenght
bool boundryCollision(snakebody &, int &); // checks snakes collision with the boundry
bool obstacleCollision(snakebody &, int &);// checks snakes collision with the obstacles
bool bodyCollision(snakebody &, int &); // checks snakes collision with its body
bool movement(snakebody &, int &, int &, int &, char &, playerinfo &, playerinfo [3], bool &); //Controls movement of snakes and changes direction
void saveGame(snakebody &, int &, int &, int &, playerinfo &, playerinfo [3], bool &); // Saves the game and records of top 3 players
bool pause(snakebody &, int &, int &, int &, playerinfo &, playerinfo [3], bool &); // Pauses the game when 'p' is pressed
void gameOver(playerinfo [3], playerinfo &); // Displays game over and record of top 3 players
void resume(snakebody &, playerinfo [3], playerinfo &, int &, int &, int &); // Loads game data
void gameBegin(); // Displays "Your game begins...."

int APIENTRY WinMain
(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	const int Width = 700;
	const int Height = 600;
	initwindow(Width, Height);
	starting();
	snakebody sb={0};
	playerinfo player={"\0", 0}, top3[3]={"\0", 0};
	char key;
	int step = 2;
	int x = 0;
	int y = 0;
	int d=100;
	bool game=true, chk=false;
	CurrentPoint fruitLoc;
	resume(sb, top3, player, x, y, step);
	cleardevice();
	for(int a=0 ; a<=2 ; a++)
	{
		bgiout<<top3[a].name<<' '<<top3[a].score<<endl;
		outstream();
	}
	getch();
	settextstyle(3,0,4);
	char menu=' ';
	do
	{
		menu=mainMenu(menu);
	}
	while(menu!='1' && menu!='2' && menu!='3');
	switch(menu)
	{
	case '1':
		{
			initializeSnake(sb, step);
			x=12;
			y=0;
			step=2;
			enterInfo(player);
		} break;
	case '2':
		{
			if(x==0 && y==0)
			{
				cleardevice();
				outtextxy(20,200,"There is no saved game present so");
				outtextxy(20,250,"the game will start from the beginning");
				delay(2000);
				initializeSnake(sb, step);
				x=12;
				y=0;
				step=2;
				enterInfo(player);
			}
		} break;
	case '3':
		{
			return 0;;
		} 
	}
	srand(time(NULL));
	generateFood(fruitLoc, sb, step);
	//gameBegin();
	while(1)
	{
		generateBoundry();
		generateObstacles();
		printSnake(sb, step);
		printFood(fruitLoc);
		displayPlayerInfo(player);

		eatFood(sb, player, fruitLoc, step);

		game=movement(sb, x, y, step, key, player, top3, chk);
		if(!game)
			break;
		game=obstacleCollision(sb, step);
		if(!game)
			break;
		game=bodyCollision(sb, step);
		if(!game)
			break;
		game=boundryCollision(sb, step);
		if(!game)
			break;

		delay(100);
		cleardevice();
	}
	if(!chk)
	{
		saveGame(sb, x, y, step, player, top3, game);
		gameOver(top3, player);
	}
}

void starting()
{
	readimagefile("stock-vector-snake-illustration-emblem-524024896.jpg", 180, 100, 500, 300);
	setcolor(YELLOW);
	settextstyle(3,0,10);
	outtextxy(190, 250, "GAME");
	delay(600);
	setcolor(BLUE);
	settextstyle(3,0,4);
	outtextxy(20, 500, "Loading");
	int b=180;
	setcolor(RED);
	for(int a=1 ; a<=50 ; a++)
	{
		delay(50);
		outtextxy(b, 500, "||");
		b=b+10;
	}
	delay(500);
}
char mainMenu(char m)
{
	cleardevice();
	readimagefile("s-l225.jpg", 50, 50, 200, 200);
	readimagefile("6015517234_00bbd0fe82_o.jpg", 500, 50, 620, 200);
	setcolor(LIGHTRED);
	outtextxy(200, 250, "1: NEW GAME");
	setcolor(LIGHTBLUE);
	outtextxy(200, 300, "2: RESUME GAME");
	setcolor(LIGHTRED);
	outtextxy(200, 350, "3: EXIT");
	m=getch();
	return m;
}
void initializeSnake(snakebody &s, int &sl)
{
	for(int a=0, b=0 ; a<sl ; a++)
	{
		b=b+12;
		s.snake[0][a]=300+b;
	}
	for(int a=0 ; a<sl ; a++)
	{
		s.snake[1][a]=250;
	}
}
void enterInfo(playerinfo &p)
{
	cleardevice();
	outtextxy(50, 250, "Player Name : ");
	int a=0, b=0;
	for(a=0, b=320 ; a<=19 ; a++, b=b+20)
	{
		char temp=getch();
		if(temp==8 && a>0)
		{
			a=a-2;
			p.name[a+1]='\0';
		}
		else if(temp==13)
		{
			break;
		}
		else if(temp==32)
		{
			p.name[a]=' ';
		}
		else if(temp<33 || temp>122)
		{
			a=a-1;
		}
		else
		{
			p.name[a]=temp;
		}
		setcolor(LIGHTBLUE);
		bgiout<<temp;
		outstreamxy(b,250);
	}
	p.name[a]='\0';
}
void printSnake(snakebody &s, int &sl)
{
	for(int a=0 ; a<sl ; a++)
	{
		setcolor(LIGHTGREEN);
		setfillstyle(INTERLEAVE_FILL,LIGHTGREEN);
		if(a==0)
		{
			fillellipse(s.snake[0][a], s.snake[1][a], 7,7);
		}
		else
		{
			fillellipse(s.snake[0][a], s.snake[1][a], 6,6);
		}
	}
}
void displayPlayerInfo(playerinfo &p)
{
	int a=p.score;
	char arr[3];
	setcolor(RED);
	settextstyle(3,0,2);
	outtextxy(10, 540, "PLAYER NAME : ");
	setcolor(LIGHTBLUE);
	outtextxy(170, 540, p.name);
	setcolor(RED);
	outtextxy(550, 540, "SCORE : ");
	itoa(a,arr,10);
	setcolor(LIGHTBLUE);
	outtextxy(635, 540, arr);
}
void generateFood(CurrentPoint &f, snakebody &sn, int&sl)
{
	int fruitX, fruitY;
	bool check=false;;
	srand(time(NULL));
	do
	{
	fruitX = rand() % (689-11-1)+11;
	fruitY = rand() % (489-11-1)+11;
	for(int a=0 ; a<sl ; a++)
	{
		if(fruitX<=10|| fruitX>=690 || fruitY<=10 || fruitY>=490)
		{
			check=false;
		}
		else if(fruitX>=100 && fruitX<=130 && fruitY>=100 && fruitY<=400)
		{
			check=false;
		}
		else if(fruitX>=550 && fruitX<=580 && fruitY>=100 && fruitY<=400)
		{
			check=false;
		}
		else if(fruitX>=200 && fruitX<=500 && fruitY>=300 && fruitY<=330)
		{
			check=false;
		}
		else
		{
			check=true;
		}
		
	}
	}
	while(!check);
	f.x = fruitX;
	f.y = fruitY;
}
void printFood(CurrentPoint &f)
{
	setcolor(LIGHTMAGENTA);
	setfillstyle(SOLID_FILL,LIGHTMAGENTA);
	fillellipse(f.x, f.y, 7, 7);
}
void eatFood(snakebody &sn, playerinfo &p, CurrentPoint &f, int &sl)
{
	if(sn.snake[0][sl-1]+10>f.x && sn.snake[0][sl-1]-10<f.x && sn.snake[1][sl-1]+10>f.y && sn.snake[1][sl-1]-10<f.y)
	{
		p.score=p.score+10;
		sl++;
		generateFood(f, sn, sl);
	}
}
void generateBoundry()
{
	setfillstyle(SLASH_FILL,YELLOW);
	bar(0, 0, 10, 500);    
	bar(700, 10, 0, 0);
	bar(0, 500, 700, 490);
	bar(690, 0, 700, 500);
}
void generateObstacles()
{
	setfillstyle( INTERLEAVE_FILL, LIGHTBLUE);
	bar(100,100,130,400);
	bar(550,100,580,400);
	bar(200,300,500,330);
}
bool boundryCollision(snakebody &s, int &sl)
{
	if(s.snake[0][sl-1]<=10|| s.snake[0][sl-1]>=690 || s.snake[1][sl-1]<=10 || s.snake[1][sl-1]>=490)
	{
		return false;
	}
	else
	{
		return true;
	}
}
bool obstacleCollision(snakebody &s, int &sl)
{
	if(s.snake[0][sl-1]>=100 && s.snake[0][sl-1]<=130 && s.snake[1][sl-1]>=100 && s.snake[1][sl-1]<=400)
	{
		return false;
	}
	else if(s.snake[0][sl-1]>=550 && s.snake[0][sl-1]<=580 && s.snake[1][sl-1]>=100 && s.snake[1][sl-1]<=400)
	{
		return false;
	}
	else if(s.snake[0][sl-1]>=200 && s.snake[0][sl-1]<=500 && s.snake[1][sl-1]>=300 && s.snake[1][sl-1]<=330)
	{
		return false;
	}
	else
	{
		return true;
	}
}
bool bodyCollision(snakebody &s, int &sl)
{
	for(int a=0 ; a<sl-1 ; a++)
	{
		if(s.snake[0][sl-1]==s.snake[0][a] &&  s.snake[1][sl-1]==s.snake[1][a])
		{
			return false;
		}
	}
	return true;
}
bool movement(snakebody &sn, int &x, int&y, int &step, char &key, playerinfo &p, playerinfo t3[3], bool &c)
{
	if (kbhit())
	{
		key = getch();
		if (key == 27)
		{
			return false;
		}
		if(key=='p')
		{
			return(pause(sn, x, y, step, p, t3, c));
		}
		switch (key)
		{
			case Up:
			{
				if(y!=12)
				{
				x = 0;
				y = -1 * 12;
				}
				break;
			}
			case Down:
			{
				if(y!=-12)
				{
				x = 0;
				y = 12;
				}
				break;
			}
			case Left:
			{
				if(x!=12)
				{
				y = 0;
				x = -1 * 12;
				}
				break;
			}
			case Right:
			{
				if(x!=-12)
				{
				y = 0;
				x = 12;
				}
				break;
			}
		}
	}
	int privX=sn.snake[0][0];
	int privY=sn.snake[1][0];
	int priv2X,priv2Y;
	for(int a=1 ; a<step ; a++)
	{
		priv2X=sn.snake[0][a];
		priv2Y=sn.snake[1][a];
		sn.snake[0][a]=privX;
		sn.snake[1][a]=privY;
		privX=priv2X;
		privY=priv2Y;
	}
	sn.snake[0][0]+=x;
	sn.snake[1][0]+=y;
	return true;
}
void saveGame(snakebody &sn, int &x, int&y, int &sl, playerinfo &p, playerinfo t3[3], bool &g)
{
	playerinfo temp, temp1;
	ofstream game;
	game.open("game_data.txt", ios::out);
	if(!g)
	{
		for(int a=0 ; a<=2 ; a++)
		{
			if(t3[a].score<p.score)
			{
				temp=t3[a];
				t3[a]=p;
				if(a<1)
				{
					temp1=t3[a+1];
					t3[a+1]=temp;
					t3[a+2]=temp1;
				}
				if(a<2)
				{
					t3[a+1]=temp;
				}
				break;
			}
		}
	}
	for(int a=0 ; a<=2 ; a++)
	{
		game<<t3[a].name<<"_"<<t3[a].score<<"_"<<endl;
	}
	if(g)
	{
		game<<p.name<<"_"<<p.score<<"_"<<endl;
		game<<x<<"_"<<y<<"_"<<sl<<"_"<<endl;
		for(int a=0 ; a<sl ; a++)
		{
			if(a+1==sl)
			{
				game<<sn.snake[0][a]<<"_"<<endl;
			}
			else
			{
				game<<sn.snake[0][a]<<"_";
			}
		}
		for(int a=0 ; a<sl ; a++)
		{
			game<<sn.snake[1][a]<<"_";
		}
	}
	game.clear();
	game.close();
}
bool pause(snakebody &sn, int &x, int &y, int &sl, playerinfo &p, playerinfo t3[3], bool &c)
{
	char opt=' ';
	do
	{
		cleardevice();
		settextstyle(3,0,4);
		setcolor(LIGHTRED);
		outtextxy(150, 200, "1: RESUME GAME");
		setcolor(LIGHTBLUE);
		outtextxy(150, 250, "2: SAVE AND EXIT GAME");
		setcolor(LIGHTRED);
		outtextxy(150, 300, "3: EXIT");
		opt=getch();
		switch(opt)
		{
		case '1':
			{
				opt='3';
				return true;
			}
		case '2':
			{
				opt='3';
				bool t=true;
				saveGame(sn, x, y, sl, p, t3, t);
				c=true;
				return false;
			}
		case '3':
			{
				return false;
				opt='3';
			}
		}
	}
	while(opt!='3');
}
void gameOver(playerinfo t3[3], playerinfo &pl)
{
	settextstyle(3,0,6);
	cleardevice();
	setcolor(YELLOW);
	outtextxy(280, 250, "GAME");
	delay(1000);
	outtextxy(280, 300, "OVER");
	delay(2000);
	cleardevice();
	outtextxy(200, 180, pl.name);
	delay(200);
	outtextxy(200, 250, "Your Score is : ");
	delay(200);
	bgiout<<pl.score;
	outstreamxy(200, 320);
	delay(2000);
	cleardevice();
	settextstyle(3,0,4);
	outtextxy(280, 50, "TOP 3");
	outtextxy(150, 200, "NAME");
	outtextxy(400, 200, "SCORE");
	for(int a=0, b=250 ; a<=2 ; a++, b=b+50)
	{
		if(t3[a].score>0 && t3[a].name[a]!='\0')
		{
			setcolor(LIGHTRED);
			outtextxy(150, b, t3[a].name);
			setcolor(LIGHTBLUE);
			bgiout<<t3[a].score;
		}
		outstreamxy(400, b);
	}
	delay(3000);
}
void resume(snakebody &sn, playerinfo t3[3], playerinfo &pl, int &x, int &y, int &sl)
{
	ifstream game;
	game.open("game_data.txt", ios::in);
	if(!game)
	{
		bgiout<<"File not found";
	}
	else
	{
		cleardevice();
		char temp[7][1000]={"\0"}, *p;
		for(int a=0, c=0 ; a<=6 && !game.eof() ; a++)
		{
			game.getline(temp[a], 1000);
			if((!temp[a][0]=='\0' && temp[a][0]!='_') && a<=2)
			{
				p=strtok(temp[a], "_");
				strcpy(t3[a].name, p);
				p=strtok(NULL, "_");
				t3[a].score=atoi(p);
			}
			else if(!temp[a][0]=='\0' && a==3)
			{
				p=strtok(temp[a], "_");
				strcpy(pl.name, p);
				p=strtok(NULL, "_");
				pl.score=atoi(p);
			}
			else if(!temp[a][0]=='\0' && a==4)
			{
				p=strtok(temp[a], "_");
				x=atoi(p);
				p=strtok(NULL, "_");
				y=atoi(p);
				p=strtok(NULL, "_");
				sl=atoi(p);
			}
			else if(!temp[a][0]=='\0' && a>4)
			{
				p=strtok(temp[a], "_");
				sn.snake[c][0]=atoi(p);
				for(int b=1 ; b<sl ; b++)
				{
					p=strtok(NULL, "_");
					sn.snake[c][b]=atoi(p);
				}
				c++;
			}
		}
	}
	game.close();
}
void gameBegin()
{
	settextstyle(3,0,6);
	setcolor(LIGHTRED);
	char p[3]=" ";
	cleardevice();
	outtextxy(90, 200, "YOUR GAME BEGINS");
	delay(1000);
	setcolor(LIGHTBLUE);
	outtextxy(320, 260, "IN");
	delay(1000);
	cleardevice();
	setcolor(YELLOW);
	settextstyle(3,0,7);
	for(int a=3 ; a>=1 ; a--)
	{
		itoa(a,p,10);
		outtextxy(330, 240, p);
		delay(1000);
		cleardevice();
	}
}