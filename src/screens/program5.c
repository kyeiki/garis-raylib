#include "src/screens/program5.h"
#include "src/algo/midcircle.h"
#include "src/algo/bresenham.h"
#include "src/ui/back_button.h"
#include "src/ui/cartesian.h"
#include "src/ui/primitives.h"
#include "coords.h"
#include "screen_type.h"
#include "raylib.h"

void DrawProgram5(void) {
    G_OriginX  = SCREEN_W / 2;
    G_OriginY  = SCREEN_H / 2 + 30;
    G_TickStep = 38;

    ClearBackground((Color){12,12,28,255});
    
    // Gambar grid kartesian menggunakan algoritma Bresenham yang sudah ada
    DrawCartesianGrid(BresenhamLine);

    // Definisi lingkaran yang akan digambar (koordinat Kartesian untuk pusat)
    struct { int cx, cy, r; Color c; const char *lbl; } circles[] = {
        { 0,  0, 5, RED,      "Lingkaran 1: pusat(0,0), r=5"},
        {-4,  3, 3, SKYBLUE,  "Lingkaran 2: pusat(-4,3), r=3"},
        { 5, -2, 4, YELLOW,   "Lingkaran 3: pusat(5,-2), r=4"},
        { 0,  5, 2, MAGENTA,  "Lingkaran 4: pusat(0,5), r=2"},
    };
    
    // Gambar setiap lingkaran
    for (int i = 0; i < 4; i++) {
        int sx = CS_X(circles[i].cx);  // Konversi x pusat ke layar
        int sy = CS_Y(circles[i].cy);  // Konversi y pusat ke layar
        int sr = circles[i].r * G_TickStep;  // Radius dalam pixel
        
        Midcircle(sx, sy, sr, circles[i].c);
        
        // Gambar titik pusat
        DrawDot(sx, sy, 3, circles[i].c);
    }

    // Header
    DrawRectangle(0, 0, SCREEN_W, 56, (Color){0,0,0,210});
    DrawBackButton();
    DrawText("PROGRAM 5 - Midpoint Circle Algorithm", 135, 10, 18, WHITE);
    DrawText("d=3-2r  if(d<0):d=d+4x+6  else:d=d+4(x-y)+10,y--  |  8-way symmetry", 135, 33, 12, LIGHTGRAY);

    // Legenda
    DrawRectangle(SCREEN_W-280, 60, 273, 115, (Color){0,0,0,185});
    DrawText("LEGENDA (koordinat Kartesian):", SCREEN_W-273, 66, 12, WHITE);
    for (int i = 0; i < 4; i++) {
        int ly = 84 + i*22;
        Midcircle(SCREEN_W-258, ly, 8, circles[i].c);
        DrawText(circles[i].lbl, SCREEN_W-244, ly-6, 11, circles[i].c);
    }

    // Info koordinat
    DrawCoordInfo("Midcircle");
    
    // Info algoritma tambahan
    DrawRectangle(8, SCREEN_H-145, 320, 90, (Color){0,0,0,180});
    DrawText("Midpoint Circle Algorithm:", 16, SCREEN_H-138, 12, (Color){180,200,255,255});
    DrawText("1. Mulai dari (0, r), hitung d = 3 - 2r", 16, SCREEN_H-120, 11, LIGHTGRAY);
    DrawText("2. Plot 8 titik simetri sekaligus", 16, SCREEN_H-106, 11, LIGHTGRAY);
    DrawText("3. Jika d < 0: pilih E, d += 4x + 6", 16, SCREEN_H-92, 11, LIGHTGRAY);
    DrawText("4. Jika d >= 0: pilih SE, d += 4(x-y)+10, y--", 16, SCREEN_H-78, 11, LIGHTGRAY);
    DrawText("5. Ulangi sampai y < x", 16, SCREEN_H-64, 11, LIGHTGRAY);

    DrawText("[ESC/BACKSPACE] Kembali  |  Klik BACK", 12, SCREEN_H-22, 13, DARKGRAY);
}
