#include "Vector.h"
struct GenericVector CreateVector(size_t capacity, size_t element_size) {
    assert(capacity >= 0);
    
    struct GenericVector vec;
    vec.size = 0;
    vec.capacity = capacity;
    vec.element_size = element_size;
    vec.storage = malloc(capacity * element_size);
    return vec; 
}

void _Reallocate(struct GenericVector* vec) {
    size_t new_capacity  = (vec -> capacity == 0) ? 1 : vec -> capacity << 1;
    struct GenericVector new_vector = CreateVector(new_capacity, vec->element_size);

    // We don't call "reallocate" here, so 
    // "push" and "reallocate" functions don't
    // call each other recursively.
    for (size_t i = 0; i < vec -> size; i++) {
        Push(&new_vector, vec->storage + (i * vec -> element_size));
    }
    // void* new_storage = malloc(new_capacity * vec->element_size);
    // for (size_t i = 0; i < vec->size; i++) {
    //     memcpy(new_storage + (i * vec->element_size), vec->storage + i * vec->element_size, vec->element_size);
    // }
    // _FreeVector(vec);
    vec -> capacity = new_capacity;
    vec -> storage = new_vector.storage;
}

void Push(struct GenericVector* vec, void* element) {
    if (vec -> size == vec -> capacity) {
        _Reallocate(vec);
    }
    memcpy(vec->storage + (vec->size * vec->element_size), element, vec->element_size);
    vec->size++;
}

void Get(struct GenericVector* vec, void* destination, size_t i) {
    assert(i >= 0 && i < vec->size);
    memcpy(destination, vec->storage + (i * vec->element_size), vec->element_size);
}

void* GetPtr(GenericVector* vector, size_t i) {
    return vector->storage + (i * vector->element_size);
}

void _FreeVector(struct GenericVector* vec) {
    free(vec->storage);
    // So that we know that this object is cleared
    vec->storage = NULL;
}