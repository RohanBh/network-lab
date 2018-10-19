#define _GLIBCXX_USE_CXX11_ABI 0
#include <bits/stdc++.h>

using namespace std;

int main(int argc, char** argv) {
  if (argc < 2) {
    printf("Usage: ./reassembly filename\n");
    return 1;
  }
  string file = argv[1];
  ifstream fin;
  fin.open(file);
  ofstream fout;
  fout.open("reassembly.txt");
  int length = 0;
  int fragment_offset = 0;
  char *s = new char[100];
  fin.read(s, 5);
  int MTU = atoi(s) + 17;
  fin.seekg(0, ios::beg);
  bool flag = true;
  while(flag) {
    streamsize size = MTU;
    fin.read(s, size);
    char temp[6];
    strncpy(temp, s, 5);
    temp[5] = '\0';
    length = atoi(temp);
    strncpy(temp, s + 7, 5);
    temp[5] = '\0';
    fragment_offset = atoi(temp);
    strncpy(temp, s + 14, 1);
    temp[1] = '\0';
    flag = atoi(temp);
    //    printf("%d %d %d %d\n", length, fragment_offset, flag, MTU);
    fout.write(s + 17, length);
    fin.seekg(1, ios::cur); // move get pointer to avoid the new line
  }
  fin.close();
  fout.close();
  return 0;
}
