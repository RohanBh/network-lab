#include <iostream>

using namespace std;

int main(int argc, char **argv) {
  string msg;
  string poly;
  cin >> msg >> poly;
  int bit = stoi(argv[1]);
  if (bit >= msg.length()) {
    cout << "Invalid Arguement supplied: msg_len=" << msg.length() << " bit=" << bit;
    return 1;
  }
  if (msg[bit] == '0') {
    msg.insert(bit, "1");
    msg.erase(bit + 1, 1);
  } else {
    msg.insert(bit, "0");
    msg.erase(bit + 1, 1);
  }
  cout << msg << "\n" << poly << "\n";
  return 0;
}
