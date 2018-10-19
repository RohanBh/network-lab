#define _GLIBCXX_USE_CXX11_ABI 0
#include <bits/stdc++.h>

using namespace std;

int main(int argc, char** argv) {
  if (argc < 3) {
    printf("Usage: ./fragment filename MTU\n");
    return 1;
  }
  int MTU = stoi(argv[2]);
  string file = argv[1];
  ifstream fin;
  fin.open(file);
  ofstream fout;
  fout.open("fragmented.txt");
  int length = 0;
  int fragment_offset = 0;
  char *s = new char[MTU - 17];
  while(fin.peek() != EOF) {
    streamsize size = MTU - 17;
    fin.read(s, size);
    length = fin.gcount();
    stringstream ss_offset;
    ss_offset << setw(5) << setfill('0') << fragment_offset;
    stringstream ss_len;
    ss_len << setw(5) << setfill('0') << length;
    fragment_offset += length;
    fout.write(ss_len.str().c_str(), 5);
    fout.write("--", 2);
    fout.write(ss_offset.str().c_str(), 5);
    fout.write("--", 2);
    if (fin.peek() == EOF) {
      fout.write("0--", 3);
      fout.write(s, (streamsize)length);
    } else {
      fout.write("1--", 3);
      fout.write(s, size);
    }
    fout.write("\n", 1);
  }
  fin.close();
  fout.close();
  return 0;
}
