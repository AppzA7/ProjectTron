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
int main()
{
	struct sockaddr_in serv_addr,cli_addr;
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
	listen(sfd,5);

	while(1)
	{
		int nsfd = accept(sfd,(struct sockaddr *)&cli_addr,&cli_len);

		players.push_back(nsfd);

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
	struct sockaddr_in a, b;
	socklen_t l;
	getpeername(players[0], (struct sockaddr*)&a, &l);
	getpeername(players[1], (struct sockaddr*)&b, &l);

	fprintf(stderr, "\nCame in");
	if(-1 == send(players[1], (char *)&a, sizeof(struct sockaddr_in), 0))
	{
		perror("Send1");
	}
	if(-1 == send(players[0], (char *)&b, sizeof(struct sockaddr_in), 0))
	{
		perror("Send2");
	}
	fprintf(stderr, "\nCame out..");
	players.erase(players.begin(), players.begin()+1);
}
