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


int framebuffer;
void* fb_pointer;
int yres;
int linelength;
int totalmem;

void init_graphics() {

    //size of mmap()’ed file = (yres_virtual field of the first struct) * (line_length field of the second)

    struct fb_var_screeninfo varScreeninfo;
    struct fb_fb_fix_screeninfo fixScreeninfo;
    struct termios termios;

    framebuffer = open("/dev/fb0", O_RDWR);

    ioctl(framebuffer, FBIOGET_VSCREENINFO, &varScreeninfo);
	ioctl(framebuffer, FBIOGET_FSCREENINFO, &fixScreeninfo);

    yres = varScreeninfo.yres_virtual;
    linelength = fixScreeninfo.line_length;
    totalmem = yres * linelength;

    fb_pointer = mmap(NULL, totalmem, PROT_WRITE, MAP_SHARED, framebuffer, 0);

    ioctl(1, TCGETS, &termios);
    termios.c_lflag &= ~ECHO;
    termios.c_lflag &= ~ICANON;
    ioctl(1, TCSETS, &termios);

}

void exit_graphics() {
    struct termios termios;

    munlock(fb_pointer, totalmem);
    ioctl(1, TCGETS, &termios);
    termios.c_lflag |= ECHO;
    termios.c_lflag |= ICANON;
    ioctl(1, TCSETS, &termios);
    close(framebuffer);
}

char getkey() {
    fd_set fdSet;
    FD_ZERO(&fdSet);
    FD_SET(0, &fdSet);
    char input;
    //not setting a timeout because its unnecessary
    if(select(STDIN_FILENO + 1, &fdSet, NULL, NULL, NULL) > 0)
        read(0, &input, 1);
    return input;
}

void sleep_ms(long ms) {
    struct timespec sleep;
    sleep.tv_nsec = ms * 1000000;
    nanosleep(&sleep, NULL);
}

void clear_screen(void *img) {
    write(1, "\033[2J", 8);
}

void draw_pixel(void *img, int x, int y, color_t color) {}

void draw_line(void *img, int x1, int y1, int x2, int y2, color_t c) {}

void *new_offscreen_buffer() {}

void blit(void *src) {

}

