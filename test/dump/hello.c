#include <stdio.h>
int main() {
  int n = 50, i;
  unsigned long long factorial = 1;
  for(i=1; i<=n; ++i) {
      factorial *= i;
  }
  // printf("Factorial of %d = %llu", n, factorial);
  return 0;
}
