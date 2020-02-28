#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<netdb.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <arpa/inet.h>//Header file for inet_ntop.
void error(const char* msg)// Error function
{
	perror(msg);// Interprets the error no. to corresponding description using stderr.
	exit(1);
}
int main(int argc, char* argv[])// argc contains the total no. of parameters that we'll pass through command line.
// and argv contains the actual parameters passed through the command line.
{
	if(argc<2)//Through command line we are passing 2 parameters, filename and portno. 
	{
		fprintf(stderr, "portno. not provided, program terminated\n");
		exit(1);
	}
	int sockfd, newsockfd, portno,n;
	char buffer[255];
	struct sockaddr_in serv_addr, cli_addr;// sockaddr_in gives internet address, and is included in netinet.
	socklen_t clilen;//socklen_t is a 32-bit data type in socket.h
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0)// implies socket() resulted in failure.
	{
		error("Error opening socket.");
		bzero((char*)&serv_addr,sizeof(serv_addr));//clear any data from the address it is referring to.
	}
	portno=atoi(argv[1]);// argv[0]->filename; argv[1]->portno
	serv_addr.sin_family=AF_INET;//No clue what is happening .
	serv_addr.sin_addr.s_addr=INADDR_ANY;// No clue what is happening.
	serv_addr.sin_port=htons(portno);// Host To Network Short. Also no clue what is happening.
	if(bind(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)// typecasting of serv_addr is done here
		error("Binding failed");
	listen(sockfd,5);//listen to whoever is in same port, 5 implies max no. of clients that can connect to server at a time is 5
	clilen=sizeof(cli_addr);
	newsockfd=accept(sockfd,(struct sockaddr*)&cli_addr,&clilen);
	if(newsockfd<0)
		error("error on accept");
	char*ip;// This will store the ip address of the client.
	while(1)//code for communication until termination.
	{
		bzero(buffer,255);//clear buffer, so that client can write to it.
		n=read(newsockfd,buffer,255);
		if(n<0)
			error("Read failed");
		//char ip[INET_ADDRSTRLEN]; 
		//inet_ntop(AF_INET, &(peer_addr.sin_addr), ip, INET_ADDRSTRLEN); 
		//printf("connection established with IP : %s and PORT : %d\n",ip, ntohs(peer_addr.sin_port)); 		
		char ip[INET_ADDRSTRLEN]; 
        inet_ntop(AF_INET, &(cli_addr.sin_addr), ip, INET_ADDRSTRLEN);//Converts converts numeric address to string.
        printf("connection established with IP : %s and PORT : %d\n",ip, ntohs(cli_addr.sin_port));
		printf("CLIENT:%s\n",buffer);
		bzero(buffer,255);//clear buffer, for the server to write.
		fgets(buffer,255,stdin);
		n=write(newsockfd,buffer,strlen(buffer));
		if(n<0)
			error("Writing failed");
		int i=strcmp("Bye",buffer);// termination condition of the loop.
		if(i==0)
			break;
	}
	close(newsockfd);//closes the new file.
	close(sockfd);// closes the old file.
	return 0;
}
