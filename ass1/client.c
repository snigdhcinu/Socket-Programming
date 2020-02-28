#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<netdb.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
void error(const char* msg)// Error function
{
	perror(msg);// Interprets the error no. to corresponding description using stderr.
	exit(1);
}
int main(int argc, char* argv[])// argc contains the total no. of parameters that we'll pass through command line.
// and argv contains the actual parameters passed through the command line.
{
	if(argc<3)//Through command line we are passing 3 parameters, filename, server_ipaddress and portno. 
	{
		fprintf(stderr, "usage %s hostname port, program terminated\n",argv[0]);
		exit(1);
	}
	int sockfd,portno,n;
	char buffer[255];
	struct sockaddr_in serv_addr;// sockaddr_in gives internet address, and is included in netinet.
	struct hostent* server;
	portno=atoi(argv[2]);// argv[0]->filename; argv[1]->server_ipaddress; argv[2]->portno.
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0)// implies socket() resulted in failure.
		error("Error opening socket.");
	server=gethostbyname(argv[1]);
	if(server==NULL)
	{
		fprintf(stderr, "Error, no suck host\n");
	}
	bzero((char*)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	bcopy((char*)server->h_addr/*copy information from *server to serv_addr*/,(char*)&serv_addr.sin_addr.s_addr,server->h_length);
	serv_addr.sin_port=htons(portno);
	if(connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)
		error("Connection failed");
	while(1)
	{
		bzero(buffer,255);
		fgets(buffer,255,stdin);
		n=write(sockfd,buffer,strlen(buffer));
		if(n<0)
			error("Error on writing");
		bzero(buffer,255);
		n=read(sockfd,buffer,255);
		if(n<0)
			error("Error on reading.");
		printf("SERVER: %s\n",buffer);
		int i=strcmp("Bye",buffer);//Termination condition.
		if(i==0)
			break;
	}
	close(sockfd);
	return 0;
		
}
