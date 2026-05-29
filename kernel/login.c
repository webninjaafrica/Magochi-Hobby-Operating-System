#include "framebuffer.h"
#include "keyboard.h"
#include "mouse.h"
#include "string.h"

static int authenticated = 0;

void draw_login_box(void) {
    int center_x = 25;
    int center_y = 8;
    
    // Draw box
    for (int y = 0; y < 10; y++) {
        fb_set_cursor(center_x, center_y + y);
        for (int x = 0; x < 30; x++) {
            if (y == 0 || y == 9) {
                if (x == 0 || x == 29) fb_putchar('+', 3, 0);
                else fb_putchar('-', 3, 0);
            } else {
                if (x == 0 || x == 29) fb_putchar('|', 3, 0);
                else if (y >= 2 && y <= 6) fb_putchar(' ', 15, 7);
                else fb_putchar(' ', 15, 0);
            }
        }
    }
    
    fb_set_cursor(center_x + 8, center_y + 1);
    fb_print("Magochi v20 Login", 14, 0);
    fb_set_cursor(center_x + 2, center_y + 3);
    fb_print("Username: ", 2, 0);
    fb_set_cursor(center_x + 2, center_y + 5);
    fb_print("Password: ", 2, 0);
}

void login_screen(void) {
    char username[32];
    char password[32];
    int username_pos = 0;
    int password_pos = 0;
    int entering_password = 0;
    int msg_display = 0;
    
    while (!authenticated) {
        fb_clear();
        draw_login_box();
        
        fb_set_cursor(33, 11);
        username[username_pos] = '\0';
        fb_print(username, 15, 7);
        
        if (entering_password) {
            fb_set_cursor(33, 13);
            for (int i = 0; i < password_pos; i++) {
                fb_putchar('*', 15, 7);
            }
            password[password_pos] = '\0';
        }
        
        if (msg_display) {
            fb_set_cursor(28, 16);
            fb_print("Invalid credentials!", 4, 0);
        }
        
        char c = keyboard_getchar();
        
        if (c == '\n') {
            if (!entering_password && username_pos > 0) {
                entering_password = 1;
                username[username_pos] = '\0';
                password_pos = 0;
                msg_display = 0;
            } else if (entering_password && password_pos > 0) {
                password[password_pos] = '\0';
                
                if ((strcmp(username, "admin") == 0 && strcmp(password, "magochi") == 0) ||
                    (strcmp(username, "guest") == 0 && strcmp(password, "guest") == 0)) {
                    authenticated = 1;
                    fb_clear();
                    fb_println("\n  Welcome to Magochi v20!", 2, 0);
                    fb_print("  Press any key to continue...", 3, 0);
                    keyboard_getchar();
                    return;
                } else {
                    msg_display = 1;
                    username_pos = 0;
                    password_pos = 0;
                    entering_password = 0;
                    memset(username, 0, 32);
                    memset(password, 0, 32);
                }
            }
        } else if (c == '\b') {
            if (!entering_password && username_pos > 0) {
                username_pos--;
                username[username_pos] = '\0';
            } else if (entering_password && password_pos > 0) {
                password_pos--;
                password[password_pos] = '\0';
            }
            msg_display = 0;
        } else if (c >= ' ' && c <= '~') {
            if (!entering_password && username_pos < 31) {
                username[username_pos++] = c;
            } else if (entering_password && password_pos < 31) {
                password[password_pos++] = c;
            }
            msg_display = 0;
        }
    }
}
