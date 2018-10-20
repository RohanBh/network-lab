#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <bits/stdc++.h>

using namespace std;
  
int main(int argc, char const *argv[]) {
  int s;
  int n;
  struct sockaddr_in servAddr = {};
  
  char buffer[1024] = {0};

  if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    cout << "Could not create client's socket\n";
    return -1;
  }
  
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  servAddr.sin_port = htons(argc == 1 ? 13 : stoi(argv[1]));
  
  if (connect(s, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
    cout << "Connection to daytime tcp server failed!\n";
    return -1;
  }
   
  n = recv(s , buffer, 1024, 0);
  buffer[n] = '\0';
  cout << buffer;
  close(s);
  return 0;
}
