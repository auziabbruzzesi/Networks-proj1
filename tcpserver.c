/* tcpserver.c */
/* Programmed by Adarsh Sethi */
/* February 21, 2018 */    

#include <ctype.h>          /* for toupper */
#include <stdio.h>          /* for standard I/O functions */
#include <stdlib.h>         /* for exit */
#include <string.h>         /* for memset */
#include <sys/socket.h>     /* for socket, bind, listen, accept */
#include <netinet/in.h>     /* for sockaddr_in */
#include <unistd.h>         /* for close */

#define STRING_SIZE 1024   

/* SERV_TCP_PORT is the port number on which the server listens for
   incoming requests from clients. You should change this to a different
   number to prevent conflicts with others in the class. */

#define SERV_TCP_PORT 45678


//struct for header
typedef struct{
    short packet_sequence_num,count;
}Header;

//header constructor method
Header new_header(short seq, short count){
    Header h;
    h.packet_sequence_num = seq;
    h.count = count;
    return h;
    
}

typedef struct{
    Header header;
    char * data;

}Packet;

Packet new_packet(short seq,short count,char * data){
    Packet p;
    p.header = new_header(seq,count);
    p.data = data;
    return p;
}
int main(void) {

   int sock_server;  /* Socket on which server listens to clients */
   int sock_connection;  /* Socket on which server exchanges data with client */

   struct sockaddr_in server_addr;  /* Internet address structure that
                                        stores server address */
   unsigned int server_addr_len;  /* Length of server address structure */
   unsigned short server_port;  /* Port number used by server (local port) */

   struct sockaddr_in client_addr;  /* Internet address structure that
                                        stores client address */
   unsigned int client_addr_len;  /* Length of client address structure */

   char sentence[STRING_SIZE];  /* receive message */
   char modifiedSentence[STRING_SIZE]; /* send message */
   unsigned int msg_len;  /* length of message */
   int bytes_sent, bytes_recd; /* number of bytes sent or received */
   unsigned int i;  /* temporary loop variable */

   /* open a socket */

   if ((sock_server = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
      perror("Server: can't open stream socket");
      exit(1);                                                
   }

   /* initialize server address information */
    
   memset(&server_addr, 0, sizeof(server_addr));
   server_addr.sin_family = AF_INET;
   server_addr.sin_addr.s_addr = htonl (INADDR_ANY);  /* This allows choice of
                                        any host interface, if more than one
                                        are present */ 
   server_port = SERV_TCP_PORT; /* Server will listen on this port */
   server_addr.sin_port = htons(server_port);

   /* bind the socket to the local server port */

   if (bind(sock_server, (struct sockaddr *) &server_addr,
                                    sizeof (server_addr)) < 0) {
      perror("Server: can't bind to local address");
      close(sock_server);
      exit(1);
   }                     

   /* listen for incoming requests from clients */

   if (listen(sock_server, 50) < 0) {    /* 50 is the max number of pending */
      perror("Server: error on listen"); /* requests that will be queued */
      close(sock_server);
      exit(1);
   }
   printf("I am here to listen ... on port %hu\n\n", server_port);
  
   client_addr_len = sizeof (client_addr);

   /* wait for incoming connection requests in an indefinite loop */

   for (;;) {

      sock_connection = accept(sock_server, (struct sockaddr *) &client_addr, 
                                         &client_addr_len);
                     /* The accept function blocks the server until a
                        connection request comes from a client */
      if (sock_connection < 0) {
         perror("Server: accept() error\n"); 
         close(sock_server);
         exit(1);
      }
 
      /* receive the message */

      bytes_recd = recv(sock_connection, sentence, STRING_SIZE, 0);

      if (bytes_recd > 0){
         printf("file name is:\n");
         printf("%s", sentence);
         printf("\nwith length %d\n\n", bytes_recd);
         msg_len = bytes_recd;
         FILE* file;
         file = fopen(sentence,"r");
         char * line = NULL;
		 size_t linelength = 0;
         int seq = 0;
		
         if (file) {
           while (getline(&line, &linelength, file) > 0) {
					printf("Reading in line:\n");
					printf("%s", line);
                    //Header h = new_header(htons(seq),linelenght);
					//unsigned short header[2] = {htons(seq), htons((unsigned short) linelength)};
                    Packet p = new_packet(htons(seq),htons((short)linelength),line);//makes packet "object." will initialize the header and data
					bytes_sent = send(sock_connection, &p.header, sizeof(p.header), 0);// send header
                    // printf("sizeof(p.header): %lu\n",sizeof(p.header));
                    // printf("bytes sent: %d \n",bytes_sent);
                    bytes_sent = 0;
					bytes_sent = send(sock_connection, &p.data, p.header.count, 0);//send data
                    printf("sizeof(p.data): %d\n",sizeof(p.data));
                    printf("bytes sent: %d \n",bytes_sent);

					printf("Sent line is:\n");
					printf("%s", p.data);
                    
                    seq +=1;
				}
				//SEND FINAL MESSAGE
				// unsigned short header[2] = {htons((count++)-1), htons(0)};
                char * last_packet_data = NULL;
                Packet lastPacket = new_packet(htons(seq),htons(0),last_packet_data);
				bytes_sent = send(sock_connection, &lastPacket.header, sizeof(lastPacket.header), 0);
			}
			printf("All lines sent\n");
			if (ferror(file)) {
				/* deal with error */
			}

			fclose(file);
		}

          break;
          }  
 
 		
      close(sock_connection);
   } 

