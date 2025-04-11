#ifndef __ACTOR_H
#define __ACTOR_H

#include <libdragon.h>
#include <stdbool.h>

struct actor_s;

typedef void (*init_func_t)(struct actor_s *);
<<<<<<< HEAD
typedef bool (*update_func_t)(struct actor_s *, joypad_buttons_t);
=======
typedef bool (*update_func_t)(struct actor_s *, struct controller_data);
>>>>>>> 24926336e643b93c6390d7ec57b62e1f5044e9cf

typedef struct actor_s {
    void *ovl_handle;
    sprite_t *sprite;
    update_func_t update;
    float x;
    float y;
    float x_scale;
    float y_scale;
    float angle;
    bool visible;
} actor_t;

typedef struct actor_class_s {
    size_t instance_size;
    init_func_t init;
    update_func_t update;
} actor_class_t;

#endif