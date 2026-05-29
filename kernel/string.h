#ifndef STRING_H
#define STRING_H

#include "stdint.h"

size_t strlen(const char* str);
char* strcpy(char* dest, const char* src);
int strcmp(const char* s1, const char* s2);
void* memset(void* ptr, int value, size_t num);

#endif
