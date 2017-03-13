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

struct buff
{
	int a;
	char buf[100];
};

int main()
{
	struct sockaddr_in serv_addr,self_addr,oppo_addr;
	socklen_t oppo_len;

	int sfd = socket(AF_INET,SOCK_STREAM,0);
	int usfd = socket(AF_INET,SOCK_DGRAM,0);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(5000);
	inet_pton(AF_INET,"172.30.106.75",&(serv_addr.sin_addr));
	memset(serv_addr.sin_zero,0,8);

	/*if(bind(sfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))==-1)
	perror("BIND FAILED");*/
	connect(sfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));

	struct buff *p = new buff;
	memset(p,0,sizeof(struct buff));

	//recv(sfd,&self_addr,sizeof(struct sockaddr_in),0);
	recv(sfd,p,sizeof(struct buff),0);

	//if(bind(usfd,(struct sockaddr *)&oppo_addr,sizeof()))

	//sendto(usfd,"from client",11,0,(struct sockaddr *)&oppo_addr,sizeof(oppo_addr));

	cout<<"Reached before printing "<<endl;
	cout<<p->a<<endl;
	cout<<p->buf<<endl;

	cout<<"EXITING"<<endl;

	return 0;
}
