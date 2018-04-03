/* tcp_ client.c */ 
/* Programmed by Adarsh Sethi */
/* February 21, 2018 */     

#include <stdio.h>          /* for standard I/O functions */
#include <stdlib.h>         /* for exit */
#include <string.h>         /* for memset, memcpy, and strlen */
#include <netdb.h>          /* for struct hostent and gethostbyname */
#include <sys/socket.h>     /* for socket, connect, send, and recv */
#include <netinet/in.h>     /* for sockaddr_in */
#include <unistd.h>         /* for close */

#define STRING_SIZE 1024
//this is a comment
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

   int sock_client;  /* Socket used by client */

   struct sockaddr_in server_addr;  /* Internet address structure that
                                        stores server address */
   struct hostent * server_hp;      /* Structure to store server's IP
                                        address */
   char server_hostname[STRING_SIZE]; /* Server's hostname */
   unsigned short server_port;  /* Port number used by server (remote port) */

   char sentence[STRING_SIZE];  /* send message */
   char modifiedSentence[STRING_SIZE]; /* receive message */
   unsigned int msg_len;  /* length of message */                      
   int bytes_sent, bytes_recd; /* number of bytes sent or received */
    //another test
   /* open a socket */

   if ((sock_client = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
      perror("Client: can't open stream socket");
      exit(1);
   }

   /* Note: there is no need to initialize local client address information 
            unless you want to specify a specific local port
            (in which case, do it the same way as in udpclient.c).
            The local address initialization and binding is done automatically
            when the connect function is called later, if the socket has not
            already been bound. */

   /* initialize server address information */

   printf("Enter hostname of server: ");
   //scanf("%s", server_hostname);
   strcpy(server_hostname,"localhost" );
   if ((server_hp = gethostbyname(server_hostname)) == NULL) {
      perror("Client: invalid server hostname");
      close(sock_client);
      exit(1);
   }

   printf("Enter port number for server: ");
   //scanf("%hu", &server_port);
   server_port = 45678;

   /* Clear server address structure and initialize with server address */
   memset(&server_addr, 0, sizeof(server_addr));
   server_addr.sin_family = AF_INET;
   memcpy((char *)&server_addr.sin_addr, server_hp->h_addr,
                                    server_hp->h_length);
   server_addr.sin_port = htons(server_port);

    /* connect to the server */
 		
   if (connect(sock_client, (struct sockaddr *) &server_addr, 
                                    sizeof (server_addr)) < 0) {
      perror("Client: can't connect to server");
      close(sock_client);
      exit(1);
   }
  
   /* user interface */

   printf("Please input the file name:\n");
   scanf("%s", sentence);
   msg_len = strlen(sentence) + 1;
   FILE * file;
   file = fopen("out.txt","wb");
   /* send message */
   bytes_sent = send(sock_client, sentence, msg_len, 0);

   /* get response from server */
  //ssize_t recv(int socket, void *buffer, size_t length, int flags);
  // I need 2 recv's -- one for the header and one for the data
  Header h;
  size_t message_bytes = 1;
  int num = 81;
  int package = 0;
  int seq = 0;
  int data_bytes = 0;
  int counter = 0;
  while(message_bytes){
   char * message = (char*)malloc((80)*sizeof(char));
    bytes_recd = recv(sock_client,&h, sizeof(long), 0); 
    seq = ntohs(h.packet_sequence_num);
    data_bytes = ntohs(h.count);
    counter += data_bytes;

    printf("TCP CLIENT:Packet %d received with %d data bytes\n",seq,data_bytes);
    fflush(stdout);
    
    bytes_recd= 0;
    bytes_recd = recv(sock_client,message,80,0);
    message_bytes = bytes_recd;
    
    fprintf(file, "%s",message);
    package+=1;
    
  }
  printf("STATS:\n");
  printf

   /* close the socket */

   close (sock_client);
}
