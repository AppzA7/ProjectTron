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

using namespace std;

/*int passfd(int usfd,int sfd)
{
	char buff[sizeof(int)];
	struct iovec iov;
	struct cmsghdr cmsg;
	struct msghdr msg;

	msg.msg_name = NULL;
	msg.msg_len = 0;
	msg.msg_control = buff;
	msg.msg_controllen = CMSG_SPACE(sizeof(int));
	msg.msg_iov = iov;
	msg.msg.iovlen = 1;

	memcpy();
	CMSG_DATA();

	return sendmsg();
}*/

#define PORT 5000
#define ADDR "172.30.106.75"
void connect2clients();
vector<int> players;
vector<struct sockaddr_in> players1;
//some code
int main()
{
	struct sockaddr_in serv_addr,cli_addr,temp_addr;
	socklen_t cli_len;

	int sfd = socket(AF_INET,SOCK_STREAM,0);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	//serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	inet_pton(AF_INET,ADDR,&(serv_addr.sin_addr));
	memset(serv_addr.sin_zero,0,8);

	if(bind(sfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) == -1)
	perror("BIND FAILED");
	fprintf(stderr, "done binding to %s", ADDR);
	cout<<endl;
	listen(sfd,5);

	while(1)
	{
		memset(&cli_addr,-1,sizeof(struct sockaddr_in));
		int nsfd = accept(sfd,(struct sockaddr *)&cli_addr,&cli_len);

		players.push_back(nsfd);
		
		temp_addr = cli_addr;

		players1.push_back(temp_addr);

		if(players.size() > 1)
		{	
			//send client details here for udp connection between clients ...	
			//how to use the sfd's for the game?
			connect2clients();
		}
	}

	return 0;
}

void connect2clients()
{
	char buf[100];
	struct sockaddr_in a, b;
	socklen_t la,lb;
	//getpeername(players[0], (struct sockaddr*)&a, &la);

	a = players1[0];

	cout<<a.sin_family<<endl;
	cout<<ntohs(a.sin_port)<<endl;
	memset(buf,'\0',100);
	inet_ntop(AF_INET,&(a.sin_addr),buf,INET_ADDRSTRLEN);
	cout<<buf<<endl;
	
	cout<<"----------------"<<endl;
	//getpeername(players[1], (struct sockaddr*)&b, &lb);

	b = players1[1];

	cout<<b.sin_family<<endl;
	cout<<ntohs(b.sin_port)<<endl;
	memset(buf,'\0',100);
	inet_ntop(AF_INET,&(b.sin_addr),buf,INET_ADDRSTRLEN);
	cout<<buf<<endl;

	fprintf(stderr, "\nCame in");
	
	if(-1 == send(players[1], &players1[1], sizeof(players1[1]), 0))
	{
		perror("Send1");
	}
	if(-1 == send(players[0], &players1[0], sizeof(players1[0]), 0))
	{
		perror("Send2");
	}
	if(-1 == send(players[1], &players1[0], sizeof(players1[0]), 0))
	{
		perror("Send3");
	}		
	if(-1 == send(players[0], &players1[1], sizeof(players1[1]), 0))
	{
		perror("Send4");
	}

	if(-1 == send(players[0],"1",1,0))
	perror("send5");
	if(-1 == send(players[1],"2",1,0))
	perror("send6");

	fprintf(stderr, "\nCame out..");
	players.erase(players.begin(), players.begin()+1);
	players.erase(players.begin(), players.begin()+1);
	players1.erase(players1.begin(), players1.begin()+1);
	players1.erase(players1.begin(), players1.begin()+1);	
}
