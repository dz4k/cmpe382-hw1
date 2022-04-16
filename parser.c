
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

Command* parseCommand(char *line) {
  Command *rv = (Command*) malloc(sizeof(Command));
  CommandInit(rv);

  bool blankLine = true;

  ParserState state = INITIAL;
  for (; *line != '\0'; line++) {
    if (!whitespace(*line)) {
      blankLine = false;
    }
    if (*line == '&') {
      *line = '\0';
      state = ARG;
      rv->parallelWith = parseCommand(++line);
      if (rv->parallelWith->parseError) {
        goto parseError;
      }
      break;
    }
    if (*line == '>') {
      if (rv->redirectStdout != NULL) {
        // Multiple > in command
        goto parseError;
      }
      *line = '\0';
      state = REDIR_OUT;
      line++;
    }

    switch (state) {
    case INITIAL:
      if (!whitespace(*line)) {
        if (rv->redirectStdout != NULL) {
          // Multiple filenames after >
          goto parseError;
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
        goto parseError;
      }

      if (!whitespace(*line)) {
        rv->redirectStdout = line;
        state = ARG;
      }
      break;
    }

  }

  if (rv->args.count == 0) {
    goto parseError;
  }

  if (blankLine) {
    CommandFree(rv);
    return NULL;
  }

  return rv;

parseError:
  rv->parseError = true;
  return rv;
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
