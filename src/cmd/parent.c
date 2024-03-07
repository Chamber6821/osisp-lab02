#define _XOPEN_SOURCE
#define _GNU_SOURCE
#include "error.h"
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

enum Choice { UNKNOWN_CHOICE, USE_GETENV, USE_ENVP, USE_ENVIRON, QUIT };

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

const char *choiceChildDir(enum Choice choice, char *const *envp) {
  switch (choice) {
  case USE_GETENV: return childPathUseGetenv();
  case USE_ENVP: return childPathUseEnvp(envp);
  case USE_ENVIRON: return childPathUseEnviron();
  default: return NULL;
  }
}

enum Choice char2choice(char ch) {
  switch (ch) {
  case '+': return USE_GETENV;
  case '*': return USE_ENVP;
  case '&': return USE_ENVIRON;
  case 'q': return QUIT;
  default: return UNKNOWN_CHOICE;
  }
}

int getch() {
  char ch;
  scanf(" %c", &ch);
  return ch;
}

void complexForkChild(enum Choice choice, char *const *envp) {
  const char *childDir = choiceChildDir(choice, envp);
  const char *childName = "/child";
  if (!childDir)
    error(CHILD_PATH_NOT_SET, "Environment variable CHILD_PATH not set");
  char childPath[strlen(childDir) + strlen(childName) + 1];

  char *const argv[] = {NULL};
  forkChild(strcat(strcpy(childPath, childDir), childName), argv, NULL);
}

bool reactToChoice(enum Choice choice, char *const *envp) {
  switch (choice) {
  case UNKNOWN_CHOICE: printf("Unknown choice\n"); return true;
  case QUIT: return false;
  default: complexForkChild(choice, envp); return true;
  }
}

int main(int argc, char **argv, char **envp) {
  (void)argc;
  (void)argv;

  sortString(envp, strcmp);
  for (char **env = envp; *env; ++env) {
    printf("%s\n", *env);
  }

  while (reactToChoice(char2choice(getch()), envp))
    ;
}
