#include "error.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  const char *childPath = getenv("CHILD_PATH");
  if (!childPath)
    error(CHILD_PATH_NOT_SET, "Environment variable CHILD_PATH not set");
  pid_t child = fork();
  if (child == 0) {
    char *argv[] = {NULL};
    if (execv("build/child", argv) == -1) return errno;
  }
  printf("I am parent!\n");

  waitpid(-1, NULL, 0);
}
