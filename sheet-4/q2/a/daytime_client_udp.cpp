#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <bits/stdc++.h>

using namespace std;
  
int main(int argc, char const *argv[]) {
  int s; // socket descriptor of client
  int n; // number of bytes recieved
  struct sockaddr_in servAddr = {}; // daytime server address
  char buffer[1024] = {0};
  
  if ((s = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
    cout << "Could not create socket!\n";
    return -1;
  }
  
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  servAddr.sin_port = htons(argc == 1 ? 13: stoi(argv[1]));
  
  string message = "any stupid string";
  sendto(s , message.c_str() , strlen(message.c_str()), 0,
	 (const struct sockaddr *) &servAddr, sizeof(servAddr));

  n = recvfrom(s , buffer, 1024, 0, NULL, NULL);
  buffer[n] = '\0';
  cout << buffer;

  close(s);
  return 0;
}
