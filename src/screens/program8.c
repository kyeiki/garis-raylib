#include "src/screens/program8.h"
#include "src/algo/midcircle.h"
#include "src/algo/bresenham.h"
#include "src/ui/back_button.h"
#include "src/ui/cartesian.h"
#include "src/ui/primitives.h"
#include "coords.h"
#include "screen_type.h"
#include "raylib.h"
#include <math.h>

#ifndef PI
#define PI 3.14159265358979323846f
#endif

/*
 * DrawArcFromAngle - Menggambar busur lingkaran murni dengan normalisasi
 * Sangat presisi untuk memotong busur di sudut berapapun.
 */
static void DrawArcFromAngle(int cx, int cy, int r, float startAngle, float endAngle, Color col) {
    if (r <= 0) return;
    
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;
    
    // Normalisasi rentang sudut 0 hingga 2*PI
    float normStart = fmodf(startAngle, 2.0f * PI);
    if (normStart < 0) normStart += 2.0f * PI;
    
    float normEnd = fmodf(endAngle, 2.0f * PI);
    if (normEnd < 0) normEnd += 2.0f * PI;
    
    while (y >= x) {
        int px[8] = {x, y, y, x, -x, -y, -y, -x};
        int py[8] = {y, x, -x, -y, -y, -x, x, y};
        
        for (int i = 0; i < 8; i++) {
            float pointAngle = atan2f((float)py[i], (float)px[i]);
            float normPoint = fmodf(pointAngle, 2.0f * PI);
            if (normPoint < 0) normPoint += 2.0f * PI;
            
            int inArc = 0;
            if (normStart <= normEnd) {
                inArc = (normPoint >= normStart && normPoint <= normEnd);
            } else {
                inArc = (normPoint >= normStart || normPoint <= normEnd);
            }
            
            if (inArc) {
                DrawPixel(cx + px[i], cy + py[i], col);
            }
        }
        
        if (d < 0) {
            d += 4 * x + 6;
        } else {
            d += 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

/*
 * DrawPetal - Menggambar kelopak Vesica Piscis murni dari titik pusat (cx, cy)
 * Kelopak ini tidak memiliki garis melingkar di titik pangkalnya.
 */
static void DrawPetal(int cx, int cy, int r, float angleDirection, Color col) {
    // cx, cy adalah titik PANGKAL kelopak (yang akan menempel di tengah layar)
    // Tinggi kelopak Vesica Piscis adalah r * sqrt(3)
    float h = r * 0.8660254f; // r * sqrt(3) / 2
    float halfR = r * 0.5f;

    // Titik tengah (center of gravity) kelopak
    float pmX = cx + cosf(angleDirection) * h;
    float pmY = cy + sinf(angleDirection) * h;

    // Pusat lingkaran 1 (Digeser 90 derajat ke kiri dari arah kelopak)
    float c1X = pmX + cosf(angleDirection - PI/2.0f) * halfR;
    float c1Y = pmY + sinf(angleDirection - PI/2.0f) * halfR;

    // Pusat lingkaran 2 (Digeser 90 derajat ke kanan dari arah kelopak)
    float c2X = pmX + cosf(angleDirection + PI/2.0f) * halfR;
    float c2Y = pmY + sinf(angleDirection + PI/2.0f) * halfR;

    // Busur 1 berpusat di C1, membuka ke arah C2 (120 derajat)
    float dirC1toC2 = angleDirection + PI/2.0f;
    float start1 = dirC1toC2 - PI/3.0f;
    float end1 = dirC1toC2 + PI/3.0f;

    // Busur 2 berpusat di C2, membuka ke arah C1 (120 derajat)
    float dirC2toC1 = angleDirection - PI/2.0f;
    float start2 = dirC2toC1 - PI/3.0f;
    float end2 = dirC2toC1 + PI/3.0f;

    // Gambar hanya busur pemotongnya
    DrawArcFromAngle((int)roundf(c1X), (int)roundf(c1Y), r, start1, end1, col);
    DrawArcFromAngle((int)roundf(c2X), (int)roundf(c2Y), r, start2, end2, col);
}

void DrawProgram8(void) {
    G_OriginX  = SCREEN_W / 2;
    G_OriginY  = SCREEN_H / 2 + 30;
    G_TickStep = 38;

    ClearBackground((Color){12,12,28,255});
    
    // Gambar grid kartesian
    DrawCartesianGrid(BresenhamLine);

    // Konfigurasi
    int cx = SCREEN_W / 2;
    int cy = SCREEN_H / 2 + 30;
    int R = 114;  // Radius pembentuk kelopak

    // Tentukan arah untuk 4 kelopak yang semuanya berpangkal di (cx, cy)
    struct { float angle; Color col; } petals[] = {
        { -PI/2.0f, RED },    // Menunjuk ke Utara (Y-)
        { PI/2.0f, BLUE },    // Menunjuk ke Selatan (Y+)
        { 0.0f, GREEN },      // Menunjuk ke Timur (X+)
        { PI, ORANGE },       // Menunjuk ke Barat (X-)
    };
    int numPetals = 4;
    
    // Render semua kelopak secara absolut
    for (int i = 0; i < numPetals; i++) {
        DrawPetal(cx, cy, R, petals[i].angle, petals[i].col);
    }

    // --- GARIS SUMBU (referensi) ---
    BresenhamLine(cx, cy - 200, cx, cy + 200, (Color){60,60,80,255});
    BresenhamLine(cx - 200, cy, cx + 200, cy, (Color){60,60,80,255});

    // --- UI & HEADER ---
    DrawRectangle(0, 0, SCREEN_W, 56, (Color){0,0,0,210});
    DrawBackButton();
    DrawText("PROGRAM 8 - Vesica Piscis (4 Kelopak Murni)", 135, 10, 18, WHITE);
    DrawText("Perhitungan titik pusat presisi tanpa lingkaran tengah (Overlap-Free)", 135, 33, 12, LIGHTGRAY);

    // Legenda
    DrawRectangle(SCREEN_W-280, 60, 273, 100, (Color){0,0,0,185});
    DrawText("HASIL RENDER:", SCREEN_W-273, 66, 12, WHITE);
    
    Midcircle(SCREEN_W-258, 90, 8, RED);
    DrawText("Kelopak Utara (Arah Y-)", SCREEN_W-244, 84, 11, RED);
    
    Midcircle(SCREEN_W-258, 112, 8, BLUE);
    DrawText("Kelopak Selatan (Arah Y+)", SCREEN_W-244, 106, 11, BLUE);
    
    Midcircle(SCREEN_W-258, 134, 8, GREEN);
    DrawText("Kelopak Timur & Barat (Sumbu X)", SCREEN_W-244, 128, 11, GREEN);

    // Info koordinat
    DrawCoordInfo("Midcircle");

    // Info algoritma
    DrawRectangle(8, SCREEN_H-145, 420, 100, (Color){0,0,0,180});
    DrawText("Kesimpulan Algoritma Lanjutan:", 16, SCREEN_H-138, 12, (Color){200,150,255,255});
    DrawText("1. Pusat koordinat (cx, cy) diletakkan tepat di UJUNG kelopak.", 16, SCREEN_H-120, 11, LIGHTGRAY);
    DrawText("2. Pusat lingkaran C1 & C2 dikalkulasi menggunakan Trigonometri.", 16, SCREEN_H-106, 11, LIGHTGRAY);
    DrawText("3. Garis sisa yang tidak diinginkan hilang secara matematis.", 16, SCREEN_H-92, 11, LIGHTGRAY);
    DrawText("4. Eksekusi loop DrawArc sangat minimal dan hemat memori.", 16, SCREEN_H-78, 11, LIGHTGRAY);
    DrawText("5. Solusi elegan untuk menyusun objek Geometris Sakral.", 16, SCREEN_H-64, 11, LIGHTGRAY);

    DrawText("[ESC/BACKSPACE] Kembali  |  Klik BACK", 12, SCREEN_H-22, 13, DARKGRAY);
}