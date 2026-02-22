#include "src/algo/dda.h"
#include <math.h>
#include <stdlib.h>

void DDALine(int x1, int y1, int x2, int y2, Color color) {
    int dx = x2 - x1, dy = y2 - y1;
    int steps = (abs(dx) > abs(dy)) ? abs(dx) : abs(dy);
    if (steps == 0) { DrawPixel(x1, y1, color); return; }
    float xInc = (float)dx / steps;
    float yInc = (float)dy / steps;
    float x = (float)x1, y = (float)y1;
    for (int i = 0; i <= steps; i++) {
        DrawPixel((int)roundf(x), (int)roundf(y), color);
        x += xInc; y += yInc;
    }
}

void DDA_DashedLine(int x1, int y1, int x2, int y2,
                    int dashLen, int gapLen, Color color) {
    float dx = (float)(x2-x1), dy = (float)(y2-y1);
    float length = sqrtf(dx*dx + dy*dy);
    if (length == 0) return;
    float nx = dx/length, ny = dy/length;
    float traveled = 0, fx = (float)x1, fy = (float)y1;
    int drawing = 1;
    while (traveled < length) {
        float remain = length - traveled;
        float seg = drawing ? (float)dashLen : (float)gapLen;
        if (seg > remain) seg = remain;
        float ex = fx + nx*seg, ey = fy + ny*seg;
        if (drawing)
            DDALine((int)roundf(fx),(int)roundf(fy),(int)roundf(ex),(int)roundf(ey), color);
        fx = ex; fy = ey; traveled += seg; drawing = !drawing;
    }
}

void DDA_ThickLine(int x1, int y1, int x2, int y2, int thick, Color color) {
    float dx = (float)(x2-x1), dy = (float)(y2-y1);
    float len = sqrtf(dx*dx + dy*dy);
    if (len == 0) return;
    float px = -dy/len, py = dx/len;
    int half = thick/2;
    for (int t = -half; t <= half; t++) {
        int ox = (int)roundf(px*t), oy = (int)roundf(py*t);
        DDALine(x1+ox, y1+oy, x2+ox, y2+oy, color);
    }
}

void DDA_DashDotLine(int x1, int y1, int x2, int y2, Color color) {
    float dx = (float)(x2-x1), dy = (float)(y2-y1);
    float length = sqrtf(dx*dx + dy*dy);
    if (length == 0) return;
    float nx = dx/length, ny = dy/length;
    float traveled = 0, fx = (float)x1, fy = (float)y1;
    int phase = 0;
    float phases[] = {18.0f, 5.0f, 5.0f, 5.0f};
    int   drawPh[] = {1, 0, 1, 0};
    while (traveled < length) {
        float remain = length - traveled;
        float seg = phases[phase % 4];
        if (seg > remain) seg = remain;
        float ex = fx + nx*seg, ey = fy + ny*seg;
        if (drawPh[phase % 4])
            DDALine((int)roundf(fx),(int)roundf(fy),(int)roundf(ex),(int)roundf(ey), color);
        fx = ex; fy = ey; traveled += seg; phase++;
    }
}
