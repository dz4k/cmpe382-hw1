
#ifndef tedshell_parser
#define tedshell_parser

#include "dynarray.h"

typedef struct {
  struct {
    char *stdin;
    char *stdout;
  } redirects;
  DynArray args;
} Command;

Command parseCommand(char *line);
void CommandFree(Command *cmd);

#endif