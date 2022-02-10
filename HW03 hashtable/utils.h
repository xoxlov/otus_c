#ifndef OTUS_XOXLOV_UTILS_H
#define OTUS_XOXLOV_UTILS_H

#define ARGS_COUNT  2
#define MAX_FILE_NAME_LEN  255
#ifndef PROGRAM_NAME
  #define PROGRAM_NAME "hashtable"
#endif

enum ErrorCodes {
    SUCCESS = 0,
    ERROR_FILE_OPEN,
};

short analyzeInputArguments(int argc);
FILE* openAndCheckFile(char *file_name, char *mode);

#endif //OTUS_XOXLOV_UTILS_H
