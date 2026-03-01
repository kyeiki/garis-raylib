#include "src/screens/program6.h"
#include "src/algo/midcircle.h"
#include "src/algo/bresenham.h"
#include "src/ui/back_button.h"
#include "src/ui/cartesian.h"
#include "src/ui/primitives.h"
#include "coords.h"
#include "screen_type.h"
#include "raylib.h"

void DrawProgram6(void) {
    G_OriginX  = SCREEN_W / 2;
    G_OriginY  = SCREEN_H / 2 + 30;
    G_TickStep = 38;

    ClearBackground((Color){12,12,28,255});
    
    // Gambar grid kartesian menggunakan algoritma Bresenham
    DrawCartesianGrid(BresenhamLine);

    // Konfigurasi Flower of Life
    int cx = SCREEN_W / 2;
    int cy = SCREEN_H / 2 + 30;
    int R = 114;  // Radius lingkaran pembentuk (3 satuan kartesian = 3 * 38)

    // --- 1. LINGKARAN PUSAT ---
    Midcircle(cx, cy, R, RAYWHITE);

    // --- 2. LINGKARAN PEMBENTUK KELOPAK (6 arah) ---
    // Setiap lingkaran berpusat di tepi lingkaran utama
    // Membentuk pola "Flower of Life" sederhana
    
    // Utara (atas)
    Midcircle(cx, cy - R, R, RED);
    
    // Selatan (bawah)
    Midcircle(cx, cy + R, R, BLUE);
    
    // Timur (kanan)
    Midcircle(cx + R, cy, R, GREEN);
    
    // Barat (kiri)
    Midcircle(cx - R, cy, R, ORANGE);
    
    // Timur Laut (kanan atas) - jarak = R, sudut 45 derajat
    Midcircle(cx + R*0.866, cy - R/2, R, YELLOW);
    
    // Barat Daya (kiri bawah)
    Midcircle(cx - R*0.866, cy + R/2, R, YELLOW);
    
    // Barat Laut (kiri atas)
    Midcircle(cx - R*0.866, cy - R/2, R, MAGENTA);
    
    // Tenggara (kanan bawah)
    Midcircle(cx + R*0.866, cy + R/2, R, MAGENTA);

    // --- 3. GARIS SUMBU (referensi) ---
    BresenhamLine(cx, cy - 200, cx, cy + 200, (Color){60,60,80,255});
    BresenhamLine(cx - 200, cy, cx + 200, cy, (Color){60,60,80,255});

    // Header
    DrawRectangle(0, 0, SCREEN_W, 56, (Color){0,0,0,210});
    DrawBackButton();
    DrawText("PROGRAM 6 - Flower of Life Pattern", 135, 10, 18, WHITE);
    DrawText("Midpoint Circle Algorithm | 1 lingkaran pusat + 6 lingkaran kelopak", 135, 33, 12, LIGHTGRAY);

    // Legenda
    DrawRectangle(SCREEN_W-280, 60, 273, 145, (Color){0,0,0,185});
    DrawText("LEGENDA:", SCREEN_W-273, 66, 12, WHITE);
    
    Midcircle(SCREEN_W-258, 90, 8, RAYWHITE);
    DrawText("Lingkaran Pusat", SCREEN_W-244, 84, 11, RAYWHITE);
    
    Midcircle(SCREEN_W-258, 112, 8, RED);
    DrawText("Kelopak Utara", SCREEN_W-244, 106, 11, RED);
    
    Midcircle(SCREEN_W-258, 134, 8, BLUE);
    DrawText("Kelopak Selatan", SCREEN_W-244, 128, 11, BLUE);
    
    Midcircle(SCREEN_W-258, 156, 8, GREEN);
    DrawText("Kelopak Timur/Barat", SCREEN_W-244, 150, 11, GREEN);
    
    Midcircle(SCREEN_W-258, 178, 8, YELLOW);
    DrawText("Kelopak Diagonal", SCREEN_W-244, 172, 11, YELLOW);

    // Info koordinat
    DrawCoordInfo("Midcircle");

    // Info algoritma tambahan
    DrawRectangle(8, SCREEN_H-145, 350, 100, (Color){0,0,0,180});
    DrawText("Flower of Life Pattern:", 16, SCREEN_H-138, 12, (Color){180,200,255,255});
    DrawText("1. Lingkaran pusat di (0,0)", 16, SCREEN_H-120, 11, LIGHTGRAY);
    DrawText("2. 6 lingkaran kelopak di tepi pusat", 16, SCREEN_H-106, 11, LIGHTGRAY);
    DrawText("3. Jarak pusat kelopak = radius", 16, SCREEN_H-92, 11, LIGHTGRAY);
    DrawText("4. Membentuk pola geometri sakral", 16, SCREEN_H-78, 11, LIGHTGRAY);
    DrawText("5. Semua menggunakan Midcircle()", 16, SCREEN_H-64, 11, LIGHTGRAY);

    DrawText("[ESC/BACKSPACE] Kembali  |  Klik BACK", 12, SCREEN_H-22, 13, DARKGRAY);
}
