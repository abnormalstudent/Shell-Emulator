#include "CommandList.h"

struct CommandList CreateCommandList() {
    struct CommandList command_list;
    command_list.list = CreateVector(DEFAULT_CONTAINER_SIZE, sizeof(struct Command));
    return command_list;
}

void PrintCommandList(struct CommandList* command_list) {
    printf("List : \n");
    for (size_t i = 0; i < command_list->list.size; i++) {
        printf("%d) {", i + 1);
        PrintCommand((struct Command*)GetPtr(&command_list->list, i));
        printf("}\n");
    }
    printf("Background : %s\n", command_list->background == true ? "true" : "false");
}

void CommandList_Push(CommandList* command_list, Command* command) {
    Push(&command_list->list, command);
}

Command CommandList_Get(CommandList* command_list, size_t i) {
    Command command;
    Get(&command_list->list, &command, i);
    return command;
}

void CommandList_Free(CommandList* command_list) {
    for (size_t i = 0; i < command_list->list.size; i++) {
        Command_Free((struct Command*)GetPtr(&command_list->list, i));
    }
    _FreeVector(&command_list->list);
}