/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>


// server side

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;

     // safety tests 
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     

     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");


     
     bzero((char *) &serv_addr, sizeof(serv_addr));//shall place n zero-valued bytes in the area pointed to by s.
     portno = atoi(argv[1]); 
     serv_addr.sin_family = AF_INET; //Protocol: change to AF_INET6 so the protocol will be ipv6  
     serv_addr.sin_addr.s_addr = INADDR_ANY; // generate random IP 
     serv_addr.sin_port = htons(portno); // establishes the order of information (host byte order : from more to less significant byte ) 
     if (bind(sockfd, (struct sockaddr *) &serv_addr, // bind a same to a socket 
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5); // listen to socket listen (socket , backlog) ; backlog = limit of connections 
     clilen = sizeof(cli_addr); // client length 
     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen); //open socket to client
     if (newsockfd < 0) 
          error("ERROR on accept");
     bzero(buffer,256); // cleaning buffer 
     n = read(newsockfd,buffer,255);	// reading message from client (String, StringSize)
     if (n < 0) error("ERROR reading from socket"); 
     printf("Here is the message: %s\n",buffer);
     n = write(newsockfd,"I got your message",18); // answers to client (String, StringSize)
     if (n < 0) error("ERROR writing to socket");
     close(newsockfd); 
     close(sockfd); 
     return 0; 
}
