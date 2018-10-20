#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <ctime>

#define SERV_PORT 8080

using namespace std;

/* 
   Another daytime service service is defined as a datagram based
   application on UDP.  A server listens for UDP datagrams on UDP port
   13.  When a datagram is received, an answering datagram is sent
   containing the current date and time as a ASCII character string (the
   data in the received datagram is ignored).
   https://tools.ietf.org/html/rfc867
*/
int main() {
  int s, n;
  struct sockaddr_in servAddr = {};
  struct sockaddr_in clientAddr = {};
  socklen_t clientAddrLen;

  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = INADDR_ANY;
  servAddr.sin_port = htons(SERV_PORT);
 
  if ((s = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
    cout << "Socket creation failed!" << "\n";
    return -1;
  }

  int opt = 1;
  if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
		 &opt, sizeof(opt))) { 
      perror("setsockopt");
      exit(EXIT_FAILURE);
  }

  if (bind(s, (struct sockaddr*) &servAddr, sizeof(servAddr))<0) {
    cout << "Socket binding failed!\n";
    return -1;
  }
  
  while (true) {
    n = recvfrom(s, NULL, 0, 0, (struct sockaddr*) &clientAddr, &clientAddrLen);
    cout << "Client connected!\n";
    time_t t = time(0); // represents the current time
    char *daytime = ctime(&t);
    sendto(s , daytime , strlen(daytime) , 0, (struct sockaddr*) &clientAddr, clientAddrLen);
    cout << "Response sent: " << daytime << "\n";
  }
  close(s);
  return 0;
}
