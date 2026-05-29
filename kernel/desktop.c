#include "framebuffer.h"
#include "keyboard.h"
#include "mouse.h"
#include "string.h"

static int running = 1;

void draw_desktop(void) {
    // Draw gradient background
    for (int i = 0; i < 25; i++) {
        fb_set_cursor(0, i);
        uint8_t color = 1 + (i * 3 / 25);
        for (int j = 0; j < 80; j++) {
            fb_putchar(' ', 15, color);
        }
    }
    
    // Draw taskbar
    for (int i = 0; i < 80; i++) {
        fb_set_cursor(i, 23);
        fb_putchar(' ', 15, 7);
    }
    fb_set_cursor(2, 23);
    fb_print("Magochi v20 Desktop", 14, 7);
    
    // Draw menu box
    for (int y = 0; y < 12; y++) {
        fb_set_cursor(2, 1 + y);
        for (int x = 0; x < 16; x++) {
            if (y == 0 || y == 11) {
                if (x == 0 || x == 15) fb_putchar('+', 3, 0);
                else fb_putchar('-', 3, 0);
            } else {
                if (x == 0 || x == 15) fb_putchar('|', 3, 0);
                else fb_putchar(' ', 15, 0);
            }
        }
    }
    
    fb_set_cursor(5, 2);
    fb_print("Applications", 14, 0);
    fb_set_cursor(4, 4);
    fb_print("1. About", 2, 0);
    fb_set_cursor(4, 6);
    fb_print("2. Terminal", 2, 0);
    fb_set_cursor(4, 8);
    fb_print("3. Calculator", 2, 0);
    fb_set_cursor(4, 10);
    fb_print("4. Shutdown", 4, 0);
    
    // Draw clock
    fb_set_cursor(65, 23);
    fb_print("14:30", 14, 7);
}

void about_app(void) {
    fb_clear();
    fb_set_cursor(30, 10);
    fb_print("Magochi v20", 14, 0);
    fb_set_cursor(25, 12);
    fb_print("64-bit GUI Operating System", 2, 0);
    fb_set_cursor(28, 14);
    fb_print("Created for education", 3, 0);
    fb_set_cursor(28, 16);
    fb_print("Press any key to exit", 8, 0);
    keyboard_getchar();
    draw_desktop();
}

void terminal_app(void) {
    fb_clear();
    fb_println("Magochi v20 Terminal", 14, 0);
    fb_println("===================", 3, 0);
    fb_println("Commands: help, clear, about, exit\n", 2, 0);
    
    char input[256];
    int input_pos;
    
    while (1) {
        fb_print("$> ", 2, 0);
        input_pos = 0;
        
        while (1) {
            char c = keyboard_getchar();
            if (c == '\n') {
                fb_putchar('\n', 15, 0);
                input[input_pos] = '\0';
                break;
            } else if (c == '\b') {
                if (input_pos > 0) {
                    input_pos--;
                    fb_putchar('\b', 15, 0);
                    fb_putchar(' ', 15, 0);
                    fb_putchar('\b', 15, 0);
                }
            } else if (c >= ' ' && c <= '~' && input_pos < 255) {
                input[input_pos++] = c;
                fb_putchar(c, 15, 0);
            }
        }
        
        if (strcmp(input, "exit") == 0) {
            break;
        } else if (strcmp(input, "help") == 0) {
            fb_println("  Commands: help, clear, about, exit", 3, 0);
        } else if (strcmp(input, "clear") == 0) {
            fb_clear();
            fb_println("Magochi v20 Terminal", 14, 0);
            fb_println("===================", 3, 0);
            fb_println("Commands: help, clear, about, exit\n", 2, 0);
        } else if (strcmp(input, "about") == 0) {
            fb_println("  Magochi v20 - GUI Operating System", 14, 0);
        } else if (input[0] != '\0') {
            fb_print("  Unknown command: ", 4, 0);
            fb_println(input, 4, 0);
        }
    }
    
    draw_desktop();
}

void calculator_app(void) {
    fb_clear();
    fb_set_cursor(35, 10);
    fb_print("Calculator", 14, 0);
    fb_set_cursor(30, 12);
    fb_print("2 + 2 = 4", 2, 0);
    fb_set_cursor(28, 14);
    fb_print("Press any key to exit", 8, 0);
    keyboard_getchar();
    draw_desktop();
}

void shutdown_app(void) {
    fb_clear();
    fb_set_cursor(32, 12);
    fb_print("Shutting down...", 14, 0);
    for(volatile int i = 0; i < 1000000; i++);
    asm volatile("int $0x80");
}

void desktop_main(void) {
    draw_desktop();
    
    while (running) {
        // Update mouse position with arrow keys
        mouse_update();
        
        // Check for key input
        if (keyboard_has_input()) {
            char c = keyboard_getchar();
            
            if (c >= '1' && c <= '4') {
                switch(c) {
                    case '1':
                        about_app();
                        break;
                    case '2':
                        terminal_app();
                        break;
                    case '3':
                        calculator_app();
                        break;
                    case '4':
                        shutdown_app();
                        break;
                }
            }
        }
        
        // Draw mouse cursor
        int mx = mouse_get_x();
        int my = mouse_get_y();
        fb_set_cursor(mx, my);
        fb_putchar('\xDB', 14, 0);
        
        // Small delay
        for (volatile int i = 0; i < 5000; i++);
    }
}
