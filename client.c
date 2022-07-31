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
    int sin_size;
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

    // Forcefully attaching socket to the port 8080
    if (setsockopt(sockfd, SOL_SOCKET, 
            SO_REUSEADDR | SO_REUSEPORT, &yes, sizeof(int)) == -1) 
    {
        perror("Setting socket option SO_REUSEADDR");
        exit(EXIT_FAILURE);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(__PORT_ADDRESS);
    memset(&(servaddr.sin_zero), '\0', 8);

    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(struct sockaddr)) == -1)
    {
        perror("binding to socket");
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd, 5) == -1)
    {
        printf("still listening on socket: 127.0.0.1:%d", __PORT_ADDRESS);
    }
    
    printf("\n success");

    while(1)
    {
        sin_size = sizeof(struct sockaddr_in);
        new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);
        if (new_sockfd == -1)
            printf("accepting connection");
        
        printf("server: got connection from %s port %d\n",
            inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        send(new_sockfd, "Hello, world!\n", 13, 0);
        recv_length = recv(new_sockfd, &buffer, 1024, 0);
        while (recv_length > 0) 
        {
            printf("RECV: %d bytes\n", recv_length);
            dump(buffer, recv_length);
            recv_length = recv(new_sockfd, &buffer, 1024, 0);
        }
        shutdown(new_sockfd, SHUT_RDWR);
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