#include "raylib.h"
#include "screen_type.h"
#include "src/ui/back_button.h"
#include "src/screens/menu.h"
#include "src/screens/program1.h"
#include "src/screens/program2.h"
#include "src/screens/program3.h"
#include "src/screens/program4.h"
#include "src/screens/program30.h"
#include "src/screens/program31.h"
#include "src/screens/about.h"

int main(void) {
    InitWindow(SCREEN_W, SCREEN_H, "Grafika Komputer - DDA & Bresenham Line");
    SetTargetFPS(60);

    Screen current = MENU;

    int abBtnX = SCREEN_W/2 - 80, abBtnY = 268 + 155 + 14;

    while (!WindowShouldClose()) {
        if (current == MENU) {
            if (IsKeyPressed(KEY_ONE)   || IsKeyPressed(KEY_KP_1)) current = PROGRAM1;
            if (IsKeyPressed(KEY_TWO)   || IsKeyPressed(KEY_KP_2)) current = PROGRAM2;
            if (IsKeyPressed(KEY_THREE) || IsKeyPressed(KEY_KP_3)) current = PROGRAM3;
            if (IsKeyPressed(KEY_FOUR)  || IsKeyPressed(KEY_KP_4)) current = PROGRAM4;
            if (IsKeyPressed(KEY_ZERO)  || IsKeyPressed(KEY_KP_0)) current = PROGRAM30;
            if (IsKeyPressed(KEY_NINE)  || IsKeyPressed(KEY_KP_9)) current = PROGRAM31;
            if (IsKeyPressed(KEY_A)) current = ABOUT;
            Vector2 m = GetMousePosition();
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
                CheckCollisionPointRec(m, (Rectangle){abBtnX, abBtnY, 160, 36}))
                current = ABOUT;
        }
        if (current != MENU && BackButtonPressed()) current = MENU;

        BeginDrawing();
        switch (current) {
            case MENU:     DrawMenu();     break;
            case PROGRAM1: DrawProgram1(); break;
            case PROGRAM2: DrawProgram2(); break;
            case PROGRAM3: DrawProgram3(); break;
            case PROGRAM4: DrawProgram4(); break;
            case PROGRAM30: DrawProgram30(); break;
            case PROGRAM31: DrawProgram31(); break;
            case ABOUT:    DrawAbout();    break;
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
