#include <bits/stdc++.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <sys/select.h>
#include <termios.h>
#include <stropts.h>
#include <sys/ioctl.h>

#define GAMESIZE 10

using namespace std;

int p1headi = 5,p1headj = 5,p2headi = 5,p2headj = 15,fruiti,fruitj;
vector <int> p1taili,p1tailj,p2taili,p2tailj;

enum class Dir {stat,up,down,left,right};
Dir p1dir = Dir::right;
Dir p2dir = Dir::left;   //change them to stat initially ..
bool gen = true,gameover = false,flag = false;
char p1in = 'd',p2in = 'a';
fd_set readfd;
struct timeval timeout;

int _kbhit() {
    static const int STDIN = 0;
    static bool initialized = false;

    if (! initialized) {
        // Use termios to turn off line buffering
        termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);		//what does it do exactly ?
        initialized = true;
    }

    int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}

void display()
{
	system("clear");

	for(int i=0;i<GAMESIZE;i++)
	{
		for(int j=0;j<GAMESIZE*2;j++)
		{
			if((i==0&&j==0) || (i==0&&j==GAMESIZE*2-1) || (i==GAMESIZE-1&&j==0) || (i==GAMESIZE-1&&j==GAMESIZE*2-1))
			cout<<"+";
			else if(i==0 || i==GAMESIZE-1)
			cout<<"-";
			else if(j==0 || j==GAMESIZE*2-1)
			cout<<"|";
			else if(i==p1headi && j==p1headj)
			{
				for(int ptail=0;ptail<p2taili.size();ptail++)
				{
					if(i==p2taili[ptail] && j==p2tailj[ptail])
					gameover = true;
				}
				cout<<"X";
			}
			else if(i==p2headi && j==p2headj)
			{
				for(int ptail=0;ptail<p1taili.size();ptail++)
				{
					if(i==p1taili[ptail] && j==p1tailj[ptail])
					gameover = true;
				}	
				cout<<"O";
			}
			else if(i==fruiti && j==fruitj)
			cout<<"F";
			else
			{
				for(int ptail=0;ptail<p1taili.size();ptail++)
				{
					if(p1taili[ptail]==i && p1tailj[ptail]==j)
					{
						cout<<"x";		
						flag = true;
					}
				}
				if(!flag)
				{
					for(int ptail=0;ptail<p2taili.size();ptail++)
					{
						if(p2taili[ptail]==i && p2tailj[ptail]==j)
						{
							cout<<"o";		
							flag = true;
						}
					}
				}
				
				if(!flag)
				cout<<" ";
				else
				flag = false;
			}
		}
		cout<<endl;
	}

}

void pinput()
{
	//cout<<"IN HERE"<<endl;

	cout<<p1in<<" "<<p2in<<endl;	

	/*FD_ZERO(&readfd);
	FD_SET(STDIN_FILENO,&readfd);
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;
	int p = select(1,&readfd,NULL,NULL,&timeout);*/
	if(_kbhit())
	{
		cout<<"success"<<endl;
		p1in = cin.get();
	}

	//how to receive p2 input ..?

	switch(p1in)
	{
		case 'w' : 	if(p1dir!=Dir::down)
				p1dir = Dir::up;
			break;
		case 's' : 	if(p1dir!=Dir::up)
				p1dir = Dir::down;
			break;
		case 'a' : 	if(p1dir!=Dir::right)	
				p1dir = Dir::left;
			break;
		case 'd' : 	if(p1dir!=Dir::left)
				p1dir = Dir::right;
			break;
		default : break;
	}
	
	switch(p2in)
	{
		case 'w' : 	if(p2dir!=Dir::down)
				p2dir = Dir::up;
			break;
		case 's' : 	if(p2dir!=Dir::up)
				p2dir = Dir::down;
			break;
		case 'a' : 	if(p2dir!=Dir::right)
				p2dir = Dir::left;
			break;
		case 'd' : 	if(p2dir!=Dir::left)
				p2dir = Dir::right;
			break;
		default : break;
	}
}

void logic()
{
	switch(p1dir)
	{
		case Dir::up : 
				p1taili.insert(p1taili.begin(),p1headi);
				p1tailj.insert(p1tailj.begin(),p1headj);
				p1headi--;	//testing ..
				if(p1headi==0)
				gameover = true;
				if(!(p1headi==fruiti && p1headj==fruitj))
				{
					p1taili.pop_back();
					p1tailj.pop_back();	
				}
				else
				gen = true;
			break;
		case Dir::down :			//fill in the rest ...
				p1taili.insert(p1taili.begin(),p1headi);
				p1tailj.insert(p1tailj.begin(),p1headj);
				p1headi++;
				if(p1headi==GAMESIZE-1)
				gameover = true;
				if(!(p1headi==fruiti && p1headj==fruitj))
				{
					p1taili.pop_back();
					p1tailj.pop_back();	
				}
				else 
				gen = true;
			break;
		case Dir::right :
				p1taili.insert(p1taili.begin(),p1headi);
				p1tailj.insert(p1tailj.begin(),p1headj);
				p1headj++;
				if(p1headj==GAMESIZE*2-1)
				gameover = true;
				if(!(p1headi==fruiti && p1headj==fruitj))
				{
					p1taili.pop_back();
					p1tailj.pop_back();	
				}
				else
				gen = true;
			break;
		case Dir::left :
				p1taili.insert(p1taili.begin(),p1headi);
				p1tailj.insert(p1tailj.begin(),p1headj);
				p1headj--;
				if(p1headj==0)
				gameover = true;
				if(!(p1headi==fruiti && p1headj==fruitj))
				{
					p1taili.pop_back();
					p1tailj.pop_back();	
				}
				else
				gen = true;
			break;
	}

	switch(p2dir)
	{
		case Dir::up : 
				p2taili.insert(p2taili.begin(),p2headi);
				p2tailj.insert(p2tailj.begin(),p2headj);
				p2headi--;	//testing ..
				if(p2headi==0)
				gameover = true;
				if(!(p2headi==fruiti && p2headj==fruitj))
				{
					p2taili.pop_back();
					p2tailj.pop_back();
				}
				else
				gen = true;
			break;
		case Dir::down :			//fill in the rest ...
				p2taili.insert(p2taili.begin(),p2headi);
				p2tailj.insert(p2tailj.begin(),p2headj);
				p2headi++;
				if(p2headi==GAMESIZE-1)
				gameover = true;
				if(!(p2headi==fruiti && p2headj==fruitj))
				{
					p2taili.pop_back();
					p2tailj.pop_back();
				}
				else
				gen = true;
			break;
		case Dir::right :
				p2taili.insert(p2taili.begin(),p2headi);
				p2tailj.insert(p2tailj.begin(),p2headj);
				p2headj++;
				if(p2headj==GAMESIZE*2-1)
				gameover = true;
				if(!(p2headi==fruiti && p2headj==fruitj))
				{
					p2taili.pop_back();
					p2tailj.pop_back();
				}
				else
				gen = true;
			break;
		case Dir::left :
				p2taili.insert(p2taili.begin(),p2headi);
				p2tailj.insert(p2tailj.begin(),p2headj);
				p2headj--;
				if(p2headj==0)
				gameover = true;
				if(!(p2headi==fruiti && p2headj==fruitj))
				{
					p2taili.pop_back();
					p2tailj.pop_back();
				}
				else
				gen = true;
			break;
	}
	
}

void generate()
{
	if(gen)
	{
		fruiti = rand()%8+1;
		if(fruiti<0)
		fruiti *= -1;
		fruitj = rand()%18+1;
		if(fruitj<0)
		fruitj *= -1;
		gen = false;
	}
}

int main()
{
	srand(time(NULL));

	//pthread_t in,out;
	
	//pthread_create(&in,NULL,&sinput,NULL);

	while(!gameover)
	{
		generate();
		display();		//link with sockets ..
		pinput();
		logic();
		sleep(1);	
	}

	return 0;
}
