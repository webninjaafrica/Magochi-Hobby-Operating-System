#include "keyboard.h"
#include "framebuffer.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

static const char scancode_map[] = {
    0,   0,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
    0,   0,   'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']',
    0,   0,   'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0,   '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,   '*',
    0,   ' ',  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
};

char keyboard_getchar(void) {
    while (1) {
        while (!(inb(KEYBOARD_STATUS_PORT) & 1));
        
        uint8_t scancode = inb(KEYBOARD_DATA_PORT);
        
        if (scancode & 0x80) continue;
        
        if (scancode == 0x1C) return '\n';
        if (scancode == 0x0E) return '\b';
        
        char c = scancode_map[scancode];
        if (c != 0) return c;
    }
}

int keyboard_has_input(void) {
    return inb(KEYBOARD_STATUS_PORT) & 1;
}

void keyboard_init(void) {
    while (inb(KEYBOARD_STATUS_PORT) & 1) {
        inb(KEYBOARD_DATA_PORT);
    }
}
