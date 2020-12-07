#include "utils.h"

u_int32_t get_length(const char* str) {
    int i = 0;
    while (str[i] != '\0') {
        i++;
    }
    return i;
}

void delete_spaces(char* string) {
    int left = 0, right = 0;
    while (string[right] != '\0') {
        int shift = 0;
        while (string[right] != '\0' &&
               string[right] == ' ') {
            right++;
            shift++;
        }
        if (shift > 0 && left != 0 && string[right] != '\0') {
            string[left++] = ' ';
        }
        string[left] = string[right];
        left++; right++;
    }
    string[left] = '\0';
}

bool end_of_stream(int c) {
    return c == '\n' || c == EOF;
}

bool end_of_word(int c) {
    return c == ' ' || end_of_stream(c);
}

bool background(int c){
    return c == '&';
}

bool separator(int c){
    return c == '|' || c == '&' || c == '>' || c == '<';
}

bool redirection(int c) {
    return c == '<' || c == '>';
}
