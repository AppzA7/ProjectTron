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
#include <cstdlib>
#include <sys/stat.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <wait.h>
#include <errno.h>

#define GAMESIZE 10

using namespace std;

int p1headi = 5,p1headj = 5,p2headi = 5,p2headj = 15,fruiti,fruitj;
vector <int> p1taili,p1tailj,p2taili,p2tailj;

enum class Dir {stat,up,down,left,right};
Dir pdir[2] = {Dir::right,Dir::left};   //change them to stat initially ..
bool gen = true,gameover = false,flag = false;
char pin[2] = {'d','a'};
fd_set readfd;
struct timeval timeout;
struct sockaddr_in oppo_addr,cli_addr,cli_addr1,cli_addr2;
socklen_t cli_len;
char buffer[1],tst[100];
char clibuff[GAMESIZE][GAMESIZE*2];
char winner = ' ';

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
	//system("clear");
	memset(clibuff,' ',200);

	for(int i=1;i<GAMESIZE*2-1;i++)
	{
		clibuff[0][i] = '-';
		clibuff[GAMESIZE-1][i] = '-';
		if(i<GAMESIZE-1)
		{	
			clibuff[i][0] = '|';
			clibuff[i][GAMESIZE-1] = '|';
		}
	}

	buff[0][0] = '+';
	buff[0][GAMESIZE*2-1] = '+';
	buff[GAMESIZE-1][0] = '+';
	buff[GAMESIZE-1][GAMESIZE*2-1] = '+';

	if(buff[p1headi][p1headj] == '-' || buff[p1headi][p1headj] == '|' || buff[p1headi][p1headj] == 'x' || buff[p1headi][p1headj] == 'O' || buff[p1headi][p1headj] =='o')
	{
		gameover = true;
		winner = '1';
		if(buff[p1headi][p1headj]=='O')
		winner = 'D';
		//set game winner ...
	}	
	if(!gameover)
	buff[p1headi][p1headj] = 'X';


	if(buff[p1headi][p1headj] == '-' || buff[p1headi][p1headj] == '|' || buff[p1headi][p1headj] == 'x' || buff[p1headi][p1headj] == 'X' || buff[p1headi][p1headj] =='o')
	{
		gameover = true;
		winner = '2';
		if(buff[p1headi][p1headj]=='X')
		winner = 'D';
		//set game winner ...
	}	
	if(!gameover)
	buff[p1headi][p1headj] = 'O';

	buff[fruiti][fruitj] = 'F';

	for(int i=0;i<p1taili.size();i++)
	buff[p1taili[i]][p1tailj[i]] = 'x';
	for(int i=0;i<p2taili.size();i++)
	buff[p2taili[i]][p2tailj[i]] = 'o';

	sendto(0,buff,GAMESIZE * GAMESIZE * 2,0,cli_addr1,sizeof(cli_addr1));
	sendto(0,buff,GAMESIZE * GAMESIZE * 2,0,cli_addr2,sizeof(cli_addr2));
}

void pinput()
{
	//cout<<"IN HERE"<<endl;

	//cout<<p1in<<" "<<p2in<<endl;	

	/*FD_ZERO(&readfd);
	FD_SET(STDIN_FILENO,&readfd);
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;
	int p = select(1,&readfd,NULL,NULL,&timeout);*/
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;

	FD_ZERO(&readfd);

	FD_SET(0,&readfd);
	FD_SET(1,&readfd);

	int selp = select(2,&readfd,NULL,NULL,&timeout);

	if(selp>0)
	{
		if(FD_ISSET(0,&readfd))
		{
			cout<<"Receiving 0"<<endl;
			memset(buffer,'\0',1);
			recvfrom(2,buffer,1,0,(struct sockaddr *)&cli_addr,&cli_len);
			pin[0] = buffer[0];
		}
		if(FD_ISSET(1,&readfd))
		{
			cout<<"Receiving 1"<<endl;
			memset(buffer,'\0',1);
			recvfrom(2,buffer,1,0,(struct sockaddr *)&cli_addr,&cli_len);
			pin[1] = buffer[0];
		}
	}	

	//how to receive p2 input ..?

	switch(pin[0])
	{
		case 'w' : 	if(pdir[0]!=Dir::down)
				pdir[0] = Dir::up;
			break;
		case 's' : 	if(pdir[0]!=Dir::up)
				pdir[0] = Dir::down;
			break;
		case 'a' : 	if(pdir[0]!=Dir::right)	
				pdir[0] = Dir::left;
			break;
		case 'd' : 	if(pdir[0]!=Dir::left)
				pdir[0] = Dir::right;
			break;
		default : break;
	}
	
	switch(pin[1])		//dunno ...
	{
		case 'w' : 	if(pdir[1]!=Dir::down)
				pdir[1] = Dir::up;
			break;
		case 's' : 	if(pdir[1]!=Dir::up)
				pdir[1] = Dir::down;
			break;
		case 'a' : 	if(pdir[1]!=Dir::right)
				pdir[1] = Dir::left;
			break;
		case 'd' : 	if(pdir[1]!=Dir::left)
				pdir[1] = Dir::right;
			break;
		default : break;
	}
}

void logic()
{
	switch(pdir[0])
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

	switch(pdir[1])
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

int main(int argc,char *argv[])
{
	srand(time(NULL));

	int port = atoi(argv[2]);

	oppo_addr.sin_family = AF_INET;
	oppo_addr.sin_port = htons(port);
	inet_pton(AF_INET,argv[1],&(oppo_addr.sin_addr));
	memset(oppo_addr.sin_zero,0,8);

	//pthread_t in,out;
	memset(tst,'\0',100);
	
	getpeername(0,(struct sockaddr *)&cli_addr1,&cli_len);
	getpeername(1,(struct sockaddr *)&cli_addr2,&cli_len);

	//sendto(2,argv[1],strlen(argv[1]),0,(struct sockaddr *)&oppo_addr,sizeof(oppo_addr));

	//recvfrom(2,tst,100,0,(struct sockaddr *)&cli_addr,&cli_len);
	//cout<<tst<<endl;

	sleep(5);

	//pthread_create(&in,NULL,&sinput,NULL);

	while(!gameover)
	{
		generate();
		display();		//link with sockets ..
		pinput();
		logic();
		usleep(300000);	
	}

	return 0;
}
                                                                                                                                                                                                                                                                                                                                                    