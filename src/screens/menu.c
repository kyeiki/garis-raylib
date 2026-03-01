#include "src/screens/menu.h"
#include "src/algo/dda.h"
#include "src/algo/bresenham.h"
#include "src/algo/midcircle.h"
#include "coords.h"
#include "screen_type.h"
#include "raylib.h"

void DrawMenu(void) {
    G_OriginX = 0; G_OriginY = 0; G_TickStep = 1;

    ClearBackground((Color){8,8,20,255});
    DrawRectangle(0, 0, SCREEN_W, 80, (Color){20,20,50,255});
    DrawText("Aplikasi Grafika Komputer", SCREEN_W/2-210, 12, 30, WHITE);
    DrawText("Algoritma DDA, Bresenham & Midcircle  |  Raylib + C", SCREEN_W/2-220, 48, 18, LIGHTGRAY);
    DDALine(0, 80, SCREEN_W, 80, (Color){60,80,160,255});

    int cW = 450, cH = 120;
    int c1 = SCREEN_W/2 - cW - 15;
    int c2 = SCREEN_W/2 + 15;
    int r1 = 95, r2 = 225, r3 = 355;

    // Program 1 - DDA Kartesian
    DrawRectangleRounded((Rectangle){c1,r1,cW,cH}, 0.1f, 8, (Color){15,40,70,255});
    DrawRectangleRoundedLines((Rectangle){c1,r1,cW,cH}, 0.1f, 8, SKYBLUE);
    DrawText("PROGRAM 1", c1+18, r1+12, 20, SKYBLUE);
    DrawText("Diagram Kartesian - DDA", c1+18, r1+36, 14, WHITE);
    DDALine(c1+18, r1+60, c1+180, r1+60, RED);
    DDALine(c1+18, r1+74, c1+180, r1+74, YELLOW);
    DDALine(c1+18, r1+88, c1+160, r1+68, SKYBLUE);
    DrawText("Garis DDA pada bidang kartesian", c1+18, r1+95, 12, LIGHTGRAY);
    DrawText("[Tekan 1]", c1+cW-100, r1+cH-24, 14, SKYBLUE);

    // Program 2 - DDA Style
    DrawRectangleRounded((Rectangle){c2,r1,cW,cH}, 0.1f, 8, (Color){50,15,60,255});
    DrawRectangleRoundedLines((Rectangle){c2,r1,cW,cH}, 0.1f, 8, VIOLET);
    DrawText("PROGRAM 2", c2+18, r1+12, 20, VIOLET);
    DrawText("Style Garis - DDA", c2+18, r1+36, 14, WHITE);
    DDA_DashedLine(c2+18, r1+60, c2+180, r1+60, 12, 8, ORANGE);
    DDA_ThickLine(c2+18, r1+76, c2+180, r1+76, 4, PINK);
    DDA_DashDotLine(c2+18, r1+90, c2+180, r1+90, YELLOW);
    DrawText("Normal, dash, tebal, dash-dot", c2+18, r1+95, 12, LIGHTGRAY);
    DrawText("[Tekan 2]", c2+cW-100, r1+cH-24, 14, VIOLET);

    // Program 3 - Bresenham Kartesian
    DrawRectangleRounded((Rectangle){c1,r2,cW,cH}, 0.1f, 8, (Color){15,50,25,255});
    DrawRectangleRoundedLines((Rectangle){c1,r2,cW,cH}, 0.1f, 8, GREEN);
    DrawText("PROGRAM 3", c1+18, r2+12, 20, GREEN);
    DrawText("Diagram Kartesian - Bresenham", c1+18, r2+36, 14, WHITE);
    BresenhamLine(c1+18, r2+65, c1+180, r2+55, (Color){255,120,80,255});
    BresenhamLine(c1+18, r2+55, c1+180, r2+65, (Color){80,220,255,255});
    BresenhamLine(c1+18, r2+80, c1+180, r2+80, (Color){180,255,100,255});
    DrawText("Garis Bresenham pada bidang kartesian", c1+18, r2+95, 12, LIGHTGRAY);
    DrawText("[Tekan 3]", c1+cW-100, r2+cH-24, 14, GREEN);

    // Program 4 - Bresenham Style
    DrawRectangleRounded((Rectangle){c2,r2,cW,cH}, 0.1f, 8, (Color){50,30,10,255});
    DrawRectangleRoundedLines((Rectangle){c2,r2,cW,cH}, 0.1f, 8, ORANGE);
    DrawText("PROGRAM 4", c2+18, r2+12, 20, ORANGE);
    DrawText("Style Garis - Bresenham", c2+18, r2+36, 14, WHITE);
    Bres_DashedLine(c2+18, r2+60, c2+180, r2+60, 12, 8, ORANGE);
    Bres_ThickLine(c2+18, r2+76, c2+180, r2+76, 4, RED);
    Bres_DashDotLine(c2+18, r2+90, c2+180, r2+90, YELLOW);
    DrawText("Normal, dash, tebal, dash-dot", c2+18, r2+95, 12, LIGHTGRAY);
    DrawText("[Tekan 4]", c2+cW-100, r2+cH-24, 14, ORANGE);

    // Program 5 - Midcircle (centered)
    int c5 = SCREEN_W/2 - cW/2;
    DrawRectangleRounded((Rectangle){c5,r3,cW,cH}, 0.1f, 8, (Color){40,20,50,255});
    DrawRectangleRoundedLines((Rectangle){c5,r3,cW,cH}, 0.1f, 8, MAGENTA);
    DrawText("PROGRAM 5", c5+18, r3+12, 20, MAGENTA);
    DrawText("Midpoint Circle Algorithm", c5+18, r3+36, 14, WHITE);
    Midcircle(c5+100, r3+70, 25, RED);
    Midcircle(c5+160, r3+70, 20, SKYBLUE);
    Midcircle(c5+210, r3+70, 15, YELLOW);
    DrawText("Lingkaran dengan 8-way symmetry (integer only)", c5+18, r3+95, 12, LIGHTGRAY);
    DrawText("[Tekan 5]", c5+cW-100, r3+cH-24, 14, MAGENTA);

    // About button
    int abBtnX = SCREEN_W/2 - 80, abBtnY = r3 + cH + 14;
    Vector2 mouse = GetMousePosition();
    int abHover = CheckCollisionPointRec(mouse, (Rectangle){abBtnX, abBtnY, 160, 36});
    DrawRectangleRounded((Rectangle){abBtnX, abBtnY, 160, 36}, 0.3f, 6,
                         abHover ? (Color){60,100,180,230} : (Color){25,35,70,220});
    DrawRectangleRoundedLines((Rectangle){abBtnX, abBtnY, 160, 36}, 0.3f, 6,
                              abHover ? WHITE : (Color){80,110,200,255});
    DrawText("[ A ] About", abBtnX + 22, abBtnY + 10, 16, WHITE);

    // Footer
    DrawRectangle(0, SCREEN_H-50, SCREEN_W, 50, (Color){15,15,35,255});
    DDALine(0, SCREEN_H-50, SCREEN_W, SCREEN_H-50, (Color){60,80,160,255});
    DrawText("[1-5] Program  [A] About  |  [ESC] Keluar",
             SCREEN_W/2-180, SCREEN_H-33, 16, GRAY);
}
