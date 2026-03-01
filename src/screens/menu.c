#include "src/screens/menu.h"
#include "src/algo/dda.h"
#include "src/algo/bresenham.h"
#include "src/algo/midcircle.h"
#include "coords.h"
#include "screen_type.h"
#include "raylib.h"

// Global variable to track clicked program
static int clickedProgram = 0;

int GetClickedProgram(void) {
    int result = clickedProgram;
    clickedProgram = 0;
    return result;
}

void DrawMenu(void) {
    G_OriginX = 0; G_OriginY = 0; G_TickStep = 1;

    ClearBackground((Color){8,8,20,255});
    DrawRectangle(0, 0, SCREEN_W, 80, (Color){20,20,50,255});
    DrawText("Aplikasi Grafika Komputer", SCREEN_W/2-210, 12, 30, WHITE);
    DrawText("Algoritma DDA, Bresenham & Midcircle  |  Raylib + C", SCREEN_W/2-220, 48, 18, LIGHTGRAY);
    DDALine(0, 80, SCREEN_W, 80, (Color){60,80,160,255});

    int cW = 450, cH = 100;
    int c1 = SCREEN_W/2 - cW - 15;
    int c2 = SCREEN_W/2 + 15;
    int r1 = 95, r2 = 205, r3 = 315;

    Vector2 mouse = GetMousePosition();
    int mousePressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    // Program 1 - DDA Kartesian
    int hover1 = CheckCollisionPointRec(mouse, (Rectangle){c1, r1, cW, cH});
    DrawRectangleRounded((Rectangle){c1,r1,cW,cH}, 0.1f, 8, hover1 ? (Color){25,55,95,255} : (Color){15,40,70,255});
    DrawRectangleRoundedLines((Rectangle){c1,r1,cW,cH}, 0.1f, 8, hover1 ? WHITE : SKYBLUE);
    DrawText("PROGRAM 1", c1+18, r1+10, 18, SKYBLUE);
    DrawText("Diagram Kartesian - DDA", c1+18, r1+32, 13, WHITE);
    DDALine(c1+18, r1+55, c1+140, r1+55, RED);
    DDALine(c1+18, r1+68, c1+140, r1+68, YELLOW);
    DDALine(c1+18, r1+81, c1+120, r1+62, SKYBLUE);
    DrawText("Garis DDA pada bidang kartesian", c1+18, r1+85, 11, LIGHTGRAY);
    DrawText("[Tekan 1]", c1+cW-90, r1+cH-20, 13, SKYBLUE);
    if (hover1 && mousePressed) clickedProgram = 1;

    // Program 2 - DDA Style
    int hover2 = CheckCollisionPointRec(mouse, (Rectangle){c2, r1, cW, cH});
    DrawRectangleRounded((Rectangle){c2,r1,cW,cH}, 0.1f, 8, hover2 ? (Color){70,30,80,255} : (Color){50,15,60,255});
    DrawRectangleRoundedLines((Rectangle){c2,r1,cW,cH}, 0.1f, 8, hover2 ? WHITE : VIOLET);
    DrawText("PROGRAM 2", c2+18, r1+10, 18, VIOLET);
    DrawText("Style Garis - DDA", c2+18, r1+32, 13, WHITE);
    DDA_DashedLine(c2+18, r1+55, c2+140, r1+55, 10, 6, ORANGE);
    DDA_ThickLine(c2+18, r1+68, c2+140, r1+68, 3, PINK);
    DDA_DashDotLine(c2+18, r1+81, c2+140, r1+81, YELLOW);
    DrawText("Normal, dash, tebal, dash-dot", c2+18, r1+85, 11, LIGHTGRAY);
    DrawText("[Tekan 2]", c2+cW-90, r1+cH-20, 13, VIOLET);
    if (hover2 && mousePressed) clickedProgram = 2;

    // Program 3 - Bresenham Kartesian
    int hover3 = CheckCollisionPointRec(mouse, (Rectangle){c1, r2, cW, cH});
    DrawRectangleRounded((Rectangle){c1,r2,cW,cH}, 0.1f, 8, hover3 ? (Color){25,65,35,255} : (Color){15,50,25,255});
    DrawRectangleRoundedLines((Rectangle){c1,r2,cW,cH}, 0.1f, 8, hover3 ? WHITE : GREEN);
    DrawText("PROGRAM 3", c1+18, r2+10, 18, GREEN);
    DrawText("Diagram Kartesian - Bresenham", c1+18, r2+32, 13, WHITE);
    BresenhamLine(c1+18, r2+58, c1+140, r2+50, (Color){255,120,80,255});
    BresenhamLine(c1+18, r2+50, c1+140, r2+58, (Color){80,220,255,255});
    BresenhamLine(c1+18, r2+70, c1+140, r2+70, (Color){180,255,100,255});
    DrawText("Garis Bresenham pada bidang kartesian", c1+18, r2+85, 11, LIGHTGRAY);
    DrawText("[Tekan 3]", c1+cW-90, r2+cH-20, 13, GREEN);
    if (hover3 && mousePressed) clickedProgram = 3;

    // Program 4 - Bresenham Style
    int hover4 = CheckCollisionPointRec(mouse, (Rectangle){c2, r2, cW, cH});
    DrawRectangleRounded((Rectangle){c2,r2,cW,cH}, 0.1f, 8, hover4 ? (Color){70,45,20,255} : (Color){50,30,10,255});
    DrawRectangleRoundedLines((Rectangle){c2,r2,cW,cH}, 0.1f, 8, hover4 ? WHITE : ORANGE);
    DrawText("PROGRAM 4", c2+18, r2+10, 18, ORANGE);
    DrawText("Style Garis - Bresenham", c2+18, r2+32, 13, WHITE);
    Bres_DashedLine(c2+18, r2+55, c2+140, r2+55, 10, 6, ORANGE);
    Bres_ThickLine(c2+18, r2+68, c2+140, r2+68, 3, RED);
    Bres_DashDotLine(c2+18, r2+81, c2+140, r2+81, YELLOW);
    DrawText("Normal, dash, tebal, dash-dot", c2+18, r2+85, 11, LIGHTGRAY);
    DrawText("[Tekan 4]", c2+cW-90, r2+cH-20, 13, ORANGE);
    if (hover4 && mousePressed) clickedProgram = 4;

    // Program 5 - Midcircle
    int hover5 = CheckCollisionPointRec(mouse, (Rectangle){c1, r3, cW, cH});
    DrawRectangleRounded((Rectangle){c1,r3,cW,cH}, 0.1f, 8, hover5 ? (Color){55,30,65,255} : (Color){40,20,50,255});
    DrawRectangleRoundedLines((Rectangle){c1,r3,cW,cH}, 0.1f, 8, hover5 ? WHITE : MAGENTA);
    DrawText("PROGRAM 5", c1+18, r3+10, 18, MAGENTA);
    DrawText("Midpoint Circle Algorithm", c1+18, r3+32, 13, WHITE);
    Midcircle(c1+80, r3+65, 22, RED);
    Midcircle(c1+130, r3+65, 18, SKYBLUE);
    Midcircle(c1+175, r3+65, 12, YELLOW);
    DrawText("Lingkaran dengan 8-way symmetry", c1+18, r3+85, 11, LIGHTGRAY);
    DrawText("[Tekan 5]", c1+cW-90, r3+cH-20, 13, MAGENTA);
    if (hover5 && mousePressed) clickedProgram = 5;

    // Program 6 - Flower of Life
    int hover6 = CheckCollisionPointRec(mouse, (Rectangle){c2, r3, cW, cH});
    DrawRectangleRounded((Rectangle){c2,r3,cW,cH}, 0.1f, 8, hover6 ? (Color){30,60,65,255} : (Color){20,45,50,255});
    DrawRectangleRoundedLines((Rectangle){c2,r3,cW,cH}, 0.1f, 8, hover6 ? WHITE : (Color){100,220,200,255});
    DrawText("PROGRAM 6", c2+18, r3+10, 18, (Color){100,220,200,255});
    DrawText("Flower of Life Pattern", c2+18, r3+32, 13, WHITE);
    // Mini Flower of Life preview
    int fcx = c2+100, fcy = r3+65, fr = 18;
    Midcircle(fcx, fcy, fr, RAYWHITE);
    Midcircle(fcx, fcy-fr, fr, RED);
    Midcircle(fcx, fcy+fr, fr, BLUE);
    Midcircle(fcx+fr, fcy, fr, GREEN);
    Midcircle(fcx-fr, fcy, fr, ORANGE);
    DrawText("Pola geometri sakral", c2+18, r3+85, 11, LIGHTGRAY);
    DrawText("[Tekan 6]", c2+cW-90, r3+cH-20, 13, (Color){100,220,200,255});
    if (hover6 && mousePressed) clickedProgram = 6;

    // About button
    int abBtnX = SCREEN_W/2 - 80, abBtnY = r3 + cH + 14;
    int abHover = CheckCollisionPointRec(mouse, (Rectangle){abBtnX, abBtnY, 160, 36});
    DrawRectangleRounded((Rectangle){abBtnX, abBtnY, 160, 36}, 0.3f, 6,
                         abHover ? (Color){60,100,180,230} : (Color){25,35,70,220});
    DrawRectangleRoundedLines((Rectangle){abBtnX, abBtnY, 160, 36}, 0.3f, 6,
                              abHover ? WHITE : (Color){80,110,200,255});
    DrawText("[ A ] About", abBtnX + 22, abBtnY + 10, 16, WHITE);

    // Footer
    DrawRectangle(0, SCREEN_H-50, SCREEN_W, 50, (Color){15,15,35,255});
    DDALine(0, SCREEN_H-50, SCREEN_W, SCREEN_H-50, (Color){60,80,160,255});
    DrawText("[1-6] Program  [A] About  |  [ESC] Keluar",
             SCREEN_W/2-180, SCREEN_H-33, 16, GRAY);
}
