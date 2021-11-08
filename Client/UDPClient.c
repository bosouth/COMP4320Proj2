#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>


// BARE MINIMUM CLIENT
int main(int argc, char **argv) {
    int sd, n;
    struct sockaddr_in server;
    // struct hostent *hp;
    char *hello = "HI";

    sd = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(12345);
    // hp = gethostbyname(argv[1]);
    // bcopy(hp->h_addr, &(server.sin_addr), hp->h_length);

    
    sendto(sd, (char *)hello, 2, 0, (struct sockaddr *) &server, sizeof(server));
    sleep(2);

    close(sd);
    return 0;
}