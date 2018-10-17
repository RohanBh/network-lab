#include <iostream>
#include <unistd.h>

using namespace std;

int main() {
  cout << "Parent Pid: " << getpid() << "\n";
  if (!(fork() && fork())) {
    cout << "Children Pid: " << getpid() << "\n";
    if (!(fork() && fork())) {
      cout << "Grand children Pid: " << getpid() << "\n";
    }
  }
  return 0;
}
