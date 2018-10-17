#include <string>
#include <iostream>
#include <algorithm>
using namespace std;

void read_input(string &msg, string &polynomial) {
  getline(cin, msg, '\n');
  getline(cin, polynomial, '\n');
}

void get_final_msg(string &msg, string &poly) {
  int k = poly.length() - 1;
  string remainder;
  for (int i = 0; i < k; i++) {
    remainder += "0";
  }
  int dividend = stoi(msg + remainder, nullptr, 2);
  int divisor = stoi(poly, nullptr, 2);
  
  while(dividend >> k) {
    int a = divisor;
    while((a << 1) < dividend) {
      a <<= 1;
    }
    dividend ^= a;
  }

  string bitstring = "";
  while(dividend) {
    bitstring += to_string(dividend & 1);
    dividend >>= 1;
  }

  reverse(bitstring.begin(), bitstring.end());
  msg += bitstring;
}

int main() {
  string msg, poly;
  read_input(msg, poly);
  get_final_msg(msg, poly);
  cout << msg << "\n" << poly << "\n";
  return 0;
}
