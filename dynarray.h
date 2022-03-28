
/// A dynamic array.

#ifndef tedshell_dynarray
#define tedshell_dynarray

typedef struct {
    int count;
    int capacity;
    void** array;
} DynArray;

// Creates a new dynamic array.
void DynArrayInit(DynArray* da);

// Frees components of a dynamic array.
void DynArrayFree(DynArray* da);

// Adds an element to the end of the array.
void DynArrayPush(DynArray* da, void* element);

#endif
