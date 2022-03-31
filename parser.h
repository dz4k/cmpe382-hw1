
#ifndef tedshell_parser
#define tedshell_parser

#include "dynarray.h"

typedef struct {
  char *stdin;
  char *stdout;
} Redirects;

typedef struct {
  Redirects redirects;
  DynArray args;
} Command;

Command parseCommand(char *line);
void CommandFree(Command *cmd);

#endif
