#include "mouse.h"
#include "framebuffer.h"

#define MOUSE_DATA 0x60
#define MOUSE_STATUS 0x64

static int mouse_x = 40;
static int mouse_y = 12;
static int mouse_left = 0;
static int mouse_right = 0;

void mouse_init(void) {
    // Enable mouse
    outb(0x64, 0xA8);
    // Enable interrupts
    outb(0x64, 0x20);
    uint8_t status = inb(0x60);
    status |= 0x02;
    outb(0x64, 0x60);
    outb(0x60, status);
    // Set defaults
    outb(0x64, 0xD4);
    outb(0x60, 0xF6);
    inb(0x60);
    outb(0x64, 0xD4);
    outb(0x60, 0xF4);
    inb(0x60);
}

void mouse_update(void) {
    if (inb(MOUSE_STATUS) & 1) {
        uint8_t data = inb(MOUSE_DATA);
        // Simple arrow key mouse control for QEMU without PS/2 mouse
        if (data == 0x48 && mouse_y > 0) mouse_y--;      // Up
        if (data == 0x50 && mouse_y < 24) mouse_y++;     // Down
        if (data == 0x4B && mouse_x > 0) mouse_x--;      // Left
        if (data == 0x4D && mouse_x < 79) mouse_x++;     // Right
        if (data == 0x1C) mouse_left = 1;                 // Enter as click
    }
}

int mouse_get_x(void) {
    return mouse_x;
}

int mouse_get_y(void) {
    return mouse_y;
}

int mouse_left_clicked(void) {
    int clicked = mouse_left;
    mouse_left = 0;
    return clicked;
}

int mouse_right_clicked(void) {
    int clicked = mouse_right;
    mouse_right = 0;
    return clicked;
}
