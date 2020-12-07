#include "Runner.h"

bool Run(CommandPipe* pipe) {
    bool got_exit = false;
    for (size_t i = 0; i < pipe->command_pipe.size; i++) {
        got_exit |= RunCommandList(GetPtr(&pipe->command_pipe, i));
    }
    return got_exit;
}

bool RunCommandList(CommandList* command_list) {
    size_t length = command_list->list.size;
    assert (length > 0);
    Command first_command = CommandList_Get(command_list, 0);
    if (first_command.ERROR_MSG != NULL) {
        PrintError(&first_command);
        return 0;
    }
    if (RunInputEmbedded(&first_command)) {
        PrintError(&first_command);  
        return strcmp(Command_GetToken(&first_command, 0), "exit") == 0 && first_command.ERROR_MSG == NULL;   
    }

    // Hence length >= 1, hence length - 1 >= 0, do the conveyor    
    int child = fork();
    if (child == -1) {
        printf("Failed to fork process before pipes in RunCommandList\n");
        return 1;
    }
    if (child == 0) {
        int* pipes = CreatePipes(length);
        for (size_t i = 0; i < length; i++) {
            int new_child = fork();
            if (new_child == 0) {
                Command command = CommandList_Get(command_list, i);
                char* empty = NULL;
                Push(&command.tokens, &empty);
                int std_output = dup(1);
                if (i == 0) { 
                    if (command.input_stream != NULL) {
                        open_read_file(command.input_stream);
                    } 
                    if (command.output_stream != NULL && length == 1) {
                        open_write_file(command.output_stream, command.append);
                    } else if (length != 1) {
                        dup2(pipes[2 * i + 1], 1);
                    }
                } else if (i + 1 != length) {
                    dup2(pipes[2 * i - 2], 0);
                    dup2(pipes[2 * i + 1], 1);
                } else {
                    if (command.output_stream != NULL) {
                        open_write_file(command.output_stream, command.append);
                    } 
                    dup2(pipes[2 * i - 2], 0);
                }
                // Close pipes in every sub-child process
                ClosePipes(pipes, 2 * (length - 1));
                char* command_name = Command_GetToken(&command, 0);
                if (!(strcmp(command_name, "exit") == 0 || strcmp(command_name, "cd") == 0)) {
                    execvp(*(char**)command.tokens.storage, (char**)command.tokens.storage);
                    dup2(std_output, 1);
                    printf("Failed to execute command on %d-th iteration\n", (i + 1));
                    fflush(stdout);
                    exit(1);
                } else { 
                    exit(0);
                }
            }
        }
        // Close pipes in child process
        ClosePipes(pipes, 2 * (length - 1));
        for (size_t i = 0; i < length; i++) {
            // We have to wait exatcly "length" processes to end
            waitpid(-1, NULL, 0);
        }
        free(pipes);
        exit(0);
    } else {
        // Waiting for the child process to terminate if
        // it's not background
        if (!command_list->background) {
            waitpid(child, NULL, 0);
        }
        return 0;
    }
}

void PrintError(Command* command) {
    if (command->ERROR_MSG != NULL)
        printf("Error : %s\n", command->ERROR_MSG);
}

int* CreatePipes(size_t length) {
    int pipes_length = (length - 1) * 2;
    int* pipes = (int *) malloc(pipes_length * sizeof(int));
    for (int i = 0; i < pipes_length; i += 2) {
        if (pipe(pipes + i) == -1) {
            printf("Error occured while creating %d-th pipe\n", i >> 1);
            fflush(stdout);
        }
    }
    return pipes;
}

void ClosePipes(int* pipes, size_t pipes_length) {
    for (size_t i = 0; i < pipes_length; i++) {
        close(pipes[i]);
    }
}

bool RunInputEmbedded(struct Command* command) {
    char* empty = NULL;
    Push(&command->tokens, &empty);
    char* command_name = Command_GetToken(command, 0);
    if (strcmp(command_name, "exit") == 0) {
        if (Command_GetToken(command, 1) != NULL) {
            command->ERROR_MSG = "exit has at least 1 argument";
        } else {
            command->is_eof = true;
        }
        return true;
    }
    if (strcmp(command_name, "cd") == 0) {
        char* first_token = Command_GetToken(command, 1);
        char* second_token = Command_GetToken(command, 2);
        if (first_token == NULL || second_token != NULL) {
            command -> ERROR_MSG = "cd has less or more than one argument";
        } else if (chdir(first_token) == -1) {
            command -> ERROR_MSG = "No such file or directory";
        }
        return true;
    }
    return false;
}

void open_read_file(const char* input) {
    int rf = open(input, O_RDONLY);
    if (rf == -1) {
        printf("File {%s} can't be opened\n", input);
        fflush(stdout);
        exit(1);
    }
    if (dup2(rf, 0) == -1) {
        printf("dup2 error inside open_read_file with filename : {%s}\n", input);
        fflush(stdout);
        exit(1);
    }
}

void open_write_file(const char* output, bool append) {
    int wf = open(output, O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC), 0666);
    if (wf == -1) {
        printf("File {%s} can't be opened\n", output);
        fflush(stdout);
        exit(1);
    }
    if (dup2(wf, 1) == -1) {
        printf("dup2 error inside open_write_file with filename : {%s}\n", output);
        fflush(stdout);
        exit(1);
    }
}