
#include "parser.h"
#include "dynarray.h"
#include <stdbool.h>
#include <stdlib.h>

static bool whitespace(char ch);

typedef enum {
  INITIAL,
  ARG,
  REDIR_IN,
  REDIR_OUT,
} ParserState;

void CommandInit(Command *cmd) {
  cmd->redirects.stdin = NULL;
  cmd->redirects.stdout = NULL;
  DynArrayInit(&cmd->args);
}

Command parseCommand(char *line) {
  Command rv;
  CommandInit(&rv);

  ParserState state = INITIAL;
  for (int i = 0; line[i] != '\0'; i++) {
    if (line[i] == '<') {
      line[i] = '\0';
      state = REDIR_IN;
      continue;
    }

    if (line[i] == '>') {
      line[i] = '\0';
      state = REDIR_OUT;
      continue;
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

    case REDIR_IN:
      if (!whitespace(line[i])) {
        rv.redirects.stdin = &line[i];
        state = ARG;
      }
      break;

    case REDIR_OUT:
      if (!whitespace(line[i])) {
        rv.redirects.stdout = &line[i];
        state = ARG;
      }
      break;
    }
  }
  return rv;
}

static bool whitespace(char ch) {
  return ch == ' ' || ch == '\t' || ch == '\t';
}

void CommandFree(Command *cmd) { DynArrayFree(&cmd->args); }
