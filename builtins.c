
#include <stdio.h>
#include <unistd.h>
#include "builtins.h"
#include "dynarray.h"

BUILTIN(cd) {
  if (args->count != 2) {
    return 1;
  }
  return chdir((char *)args->array[0]);
}

BUILTIN(exit_) {
  if (args->count > 1) {
    return 1;
  }
  exit(0);
}

BUILTIN(path) {
  for (int i = 1; i < args->count; i++) {
    DynArrayPush(&state->path, args->array[i]);
  }
  return 0;
}

