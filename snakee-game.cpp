#include<iostream>
#include<iomanip>
#include<conio.h>
#include<time.h>
#include<windows.h>
using namespace std;
#define RED 1
#define BLUE 2
#define GREEN 3
#define BLACK 4

void gotoRowCol(int rpos, int cpos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = cpos;
	scrn.Y = rpos;
	SetConsoleCursorPosition(hOuput, scrn);
}
void SetClr(int tcl, int bcl)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (tcl + (bcl * 16)));
}
enum DIRECTION {
	UP,DOWN,RIGHT,LEFT
};
struct position
{
	int ri, ci;
};
struct snake
{
	position* ps;
	int size;
	int score;
	char sym;
	DIRECTION di;
	string pname;
	int LEFTKEY, RIGHTKEY, UPKEY, DOWNKEY;
};
struct food
{
	position fp;
	bool alive;
	int score;
	char sym;
};
void generatef(food& f, int h, int w, snake& s);
void init(snake& s,snake &s1, food& f, int h, int w)
{
	s.ps = new position[3]{};
	s.ps[0].ri = h / 2;
	s.ps[0].ci = w / 2;


	s.ps[1].ri = h / 2;
	s.ps[1].ci = w / 2 - 1;

	s.ps[2].ri = h / 2;
	s.ps[2].ci = w / 2 - 2;
	s.size = 3;
	s.score = 0;
	s.sym = -37;
	s.pname = "player 1";
	s.di = RIGHT;
	s.RIGHTKEY = 77, s.LEFTKEY = 75, s.UPKEY = 72, s.DOWNKEY = 80;

	s1.ps = new position[3]{};
	s1.ps[0].ri = h / 2;
	s1.ps[0].ci = w / 2+10;


	s1.ps[1].ri = h / 2;
	s1.ps[1].ci = w / 2 +11;

	s1.ps[2].ri = h / 2;
	s1.ps[2].ci = w / 2 + 12;
	s1.size = 3;
	s1.score = 0;
	s1.sym = 'O';
	s1.pname = "player 2";
	s1.di = LEFT;
	s1.RIGHTKEY = 100, s1.LEFTKEY = 97, s1.UPKEY = 119, s1.DOWNKEY = 115;
	generatef(f, h, w, s);
	f.sym = -37;

}
void display(const snake& s)
{
	for (int i = 0; i < s.size; i++)
	{
		gotoRowCol(s.ps[i].ri, s.ps[i].ci);
		SetClr(1, 0);
		cout << s.sym;
	}
}
void erase(const snake& s)
{
	for (int i = 0; i < s.size; i++)
	{
		gotoRowCol(s.ps[i].ri, s.ps[i].ci);
		cout << ' ';
	}
}
void move(snake& s, int h, int w)
{
	for (int i = s.size - 1; i - 1 >= 0; i--)
	{
		s.ps[i] = s.ps[i - 1];
	}
	switch (s.di)
	{
	case UP:
		s.ps[0].ri--;
		if (s.ps[0].ri == -1)
			s.ps[0].ri = h - 1;
		break;
	case DOWN:
		s.ps[0].ri++;
		if (s.ps[0].ri == h)
			s.ps[0].ri = 0;
		break;
	case LEFT:
		s.ps[0].ci--;
		if (s.ps[0].ci == -1)
			s.ps[0].ci = w - 1;
		break;
	case RIGHT:
		s.ps[0].ci++;
		if (s.ps[0].ci == w)
			s.ps[0].ci = 0;
		break;
	}
}
void changeDir(snake& s, char key)
{
	if (key == s.LEFTKEY)
		if (s.di != RIGHT)
		{
			s.di = LEFT;
		}
	if (key == s.RIGHTKEY)
		if (s.di != LEFT)
		{
			s.di = RIGHT;
		}
	if (key == s.DOWNKEY)
		if (s.di != UP)
		{
			s.di = DOWN;
		}
	if (key == s.UPKEY)
		if (s.di != DOWN)
		{
			s.di = UP;
		}
}
bool isLegal(position p, snake& s)
{
	for (int i = 0; i < s.size; i++)
	{
		if ((s.ps[i].ri = p.ri) && (s.ps[i].ci == p.ci))
		{
			return false;
		}

	}
	return true;
}
void printBoundary(char b[][80], int w, int h)
{
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			if (j == 0 || j == w - 1) {
				b[i][j] = '#';
			}
			else {
				b[i][j] = ' ';
			}
		}
	}
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			
			cout << b[i][j];
		}
		cout << endl;
	}
}
bool boundaryCheck(snake& s,int h,int w)
{
	if ( s.ps[0].ci >= w - 1 || s.ps[0].ci == 0)
	{
		return true;
	}
	return false;
}
void generatef(food& f, int h, int w, snake& s)
{
	do
	{
		f.fp.ri = rand() % h;
		f.fp.ci = rand() % w;
	} while (!isLegal(f.fp, s));
	f.alive = true;
	f.score = 1;
}
void displayFood(food f)
{
	gotoRowCol(f.fp.ri, f.fp.ci);
	cout << f.sym;
}
void eraseFood(food f)
{
	gotoRowCol(f.fp.ri, f.fp.ci);
	cout << ' ';
}
bool foodeaten(snake& s, food f)
{
	return s.ps[0].ri == f.fp.ri && s.ps[0].ci == f.fp.ci;
}
void expandsnake(snake& s)
{
	position* hp = new position[s.size + 1];
	for (int i = 0; i < s.size; i++)
	{
		hp[i] = s.ps[i];
	}
	hp[s.size] = s.ps[s.size - 1];
	s.size++;
	delete[] s.ps;
	s.ps = hp;
}
void iswin(snake& s1, snake& s2)
{
	if (s1.score > s2.score)
	{
		cout << "winner is..." << s1.pname<<endl;

	}
	else if (s1.score < s2.score)
	{
		cout << "winner is....." << s2.pname << endl;

	}
	else if(s1.score==s2.score)
	{
		cout << "nobody won!!!!" << endl;
	}
}
void gameover(snake &s,snake &s1,int h,int w)
{   
	char ch = 0;
	system("cls");
	gotoRowCol(h/2, w/2);
	cout << " GAME OVER....... " << endl;
	gotoRowCol(h / 3, w / 3);
	cout << "score of player 1 is: " << s.score << "    " << "score of player 2 is: " << s1.score << endl;
	gotoRowCol(h / 4, w / 4);
	iswin(s, s1);
	gotoRowCol(h / 2+2, w / 2+2);
	cout << "press any key to exit...." << endl;
	ch = _getch();
	exit(0);


}
bool checkT(snake& s1, snake& s2)
{
	for (int i = 0; i < s1.size; i++)
	{
		for (int j = 0; j < s2.size; j++)
		{
			if (s1.ps[i].ci == s2.ps[j].ci && s1.ps[i].ri == s2.ps[j].ri)
			{
				return true;
			}
		}
	}
	return false;
}
void printScore(snake& s, snake& s1)
{
	gotoRowCol(90, 100);

	cout << " score of " << s.pname << " is: " << s.score;
	cout << endl;
	cout << " score of " << s1.pname << " is: " << s1.score;
}
void keys(snake& s, snake& s1)
{
	gotoRowCol(80, 100);
	cout << " player 1 should use arrow keys...." << endl;
	cout << " player 2 should use a,s,d,w keys...." << endl;
}
void updateScore(snake& S)
{
	S.score++;
}
bool eatItself(snake& s)
{
	position h = s.ps[0];
	for (int i = 1; i < s.size; i++)
	{
		if (h.ci == s.ps[i].ci && h.ri == s.ps[i].ri)
		{
			return true;
		}
		
	}
	return false;
}
void eraseSnake(snake &s)
{
	delete[] s.ps;
	s.ps = nullptr;
	s.size = 0;
}

int main()
{

	char c; 
	snake s,s1;
	food f;
	const int h = 60; const int w = 80;
	char b[h][w] = {};
	init(s,s1, f, h, w);
	printBoundary(b, w, h);
	keys(s, s1);
	
	displayFood(f);
	while (true)
	{
		
		if (_kbhit())
		{
			c = _getch();
			if (c == -32)
			{
				c = _getch();
				changeDir(s, c);

			}
			else if (c == 100 || c == 97 || c == 119 || c == 115)
			{
				
				changeDir(s1, c);

			}
		}
		
		display(s);
		Sleep(95);
		display(s1);
		Sleep(95);
		erase(s);
		erase(s1);
		
		move(s, h, w);
		
		move(s1, h, w);
		if (boundaryCheck(s,h,w) == true || boundaryCheck(s1, h, w) == true)
		{    
			
			gameover(s, s1, h, w);
		}
		
	
		if (checkT(s, s1) == true)
		{   
			
			gameover(s, s1,h,w);
			
		}
		if ((eatItself(s) == true))
		{
			
			gameover(s, s1, h, w);
			
		}
		if ((eatItself(s1) == true))
		{   
			
			gameover(s, s1, h, w);
			
		}

		if (foodeaten(s, f))
		{   
			eraseFood(f);
			expandsnake(s);
			generatef(f, h, w, s);
			displayFood(f);
			updateScore(s);
		}
		else if (foodeaten(s1, f))
		{   
			eraseFood(f);
			expandsnake(s1);
			generatef(f, h, w, s1);
			displayFood(f);
			updateScore(s1);
		}
		printScore(s, s1);
		

		
		
	}



	return 0;
}
