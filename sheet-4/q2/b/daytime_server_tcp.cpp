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
   One daytime service is defined as a connection based application on
   TCP.  A server listens for TCP connections on TCP port 13.  Once a
   connection is established the current date and time is sent out the
   connection as a ascii character string (and any data received is
   thrown away).  The service closes the connection after sending the
   quote.
   https://tools.ietf.org/html/rfc867
*/
int main() {
  int ls, s, n;
  struct sockaddr_in servAddr = {};
  struct sockaddr_in clientAddr = {};
  socklen_t clientAddrLen;
 
  if ((ls = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    cout << "listen socket creation failed!" << "\n";
    return -1;
  }

  int opt = 1;
  if (setsockopt(ls, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
		 &opt, sizeof(opt))) { 
      perror("setsockopt");
      exit(EXIT_FAILURE);
  } 

  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = INADDR_ANY;
  servAddr.sin_port = htons(SERV_PORT);

  if (bind(ls, (struct sockaddr*) &servAddr, sizeof(servAddr))<0) {
    cout << "listen socket binding failed!\n";
    return -1;
  }

  if (listen(ls, 5) < 0) {
    cout << "Could not complete connection establishment phase!\n";
    return -1;
  }
  
  while (true) {
    if ((s = accept(ls, (struct sockaddr*) &clientAddr, &clientAddrLen)) < 0) {
      cout << "Could not accept client!\n";
      continue;
    }
    cout << "Client connected!\n";
    time_t t = time(0); // represents the current time
    char *daytime = ctime(&t);
    send(s , daytime , strlen(daytime) , 0);
    cout << "Response sent: " << daytime << "\n";
  }
  close(s);
  close(ls);
  return 0;
}
