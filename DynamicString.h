#pragma once 

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "utils.h"

struct DynamicString {
    char* string;
    int capacity;
    int size;
};

typedef struct DynamicString DynamicString;

// Let's say, "default constructor".
// Allocates new memory to the string, 
// Capacity MUST be > 0, because
// according to architecture we always have to 
// store the '\0' symbol at the end of the string.
struct DynamicString CreateString(int capacity);


// Doubles the strings' capacity and reallocates the memory,
// so we can store more information in the given string.
void _ReallocString(struct DynamicString* str, u_int32_t length, bool verbose);


// Appends "const char*" string to the given dynamic string.
// Keeps '\0' symbol at the end.  
void Append(struct DynamicString* str, const char* right);

// Appends "char" to the given dynamic string.
// Keeps '\0' symbol at the end.
void AppendChar(struct DynamicString* str, char symbol);

// Prints the dynamic string.
void PrintString(struct DynamicString* str);

// De-allocates the memory for the given dynamic string.
void FreeString(struct DynamicString* str);

// Prints all fields of this DS in appropriate way.
void GetInfo(struct DynamicString* str);