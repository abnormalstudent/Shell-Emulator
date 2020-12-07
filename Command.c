#include "Command.h"

struct Command CreateCommand() {
    struct Command command;
    command.tokens = CreateVector(DEFAULT_CONTAINER_SIZE, sizeof(char*));
    command.input_stream = NULL;
    command.output_stream = NULL;

    command.ERROR_MSG = NULL;
    command.is_eof = false;
    command.append = false;
    return command;
}

void PrintCommand(struct Command* command) {
    for (size_t j = 0; j < command->tokens.size; j++) {
        char* temp = Command_GetToken(command, j);
        printf("{%s}", temp);
        if (j + 1 != command->tokens.size) {
            printf(", ");
        }
    }
    if (command->input_stream != NULL) {
        printf(", input stream : {%s}", command->input_stream);
    }
    if (command->output_stream != NULL) {
        printf(", output stream : {%s}, append : {%s}", command->output_stream, command->append ? "true" : "false");
    }
}
char* Command_GetToken(Command* command, size_t i) {
    char* element;
    Get(&command->tokens, &element, i);
    return element;
}
void Command_Free(struct Command* command) {
    for (size_t i = 0; i < command->tokens.size; i++) {
        free(*(char**) GetPtr(&command->tokens, i));
    }
    _FreeVector(&command->tokens);
    free(command->input_stream);

    command->input_stream = NULL;

    free(command->output_stream);
    command->output_stream = NULL;
}