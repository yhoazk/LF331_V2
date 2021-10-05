#include <iostream>
#include <cstdlib>
#include <sys/types.h>
#include <unistd.h>

int main() {
  std::cout << "PID: " << getpid();
  return 0;
}
