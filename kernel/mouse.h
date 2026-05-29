#ifndef MOUSE_H
#define MOUSE_H

#include "stdint.h"

void mouse_init(void);
void mouse_update(void);
int mouse_get_x(void);
int mouse_get_y(void);
int mouse_left_clicked(void);
int mouse_right_clicked(void);

#endif
