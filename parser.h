
#ifndef tedshell_parser
#define tedshell_parser

#include <stdbool.h>
#include "dynarray.h"

typedef struct sCommand {
  char *redirectStdout;
  bool parseError;
  DynArray args;
  struct sCommand *parallelWith;
} Command;

Command* parseCommand(char *line);
void CommandFree(Command *cmd);

#endif
