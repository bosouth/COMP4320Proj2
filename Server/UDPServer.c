#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define PORT     8080


// BARE MINIMUM SERVER
int main() {
   int sd, n;
   struct sockaddr_in serveraddr, clientaddr; // <- Socket address of server and client
   char buffer[512];


   sd = socket(AF_INET, SOCK_DGRAM, 0);
   if (sd < 0) {
      perror("socket creation failed");
      exit(EXIT_FAILURE);
   }
   
   memset(&serveraddr, 0, sizeof(serveraddr));
   memset(&clientaddr, 0, sizeof(clientaddr));   
   
   serveraddr.sin_family = AF_INET; // IPv4
   serveraddr.sin_addr.s_addr = INADDR_ANY;
   serveraddr.sin_port = htons(PORT);   
   
   
         
   // bind socket to address
   if (bind(sd, (const struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0) {
      perror("bind failed");
      exit(EXIT_FAILURE);
   }      
   char *hello = "Server says Hi";
   socklen_t len;
   len = sizeof(clientaddr);

   
   for (;;) {
   
      n = recvfrom(sd, (char *) buffer, sizeof(buffer), MSG_WAITALL, (struct sockaddr *) &clientaddr, &len);         
      printf("Received: %s\n", buffer);
      
      sendto(sd, (const char *)hello, strlen(hello), 
         0, (const struct sockaddr *) &clientaddr,
            len);            
            printf("Responded something");   
   }
   
   close(sd);
   return 0;
}