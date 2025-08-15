#ifndef STR_H
#define STR_H

typedef struct String String;

typedef struct Suffix Suffix;

String *str_init(char *c, int len);

int str_compare_from(String *s1, String *s2, int i);

int str_compare(const void *s1, const void *s2);

char str_char_at(String *s, int i);

char *str_get(String *s);

int str_len(String *s);

void str_print(const void *string);

void str_print_at(String *s, int i);

void str_print_begin_end(String *s, int begin, int end);

void str_destroy(void *string);

String *str_concat(String *s1, String *s2);

#endif