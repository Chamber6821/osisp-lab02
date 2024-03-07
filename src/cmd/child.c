#include <stdio.h>

int main(int argc, char **argv) {
  if (argc < 1) printf("I am child without name!!!\n");
  else
    printf("I am %s!\n", argv[0]);
  return 42;
}
