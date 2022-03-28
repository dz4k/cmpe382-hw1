
#include "shell_state.h"
#include "dynarray.h"

void ShellStateInit(ShellState* state) {
    DynArrayInit(&state->path);
    DynArrayPush(&state->path, (void*) "/bin");
}

void ShellStateFree(ShellState* state) {
    DynArrayFree(&state->path);
}
