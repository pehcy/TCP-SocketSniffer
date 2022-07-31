#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define __PORT_ADDRESS 8080

struct packet {
    char data[1024];
};

int main(int argc, char * argv[])
{
    printf("Creating server socket");
    
    int sockfd, new_sockfd, connfd, len;
    struct sockaddr_in servaddr, client_addr;
    struct hostent *hp;
    int recv_length=1, yes=1;

    char buffer[1024];

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    {
        perror("socket connection failed.");
        exit(EXIT_FAILURE);
    }

    bzero((char *)&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(__PORT_ADDRESS);

    // Forcefully attaching socket to the port 8080
    if (setsockopt(sockfd, SOL_SOCKET, 
            SO_REUSEADDR | SO_REUSEPORT, &yes, sizeof(int)) == -1) 
    {
        perror("Setting socket option SO_REUSEADDR");
        exit(EXIT_FAILURE);
    }

    while(1)
    {
        int sin_size = sizeof(struct sockaddr_in);
        new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, sin_size);
        return 0;
    }

    // closing the listening socket
    shutdown(sockfd, SHUT_RDWR);
    return 0;
}

void dump(const unsigned char * data_buffer, const unsigned int len) 
{
    unsigned char byte;
    unsigned int i, j;
    for (i = 0; i < len; i++) 
    {
        byte = data_buffer[i];
        /* Display byte in Hex */
        printf("%02x ", data_buffer[i]);
        if ((i % 16==15) || (i == len - 1)) 
        {
            for (j=0; j < (i - (i%16)); j++) 
            {
                printf("  ");
            }
            printf("| ");
            for (j=(i-(i%16)); j <= i; j++) 
            {
                byte = data_buffer[j];
                /* printable char range */
                if ((byte > 31) && (byte < 127)) 
                    printf("%c", byte);
                else
                    printf(".");
            }
            printf("\n");
        }
    }
}