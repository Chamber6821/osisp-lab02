enum Error { JUST_QUIT, CHILD_PATH_NOT_SET, CHILD_FAILED, NO_ENOUGH_ARGUMENTS };

void error(enum Error code, const char *format, ...)
    __attribute__((format(printf, 2, 3)));
