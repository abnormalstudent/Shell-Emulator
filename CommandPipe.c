#include "CommandPipe.h"

CommandPipe Pipe_Create() {
    CommandPipe command_pipe;
    command_pipe.command_pipe = CreateVector(DEFAULT_CONTAINER_SIZE, sizeof(CommandList));
    return command_pipe;
}

void Pipe_Push(CommandPipe* pipe, CommandList* list) {
    Push(&pipe->command_pipe, list);
}

CommandList Pipe_Get(CommandPipe* pipe, size_t i) {
    CommandList element;
    Get(&pipe->command_pipe, &element, i);
    return element;
}

void Pipe_Free(CommandPipe* pipe) {
    for (size_t i = 0; i < pipe->command_pipe.size; i++) {
        CommandList_Free((struct CommandList*)GetPtr(&pipe->command_pipe, i));
    }
    _FreeVector(&pipe->command_pipe);
}

void Pipe_Print(CommandPipe* pipe) {
    for (size_t i = 0; i < pipe->command_pipe.size; i++) {
        if (i == 0)
            printf("---------------\n");
        printf("Command #%d : \n", i + 1);
        CommandList command_list = Pipe_Get(pipe, i);
        PrintCommandList(&command_list);
        printf("---------------\n");
    }
}