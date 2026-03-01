#include "src/screens/program9.h"
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
 * DrawQuarterArc - Menggambar busur SEPEREMPAT lingkaran menggunakan
 * algoritma Midpoint Circle, hanya dalam rentang sudut startAngle hingga endAngle.
 *
 * Metode: Plot seluruh oktan lingkaran, tapi hanya gambar piksel
 * yang sudut-nya masuk dalam rentang [startAngle, endAngle].
 */
static void DrawQuarterArc(int cx, int cy, int r, float startAngle, float endAngle, Color col) {
    if (r <= 0) return;

    int x = 0;
    int y = r;
    int d = 3 - 2 * r;

    /* Normalisasi rentang sudut ke 0..2PI */
    float normStart = fmodf(startAngle, 2.0f * PI);
    if (normStart < 0) normStart += 2.0f * PI;
    float normEnd = fmodf(endAngle, 2.0f * PI);
    if (normEnd < 0) normEnd += 2.0f * PI;

    while (y >= x) {
        /* 8 titik simetri lingkaran */
        int px[8] = { x,  y,  y,  x, -x, -y, -y, -x };
        int py[8] = { y,  x, -x, -y, -y, -x,  x,  y };

        for (int i = 0; i < 8; i++) {
            float angle = atan2f((float)py[i], (float)px[i]);
            float norm  = fmodf(angle, 2.0f * PI);
            if (norm < 0) norm += 2.0f * PI;

            int inArc;
            if (normStart <= normEnd) {
                inArc = (norm >= normStart && norm <= normEnd);
            } else {
                /* Melewati batas 2PI */
                inArc = (norm >= normStart || norm <= normEnd);
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
 * DrawLens - Menggambar bentuk lensa (Vesica Piscis) menggunakan
 * metode PERGESERAN TITIK JARI-JARI.
 *
 * Konsep:
 *   Titik A = ujung kiri,  titik B = ujung kanan  (jarak A-B = r)
 *   Busur 1: Berpusat di A, menggambar seperempat kanan-atas & kanan-bawah
 *            yaitu dari sudut -60° hingga +60° (busur 120° tapi kita bagi 2 kuadran)
 *   Busur 2: Berpusat di B, menggambar seperempat kiri-atas & kiri-bawah
 *            yaitu dari sudut 120° hingga 240°
 *
 * Pergeseran: Pusat C1 digeser ke kiri sejauh r/2, pusat C2 ke kanan sejauh r/2
 * sehingga kedua busur bertemu persis di titik atas dan bawah.
 *
 * Parameter:
 *   mx, my  = titik tengah lensa
 *   r       = jari-jari pembentuk busur
 *   angle   = rotasi lensa (radian, 0 = horizontal)
 *   col1    = warna busur kiri
 *   col2    = warna busur kanan
 */
static void DrawLens(int mx, int my, int r, float angle, Color col1, Color col2) {
    /*
     * Untuk lensa horizontal (angle=0):
     *   C1 berada di (mx - r/2, my)  → busur sebelah kanan: -60° s/d +60°
     *   C2 berada di (mx + r/2, my)  → busur sebelah kiri:  120° s/d 240°
     *
     * Rotasi diterapkan dengan cosf/sinf terhadap sudut.
     */
    float halfR = r * 0.5f;

    /* Pusat busur 1 (digeser ke arah -angle) */
    float c1x = mx - cosf(angle) * halfR;
    float c1y = my - sinf(angle) * halfR;

    /* Pusat busur 2 (digeser ke arah +angle) */
    float c2x = mx + cosf(angle) * halfR;
    float c2y = my + sinf(angle) * halfR;

    /*
     * Sudut busur yang benar untuk pertemuan tepat:
     *   Titik pertemuan atas/bawah berjarak r dari kedua C1 dan C2.
     *   Antara C1 dan C2 berjarak r, jadi segitiga sama sisi → sudut 60°.
     *
     *   Busur dari C1 menghadap ke C2: arah C1→C2 = angle
     *   Busur membentang dari  (angle - PI/3) hingga (angle + PI/3)  [120° = 2×60°]
     *
     *   Busur dari C2 menghadap ke C1: arah C2→C1 = angle + PI
     *   Busur membentang dari  (angle + PI - PI/3) hingga (angle + PI + PI/3)
     */
    float start1 = angle - PI / 3.0f;
    float end1   = angle + PI / 3.0f;

    float start2 = angle + PI - PI / 3.0f;
    float end2   = angle + PI + PI / 3.0f;

    DrawQuarterArc((int)roundf(c1x), (int)roundf(c1y), r, start1, end1, col1);
    DrawQuarterArc((int)roundf(c2x), (int)roundf(c2y), r, start2, end2, col2);
}


void DrawProgram9(void) {
    G_OriginX  = SCREEN_W / 2;
    G_OriginY  = SCREEN_H / 2 + 30;
    G_TickStep = 38;

    ClearBackground((Color){10, 10, 24, 255});
    DrawCartesianGrid(BresenhamLine);

    int cx = SCREEN_W / 2;
    int cy = SCREEN_H / 2 + 30;
    int R  = 114;  /* Jari-jari busur (sama dengan program8 untuk konsistensi) */

    /*
     * ================================================================
     * 4 VESICA PISCIS - ARAH MATA ANGIN
     *
     * Prinsip pergeseran titik jari-jari:
     *   - DrawLens(mx, my, r, angle): sumbu C1-C2 mengikuti `angle`,
     *     sehingga ujung (tip) lensa tegak lurus terhadap angle.
     *
     *   - angle=0    → C1-C2 horizontal → ujung lensa di atas & bawah
     *   - angle=PI/2 → C1-C2 vertikal   → ujung lensa di kiri & kanan
     *
     *   Jarak pusat lensa ke tiap ujung = R * sqrt(3)/2 ≈ 0.866 * R
     *
     *   Agar semua ujung dalam bertemu tepat di (cx, cy):
     *     Utara: DrawLens(cx, cy - h, R, 0)        → ujung bawah = (cx, cy)
     *     Selatan: DrawLens(cx, cy + h, R, 0)      → ujung atas  = (cx, cy)
     *     Timur: DrawLens(cx + h, cy, R, PI/2)     → ujung kiri  = (cx, cy)
     *     Barat: DrawLens(cx - h, cy, R, PI/2)     → ujung kanan = (cx, cy)
     * ================================================================
     */
    float h = R * 0.8660254f;  /* R * sqrt(3)/2 */

    int lx[4], ly[4];
    float la[4];
    Color lc1[4], lc2[4];

    /* Utara (Atas) - merah */
    lx[0] = cx;         ly[0] = (int)(cy - h); la[0] = 0.0f;
    lc1[0] = (Color){255,  60,  60, 255};
    lc2[0] = (Color){255, 130,  80, 255};

    /* Selatan (Bawah) - biru */
    lx[1] = cx;         ly[1] = (int)(cy + h); la[1] = 0.0f;
    lc1[1] = (Color){ 60, 120, 255, 255};
    lc2[1] = (Color){ 80, 200, 255, 255};

    /* Timur (Kanan) - hijau */
    lx[2] = (int)(cx + h); ly[2] = cy;         la[2] = PI / 2.0f;
    lc1[2] = (Color){ 60, 220,  80, 255};
    lc2[2] = (Color){140, 255, 110, 255};

    /* Barat (Kiri) - oranye */
    lx[3] = (int)(cx - h); ly[3] = cy;         la[3] = PI / 2.0f;
    lc1[3] = (Color){255, 160,  20, 255};
    lc2[3] = (Color){255, 210,  80, 255};

    for (int i = 0; i < 4; i++) {
        DrawLens(lx[i], ly[i], R, la[i], lc1[i], lc2[i]);
    }

    /* Titik pusat pertemuan semua ujung dalam */
    for (int dx = -2; dx <= 2; dx++)
        for (int dy = -2; dy <= 2; dy++)
            DrawPixel(cx + dx, cy + dy, (Color){255, 255, 255, 200});

    /* Sumbu referensi tipis */
    BresenhamLine(cx, cy - (int)(h * 2.1f), cx, cy + (int)(h * 2.1f), (Color){50, 50, 80, 160});
    BresenhamLine(cx - (int)(h * 2.1f), cy, cx + (int)(h * 2.1f), cy, (Color){50, 50, 80, 160});

    /* Garis jari-jari penghubung (panduan pergeseran) - ke tiap pusat lensa */
    BresenhamLine(cx, cy, lx[0], ly[0], (Color){255,  80,  80, 80});
    BresenhamLine(cx, cy, lx[1], ly[1], (Color){ 80, 140, 255, 80});
    BresenhamLine(cx, cy, lx[2], ly[2], (Color){ 60, 200,  80, 80});
    BresenhamLine(cx, cy, lx[3], ly[3], (Color){255, 160,  40, 80});

    /* --- UI & HEADER --- */
    DrawRectangle(0, 0, SCREEN_W, 56, (Color){0, 0, 0, 210});
    DrawBackButton();
    DrawText("PROGRAM 9 - Vesica Piscis (4 Arah Mata Angin)", 135, 10, 18, WHITE);
    DrawText("Metode: pergeseran titik pusat jari-jari r/2, busur 120 derajat dari tiap pusat", 135, 33, 11, LIGHTGRAY);

    /* Legenda */
    DrawRectangle(SCREEN_W - 260, 60, 252, 106, (Color){0, 0, 0, 185});
    DrawText("4 VESICA PISCIS:", SCREEN_W - 253, 66, 11, WHITE);

    DrawRectangleLinesEx((Rectangle){SCREEN_W-253, 81, 8, 8}, 1, lc1[0]);
    DrawText("UTARA  (angle = 0, Y-)", SCREEN_W - 240, 82, 10, lc1[0]);

    DrawRectangleLinesEx((Rectangle){SCREEN_W-253, 97, 8, 8}, 1, lc1[1]);
    DrawText("SELATAN (angle = 0, Y+)", SCREEN_W - 240, 98, 10, lc1[1]);

    DrawRectangleLinesEx((Rectangle){SCREEN_W-253, 113, 8, 8}, 1, lc1[2]);
    DrawText("TIMUR  (angle = 90, X+)", SCREEN_W - 240, 114, 10, lc1[2]);

    DrawRectangleLinesEx((Rectangle){SCREEN_W-253, 129, 8, 8}, 1, lc1[3]);
    DrawText("BARAT  (angle = 90, X-)", SCREEN_W - 240, 130, 10, lc1[3]);

    DrawPixel(SCREEN_W-253, 148, WHITE); DrawPixel(SCREEN_W-252, 148, WHITE);
    DrawPixel(SCREEN_W-253, 149, WHITE); DrawPixel(SCREEN_W-252, 149, WHITE);
    DrawText("Titik temu (0,0)", SCREEN_W - 240, 146, 10, WHITE);

    /* Keterangan algoritma */
    DrawRectangle(8, SCREEN_H - 130, 490, 88, (Color){0, 0, 0, 180});
    DrawText("Algoritma Pergeseran Titik Jari-Jari:", 16, SCREEN_H - 123, 12, (Color){200, 150, 255, 255});
    DrawText("1. C1 = pusat lensa - r/2 (digeser searah sumbu pendek)", 16, SCREEN_H - 105, 11, LIGHTGRAY);
    DrawText("2. C2 = pusat lensa + r/2 (berlawanan dengan C1)", 16, SCREEN_H - 91, 11, LIGHTGRAY);
    DrawText("3. Jarak C1-C2 = r  ->  segitiga sama sisi  ->  sudut 60 derajat", 16, SCREEN_H - 77, 11, LIGHTGRAY);
    DrawText("4. Busur C1: 120 deg menghadap C2  |  Busur C2: 120 deg menghadap C1", 16, SCREEN_H - 63, 11, (Color){100, 220, 150, 255});

    DrawCoordInfo("QuarterArc (Midpoint)");
    DrawText("[ESC/BACKSPACE] Kembali  |  Klik BACK", 12, SCREEN_H - 22, 13, DARKGRAY);
}

