#pragma once

#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>

#include <stdlib.h>

struct WordWithFlag {
    char* word;
    bool is_eof;
};

// Returns the length of the given string
u_int32_t get_length(const char* str);

// Deletes redundant spaces from the string 
void delete_spaces(char* str);

// Determines if given symbol is the end of the stream
bool end_of_stream(int c);

// Determines if given symbol is end of word
bool end_of_word(int c);

// Check is given separator (symbol) declares background;
bool background(int c);

// Checks if given symbol is the separator;
bool separator(int c);

// Checks if symbol is a redirector
bool redirection(int c);