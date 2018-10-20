#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <stdlib.h>
#define SERV_PORT 8080

using namespace std;

int main() {
  int ls; // the listen socket descriptor
  int s; // the data transfer socket descriptor
  int n; 
  struct sockaddr_in servAddr = {}; // initialize struct
  struct sockaddr_in clientAddr = {};
  socklen_t clientAddrLen;
  char buffer[1024] = {0};

  //  https://stackoverflow.com/questions/6729366/what-is-the-difference-between-af-inet-and-pf-inet-in-socket-programming
  if ((ls = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    cout << "listen socket creation failed!" << "\n";
    return -1;
  }

  int opt = 1;
  // Forcefully attaching socket to the port 8080, copied from geeksforgeeks
    if (setsockopt(ls, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 

  // AF_INET for TCP/IP protocol suite
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = INADDR_ANY;
  servAddr.sin_port = htons(SERV_PORT);

  // Fills the local socket address field of listen socket
  if (bind(ls, (struct sockaddr*) &servAddr, sizeof(servAddr))<0) {
    cout << "listen socket binding failed!\n";
    return -1;
  }

  if (listen(ls, 5) < 0) {
    cout << "Could not complete connection establishment phase!\n";
    return -1;
  }
  
  while (true) {
    memset(buffer, 0, 1024);
    if ((s = accept(ls, (struct sockaddr*) &clientAddr, &clientAddrLen)) < 0) {
      cout << "Could not accept client!\n";
      continue;
    }
    cout << "Client connected!\n";
    n = recv(s, buffer, 1024, 0);
    buffer[n] = '\0';
    cout << "Packet recieved: " << buffer << "\n";
    string upper = buffer;
    transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
    send(s , upper.c_str() , strlen(upper.c_str()) , 0);
    cout << "Response sent: " << upper << "\n";
  }
  close(ls);
  close(s);
  return 0;
}
