#include "Parse.h"

// Memory leak-free function.
int ParseWord(struct Command* command, int c) {
    while (!end_of_stream(c) && !separator(c)) {
        DynamicString string = CreateString(DEFAULT_CONTAINER_SIZE);
        // Skip initial spaces
        while (c == ' ') c = getchar();

        bool just_space = true;
        while (!end_of_word(c) && !separator(c)) {
            just_space = false;
            if (c == '\'') {
                while ((c = getchar()) != '\'') {
                    if (c == '\n') {
                        printf("> ");
                    }
                    AppendChar(&string, c);
                }
            } else if (c == '\"') {
                while ((c = getchar()) != '\"') {
                    if (c == '\n') {
                        printf("> ");
                        fflush(stdout);
                    } else if (c == '\\') {
                        int next = getchar();
                        if (next == '\"') {
                            AppendChar(&string, '\"');
                        } else if (next == '\n') {
                            printf("> ");
                            fflush(stdout);
                            continue;
                        } else {
                            AppendChar(&string, '\\');
                            AppendChar(&string, next);
                        }
                    } else {
                        AppendChar(&string, c);
                    }
                }
            } else if (c == '\\') {
                // Next can't be EOF here, checked it manually in bash
                int next = getchar();
                if (c == '\\') {
                    AppendChar(&string, '\\');
                } else if (c == '\n' || c == EOF) {
                    continue;
                } else {
                    AppendChar(&string, c);
                    AppendChar(&string, next);
                }
            } else {
                // Simple symbol, have to append
                AppendChar(&string, c);
            }
            c = getchar();
        }
        // Dynamic memory allocated in "string" is saved
        // in command -> tokens, so we can deallocate it later.
        if (!just_space) {
            Push(&command->tokens, &string.string);
        } else {
            FreeString(&string);
        }
    }
    return c;
}

int ParseRightArrow(CommandList* command_list, Command* command, int c) {
    Command single_argument = CreateCommand();
    c = getchar();
    bool append = c == '>';
    if (append) {
        c = getchar();
    }
    c = ParseWord(&single_argument, c);
    if (append && single_argument.tokens.size != 1) {
        printf("Number of arguments in '>>' redirection doesn't match : must be 1, but got %d\n", single_argument.tokens.size);
    } else if (!append && single_argument.tokens.size != 1) {
        printf("Number of arguments in '>' redirection doesn't match : must be 1, but got %d\n", single_argument.tokens.size);
    } else {
        command->output_stream = Command_GetToken(&single_argument, 0);
        command->append = append;
        // printf("New output stream : {%s}\n", Command_GetToken(&single_argument, 0));
    }
    if (end_of_stream(c)) {
        CommandList_Push(command_list, command);
    }
    _FreeVector(&single_argument.tokens);
    return c;
}

int ParseLeftArrow(CommandList* command_list, Command* command, int c) {
    Command single_argument = CreateCommand();
    c = getchar();
    c = ParseWord(&single_argument, c);
    if (single_argument.tokens.size != 1) {
        printf("Number of arguments in '<' redirection doesn't match : must be 1, but got %d\n", single_argument.tokens.size);
    } else {
        // printf("New input stream : {%s}\n", Command_GetToken(&single_argument, 0));
        command -> input_stream = Command_GetToken(&single_argument, 0);
    }
    if (end_of_stream(c)) {
        CommandList_Push(command_list, command);
    }
    _FreeVector(&single_argument.tokens);
    return c;
}

bool Parse(CommandPipe* commands) {
    int c = getchar();
    
    CommandList command_list = CreateCommandList();
    struct Command command;
    bool new_command = true;
    while (!end_of_stream(c)) {
        if (new_command)
            command  = CreateCommand();
        if (!separator(c))
            c = ParseWord(&command, c);
        if (end_of_stream(c)) {
            CommandList_Push(&command_list, &command);
            command_list.background = false;
            Pipe_Push(commands, &command_list);
            command_list = CreateCommandList();
            break;
        } else if (c == '&') {
            CommandList_Push(&command_list, &command);
            command_list.background = true;
            // Since we copy data of the list, we can clear it later
            Pipe_Push(commands, &command_list);
            command_list = CreateCommandList();
            new_command = true;
        } else if (c == '|') {
            CommandList_Push(&command_list, &command);
            new_command = true;
        } else if (c == '<') {
            c = ParseLeftArrow(&command_list, &command, c);
            new_command = false;
            continue;
        } else if (c == '>') {
            c = ParseRightArrow(&command_list, &command, c);
            new_command = false;
            continue;
        }
        c = getchar();
        fflush(stdout);
    }
    if (command_list.list.size != 0) {
        Pipe_Push(commands, &command_list);
    } else {
        CommandList_Free(&command_list);
    }
    return c == EOF;
}