#pragma once

#include <stdbool.h>

#include "DynamicString.h"
#include "globals.h"
#include "Vector.h"

static const size_t DEFAULT_VECTOR_CAPACITY = 10;

struct Command {
    struct GenericVector tokens;
    char* output_stream;
    char* input_stream;

    const char* ERROR_MSG;
    bool is_eof;
    bool append;
};

typedef struct Command Command;

// Creates command
struct Command CreateCommand();

// Prints tokens
void PrintCommand(struct Command* command);

char* Command_GetToken(Command* command, size_t i);

// Deallocates all memory, which was taken by struct Command. 
void Command_Free(struct Command* command);