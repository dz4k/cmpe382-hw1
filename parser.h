
#ifndef tedshell_parser
#define tedshell_parser

#include <stdbool.h>
#include "dynarray.h"

typedef struct {
  char *stdin;
  char *stdout;
} Redirects;

typedef struct {
  Redirects redirects;
  bool parseError;
  DynArray args;
} Command;

Command parseCommand(char *line);
void CommandFree(Command *cmd);

#endif
