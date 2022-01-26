// Домашнее задание "Типы данных"
// Цель:
//   В процессе выполнения ДЗ получить навык работы с файлами со специализированной структурой.
// Задание:
//   Написать программу, определяющую, является ли заданный файл т.н. Rarjpeg-ом (изображением, в конец которого дописан
//   архив), и выводящую список файлов в архиве, если заданный файл таковым является. Для простоты подразумевается
//   формат архива zip, а не rar. Сторонние библиотеки не использовать.
// Требования:
//   Создано консольное приложение, принимающее аргументом командной строки входной файл.
//   Приложение корректно определяет файл, не содержащий внутри архив (файл для проверки прилагается).
//   Приложение корректно определяет файл, содержащий внутри архив, и выводит список файлов внутри архива (файл для
//   проверки прилагается).
//   Код компилируется без warning'ов с ключами компилятора -Wall -Wextra -Wpedantic -std=c11.
//   Далее успешность определяется code review.
// Критерии оценки:
//   Студент коммитит все необходимое в свой github/gitlab репозиторий.
//   Далее необходимо зайти в ЛК, найти занятие, ДЗ по которому выполнялось, нажать "Отправить ДЗ" и отправить ссылку.
//   После этого ревью и общение на тему ДЗ будет происходить в рамках этого чата.
// References:
//   https://ru.wikipedia.org/wiki/Список_сигнатур_файлов
//   https://habr.com/ru/company/infowatch/blog/337084/
//   https://users.cs.jmu.edu/buchhofp/forensics/formats/pkzip.html
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PROGRAM_NAME "step001"
#define MAX_FILE_NAME_LEN   255
#define SIGNATURE_SIZE  18

enum ErrorCodes {
    SIGNATURE_VALID = 0,
    ERROR_ARGUMENTS,
    ERROR_UNRECOGNIZED,
    ERROR_END_OF_FILE,
    ERROR_NOT_VALID,
    ARC_IS_EMPTY,
};

unsigned char JPEG_start[] = {0xFF, 0xD8};
unsigned char JPEG_end[] = {0xFF, 0xD9};
unsigned char ZIP_start[] = {0x50, 0x4B};
unsigned char ZIP_regular[] = {0x03, 0x04}; // regular archive signature
unsigned char ZIP_empty[] = {0x05, 0x06}; // empty archive signature
unsigned char ZIP_central[] = {0x50, 0x4b, 0x01, 0x02}; // archive central directory signature
unsigned char FILENAME_len[] = {0x00, 0x00};

short analyzeInputArguments(int argc) {
    if (2 != argc) {
        printf("Usage: %s infile\n    infile: the input file\n", PROGRAM_NAME);
        return ERROR_ARGUMENTS;
    }
    return EXIT_SUCCESS;
}

short compareSignature(unsigned char *src1, unsigned char *src2, size_t size) {
    short ret_code = 1;
    for (size_t i = 0; i < size; ++i)
        ret_code *= (short) (src1[i] == src2[i]);
    return ret_code ? SIGNATURE_VALID : ERROR_NOT_VALID;
}

short readSignature(FILE *input, unsigned char *signature, size_t size) {
    short ret_code = EXIT_SUCCESS;
    int chr;
    for (size_t i = 0; i < size && ERROR_END_OF_FILE != ret_code; ++i) {
        chr = fgetc(input);
        if (EOF == chr)
            ret_code = ERROR_END_OF_FILE;
        signature[i] = (unsigned char) chr;
    }
    return ret_code;
}

short findSignature(FILE *input, unsigned char *signature, unsigned char* sign2find, size_t size) {
    short ret_code = ERROR_NOT_VALID;
    int chr;
    do {
        for (size_t i = 0; i < size - 1; ++i) // shift signature bytes to the left
            signature[i] = signature[i+1];
        chr = fgetc(input); // need to use 'more than char' to handle EOF
        signature[size - 1] = (unsigned char) chr;
        if (EOF == chr)
            ret_code = ERROR_END_OF_FILE;
        else if (compareSignature(signature, sign2find, size) == EXIT_SUCCESS)
            ret_code = EXIT_SUCCESS;
    } while (ERROR_NOT_VALID == ret_code);
    return ret_code;
}

short readSkipBytes(FILE* input, size_t size) {
    short ret_code = EXIT_SUCCESS;
    int chr;
    size_t count = 0;
    do {
        chr = fgetc(input);
        if (EOF == chr)
            ret_code = ERROR_END_OF_FILE;
        ++count;
    } while (EXIT_SUCCESS == ret_code && count < size);
    return ret_code;
}

short listFilesFromCentralDirectory(FILE* input, unsigned char* signature) {
    do {
        if (ERROR_END_OF_FILE == findSignature(input, signature, ZIP_central, sizeof ZIP_central))
            break;
        readSkipBytes(input, 24);
        if (ERROR_END_OF_FILE == readSignature(input, signature, sizeof FILENAME_len))
            break;
        readSkipBytes(input, 16);
        int filename_len = signature[0];
        if (ERROR_END_OF_FILE == readSignature(input, signature, filename_len))
            break;
        signature[filename_len] = 0; // set end of z-string
        printf("%s\n", signature);
    } while (!feof(input));
    return EXIT_SUCCESS;
}

void printErrorMessage(short ret_code, char* file_name) {
    switch (ret_code) {
        case ERROR_END_OF_FILE:
        case ERROR_NOT_VALID:
        case ERROR_UNRECOGNIZED:
            printf("Input file '%s' is not RarJpeg\n", file_name);
            break;
        case ARC_IS_EMPTY:
            printf("Input file '%s' doesn't contain any files\n", file_name);
            break;
        case EXIT_SUCCESS:
        default: // defensive code
            break;
    }
}

int main(int argc, char* argv[]) {
    if (ERROR_ARGUMENTS == analyzeInputArguments(argc))
        exit(ERROR_ARGUMENTS);

    size_t ret_code = EXIT_SUCCESS;
    char input_file[MAX_FILE_NAME_LEN + 1];
    FILE* input;

    strcpy_s(input_file, MAX_FILE_NAME_LEN, argv[1]); // remember input file name
    fopen_s(&input, input_file, "rb");
    unsigned char signature[SIGNATURE_SIZE];
    do {
        // read and check JPEG signature
        if (((ret_code = readSignature(input, signature, sizeof JPEG_start)) == ERROR_END_OF_FILE) ||
            ((ret_code = compareSignature(signature, JPEG_start, sizeof JPEG_start)) == ERROR_NOT_VALID) ||
            ((ret_code = findSignature(input, signature, JPEG_end, sizeof JPEG_end)) == ERROR_END_OF_FILE))
            break;
        // read and check ZIP Signature
        if (((ret_code = readSignature(input, signature, sizeof ZIP_start)) == ERROR_END_OF_FILE) ||
            ((ret_code = compareSignature(signature, ZIP_start, sizeof ZIP_start)) == ERROR_NOT_VALID) ||
            ((ret_code = readSignature(input, signature, sizeof ZIP_regular)) == ERROR_END_OF_FILE))
            break;
        if ((SIGNATURE_VALID == compareSignature(signature, ZIP_empty, sizeof ZIP_empty))) {
            ret_code = ARC_IS_EMPTY;
            break;
        }
        if (SIGNATURE_VALID == compareSignature(signature, ZIP_regular, sizeof ZIP_regular)) {
            ret_code = listFilesFromCentralDirectory(input, signature);
            break;
        }
        ret_code = ERROR_UNRECOGNIZED;
    } while (0);
    fclose(input);
    printErrorMessage(ret_code, input_file);
    return ret_code;
}
