#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int main()
{
	int status;
	int pid = fork();
	
	if(pid == 0)
	{
		if(system("gnome-terminal -x sh -c \"./game1\"")==-1)
		perror("EXEC");

		exit(0);
	}
	else
	{
		waitpid(pid,&status,0);
		cout<<"EXITING"<<endl;
	}
	
	return 0;
}
