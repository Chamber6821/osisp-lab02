#include <string.h>
#include <unistd.h>

static char *const *myEnviron = NULL;

static const char *getFromList(const char *name, char *const *envp) {
  char prefix[strlen(name) + 1];
  strcat(strcpy(prefix, name), "=");
  for (char *const *it = envp; *it; ++it) {
    if (strncmp(prefix, *it, strlen(prefix)) != 0) continue;
    return strchr(*it, '=') + 1;
  }
  return NULL;
}

static const char *getFromMyEnviron(const char *name) {
  return getFromList(name, myEnviron);
}

static const char *getFromEnviron(const char *name) {
  return getFromList(name, environ);
}
