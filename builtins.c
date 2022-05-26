
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "builtins.h"
#include "dynarray.h"
#include "error.h"


const BuiltinsTableEntry builtins[] = {
    {"cd", cd},
    {"exit", exit_},
    {"path", path},
    {NULL, NULL},
};

BUILTIN(cd) {
  if (args->count != 2) {
    ERROR();
    return 1;
  }
  if (chdir((char *)args->array[1]) != 0) {
    ERROR();
    return 1;
  };
  return 0;
}

BUILTIN(exit_) {
  if (args->count > 1) {
    ERROR();
    return 1;
  }
  exit(0);
}

BUILTIN(path) {
  DynArrayClear(&state->path);
  for (int i = 1; i < args->count; i++) {
    DynArrayPush(&state->path, strdup((char *)args->array[i]));
  }
  return 0;
}

