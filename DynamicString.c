#include "DynamicString.h"

struct DynamicString CreateString(int capacity) {
    assert(capacity > 0);
    
    struct DynamicString str;
    str.capacity = capacity;
    str.string = (char*) malloc((str.capacity) * sizeof(char));
    str.size = 0;

    str.string[0] = '\0';
    return str;
}

void _ReallocString(struct DynamicString* str, u_int32_t length, bool verbose) {
    char* new_string = (char*) malloc((str -> capacity + length + 1) * sizeof(char));
    strcpy(new_string, str -> string);
    free(str -> string);
    
    str -> string = new_string;
    str -> capacity = 2 * str -> capacity;
    if (verbose == true) {
        printf("Reallocation happend, current str.capacity is ");
        printf("%d%c", str -> capacity, '\n');
    }
}

void Append(struct DynamicString* str, const char* right) {
    int length = get_length(right);

    // Bad algorithmic decision
    // Must be changed
    while (str -> size + length + 1 > str -> capacity) {
        _ReallocString(str, length, false);
    }

    char* string = str -> string;
    int i = 0;
    while (right[i] != '\0') {
        string[str -> size++] = right[i++];
    }
    string[str -> size] = '\0';
}

void AppendChar(struct DynamicString* str, char symbol) {
    if (str -> size + 2 > str -> capacity)
        _ReallocString(str, str -> capacity, false);
    char* string = str -> string;
    string[str -> size++] = symbol;
    string[str -> size] = '\0';
}

void PrintString(struct DynamicString* str) {
    assert(str->size >= 0);
    for (size_t i = 0; i < str ->size; i++) {
        if (str->string[i] == '\n') {
            printf("{\\n}");
        } else {
            printf("{%c}", (char) str->string[i]);
        }
        if (i + 1 != str->size) {
            printf(", ");
        }
    }  
}

void FreeString(struct DynamicString* str) {
    free(str -> string);
    str -> size = 0;
    str -> capacity = 1;
}

void GetInfo(struct DynamicString* str) {
    PrintString(str);
    printf("%s%d%c", "Current string size : ", str -> size, '\n');
    printf("%s%d%c", "Current string capacity : ",  str -> capacity, '\n');
}