
#ifndef tedshell_state
#define tedshell_state

#include "dynarray.h"

typedef struct {
    DynArray path;
} ShellState;

void ShellStateInit(ShellState* state);

void ShellStateFree(ShellState* state);

#endif
