#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <stdio.h>

#include "Command.h"
#include "Parse.h"
#include "CommandPipe.h"
#include "Runner.h"

void print_invitation(char* directory, const char* username) {
    getcwd(directory, BUFFER_SIZE);
    printf("%s%s$ ", username, directory);
}

int main() {
    // Is there a proper way to get a username?
    char* username  = "eduard@ml-dl-fanboy:~";
    char directory[BUFFER_SIZE];
    const bool verbose = false;
    bool is_eof = false;
    bool exit_called = false;
    do {
        print_invitation(&directory, username);
        CommandPipe command_pipe = Pipe_Create();
        is_eof |= Parse(&command_pipe);
        if (verbose) {
            Pipe_Print(&command_pipe);    
        }
        // sleep(1);
        exit_called |= Run(&command_pipe);
        Pipe_Free(&command_pipe);
        while (waitpid(0, NULL, WNOHANG) > 0) {}
    } while (!is_eof && !exit_called);
    return 0;
}