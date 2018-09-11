#include "graphics.h"
#include "sys/stat.h"
#include <fcntl.h>
#include <sys/mman.h>


static int framebuffer;

void init_graphics() {
	framebuffer = open("/dev/fb0", O_RDWR);
	
}

void exit_graphics() {}

char getkey() {}

void sleep_ms(long ms) {}

void clear_screen(void *img) {}

void draw_pixel(void *img, int x, int y, color_t color) {}

void draw_line(void *img, int x1, int y1, int x2, int y2, color_t c) {}

void *new_offscreen_buffer() {}

void blit(void *src) {}

