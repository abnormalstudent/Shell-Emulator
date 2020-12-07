#pragma once

#include <assert.h>

#include "DynamicString.h"

struct GenericVector {
    void* storage;

    size_t element_size;
    size_t size;
    size_t capacity;
};

typedef struct GenericVector GenericVector;

// Initializes vector with given capacity.
struct GenericVector CreateVector(size_t capacity, size_t element_size);

// Appends element to the vector.
void Push(struct GenericVector* vec, void* element);

// Copies i-th elemnt to destination.
// sizeof(destination) before convertion to void* 
// must be equal to vec -> element_size;
void Get(struct GenericVector* vec, void* destination, size_t i);

// Return ptr to i-th element
void* GetPtr(GenericVector* vector, size_t i);

// Increases capacity by the factor of two, reallocates the memory,
// and also copies previous memory to the new memory. 
void _Reallocate(struct GenericVector* vec);

// De-allocates all the memory which vector got
void _FreeVector(struct GenericVector* vec);