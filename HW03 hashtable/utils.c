#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

short analyzeInputArguments(int argc) {
    if ((ARGS_COUNT != argc)) {
        printf("Count words frequency in text. Usage: \n%s input_file\n", PROGRAM_NAME);
        printf("    input_file: the input file (text).\n");
        abort();
    }
    return EXIT_SUCCESS;
}

FILE* openAndCheckFile(char* file_name, char* mode) {
    FILE* file;
    if (NULL == (file = fopen(file_name, mode))) {
        printf("Cannot open file %s.\n", file_name);
        exit(ERROR_FILE_OPEN);
    }
    return file;
}
