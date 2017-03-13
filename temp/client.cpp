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

int main()
{
	char buf[100];
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

	//recv(sfd,&self_addr,sizeof(struct sockaddr_in),0);
	cout<<"Before"<<endl;
	memset(buf,'\0',100);
	//recv(sfd,buf,100,0);
	recv(sfd,&oppo_addr,sizeof(struct sockaddr_in),0);

	/*if(strcmp(buf,"admin"))
	{
		
	}
	else
	{
		
	}*/
	cout<<"After"<<endl;

	cout<<oppo_addr.sin_family<<endl;
	cout<<ntohs(oppo_addr.sin_port)<<endl;
	memset(buf,'\0',100);
	inet_ntop(AF_INET,&(oppo_addr.sin_addr),buf,INET_ADDRSTRLEN);
	cout<<buf<<endl;
	//if(bind(usfd,(struct sockaddr *)&oppo_addr,sizeof()))

	sleep(2);

	sendto(usfd,"from client",11,0,(struct sockaddr *)&oppo_addr,sizeof(oppo_addr));

	cout<<"EXITING"<<endl;

	return 0;
}
