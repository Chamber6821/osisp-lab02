#include <stdio.h>

enum Choice { UNKNOWN_CHOICE, USE_GETENV, USE_ENVP, USE_ENVIRON, QUIT };

int main(int argc, char **argv) {
  (void)argc;
  enum Choice choice;
  sscanf(argv[2], "%d", (int *)&choice);
  printf("I am %s! File is %s. Choice %d\n", argv[0], argv[1], choice);
  return 42;
}
