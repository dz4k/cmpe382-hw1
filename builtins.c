
#include "builtins.h"
#include "dynarray.h"

BUILTIN(path) {
    for (int i = 1; i < args->count; i++) {
        DynArrayPush(&state->path, args->array[i]);
    }
    return 0;
}
