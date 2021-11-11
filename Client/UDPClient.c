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
int mutexSocketLock = 0; // Prevents multiple threads from becoming socket timer

   // RUN TIMER FOR PACKET ACK OR NAK
void packetTimer(int timerMax, int seqNum)
{
   clock_t before = clock(); // We want to start the clock
   int startTime = 0;
   
   while(startTime < timerMax) {
      clock_t difference = clock() - before;
      startTime = difference * 1000 / CLOCKS_PER_SEC;
     
   }
   printf("Packet with sequence: %d has not been acknowledged by server...\n", seqNum);


}


// Create a new timer for a packet ACK timer length defined by timerMax
void createPacketTimer(int seqNum, int timerMax) {

   pthread_t  tid; // Create a unique thread ID
   pid_t pid2 = fork(); // Forking can be an issue since if we already forked a thread in two then forking the main thread can fork into 4 threads and so on
// Thus we will only fork child threads to be safe
// This pid value of 0 will represent the forked child process

   if (pid2 < 0) { /* error occurred */
      perror("Fork of pid2 failed");
   }
   // Needs to be a child since we want to resend a packet when timer expires and not stop the program
   if (pid2 == 0) { /* child process */
      printf("I am the parent starting the packet timer for %d miliseconds for sequence: %d\n", timerMax,seqNum);
      packetTimer(timerMax,seqNum);
      // Do stuff here since paket was not acknowleged by server
      
      
      exit(0); // kill this thread
   }

// Parent process returns to last function call
      

}


// SELECTIVE REPEAT PROTOCOL
// window size for sender and receiver is 8
// sequence number is modulo 24
char * selectiveRep(char *packet[PACKET_SIZE]) {
// we are the sender so no response the first time
// we start with sequence numbers 0 through 7 being in our window
// sequence number is first byte of our packet packet[0-7]

   int windowSize = 8, timerMax = 1000;
   // Timer is in miiseconds
   
   
   createPacketTimer(0,timerMax);
  
   return 0;

}

   
// RUN TIMER FOR MAIN SOCKET CONNECTION
// timerOverload needs to be global so multiple threads can refer and update the same number
void socketTimer(int sd)
{
            
   clock_t before = clock(); // We want to start the clock
   while (startTime < timerOverload) {
      
      
      clock_t difference = clock() - before;
      startTime = difference * 1000 / CLOCKS_PER_SEC;
      
      
   }
   printf("Timer overload: No response from server... Closing Socket");
   close(sd);
   
   exit(0); // Should close all threads in the client program <- only the original thread can do this
   // Child threads will only close themselves and not end their parents
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
   
   if (argc < 2) {
      printf("Please use an argument for Gremlin. Default Gremlin is 0\n");
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
   server.sin_addr.s_addr = INADDR_ANY;
   // hp = gethostbyname(argv[1]);
   // bcopy(hp->h_addr, &(server.sin_addr), hp->h_length);

   
   sendto(sd, (const char *)hello, strlen(hello), 0, (const struct sockaddr *) &server, sizeof(server) );    
   socklen_t len = sizeof(server);

   
   pthread_t  tid; // Create a unique thread ID
   pid_t pid = fork(); // Forking can be an issue since if we already forked a thread in two then forking the main thread can fork into 4 threads and so on
// Thus we will only fork child threads to be safe
// This pid value of 0 will represent the forked child process

   if (pid < 0) { /* error occurred */
      perror("Fork failed");
   }
   if (pid != 0) { /* Parent process */
      printf("I am the parent starting the timer as process %d\n", (int) getpid());
      socketTimer(sd);
      exit(0);
   }
   /* Child process */
   originalOverload = timerOverload; // set the increase timer amount to the original timer amount
   printf("I am the child connecting to server as process %d\n", (int) getpid());

   for (;;) {
   
      
      n = recvfrom(sd, (char *)buffer, 512, 0, (struct sockaddr *) &server, &len ); 
      //buffer[n] = '\0';
      printf("Client received : %s\n", (char*)buffer);
      
      
      sleep(1);
      selectiveRep(buffer);
      // server responded so refresh timer. We cant stop the timer but we can increase the time limit which is basically a refresh
      timerOverload = timerOverload + originalOverload;
   
   }


   return 0;
}