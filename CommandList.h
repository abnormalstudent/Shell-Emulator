#pragma once

#include "Vector.h"
#include "DynamicString.h"
#include "Command.h"
#include "globals.h"

// Simple vector of commands.
struct CommandList {  
    struct GenericVector list;
    bool background;
};

typedef struct CommandList CommandList;
// init function
struct CommandList CreateCommandList();

// Print CommandList
void PrintCommandList(struct CommandList* command_list);

// Push command to the command list
void CommandList_Push(CommandList* command_list, Command* command);

// Getter
Command CommandList_Get(CommandList* command_list, size_t i);

// Free all the memory of given command list (recursively)
void CommandList_Free(CommandList* command_list);