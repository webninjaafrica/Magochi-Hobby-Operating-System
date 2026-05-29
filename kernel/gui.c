#include "gui.h"
#include "framebuffer.h"
#include "string.h"

void draw_rounded_box(int x, int y, int w, int h, uint8_t color) {
    for (int i = 0; i < h; i++) {
        fb_set_cursor(x, y + i);
        for (int j = 0; j < w; j++) {
            if (i == 0 || i == h-1) {
                if (j == 0 || j == w-1) fb_putchar('+', color, 0);
                else fb_putchar('-', color, 0);
            } else {
                if (j == 0 || j == w-1) fb_putchar('|', color, 0);
                else fb_putchar(' ', 15, 0);
            }
        }
    }
}

void draw_gradient(int x, int y, int w, int h, uint8_t start_color, uint8_t end_color) {
    for (int i = 0; i < h; i++) {
        fb_set_cursor(x, y + i);
        uint8_t color = start_color + (i * (end_color - start_color) / h);
        for (int j = 0; j < w; j++) {
            fb_putchar(' ', 15, color);
        }
    }
}

void window_init(Window* win, int x, int y, int w, int h, uint8_t bg, uint8_t border) {
    win->x = x;
    win->y = y;
    win->width = w;
    win->height = h;
    win->bg_color = bg;
    win->border_color = border;
    win->visible = 1;
    win->focused = 0;
}

void window_draw(Window* win, const char* title) {
    if (!win->visible) return;
    
    draw_rounded_box(win->x, win->y, win->width, win->height, win->border_color);
    
    // Fill background
    for (int i = 1; i < win->height - 1; i++) {
        fb_set_cursor(win->x + 1, win->y + i);
        for (int j = 1; j < win->width - 1; j++) {
            fb_putchar(' ', 15, win->bg_color);
        }
    }
    
    // Draw title bar
    fb_set_cursor(win->x + 2, win->y);
    fb_print(title, 14, win->border_color);
}

void button_init(Button* btn, int x, int y, int w, const char* text, uint8_t color) {
    btn->x = x;
    btn->y = y;
    btn->width = w;
    strcpy(btn->text, text);
    btn->color = color;
    btn->hovered = 0;
    btn->clicked = 0;
    btn->on_click = NULL;
}

void button_draw(Button* btn) {
    uint8_t color = btn->hovered ? btn->color + 8 : btn->color;
    
    fb_set_cursor(btn->x, btn->y);
    for (int i = 0; i < btn->width; i++) fb_putchar('-', color, 0);
    
    fb_set_cursor(btn->x, btn->y + 1);
    fb_putchar('|', color, 0);
    int text_x = (btn->width - strlen(btn->text)) / 2;
    for (int i = 0; i < text_x - 1; i++) fb_putchar(' ', 15, 0);
    fb_print(btn->text, 15, 0);
    for (int i = 0; i < btn->width - text_x - strlen(btn->text) - 1; i++) fb_putchar(' ', 15, 0);
    fb_putchar('|', color, 0);
    
    fb_set_cursor(btn->x, btn->y + 2);
    for (int i = 0; i < btn->width; i++) fb_putchar('-', color, 0);
}

void button_handle(Button* btn, int mouse_x, int mouse_y, int click) {
    int hover = (mouse_x >= btn->x && mouse_x < btn->x + btn->width &&
                 mouse_y >= btn->y && mouse_y < btn->y + 3);
    
    if (hover != btn->hovered) {
        btn->hovered = hover;
        button_draw(btn);
    }
    
    if (hover && click && btn->on_click) {
        btn->on_click();
    }
}

void textbox_init(TextBox* tb, int x, int y, int w, const char* label) {
    tb->x = x;
    tb->y = y;
    tb->width = w;
    strcpy(tb->label, label);
    tb->text[0] = '\0';
    tb->cursor_pos = 0;
    tb->focused = 0;
}

void textbox_draw(TextBox* tb) {
    fb_set_cursor(tb->x, tb->y);
    fb_print(tb->label, 2, 0);
    
    uint8_t border_color = tb->focused ? 14 : 8;
    
    fb_set_cursor(tb->x, tb->y + 1);
    fb_putchar('[', border_color, 0);
    fb_print(tb->text, 15, 0);
    int len = strlen(tb->text);
    for (int i = len; i < tb->width - 2; i++) fb_putchar(' ', 15, 0);
    fb_putchar(']', border_color, 0);
    
    if (tb->focused) {
        fb_set_cursor(tb->x + 1 + tb->cursor_pos, tb->y + 1);
    }
}

void textbox_handle(TextBox* tb, char key) {
    if (!tb->focused) return;
    
    if (key == '\b') {
        if (tb->cursor_pos > 0) {
            tb->cursor_pos--;
            tb->text[tb->cursor_pos] = '\0';
        }
    } else if (key >= ' ' && key <= '~' && tb->cursor_pos < tb->width - 3) {
        tb->text[tb->cursor_pos] = key;
        tb->cursor_pos++;
        tb->text[tb->cursor_pos] = '\0';
    }
}

void label_init(Label* lbl, int x, int y, const char* text, uint8_t color) {
    lbl->x = x;
    lbl->y = y;
    strcpy(lbl->text, text);
    lbl->color = color;
}

void label_draw(Label* lbl) {
    fb_set_cursor(lbl->x, lbl->y);
    fb_print(lbl->text, lbl->color, 0);
}
