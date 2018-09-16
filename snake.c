// Created by Eric Hunzeker on 9/13/18.

#include "graphics.h"

int direction = 0;
int curr_x = 0;
int curr_y = 0;
int snakex = 1;
int snakey= 1;

void turn_left(int degrees)
{
    direction = (direction + degrees + 360) % 360;
}

void go_forward(void *img, int distance)
{
    int new_x = curr_x;
    int new_y = curr_y;

    if (direction == 0)
        new_x += distance;
    else if (direction == 90)
        new_y += distance;
    else if (direction == 180)
        new_x -= distance;
    else if (direction == 270)
        new_y -= distance;

    draw_line(img, curr_x, curr_y, new_x, new_y, RGB(31, 0, 0));
    curr_x = new_x;
    curr_y = new_y;
}

void hilbert_recurse(void *img, int n, int parity, int dist)
{
    if (n == 0)
        return;

    turn_left(parity * 90);

    hilbert_recurse(img, n - 1, -parity, dist);
    go_forward(img, dist);
    turn_left(-parity * 90);

    hilbert_recurse(img, n - 1, +parity, dist);
    go_forward(img, dist);

    hilbert_recurse(img, n - 1, +parity, dist);
    turn_left(-parity * 90);
    go_forward(img, dist);

    hilbert_recurse(img, n - 1, -parity, dist);
    turn_left(parity * 90);
}

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
            right = 0;
            left = 1;
            down = 0;
            up = 0;
        } else if(left == 1 && snakex == 0) {
            right = 1;
            left = 0;
            down = 0;
            up = 0;
        } else if(up == 1 && snakey == 0) {
            right = 0;
            left = 0;
            down = 1;
            up = 0;
        }else if(down == 1 && snakey == 479) {
            right = 0;
            left = 0;
            down = 0;
            up = 1;
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

        if (key == 'q')
            break;
        else if(key == 'w') {
            right = 0;
            left = 0;
            down = 0;
            up = 1;
        } else if(key == 'a') {
            right = 0;
            left = 1;
            down = 0;
            up = 0;
        } else if(key == 's') {
            right = 0;
            left = 0;
            down = 1;
            up = 0;
        } else if(key == 'd') {
            right = 1;
            left = 0;
            down = 0;
            up = 0;
        }
        sleep_ms(2);
    }
    while (1);

    exit_graphics();
    return 0;

}