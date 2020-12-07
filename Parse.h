#pragma once

#include "Command.h"
#include "CommandList.h"
#include "DynamicString.h"
#include "globals.h"
#include "CommandPipe.h"

// Read one command
int read_command(struct Command* command, int c);

// Parse change of input
int ParseLeftArrow(CommandList* command_list, Command* command, int c);

// Parse change of output
int ParseRightArrow(CommandList* command_list, Command* command, int c);

// Parse the whole input and return CommandPipe
bool Parse(CommandPipe* commands);
