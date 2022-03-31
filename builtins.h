
#ifndef TEDShell_builtins
#define TEDShell_builtins

#include "dynarray.h"
#include "shell_state.h"
#include <stdlib.h>

#define BUILTIN(name) int name(ShellState *state, DynArray *args)

typedef BUILTIN((*Builtin));

typedef struct {
  const char *const name;
  Builtin builtin;
} BuiltinsTableEntry;

BUILTIN(path);

const BuiltinsTableEntry builtins[] = {
    {"path", path},
    {NULL, NULL},
};

#endif
