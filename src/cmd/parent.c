#define _XOPEN_SOURCE
#define _GNU_SOURCE
#include "error.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void sortString(char **strings, int (*comparator)(const char *, const char *)) {
  for (char **left = strings; *left; ++left)
    for (char **right = left + 1; *right; ++right) {
      if (comparator(*left, *right) > 0) {
        char *temp = *left;
        *left = *right;
        *right = temp;
      }
    }
}

int main(int argc, char **argv, char **envp) {
  (void)argc;
  (void)argv;

  sortString(envp, strcmp);
  for (char **env = envp; *env; ++env) {
    printf("%s\n", *env);
  }

  const char *childDir = getenv("CHILD_PATH");
  const char *childName = "/child";
  if (!childDir)
    error(CHILD_PATH_NOT_SET, "Environment variable CHILD_PATH not set");
  char childPath[strlen(childDir) + strlen(childName) + 1];
  strcat(strcpy(childPath, childDir), childName);
}
