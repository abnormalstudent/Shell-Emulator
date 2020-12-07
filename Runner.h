#pragma once

#include "sys/wait.h"
#include "sys/fcntl.h"
#include "signal.h"

#include "CommandPipe.h"

// Run parsed input 
bool Run(CommandPipe* pipe);

// Run command list
bool RunCommandList(CommandList* command_list);

// Run single command
void RunSingle(struct Command* command);

// If command has an error, print it
void PrintError(Command* command);

// If using conveyor, create needed amount of pipes
int* CreatePipes(size_t length);

// Close previously created pipes
void ClosePipes(int* pipes, size_t pipes_length);

// Check if command has errors and run embedded if can.
bool RunInputEmbedded(struct Command* command);

// Open new input stream
void open_read_file(const char* input);

// Open new output stream
void open_write_file(const char* output, bool append);