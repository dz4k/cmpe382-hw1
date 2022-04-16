
#ifndef tedshell_error
#define tedshell_error

#define ERROR() write(STDERR_FILENO, "An error has occurred\n", 22)

#endif
