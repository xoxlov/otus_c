// Домашнее задание
//   Статические структуры данных
// Цель:
//   В процессе выполнения ДЗ вы получите навык работы с различными текстовыми кодировками.
// Задание:
//   Написать конвертор из заданной кодировки (cp-1251, koi8, iso-8859-5) в utf-8.
//   Сторонние библиотеки не использовать.
// Требования:
//   Создано консольное приложение, принимающее аргументами командной строки входной файл, заданную кодировку и выходной файл.
//   Конвертация из каждой из трёх указанных кодировок корректно отрабатывает (файлы для проверки прилагаются).
//   Приложение корректно обрабатывает ошибки доступа к файлам.
//   Код компилируется без warning'ов с ключами компилятора -Wall -Wextra -Wpedantic -std=c11.
//   Далее успешность определяется code review.
// ---------------------------------------------------------------------------------------------------------------------
// Есть только одна придирка к стилю и одна проблема в коде:
// 1. https://github.com/xoxlov/otus_c/blob/84401269222244543b25fa5edfb5ba26e375554b/HW02%20encoding/main.c#L31 -
//    довольно замысловатая проверка, можно было ругнуться справочным текстом сразу, если ARGS_COUNT == argc.
// 2. https://github.com/xoxlov/otus_c/blob/84401269222244543b25fa5edfb5ba26e375554b/HW02%20encoding/main.c#L59-L60 -
//    тут опять-таки проблема с необязательностью _s расширений, и тоже не хватает проверки на успешность открытия файла.
// ---------------------------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "encode.h"
#include "strlcpy.h"

#define ARGS_COUNT  4
#ifndef PROGRAM_NAME
  #define PROGRAM_NAME "decode"
#endif

#define MAX_FILE_NAME_LEN  255
#define ERROR_FILE_OPEN    2

short analyzeInputArguments(int argc, char* argv[]) {
    size_t encodings_len = getEncodingsCount();
    bool isArgsCountCorrect = (ARGS_COUNT == argc);
    bool isEncodingNameWrong = true;
    if (isArgsCountCorrect)
        for (size_t i = 0; i < encodings_len; ++i)
            if (false == (isEncodingNameWrong = strncmp(argv[2], encodings[i], strlen(encodings[i]))))
                break;
    if (isEncodingNameWrong) { // includes isArgsCountCorrect == false
        printf("Converting text encodings to UTF-8. Usage: \n%s infile codepage outfile\n", PROGRAM_NAME);
        printf("    infile: the input file,\n");
        printf("    codepage: the encoding name [");
        for (size_t i = 0; i < encodings_len - 1 ; ++i)
            printf("%s|", encodings[i]);
        printf("%s", encodings[encodings_len - 1]);
        printf("],\n    outfile: the output file.\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int main(int argc, char* argv[]) {
    if (EXIT_FAILURE == analyzeInputArguments(argc, argv))
        exit(EXIT_FAILURE);

    char src_file[MAX_FILE_NAME_LEN + 1];
    char dst_file[MAX_FILE_NAME_LEN + 1];
    FILE* src;
    FILE* dst;

    int ret_code = EXIT_SUCCESS;
    strlcpy(src_file, argv[1], MAX_FILE_NAME_LEN);
    strlcpy(dst_file, argv[3], MAX_FILE_NAME_LEN);
    if (NULL == (src = fopen(src_file, "rb"))) {
        printf("Cannot open file %s.\n", src_file);
        exit(ERROR_FILE_OPEN);
    }
    if (NULL == (dst = fopen(dst_file, "wb"))) {
        printf("Cannot open file %s.\n", dst_file);
        exit(ERROR_FILE_OPEN);
    }
    int16_t* table = recognizeEncodingTable(argv[2]);

    for (;;) {
        int16_t chr = fgetc(src);
        if (feof(src))
            break;
        if (chr < ASCII_CONSTANT) {
            fputc((unsigned char) chr, dst);
            continue;
        }
        int16_t utf_chr = table[chr - 0x80];
        if (utf_chr < 0x800) {
            fputc(((utf_chr >> 6) & 0x1F) | 0xC0, dst);
            fputc((utf_chr & 0x3F) | 0x80, dst);
        } else {
            fputc(((utf_chr >> 12) & 0x0F) | 0xE0, dst);
            fputc(((utf_chr >> 6) & 0x3F) | 0x80, dst);
            fputc((utf_chr & 0x3F) | 0x80, dst);
        }
    }
    fclose(src);
    fclose(dst);

    return ret_code;
}
