#include <ctype.h>          /* for toupper */
#include <stdio.h>          /* for standard I/O functions */
#include <stdlib.h>         /* for exit */
#include <string.h>         /* for memset */
#include <sys/socket.h>     /* for socket, bind, listen, accept */
#include <netinet/in.h>     /* for sockaddr_in */
#include <unistd.h>         /* for close */

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
}

int main(int argc, char ** argv){

    FILE * file;
    file = fopen("test_file.txt","r");
    char * line = (char*)malloc(80*sizeof(char));/* or other suitable maximum line size */
    int j = 0;
    printf("here2\n");
    //printf("%s",fgets(line,sizeof(line),file));
    while ( fgets ( line, sizeof(line), file ) != NULL ){
        printf("in the while loop \n");
//fputs ( line, stdout ); /* write the line */

                 Packet packet = new_packet((short) j+1,80,*line);
                 j++;
                 int k;
                 //printf("here\n");
                 printf("%s\n",packet.data);
                }
                fclose ( file );
            
         
        

    return 0;
}