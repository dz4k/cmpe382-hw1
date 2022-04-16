
#include "parser.h"
#include "dynarray.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static bool whitespace(char ch);

typedef enum {
  INITIAL,
  ARG,
  REDIR_OUT,
} ParserState;

void CommandInit(Command *cmd) {
  cmd->redirectStdout = NULL;
  cmd->parseError = false;
  cmd->parallelWith = NULL;
  DynArrayInit(&cmd->args);
}

static void parse(Command* rv, char* line);

Command parseCommand(char *line) {
  Command rv;
  CommandInit(&rv);

  parse(&rv, line);

  return rv;
}

static void parse(Command* rv, char* line) {

  ParserState state = INITIAL;
  for (; *line != '\0'; line++) {
    if (*line == '&') {
      *line = '\0';
      rv->parallelWith = (Command*) malloc(sizeof(Command));
      parse(rv->parallelWith, line++);
      break;
    }
    if (*line == '>') {
      *line = '\0';
      state = REDIR_OUT;
      line++;
    }

    switch (state) {

    case INITIAL:
      if (!whitespace(*line)) {
        if (rv->redirectStdout != NULL) {
          // Multiple filenames after >
          rv->parseError = true;
          break;
        }
        DynArrayPush(&rv->args, line);
        state = ARG;
      }
      break;

    case ARG:
      if (whitespace(*line)) {
        *line = '\0';
        state = INITIAL;
      }
      break;

    case REDIR_OUT:
      if (*line == '\0') {
        rv->parseError = true;
      }

      if (!whitespace(*line)) {
        rv->redirectStdout = line;
        state = ARG;
      }
      break;
    }

  }
}

static bool whitespace(char ch) {
  return ch == ' ' || ch == '\t' || ch == '\t';
}

void CommandFree(Command *cmd) {
  DynArrayFree(&cmd->args);
  if (cmd->parallelWith != NULL) {
    CommandFree(cmd->parallelWith);
  }
}
