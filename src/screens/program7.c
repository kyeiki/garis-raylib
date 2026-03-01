#include "src/screens/program7.h"
#include "src/algo/midcircle.h"
#include "src/algo/bresenham.h"
#include "src/ui/back_button.h"
#include "src/ui/cartesian.h"
#include "src/ui/primitives.h"
#include "coords.h"
#include "screen_type.h"
#include "raylib.h"
#include <math.h>
#include <stdio.h>

#ifndef PI
#define PI 3.14159265358979323846f
#endif

// Static variables for animation state
static float animTime = 0.0f;

/*
 * DrawArcFromAngle - Menggambar busur lingkaran dari sudut tertentu
 * (Sudah Diperbaiki dengan Normalisasi Sudut 0 - 2PI yang Kuat)
 */
static void DrawArcFromAngle(int cx, int cy, int r, float startAngle, float endAngle, Color col) {
    if (r <= 0) return;
    
    // Jika busur adalah lingkaran penuh (360 derajat), abaikan filter sudut agar lebih optimal
    float angleDiff = fabsf(endAngle - startAngle);
    int isFullCircle = (angleDiff >= 2.0f * PI - 0.01f);
    
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;
    
    // Normalisasi sudut input ke rentang 0 hingga 2*PI
    float normStart = fmodf(startAngle, 2.0f * PI);
    if (normStart < 0) normStart += 2.0f * PI;
    
    float normEnd = fmodf(endAngle, 2.0f * PI);
    if (normEnd < 0) normEnd += 2.0f * PI;
    
    while (y >= x) {
        //8 titik simetri
        int px[8] = {x, y, y, x, -x, -y, -y, -x};
        int py[8] = {y, x, -x, -y, -y, -x, x, y};
        
        for (int i = 0; i < 8; i++) {
            if (isFullCircle) {
                // Langsung gambar jika ini adalah fase lingkaran penuh
                DrawPixel(cx + px[i], cy + py[i], col);
            } else {
                // Hitung dan normalisasi sudut piksel ke rentang 0 hingga 2*PI
                float pointAngle = atan2f((float)py[i], (float)px[i]);
                float normPoint = fmodf(pointAngle, 2.0f * PI);
                if (normPoint < 0) normPoint += 2.0f * PI;
                
                int inArc = 0;
                
                // Logika wrap-around yang sudah aman
                if (normStart <= normEnd) {
                    inArc = (normPoint >= normStart && normPoint <= normEnd);
                } else {
                    // Jika busur melewati titik 0 derajat (misal dari 300 ke 60 derajat)
                    inArc = (normPoint >= normStart || normPoint <= normEnd);
                }
                
                if (inArc) {
                    DrawPixel(cx + px[i], cy + py[i], col);
                }
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
 * DrawVesicaPiscisAnim - Menggambar Vesica Piscis dengan efek animasi pemotongan
 * animProgress: 0.0 (Lingkaran Penuh) -> 1.0 (Bentuk Lensa Vesica)
 */
static void DrawVesicaPiscisAnim(int cx1, int cy1, int cx2, int cy2, int r, float animProgress, Color col) {
    // Hitung sudut dari cx1 ke cx2
    float dx = (float)(cx2 - cx1);
    float dy = (float)(cy2 - cy1);
    float angleToCenter = atan2f(dy, dx);
    
    // Logika Animasi Geometris:
    // Saat animProgress = 0.0, halfArc = 180 derajat (PI) -> Lingkaran Penuh
    // Saat animProgress = 1.0, halfArc = 60 derajat (PI/3) -> Vesica Piscis murni
    float targetHalfArc = PI / 3.0f; // 60 derajat
    float currentHalfArc = PI - (animProgress * (PI - targetHalfArc));
    
    // Busur dari lingkaran 1
    float start1 = angleToCenter - currentHalfArc;
    float end1 = angleToCenter + currentHalfArc;
    DrawArcFromAngle(cx1, cy1, r, start1, end1, col);
    
    // Busur dari lingkaran 2
    float start2 = angleToCenter + PI - currentHalfArc;
    float end2 = angleToCenter + PI + currentHalfArc;
    DrawArcFromAngle(cx2, cy2, r, start2, end2, col);
}

void DrawProgram7(void) {
    G_OriginX  = SCREEN_W / 2;
    G_OriginY  = SCREEN_H / 2 + 30;
    G_TickStep = 38;

    ClearBackground((Color){12,12,28,255});
    
    // Gambar grid kartesian
    DrawCartesianGrid(BresenhamLine);

    // Update animasi
    animTime += GetFrameTime();
    float cycleTime = 6.0f;  // 6 detik per siklus
    float t = fmodf(animTime, cycleTime) / cycleTime;
    
    // Progress loop utama: 0 -> 1 -> 0 (ping-pong)
    float progress;
    if (t < 0.5f) {
        progress = t * 2.0f;  // 0 -> 1
    } else {
        progress = (1.0f - t) * 2.0f;  // 1 -> 0
    }

    // Pemetaan Progress untuk fase Geometris
    float geomAnimProgress = 0.0f;
    if (progress <= 0.3f) {
        geomAnimProgress = 0.0f; // Fase 1: Lingkaran penuh ditahan sebentar
    } else if (progress >= 0.7f) {
        geomAnimProgress = 1.0f; // Fase 3: Vesica penuh ditahan sebentar
    } else {
        geomAnimProgress = (progress - 0.3f) / 0.4f; // Fase 2: Proses pemotongan
    }

    // Konfigurasi
    int cx = SCREEN_W / 2;
    int cy = SCREEN_H / 2 + 30;
    int R = 114;  // Radius

    // Tentukan pasangan pusat untuk setiap arah
    struct { int cx1, cy1, cx2, cy2; Color col; const char *name; } pairs[] = {
        { cx, cy, cx, cy - R, RED, "Utara" },           // Pusat ke Utara
        { cx, cy, cx, cy + R, BLUE, "Selatan" },        // Pusat ke Selatan
        { cx, cy, cx + R, cy, GREEN, "Timur" },         // Pusat ke Timur
        { cx, cy, cx - R, cy, ORANGE, "Barat" },        // Pusat ke Barat
    };
    int numPairs = 4;
    
    // Menggambar bentuk untuk setiap pasangan sumbu
    for (int i = 0; i < numPairs; i++) {
        DrawVesicaPiscisAnim(pairs[i].cx1, pairs[i].cy1, pairs[i].cx2, pairs[i].cy2, R, geomAnimProgress, pairs[i].col);
    }

    // --- GARIS SUMBU (referensi) ---
    BresenhamLine(cx, cy - 200, cx, cy + 200, (Color){60,60,80,255});
    BresenhamLine(cx - 200, cy, cx + 200, cy, (Color){60,60,80,255});

    // Header
    DrawRectangle(0, 0, SCREEN_W, 56, (Color){0,0,0,210});
    DrawBackButton();
    DrawText("PROGRAM 7 - Lingkaran ke Vesica Piscis", 135, 10, 18, WHITE);
    DrawText("Animasi Geometris: Pemotongan Sudut Lingkaran Penuh -> Vesica Piscis", 135, 33, 12, LIGHTGRAY);

    // Info progress
    DrawRectangle(SCREEN_W-280, 60, 273, 110, (Color){0,0,0,185});
    DrawText("ANIMASI:", SCREEN_W-273, 66, 12, WHITE);
    
    // Progress bar
    int barX = SCREEN_W-273, barY = 86, barW = 240, barH = 16;
    DrawRectangle(barX, barY, barW, barH, (Color){40,40,60,255});
    int fillW = (int)(progress * barW);
    DrawRectangle(barX, barY, fillW, barH, (Color){200,150,255,255});
    DrawRectangleLines(barX, barY, barW, barH, (Color){150,100,200,255});
    
    // Teks Fase Animasi
    char progressText[64];
    sprintf(progressText, "Progress: %.0f%%", progress * 100.0f);
    DrawText(progressText, barX, barY + 20, 12, LIGHTGRAY);
    
    if (progress < 0.3f) {
        DrawText("Fase 1: Lingkaran Penuh (360 derajat)", barX, barY + 38, 11, (Color){255,200,100,255});
    } else if (progress < 0.7f) {
        DrawText("Fase 2: Pemotongan Busur Dinamis", barX, barY + 38, 11, (Color){255,255,100,255});
    } else {
        DrawText("Fase 3: Vesica Piscis (120 derajat)", barX, barY + 38, 11, (Color){200,150,255,255});
    }
    DrawText("Vesica = irisan 2 lingkaran", barX, barY + 54, 10, (Color){120,120,140,255});

    // Legenda
    DrawRectangle(SCREEN_W-280, 180, 273, 90, (Color){0,0,0,185});
    DrawText("LEGENDA:", SCREEN_W-273, 186, 12, WHITE);
    
    Midcircle(SCREEN_W-258, 210, 8, RED);
    DrawText("Vesica Utara", SCREEN_W-244, 204, 11, RED);
    
    Midcircle(SCREEN_W-258, 232, 8, BLUE);
    DrawText("Vesica Selatan", SCREEN_W-244, 226, 11, BLUE);
    
    Midcircle(SCREEN_W-258, 254, 8, GREEN);
    DrawText("Vesica Timur & Barat", SCREEN_W-244, 248, 11, GREEN);

    // Info koordinat
    DrawCoordInfo("Midcircle");

    // Info algoritma
    DrawRectangle(8, SCREEN_H-145, 420, 100, (Color){0,0,0,180});
    DrawText("Vesica Piscis (Mata Ikan/Simbol Christ):", 16, SCREEN_H-138, 12, (Color){200,150,255,255});
    DrawText("1. Dibentuk dari irisan 2 lingkaran sama r", 16, SCREEN_H-120, 11, LIGHTGRAY);
    DrawText("2. Pusat lingkaran2 di tepi lingkaran1", 16, SCREEN_H-106, 11, LIGHTGRAY);
    DrawText("3. Setiap lingkaran sumbang busur 120 derajat", 16, SCREEN_H-92, 11, LIGHTGRAY);
    DrawText("4. Rasio lebar:tinggi = sqrt(3):1", 16, SCREEN_H-78, 11, LIGHTGRAY);
    DrawText("5. Dasar geometri sakral & Flower of Life", 16, SCREEN_H-64, 11, LIGHTGRAY);

    DrawText("[ESC/BACKSPACE] Kembali  |  Klik BACK", 12, SCREEN_H-22, 13, DARKGRAY);
}
