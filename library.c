#include "graphics.h"
#include "sys/stat.h"
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#include <time.h>
#include <linux/fb.h>
#include <termios.h>
#include <stdlib.h>

int framebuffer;
void* fb_pointer;
int yres;
int linelength;
int memsize;

void init_graphics() {
    //size of mmap()’ed file = (yres_virtual field of the first struct) * (line_length field of the second)
    struct fb_var_screeninfo varScreeninfo;
    struct fb_fix_screeninfo fixScreeninfo;
    struct termios termios;

    framebuffer = open("/dev/fb0", O_RDWR);

    ioctl(framebuffer, FBIOGET_VSCREENINFO, &varScreeninfo);
	ioctl(framebuffer, FBIOGET_FSCREENINFO, &fixScreeninfo);

    yres = varScreeninfo.yres_virtual;
    linelength = fixScreeninfo.line_length / 2;
    memsize = yres * linelength * 2;

    fb_pointer = mmap(NULL, memsize, PROT_READ | PROT_WRITE, MAP_SHARED, framebuffer, 0);

    ioctl(framebuffer, TCGETS, &termios);
    termios.c_lflag &= ~ECHO;
    termios.c_lflag &= ~(ICANON);
    ioctl(framebuffer, TCSETS, &termios);

}

void exit_graphics() {
    struct termios t;
    clear_screen(fb_pointer);
    munmap(fb_pointer, memsize);
    close(framebuffer);

    ioctl(framebuffer, TCGETS, &t);
    t.c_lflag |= ECHO;
    t.c_lflag |= ICANON;
    ioctl(framebuffer, TCSETS, &t);
}

char getkey() {
    fd_set rfds;
    struct timeval tv;
    int retval;

    FD_ZERO(&rfds);
    FD_SET(0, &rfds);
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    char input;
    retval = select(1, &rfds, NULL, NULL, &tv);

    if(retval)
        read(0, &input, sizeof(input));
    return input;
}

void sleep_ms(long ms) {
    struct timespec sleep;
    sleep.tv_nsec = ms * 1000000;
    nanosleep(&sleep, NULL);
}

void clear_screen(void *img) {
    size_t i;

    for (i = 0; i < memsize; i++)
        *((char*)(img) + i) = RGB(0, 0, 0);
}

void draw_pixel(void *img, int x, int y, color_t color) {
    *((color_t*)(img) + (y * linelength + x)) = color;
}

void draw_line(void *img, int x1, int y1, int x2, int y2, color_t c) {
    //https://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C

    int dx = abs(x2-x1), sx = x1<x2 ? 1 : -1;
    int dy = abs(y2-y1), sy = y1<y2 ? 1 : -1;
    int err = (dx>dy ? dx : -dy)/2, e2;

    for(;;){
        draw_pixel(img, x1,y1, c);
        if (x1==x2 && y1==y2)
            break;
        e2 = err;
        if (e2 >-dx) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dy) {
            err += dx;
            y1 += sy;
        }
    }
}

void* new_offscreen_buffer() {
    void* buff = mmap(NULL, memsize, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    return buff;
}

void blit(void *src) {
    //copy every byte from *src buffer to framebuffer
    int i;
    for (i = 0; i < memsize; i++) {
        *(i + (char *) (fb_pointer)) = *(i + (char *) (src)); //get single byte in form of char
    }
}