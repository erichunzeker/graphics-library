// Created by Eric Hunzeker on 9/13/18.

#include "graphics.h"

int snakex = 1;
int snakey= 1;


int main(int argc, char **argv)
{
    init_graphics();

    void *buf = new_offscreen_buffer();

    char key;

    int right = 1;
    int left = 0;
    int down = 0;
    int up = 0;

    int oldsnakex = -1;
    int oldsnakey = -1;

    do {
        if(oldsnakex != -1)
            draw_pixel(buf, oldsnakex, oldsnakey, RGB(0, 0, 0));
        draw_pixel(buf, snakex, snakey, RGB(31, 0, 0));
        blit(buf);

        oldsnakex = snakex;
        oldsnakey = snakey;


        if(right == 1 && snakex == 639) {
            snakex = 0;
        } else if(left == 1 && snakex == 0) {
            snakex = 639;
        } else if(up == 1 && snakey == 0) {
            snakey = 479;
        }else if(down == 1 && snakey == 479) {
            snakey = 0;
        }

        if(right == 1 && snakex < 640)
            snakex = snakex + 1;
        else if(left == 1 && snakex > 0)
            snakex = snakex - 1;
        else if(up == 1 && snakey > 0)
            snakey = snakey - 1;
        else if(down == 1 && snakey < 480)
            snakey = snakey + 1;

        key = getkey();

        if(key == 27){
            getkey();
            key = getkey();
        }

        if (key == 'q')
            break;
        else if(key == 'A') {
            right = 0;
            left = 0;
            down = 0;
            up = 1;
        } else if(key == 'D') {
            right = 0;
            left = 1;
            down = 0;
            up = 0;
        } else if(key == 'B') {
            right = 0;
            left = 0;
            down = 1;
            up = 0;
        } else if(key == 'C') {
            right = 1;
            left = 0;
            down = 0;
            up = 0;
        }


        sleep_ms(2);
    }
    while (1);
    clear_screen(buf);
    blit(buf);
    exit_graphics();
    return 0;

}