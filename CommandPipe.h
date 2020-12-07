#pragma once

#include "CommandList.h"
#include "globals.h"

// CommandPipe is a Data Structure (along with all others)
// which holds list of commands.
// It is much easier to implement conveyor if we store commands in that way. 
struct CommandPipe {
    GenericVector command_pipe;
    bool is_eof;
};

typedef struct CommandPipe CommandPipe;

// Creates a list of command lists.
CommandPipe Pipe_Create();

// Push commnad list to a pipe
void Pipe_Push(CommandPipe* pipe, CommandList* list);

// Getter
CommandList Pipe_Get(CommandPipe* pipe, size_t i);

// Free all the memory allocated by pipe (included recursive freeing)
void Pipe_Free(CommandPipe* pipe);

// If verbose, prints data structure used to store parsed command
void Pipe_Print(CommandPipe* pipe);