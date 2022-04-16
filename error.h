
#ifndef tedshell_error
#define tedshell_error

//#define ERROR() write(STDERR_FILENO, "An error has occurred\n", 22)
#define ERROR() fprintf(stderr, "An error has occurred at %s:%d\n", __FILE__, __LINE__)

#endif
