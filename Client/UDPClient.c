#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time.h> // Ued for generating timers to know when to stop listening for server and when to send duplicate packets
#include <pthread.h> // Used for creating multiple threads which allows for multiple timers

#define PACKET_SIZE 512
#define PORT 8080

int startTime = 0, timerOverload = 5000;

// SELECTIVE REPEAT PROTOCOL
// window size for sender and receiver is 8
// sequence number is modulo 24
char * selectiveRep(char *packet[PACKET_SIZE]) {
// we are the sender so no response the first time
// we start with sequence numbers 0 through 7 being in our window
// sequence number is first byte of our packet packet[0-7]

   int windowSize = 8;
  
   return 0;

}

// RUN TIMER FOR MAIN SOCKET CONNECTION
// time needs to be global so multiple threads can refer to the same number
void *socketTimer(void *argp, int timerOverload, int sd)
{

   clock_t before = clock(); // We want to start the clock
   
   while (startTime < timerOverload) {
   
   
      clock_t difference = clock() - before;
      startTime = difference * 1000 / CLOCKS_PER_SEC;
   
   
   }
   printf("Timer overload: No response from server... Closing Socket");
   close(sd);

   exit(0); // Should close all threads in the client program
}



// BARE MINIMUM CLIENT
int main(int argc, char **argv) {
   int sd, n, seqNum, originalOverload;
   float gremlinVal;
   char* buffer[PACKET_SIZE];
   struct sockaddr_in server;
   // struct hostent *hp;
   char *hello = "HI", *p;
       
   // Argument 0 will be the program name
   // Argument 1 should be the gremlin probability
   
   if (argc < 1) {
      printf("Incorrect usage of this application.\n Please use an argument for Gremlin. Default Gremlin is 0\n");
      // Give a default gremlin of 0 meaning no errors
      gremlinVal = 0.0;    
   }
   else {
    // define our gremlin based on argument 2
    
      gremlinVal = strtol(argv[2],&p,10);
    
   }

   sd = socket(AF_INET, SOCK_DGRAM, 0); //using a datagram

   server.sin_family = AF_INET;
   server.sin_port = htons(PORT);
   // hp = gethostbyname(argv[1]);
   // bcopy(hp->h_addr, &(server.sin_addr), hp->h_length);

   
   sendto(sd, (const char *)hello, strlen(hello), 0, (const struct sockaddr *) &server, sizeof(server) );    
   sleep(2);
   
   pthread_t  tid; // Create a unique thread ID
   pthread_create(&tid, NULL, socketTimer((void *)&tid, timerOverload,sd), (void *)&tid); // Create our socket timer with our new thread and ID
   originalOverload = timerOverload; // set the increase timer amount to the original timer amount
   
   for (;;) {
   
      
      
      n = recv(sd, buffer, sizeof(buffer), 0);
   
      selectiveRep(buffer);
      // server responded so refresh timer. We cant stop the timer but we can increase the time limit which is basically a refresh
      timerOverload = timerOverload + originalOverload;
   
   }


   return 0;
}