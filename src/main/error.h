enum Error { JUST_QUIT };

void error(enum Error code, const char *format, ...)
    __attribute__((format(printf, 2, 3)));
