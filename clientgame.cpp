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
int playno;
struct timeval timeout;
struct sockaddr_in oppo_addr,cli_addr;
socklen_t cli_len;
char buffer[1],tst[100];

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

}

void pinput(int usfd)	//get the player input and then send it to the game server
{
	char c;
	if(_kbhit())
	{
		buffer = cin.get();
		sendto(usfd,&c,1,0,(struct sockaddr *)&oppo_addr,sizeof(oppo_addr));	//send the key input to the server
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
	
	//sendto(2,argv[1],strlen(argv[1]),0,(struct sockaddr *)&oppo_addr,sizeof(oppo_addr));

	//recvfrom(2,tst,100,0,(struct sockaddr *)&cli_addr,&cli_len);
	//cout<<tst<<endl;

	playno = atoi(argv[3]) - 1;
	cout<<playno<<endl;

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
