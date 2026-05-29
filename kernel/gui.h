#ifndef GUI_H
#define GUI_H

#include "stdint.h"

typedef struct {
    int x, y;
    int width, height;
    uint8_t bg_color;
    uint8_t border_color;
    int visible;
    int focused;
} Window;

typedef struct {
    int x, y;
    int width;
    char text[64];
    uint8_t color;
    int hovered;
    int clicked;
    void (*on_click)(void);
} Button;

typedef struct {
    int x, y;
    int width;
    char label[32];
    char text[256];
    int cursor_pos;
    int focused;
} TextBox;

typedef struct {
    int x, y;
    char text[128];
    uint8_t color;
} Label;

void window_init(Window* win, int x, int y, int w, int h, uint8_t bg, uint8_t border);
void window_draw(Window* win, const char* title);
void button_init(Button* btn, int x, int y, int w, const char* text, uint8_t color);
void button_draw(Button* btn);
void button_handle(Button* btn, int mouse_x, int mouse_y, int click);
void textbox_init(TextBox* tb, int x, int y, int w, const char* label);
void textbox_draw(TextBox* tb);
void textbox_handle(TextBox* tb, char key);
void label_init(Label* lbl, int x, int y, const char* text, uint8_t color);
void label_draw(Label* lbl);
void draw_rounded_box(int x, int y, int w, int h, uint8_t color);
void draw_gradient(int x, int y, int w, int h, uint8_t start_color, uint8_t end_color);

#endif
