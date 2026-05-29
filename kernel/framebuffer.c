#include "framebuffer.h"
#include "string.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

static uint16_t* vga_buffer = (uint16_t*)VGA_MEMORY;
static int cursor_x = 0;
static int cursor_y = 0;

static uint16_t make_vga_char(char c, uint8_t fg, uint8_t bg) {
    return (uint16_t)c | ((uint16_t)fg << 8) | ((uint16_t)bg << 12);
}

void outb(uint16_t port, uint8_t value) {
    __asm__ volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void fb_init(void) {
    fb_clear();
}

void fb_clear(void) {
    uint16_t blank = make_vga_char(' ', 15, 0);
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = blank;
    }
    cursor_x = 0;
    cursor_y = 0;
}

void fb_putchar(char c, uint8_t fg, uint8_t bg) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
        return;
    }
    
    if (c == '\r') {
        cursor_x = 0;
        return;
    }
    
    if (c == '\b') {
        if (cursor_x > 0) {
            cursor_x--;
            vga_buffer[cursor_y * VGA_WIDTH + cursor_x] = make_vga_char(' ', fg, bg);
        }
        return;
    }
    
    vga_buffer[cursor_y * VGA_WIDTH + cursor_x] = make_vga_char(c, fg, bg);
    cursor_x++;
    
    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }
    
    if (cursor_y >= VGA_HEIGHT) {
        for (int i = 0; i < (VGA_HEIGHT - 1) * VGA_WIDTH; i++) {
            vga_buffer[i] = vga_buffer[i + VGA_WIDTH];
        }
        for (int i = (VGA_HEIGHT - 1) * VGA_WIDTH; i < VGA_HEIGHT * VGA_WIDTH; i++) {
            vga_buffer[i] = make_vga_char(' ', 15, 0);
        }
        cursor_y = VGA_HEIGHT - 1;
    }
}

void fb_print(const char* str, uint8_t fg, uint8_t bg) {
    while (*str) {
        fb_putchar(*str++, fg, bg);
    }
}

void fb_println(const char* str, uint8_t fg, uint8_t bg) {
    fb_print(str, fg, bg);
    fb_putchar('\n', fg, bg);
}

void fb_set_cursor(int x, int y) {
    if (x >= 0 && x < VGA_WIDTH && y >= 0 && y < VGA_HEIGHT) {
        cursor_x = x;
        cursor_y = y;
    }
}

void fb_get_cursor(int* x, int* y) {
    *x = cursor_x;
    *y = cursor_y;
}
