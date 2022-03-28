
/// A dynamic array.

#include <stdlib.h>

#include "dynarray.h"

static void DynArrayRealloc(DynArray* da) {
    da->array = (void**) realloc(da->array, da->capacity);
}

void DynArrayInit(DynArray* da) {
    da->capacity = 4;
    da->count = 0;
    da->array = (void**) malloc(sizeof(void*) * da->capacity);
}

void DynArrayFree(DynArray* da) {
    free(da->array);
}

// Adds an element to the end of the array.
void DynArrayPush(DynArray* da, void* element) {
    if (da->capacity <= da->count) {
        da->capacity *= 2;
        DynArrayRealloc(da);
    }

    da->array[da->count++] = element;
}
