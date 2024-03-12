#define _GNU_SOURCE
#include "env.h"
#include <stdio.h>
#include <stdlib.h>

enum Choice { UNKNOWN_CHOICE, USE_GETENV, USE_ENVP, USE_ENVIRON, QUIT };

typedef const char *(*getenv_f)(const char *);

getenv_f getenvByChoice(enum Choice choice) {
  switch (choice) {
  case USE_GETENV: return (getenv_f)getenv;
  case USE_ENVP: return getFromMyEnviron;
  case USE_ENVIRON: return getFromEnviron;
  default: return NULL;
  }
}

int main(int argc, char **argv, char **envp) {
  (void)argc;
  const char *name = argv[0];
  const char *filename = argv[1];

  enum Choice choice;
  sscanf(argv[2], "%d", (int *)&choice);
  getenv_f myGetenv = getenvByChoice(choice);

  myEnviron = envp;

  printf("%s pid=%d ppid=%d\n", name, getpid(), getppid());

  FILE *file = fopen(filename, "r");
  while (!feof(file)) {
    char var[256] = {0};
    fscanf(file, "%s;", var);
    if (strlen(var) == 0) continue;
    printf("%s=%s\n", var, myGetenv(var));
  }
  fclose(file);
}
