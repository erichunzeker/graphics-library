//graphics.h - project 1 - emh128

typedef unsigned short int color_t;

#define RGB(R, G, B) (((R & 0x1F) << 11) | ((G & 0x3F) << 5) | (B &0x1F))

void init_graphics();
void exit_graphics();
char getkey();
void sleep_ms(long ms);
void clear_screen(void *img);
void draw_pixel(void *img, int x, int y, color_t color);
void draw_line(void *img, int x1, int y1, int x2, int y2, color_t c);
void* new_offscreen_buffer();
void blit(void *src);