#include "src/algo/bresenham.h"
#include <math.h>
#include <stdlib.h>

void BresenhamLine(int x1, int y1, int x2, int y2, Color color) {
    int dx = abs(x2-x1), dy = abs(y2-y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    while (1) {
        DrawPixel(x1, y1, color);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2*err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 <  dx) { err += dx; y1 += sy; }
    }
}

void Bres_DashedLine(int x1, int y1, int x2, int y2,
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
            BresenhamLine((int)roundf(fx),(int)roundf(fy),(int)roundf(ex),(int)roundf(ey), color);
        fx = ex; fy = ey; traveled += seg; drawing = !drawing;
    }
}

void Bres_ThickLine(int x1, int y1, int x2, int y2, int thick, Color color) {
    float dx = (float)(x2-x1), dy = (float)(y2-y1);
    float len = sqrtf(dx*dx + dy*dy);
    if (len == 0) return;
    float px = -dy/len, py = dx/len;
    int half = thick/2;
    for (int t = -half; t <= half; t++) {
        int ox = (int)roundf(px*t), oy = (int)roundf(py*t);
        BresenhamLine(x1+ox, y1+oy, x2+ox, y2+oy, color);
    }
}

void Bres_DashDotLine(int x1, int y1, int x2, int y2, Color color) {
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
            BresenhamLine((int)roundf(fx),(int)roundf(fy),(int)roundf(ex),(int)roundf(ey), color);
        fx = ex; fy = ey; traveled += seg; phase++;
    }
}
