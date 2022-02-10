// Домашнее задание
//   Структуры данных
// Цель:
//   В этом ДЗ вы получите навык создания и использования структур данных.
// Задание:
//   Написать реализацию хэш-таблицы с открытой адресацией со строками в качестве ключей и целыми числами в качестве значений.
//   На основе полученной реализации написать программу, подсчитывающую частоту слов в заданном файле.
//   Сторонние библиотеки не использовать.
// Требования:
//   Создано консольное приложение, принимающее аргументом командной строки входной файл.
//   Приложение корректно обрабатывает ошибки доступа к файлу.
//   Приложение корректно подсчитывает и выводит на экран информацию о том, сколько раз в файле встречается каждое слово, которое есть в файле.
//   Код компилируется без warning'ов с ключами компилятора -Wall -Wextra -Wpedantic -std=c11.
//   Далее успешность определяется code review.
// References:
//   https://www.cyberforum.ru/c-beginners/thread2119733.html
//   https://habr.com/ru/post/509220/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h> 
#include "utils.h"
#include "strlcpy.h"

#define MAX_WORD_SIZE 255
#define HT_ISIZE  16
const char delimiters[] = " .,;:!?/";

typedef void   (tb_free)(void*,uintptr_t);
typedef int    (tb_cmp)(const void*, const void*);
typedef size_t (tb_hash)(const void*);

typedef struct node {
    struct node* next;
    void* key;
    uintptr_t val;
} slist;

typedef struct {
    tb_free* pfn_free; // функтор для освобождения памяти
    tb_cmp*  pfn_cmp;  // компаратор x == y
    tb_hash* pfn_hash; // хеш-функция
    slist**  tabs;
    size_t   cnt;
    size_t   size;
} table_t;

static int compare(const void* a, const void* b) {
    return strcmp((const char*)a, (const char*)b) == 0;
}

static size_t get_hash(const void* p) {
    const char* s = (const char*)p;
    size_t hash = 0;
    for(; *s; ++s)
        hash += hash ^ (size_t)*s;
    return hash;
}

void table_init(table_t* tb, tb_cmp* cmp, tb_hash* hash, tb_free* pfr){
    tb->pfn_cmp  = cmp;
    tb->pfn_hash = hash;
    tb->pfn_free = pfr;
    tb->tabs = NULL;
    tb->cnt  = 0;
    tb->size = 0;
}

/*static slist* slist_add(slist** ls, void* key, uintptr_t val){
    slist* p = (slist*)malloc(sizeof(slist));
    if(p != NULL){
        p->key  = key;
        p->val  = val;
        p->next = *ls;
        *ls = p;
    }
    return p;
}*/

slist *table_insert(table_t *tb, void *key, uintptr_t val) {
    size_t n, i;
    slist *p, **t;

    if (tb->tabs != NULL) {
        if ((p = slist_find(tb->tabs[(*tb->pfn_hash)(key) % tb->size], key, tb)) != NULL)
            return p;
    }

    if (tb->tabs == NULL) {
        n = HT_ISIZE;
        tb->tabs = (slist **)malloc(n * sizeof(slist *));
        if (tb->tabs == NULL)
            return NULL;

        tb->size = n;
        for (i = 0; i < tb->size; ++i)
            tb->tabs[i] = NULL;
    }
    else if ((tb->cnt >> 2) > tb->size) {
        n = tb->cnt << 1;
        t = (slist **)realloc(tb->tabs, n * sizeof(slist *));
        if (t == NULL)
            return NULL;
        tb->tabs = t;

        i = tb->size;
        tb->size = n;
        _table_rebuild(tb, i);
    }

    p = slist_add(&tb->tabs[(*tb->pfn_hash)(key) % tb->size], key, val);
    if (p != NULL)
        ++tb->cnt;
    return p;
}

/*static void slist_clear(slist* ls, table_t* tb){
    slist* t;
    while(ls != NULL){
        t  = ls;
        ls = ls->next;
        if(tb->pfn_free != NULL)
            (*tb->pfn_free)(t->key, t->val);
        free(t);
    }
}

void table_clear(table_t* tb){
    size_t i;
    if(tb->tabs != NULL){
        for(i = 0; i < tb->size; ++i)
            slist_clear(tb->tabs[i], tb);
        free(tb->tabs);
        tb->tabs = NULL;
        tb->cnt  = tb->size = 0;
    }
}*/

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "rus");
    analyzeInputArguments(argc);

    FILE* src = openAndCheckFile(argv[1], "rb");
    table_t tb;
    char word[MAX_WORD_SIZE];
    size_t cur_word_pos = 0;
    table_init(&tb, &compare, &get_hash, NULL);
    for (;;) {
        int chr = fgetc(src);
        char substr[2];
        substr[1] = 0;
        substr[0] = (unsigned char)chr;
        if (strstr(delimiters, substr) == 0 && !feof(src))
            word[cur_word_pos++] = chr;
        else {
            word[cur_word_pos] = 0;
            cur_word_pos = 0;
            slist* p = table_insert(&tb, word, 0);
            /*if(p != NULL)
                ++p->val;*/
            printf("%s\t", word);
        }
        if (feof(src)) break;
    }
    /*table_clear(&tb);*/
    fclose(src);

    return EXIT_SUCCESS;
}
