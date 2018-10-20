#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <netdb.h>
#include <regex>
#define SERV_PORT 8080

using namespace std;

bool parse(string URL, string &remote, int &port, string &file) {
  regex urlRegex("(\\w+://)?[a-z]+\\.(\\w+\\.)+[a-zA-Z]+(:\\d+)?(/[\\w$\\-_\\!~\\+]+)*/\\w+\\.\\w+");
  if (!regex_match(URL, urlRegex)) {
    cout << "Bad URL: " << URL << "\n";
    return false;
  }
  regex hostnameRegex("[a-z]+\\.(\\w+\\.)+[a-zA-Z]+");
  smatch matcher;
  regex_search(URL, matcher, hostnameRegex);
  remote = matcher.str(0);
  regex portRegex(":\\d+");
  port = 80;
  regex_search(URL, matcher, portRegex);
  if (!matcher.str(0).empty()) {
    port = stoi((matcher.str(0).c_str()) + 1);
  }
  regex fileRegex("((/[\\w$\\-_!~\\+]+)*/\\w+\\.\\w+)$");
  regex_search(URL, matcher, fileRegex);
  file = matcher.str(0);
  return true;
}

// See: https://askubuntu.com/a/16583/672640
bool openFile(string file, string mime) {
  
}
  
int main(int argc, char **argv) {
  if (argc < 2) {
    cout << "Usage: web_browser URL\n";
    return -1;
  }

  string url, host, file;
  int port;
  url = argv[1];
  if (parse(url, host, port, file)) {
    printf("---------------\nURL: %s\nHost: %s\nPort: %d\nFile: %s\n---------------\n\n",
	   url.c_str(), host.c_str(), port, file.c_str());
  } else {
    return -1;
  }
  
  int s = 0; // client's socket descriptor
  int n = 1; // number of bytes recv from server in one call to recv
  struct sockaddr_in clientAddr = {};
  struct sockaddr_in servAddr = {};

  char buffer[1024] = {0};
    
  if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    cout << "Could not create socket!\n";
    return -1;
  }
    
  servAddr.sin_family = AF_INET;
  struct hostent *host_entry;
  host_entry =  gethostbyname(host.c_str());
  servAddr.sin_addr.s_addr = inet_addr(inet_ntoa(*((struct in_addr*)host_entry->h_addr_list[0])));
  servAddr.sin_port = htons(port);
  
  if (connect(s,(struct sockaddr*) &servAddr, sizeof(servAddr)) < 0) {
    cout << "Connection to server failed!\n";
    return -1;
  }

  // Use CRLF line ending: https://stackoverflow.com/a/5757349
  string message = "GET " + file + " HTTP/1.0\r\n\r\n";
  ofstream outfile("tmp", ios::out | ios::trunc);
  bool flag = false;
  regex mimeRegex("Content\\-Type: .+?;");
  cmatch matcher;
  string mimeType;
  
  cout << "Sending request...\n" << message;
  send(s , message.c_str() , strlen(message.c_str()), 0);
  while (n > 0) {
    n = recv(s, buffer, 1024, 0);
    buffer[n] = '\0';
    if (!flag) {
      regex_search(buffer, matcher, mimeRegex);
      string str = matcher.str(0);
      if (!str.empty()) {
	mimeType = str.substr(14, str.length() - 15);
	flag = true;
      }
    }
    outfile << buffer;
  }
  cout << "Request Successful!\n";
  close(s);
  outfile.close();
  
  return 0;
}
