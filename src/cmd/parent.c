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

void forkChild(const char *path, char *const *argv, char *const *envp) {
  if (fork() == 0) {
    if (execve(path, argv, envp) == -1)
      error(CHILD_FAILED, "execve(...) failed, errno: %d", errno);
  }
}

const char *childPathUseGetenv() { return getenv("CHILD_PATH"); }

const char *childPathUseEnvp(char *const *envp) {
  const char *prefix = "CHILD_PATH=";
  for (char *const *it = envp; *it; ++it) {
    if (strncmp(prefix, *it, strlen(prefix)) != 0) continue;
    return strchr(*it, '=') + 1;
  }
  return NULL;
}

const char *childPathUseEnviron() { return childPathUseEnvp(environ); }

int main(int argc, char **argv, char **envp) {
  (void)argc;
  (void)argv;

  sortString(envp, strcmp);
  for (char **env = envp; *env; ++env) {
    printf("%s\n", *env);
  }

  printf("childPathUseGetenv:  '%s'\n", childPathUseGetenv());
  printf("childPathUseEnvp:    '%s'\n", childPathUseEnvp(envp));
  printf("childPathUseEnviron: '%s'\n", childPathUseEnviron());

  const char *childDir = childPathUseGetenv();
  const char *childName = "/child";
  if (!childDir)
    error(CHILD_PATH_NOT_SET, "Environment variable CHILD_PATH not set");
  char childPath[strlen(childDir) + strlen(childName) + 1];
  strcat(strcpy(childPath, childDir), childName);

  forkChild(childPath, NULL, NULL);

  printf("child: '%s'\n", childPath);
}
