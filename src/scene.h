#ifndef _SCENE_H
#define _SCENE_H

#include "object.h"

int scene_init();
void scene_deinit();
void scene_animate(float secs);
void scene_update();

extern object_list * objects;

#endif
