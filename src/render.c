#include "render.h"
#include <zephyr/display/cfb.h>
#include <math.h>
#include <string.h>

typedef struct { float x; float y; } Point2D;
typedef struct { int16_t x; int16_t y; float radius; float z; bool visible; } RenderObject;

struct Line {
  float x;       
  int16_t y;
  int16_t vx;
  int16_t length;
};

#define MAX_RENDER_HEIGHT 240
static struct Line lines[MAX_RENDER_HEIGHT];

void render_init_lines(uint16_t width, uint16_t height) {
    uint16_t max_lines = (height > MAX_RENDER_HEIGHT) ? MAX_RENDER_HEIGHT : height;
    
    for (int i = 0; i < max_lines; i++) {
        lines[i].x = 0.0f;   
        lines[i].y = i;
        lines[i].vx = width + i;
        lines[i].length = 20;
    }
}

void render_frame_all(const struct device *display, float angle, float dt, uint16_t width, uint16_t height) {
    render_frame(display, angle);

    uint16_t max_lines = (height > MAX_RENDER_HEIGHT) ? MAX_RENDER_HEIGHT : height;
    
    for (int i = 0; i < max_lines; i++) {
        lines[i].x += lines[i].vx * dt;

        if (lines[i].x > width) {
            lines[i].x = (float)(-lines[i].length);
        }

        struct cfb_position p1 = { .x = (int)lines[i].x, .y = lines[i].y };
        struct cfb_position p2 = { .x = (int)lines[i].x + lines[i].length, .y = lines[i].y };

        cfb_draw_line(display, &p1, &p2);
    }
}

void render_frame(const struct device *display, float angle) {
  struct cfb_position pA = {
    x: 10,
    y: 10
  };

  struct cfb_position pB = {
    x: pA.x + (int16_t)lroundf(cosf(angle) * 20.0f),
    y: pA.y + (int16_t)lroundf(sinf(angle) * 20.0f)
  };

  cfb_draw_line(display, &pA, &pB);
}