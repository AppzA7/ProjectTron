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
	struct sockaddr_in serv_addr,cli_addr;
	socklen_t cli_len;

	int sfd = socket(AF_INET,SOCK_STREAM,0);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(5000);
	inet_pton(AF_INET,"172.30.106.75",&(serv_addr.sin_addr));
	memset(serv_addr.sin_zero,0,8);

	if(bind(sfd,(struct sockaddr *)&serv_addr,sizeof(struct sockaddr))==-1)
	perror("BIND FAILED");

	listen(sfd,2);

	int nsfd = accept(sfd,(struct sockaddr *)&cli_addr,&cli_len);

	int nsfd1 = accept(sfd,(struct sockaddr *)&cli_addr,&cli_len);

	send(nsfd,&cli_addr,cli_len,0);
	send(nsfd1,&cli_addr,cli_len,0);

	cout<<"SERVER EXITING"<<endl;
	
	return 0;
}
