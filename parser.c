
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
  DynArrayInit(&cmd->args);
}

Command parseCommand(char *line) {
  Command rv;
  CommandInit(&rv);

  ParserState state = INITIAL;
  for (int i = 0; line[i] != '\0'; i++) {
    if (line[i] == '>') {
      line[i] = '\0';
      state = REDIR_OUT;
      i++;
    }

    switch (state) {

    case INITIAL:
      if (!whitespace(line[i])) {
        DynArrayPush(&rv.args, &line[i]);
        state = ARG;
      }
      break;

    case ARG:
      if (whitespace(line[i])) {
        line[i] = '\0';
        state = INITIAL;
      }
      break;

    case REDIR_OUT:
      if (line[i] == '\0') {
        rv.parseError = true;
      }

      if (!whitespace(line[i])) {
        rv.redirectStdout = &line[i];
        state = ARG;
      }
      break;
    default:
    fprintf(stderr, "what");
    }

  }
  return rv;
}

static bool whitespace(char ch) {
  return ch == ' ' || ch == '\t' || ch == '\t';
}

void CommandFree(Command *cmd) { DynArrayFree(&cmd->args); }
