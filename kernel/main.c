#include "framebuffer.h"
#include "keyboard.h"
#include "mouse.h"
#include "login.h"
#include "desktop.h"

void kernel_main(void) {
    fb_init();
    keyboard_init();
    mouse_init();
    
    fb_clear();
    fb_set_cursor(30, 10);
    fb_print("Magochi v20 GUI", 14, 0);
    fb_set_cursor(28, 12);
    fb_print("Loading kernel...", 2, 0);
    
    for (volatile int i = 0; i < 500000; i++);
    
    login_screen();
    desktop_main();
}
