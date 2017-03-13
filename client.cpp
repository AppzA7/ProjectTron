#include <bits/stdc++.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdlib>
#include <sys/stat.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <wait.h>

using namespace std;

string game = "gnome-terminal -x sh -c ";
string game1 = "./game1";
string player1 = "1",player2 = "2";
bool gamecont = true;
char ch;

int main()
{
	char buf[100];
	char *buff[5];
	struct sockaddr_in serv_addr,oppo_addr,self_addr;
	socklen_t oppo_len;

	int tsfd = socket(AF_INET,SOCK_STREAM,0);
	if(tsfd == -1)
	perror("SOCKET");

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(5000);
	inet_pton(AF_INET,"172.30.106.75",&(serv_addr.sin_addr));
	memset(serv_addr.sin_zero,0,8);

	while(gamecont)
	{
		int status;
		int usfd = socket(AF_INET,SOCK_DGRAM,0);

		if(connect(tsfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))==-1)
		perror("CONNECT");	

		recv(tsfd,&self_addr,sizeof(struct sockaddr_in),0);

		cout<<self_addr.sin_family<<endl;
		cout<<ntohs(self_addr.sin_port)<<endl;
		memset(buf,'\0',100);
		inet_ntop(AF_INET,&(self_addr.sin_addr),buf,INET_ADDRSTRLEN);
		cout<<buf<<endl;

		recv(tsfd,&oppo_addr,sizeof(struct sockaddr_in),0);

		cout<<"----------------------"<<endl;

		cout<<oppo_addr.sin_family<<endl;
		cout<<ntohs(oppo_addr.sin_port)<<endl;
		memset(buf,'\0',100);
		inet_ntop(AF_INET,&(oppo_addr.sin_addr),buf,INET_ADDRSTRLEN);
		cout<<buf<<endl;	

		int port = ntohs(oppo_addr.sin_port);
		memset(buf,'\0',100);
		inet_ntop(AF_INET,&(oppo_addr.sin_addr),buf,INET_ADDRSTRLEN);
		
		buff[0] = strdup(game1.c_str());
		string temp = buf;
		buff[1] = strdup(temp.c_str());
		memset(buf,'\0',100);
		sprintf(buf,"%d",port);
		temp = buf;
		buff[2] = strdup(temp.c_str());

		memset(buf,'\0',100);
		recv(tsfd,buf,100,0);
		if(strcmp(buf,"1"))		//wont work on different computers .. :(
		buff[3] = strdup(player1.c_str());
		else
		buff[3] = strdup(player2.c_str());
	
		buff[4] = NULL;

		for(int i=0;i<4;i++)
		{
			cout<<buff[i]<<endl;
		}

		sleep(2);
	
		/*string temp1 = game;
		temp1 += "\"";
		temp1 += temp;
		temp1 += "\"";*/

		if(bind(usfd,(struct sockaddr *)&self_addr,sizeof(self_addr))==-1)
		perror("BIND");

		int pid = fork();
		if(pid == 0)
		{
			dup2(usfd,2);
			if(execv(buff[0],buff)==-1)
			perror("EXEC");				//execv game here in new terminal
			//exit(0);
		}
		
		waitpid(pid,&status,0);

		//close(tsfd);
		close(usfd);
		//close sockets?
			
		while(1)
		{
			cout<<"Would you like to continue playing ? (Y/N)";
			cin>>ch;
			if(ch=='N' || ch=='n')
			{
				gamecont = false;
				break;
			}
			else if(ch=='Y' || ch=='y')
			break;
		}
	}

	return 0;
}
