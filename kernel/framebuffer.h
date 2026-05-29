#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "stdint.h"

void fb_init(void);
void fb_clear(void);
void fb_putchar(char c, uint8_t fg, uint8_t bg);
void fb_print(const char* str, uint8_t fg, uint8_t bg);
void fb_println(const char* str, uint8_t fg, uint8_t bg);
void fb_set_cursor(int x, int y);
void fb_get_cursor(int* x, int* y);
uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t value);

#endif
