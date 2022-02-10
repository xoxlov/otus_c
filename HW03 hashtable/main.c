// Домашнее задание
//   Структуры данных
// Цель:
//   В этом ДЗ вы получите навык создания и использования структур данных.
// Задание:
//   Написать реализацию хеш-таблицы с открытой адресацией со строками в качестве ключей и целыми числами в качестве значений.
//   На основе полученной реализации написать программу, подсчитывающую частоту слов в заданном файле.
//   Сторонние библиотеки не использовать.
// Требования:
//   Создано консольное приложение, принимающее аргументом командной строки входной файл.
//   Приложение корректно обрабатывает ошибки доступа к файлу.
//   Приложение корректно подсчитывает и выводит на экран информацию о том, сколько раз в файле встречается каждое слово, которое есть в файле.
//   Код компилируется без warning'ов с ключами компилятора -Wall -Wextra -Wpedantic -std=c11.
//   Далее успешность определяется code review.
// References:
//   https://www.cyberforum.ru/c-beginners/thread2119733.html # Хеш-таблицы
//   https://habr.com/ru/post/509220/ # Хеш-таблицы
//   https://algorithmica.org/ru/hashing # Хеширование в строковых задачах (https://ru.algorithmica.org/cs/hashing/)
//   https://ci-plus-plus-snachala.ru/?p=89 # Бинарное дерево. Первое знакомство
//   https://ru.wikipedia.org/wiki/%D0%9E%D0%B1%D1%85%D0%BE%D0%B4_%D0%B4%D0%B5%D1%80%D0%B5%D0%B2%D0%B0 # Обход дерева
//   https://ru.wikipedia.org/wiki/MD5 # MD5
//   http://ishodniki.ru/art/artshow.php?id=233 # Как на СИ рассчитать хеш по алгоритму MD5?
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <stdint.h>
#include <ctype.h>
#include "utils.h"
#include "strlcpy.h"

// TODO: предусмотреть проверку длины слова
// TODO: вообще снять ограничения на длину слова
#define MAX_WORD_SIZE 255

// TODO: хеш-таблица? я выбрал бинарное дерево с хешами в качестве ключей, насколько это верно?
typedef struct tree {
    struct tree* left;
    struct tree* right;
    char* word;
    size_t count;
    uint32_t hash;
} TreeNode;

// TODO: заменить на MD5() из <openssl/md5.h> или на метод Горнера, в текущем виде - профанация
static uint32_t calc_str_hash(const void* str) {
    const char* ptr = (const char*)str;
    uint32_t hash = 0x1234;
    for(; *ptr; ++ptr)
        hash = (hash ^ (uint32_t)*ptr) << 4;
    return hash;
}

void add_word_in_tree(char* word, const uint32_t newhash, TreeNode**node) {
    TreeNode* tmp = *node;
    if (tmp == NULL) {
        tmp = malloc(sizeof(TreeNode));
        tmp->left = NULL;
        tmp->right = NULL;
        size_t word_len = strlen(word) + 1; // чтобы два раза не вызывать функцию strlen
        tmp->word = malloc(word_len);
        strlcpy(tmp->word, word, word_len);
        tmp->count = 1;
        tmp->hash = calc_str_hash(word);
        *node = tmp;
    } else {
        if (tmp->hash == newhash)
            tmp->count++;
        else if (tmp->hash > newhash)
            add_word_in_tree(word, newhash, &(tmp->left));
        else
            add_word_in_tree(word, newhash, &(tmp->right));
    }
}

void print_words_count(TreeNode* node) {
    if (node == NULL)
        return;
    print_words_count(node->left);
    printf("%s: %lld\n", (char*)node->word, node->count);
    print_words_count(node->right);
}

void clear_tree(TreeNode** node) {
    TreeNode* tmp = *node;
    if (tmp == NULL) {
        return;
    }
    clear_tree(&(tmp->left));
    tmp->left = NULL; // TODO: поисследовать, надо ли делать это присвоение?
    clear_tree(&(tmp->right));
    tmp->right = NULL; // TODO: поисследовать, надо ли делать это присвоение?
    free(tmp->word);
    free(tmp);
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "rus");
    analyzeInputArguments(argc);

    FILE* src = openAndCheckFile(argv[1], "rb");
    TreeNode* dict = NULL;
    char word[MAX_WORD_SIZE];
    size_t cur_word_pos = 0;
    for (;;) {
        int chr = fgetc(src);
        // проверить текущий символ, учесть апострофы и дефисы внутри слов
        if ((isalnum(chr) != 0 || (chr == '\'') || (chr == '-')) && !feof(src))
            word[cur_word_pos++] = chr;
        else {
            word[cur_word_pos] = 0;
            cur_word_pos = 0;
            if (word[0] != 0) // вместо проверки длины слова - проверить первый байт
                add_word_in_tree(word, calc_str_hash(word), &dict);
        }
        if (feof(src)) break;
    }
    // по заданию вывод не обязан выполняться в файл, значит вывести на консоль
    print_words_count(dict);
    clear_tree(&dict);
    fclose(src);

    return EXIT_SUCCESS;
}
