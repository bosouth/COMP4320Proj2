#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>


// BARE MINIMUM SERVER
int main() {
    int sd, n;
    struct sockaddr_in server;
    char buffer[512];

    server.sin_family = AF_INET;
    server.sin_port = htons(12345);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    n = 

    bind(sd, (struct sockaddr *) &server, sizeof(server));

   
    n = recv(sd, buffer, sizeof(buffer), 0);
    buffer[n] = '\0';
    printf("Received: %s\n", buffer);
    
    close(sd);
    return 0;
}