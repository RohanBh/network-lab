#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <bits/stdc++.h>
#define SERV_PORT 8080

using namespace std;
  
int main(int argc, char const *argv[]) {
  int s = 0; // client's socket descriptor
  int n; // number of bytes recv from server in one call to recv
  struct sockaddr_in clientAddr = {};
  struct sockaddr_in servAddr = {};

  char buffer[1024] = {0};
    
  if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    cout << "Could not create socket!\n";
    return -1;
  }
    
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  servAddr.sin_port = htons(SERV_PORT);
  
  if (connect(s,(struct sockaddr*) &servAddr, sizeof(servAddr)) < 0) {
    cout << "Connection to server failed!\n";
    return -1;
  }
  
  string message;
  getline(cin, message);
  send(s , message.c_str() , strlen(message.c_str()), 0);
  n = recv(s, buffer, 1024, 0);
  buffer[n] = '\0';
  cout << buffer << "\n";
  close(s);
  
  return 0;
}
