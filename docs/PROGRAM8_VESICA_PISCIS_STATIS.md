# Program 8 — Vesica Piscis (4 Kelopak Murni)

## Konsep Matematika

**Vesica Piscis** (Latin untuk "kantung ikan") adalah bentuk lensa yang dibentuk oleh irisan dua lingkaran dengan radius yang sama. Program 8 menggambar 4 kelopak Vesica Piscis murni yang berpangkal di titik pusat layar, **tanpa lingkaran tengah** yang tidak diinginkan.

### Keunggulan Algoritma Program 8

Berbeda dengan Program 7 yang menggunakan pasangan lingkaran terpisah, Program 8 menggunakan pendekatan **trigonometri murni** untuk menghitung posisi pusat lingkaran pembentuk kelopak. Hasilnya:

- ✅ **Tidak ada garis berlebih** di titik pangkal kelopak
- ✅ **Semua kelopak bertemu tepat** di satu titik pusat
- ✅ **Perhitungan presisi** menggunakan `sin()`, `cos()`, dan `sqrt(3)/2`

---

## Struktur File dan Include

### Header Files

```c
#include "src/screens/program8.h"    // Deklarasi DrawProgram8()
#include "src/algo/midcircle.h"     // Algoritma Midpoint Circle (Midcircle)
#include "src/algo/bresenham.h"     // Algoritma Bresenham untuk garis
#include "src/ui/back_button.h"     // Komponen UI: Tombol kembali
#include "src/ui/cartesian.h"       // Grid kartesian (DrawCartesianGrid)
#include "src/ui/primitives.h"      // Primitif UI (DrawCoordInfo, dll)
#include "coords.h"                 // Sistem koordinat (G_OriginX, G_OriginY, G_TickStep)
#include "screen_type.h"            // Enum ScreenType untuk navigasi
#include "raylib.h"                 // Library grafis Raylib
#include <math.h>                   // Fungsi matematika (sin, cos, atan2, fmod)
```

### Konstanta

```c
#ifndef PI
#define PI 3.14159265358979323846f
#endif
```

---

## Algoritma Utama

### 1. DrawArcFromAngle — Menggambar Busur dengan Filter Sudut

Fungsi ini menggambar sebagian dari lingkaran (busur) berdasarkan rentang sudut menggunakan algoritma Midpoint Circle dengan filter sudut.

```c
static void DrawArcFromAngle(int cx, int cy, int r, float startAngle, float endAngle, Color col)
```

#### Parameter

| Parameter | Tipe | Deskripsi |
| --- | --- | --- |
| cx, cy | int | Pusat lingkaran (koordinat layar) |
| r | int | Radius lingkaran |
| startAngle | float | Sudut awal busur (radian) |
| endAngle | float | Sudut akhir busur (radian) |
| col | Color | Warna busur |

#### Algoritma

1. Validasi radius (`r > 0`).
2. Normalisasi sudut `startAngle` dan `endAngle` ke rentang `[0, 2π]`.
3. Iterasi menggunakan Midpoint Circle Algorithm:
   - Hitung 8 titik simetri untuk setiap koordinat `(x, y)`.
   - Untuk setiap titik simetri, hitung sudut posisinya menggunakan `atan2f()`.
   - Normalisasi sudut titik ke rentang `[0, 2π]`.
   - Periksa apakah sudut berada dalam rentang busur (menangani *wrap-around*).
   - Jika lulus filter sudut, gambar piksel tersebut.
4. Update *decision parameter* dan lanjutkan iterasi.

---

### 2. DrawPetal — Menggambar Satu Kelopak Vesica Piscis

Fungsi ini adalah inti dari Program 8. Ia menggambar satu kelopak Vesica Piscis yang berpangkal di titik `(cx, cy)` dan menunjuk ke arah `angleDirection`.

```c
static void DrawPetal(int cx, int cy, int r, float angleDirection, Color col)
```

#### Parameter

| Parameter | Tipe | Deskripsi |
| --- | --- | --- |
| cx, cy | int | Titik PANGKAL kelopak (titik temu semua kelopak) |
| r | int | Radius lingkaran pembentuk kelopak |
| angleDirection | float | Arah kelopak (radian, 0 = kanan, PI/2 = bawah) |
| col | Color | Warna kelopak |

#### Algoritma Perhitungan Pusat Lingkaran

Inilah keunikan Program 8 — menghitung posisi pusat kedua lingkaran pembentuk kelopak secara matematis:

```c
// Tinggi kelopak Vesica Piscis = r * sqrt(3)
float h = r * 0.8660254f;     // r * sqrt(3) / 2
float halfR = r * 0.5f;       // r / 2

// Titik tengah (center of gravity) kelopak
float pmX = cx + cosf(angleDirection) * h;
float pmY = cy + sinf(angleDirection) * h;

// Pusat lingkaran 1 (Digeser 90° ke kiri dari arah kelopak)
float c1X = pmX + cosf(angleDirection - PI/2.0f) * halfR;
float c1Y = pmY + sinf(angleDirection - PI/2.0f) * halfR;

// Pusat lingkaran 2 (Digeser 90° ke kanan dari arah kelopak)
float c2X = pmX + cosf(angleDirection + PI/2.0f) * halfR;
float c2Y = pmY + sinf(angleDirection + PI/2.0f) * halfR;
```

#### Visualisasi Geometri Kelopak

```
                    ╱╲
                  ╱    ╲
                ╱  C1    ╲      ← Busur dari C1 (120°)
              ╱            ╲
            ╱                ╲
          ╱                    ╲
        ╱__________PM__________╲    ← PM = Titik tengah kelopak
         \                     /
          \                   /
           \                 /
            \      C2      /      ← Busur dari C2 (120°)
             \             /
              \           /
               \         /
                \       /
                 \     /
                  \   /
                   \ /
                    ●  ← (cx, cy) = Titik pangkal kelopak
```

#### Perhitungan Sudut Busur

```c
// Busur 1 berpusat di C1, membuka ke arah C2 (120°)
float dirC1toC2 = angleDirection + PI/2.0f;
float start1 = dirC1toC2 - PI/3.0f;   // -60° dari arah tengah
float end1   = dirC1toC2 + PI/3.0f;   // +60° dari arah tengah

// Busur 2 berpusat di C2, membuka ke arah C1 (120°)
float dirC2toC1 = angleDirection - PI/2.0f;
float start2 = dirC2toC1 - PI/3.0f;
float end2   = dirC2toC1 + PI/3.0f;
```

---

## Konfigurasi Program

### Posisi dan Arah Kelopak

```c
int cx = SCREEN_W / 2;        // Pusat layar X
int cy = SCREEN_H / 2 + 30;   // Pusat layar Y (sedikit di bawah)
int R = 114;                  // Radius pembentuk kelopak

// 4 kelopak yang semuanya berpangkal di (cx, cy)
struct { float angle; Color col; } petals[] = {
    { -PI/2.0f, RED },    // Menunjuk ke Utara (Y-)
    { PI/2.0f, BLUE },    // Menunjuk ke Selatan (Y+)
    { 0.0f, GREEN },      // Menunjuk ke Timur (X+)
    { PI, ORANGE },       // Menunjuk ke Barat (X-)
};
```

### Diagram Arah Kelopak

```
                    ▲
                    │
                    │ RED (Utara)
                    │
                    │
    ◄───────────────┼───────────────►
     ORANGE (Barat) │ GREEN (Timur)
                    │
                    │
                    │ BLUE (Selatan)
                    │
                    ▼
```

---

## Perbandingan Program 7 vs Program 8

| Aspek | Program 7 | Program 8 |
|-------|-----------|-----------|
| **Pendekatan** | Pasangan lingkaran terpisah | Kelopak tunggal terkalkulasi |
| **Lingkaran tengah** | Ada (berlebih) | **Tidak ada** |
| **Titik temu** | Tidak presisi | **Tepat di (cx, cy)** |
| **Animasi** | Ya (transisi 360° → 120°) | Tidak (statis) |
| **Perhitungan pusat** | Offset sederhana | **Trigonometri penuh** |
| **Jumlah kelopak** | 4 pasangan = 8 lingkaran | 4 kelopak = 8 busur |

---

## Proporsi Vesica Piscis

### Rasio Geometri

**Rasio tinggi : lebar = √3 : 1 ≈ 1.732 : 1**

```
Tinggi kelopak = r × √3 ≈ r × 1.732
Lebar maksimal = r
```

### Konstanta Penting

```c
#define SQRT3_OVER_2  0.8660254f   // √3 / 2
#define HALF          0.5f          // 1 / 2
```

---

## Kompleksitas Algoritma

| Fungsi | Time Complexity | Keterangan |
|--------|-----------------|------------|
| `DrawArcFromAngle` | O(R) | Iterasi sebanding dengan nilai radius |
| `DrawPetal` | O(2R) | Dua busur per kelopak |
| `DrawProgram8` | O(8R) | 4 kelopak × 2 busur |

**Space Complexity:** O(1) — Tidak ada alokasi memori dinamis, sepenuhnya berjalan di stack.

---

## Makna Geometri Sakral

### Sejarah Penggunaan Vesica Piscis

| Periode | Budaya | Signifikansi |
|---------|--------|--------------|
| 2000 SM | Mesir Kuno | Proporsi dalam arsitektur sakral dan simbol dewi Isis |
| 500 SM | Yunani Kuno | Studi rasio matematika oleh Pythagoras |
| Kekristenan Awal | Romawi/Eropa | Simbol *Ichthys* (Ikan) dan *Mandorla* |
| Abad Pertengahan | Arsitektur Gotik | Rasio utama desain lengkungan katedral |

### Simbolisme

1. **Irisan:** Tempat pertemuan dua dunia (spiritual & material)
2. **Rasio √3:** Proporsi keseimbangan sempurna
3. **Dasar Geometri:** Langkah pertama dalam konstruksi *Flower of Life*
4. **Luas Area:** $Luas = 2 \times r^2 \times (\frac{\pi}{3} - \frac{\sqrt{3}}{4})$

---

## Troubleshooting

| Kasus | Penyebab | Solusi |
|-------|----------|--------|
| **Kelopak tidak bertemu di pusat** | Perhitungan `pmX/pmY` salah | Pastikan `h = r * sqrt(3)/2` |
| **Busur berlubang** | Normalisasi sudut gagal | Periksa logika `fmodf` dan wrap-around |
| **Bentuk tidak proporsional** | Nilai `halfR` tidak tepat | Gunakan `r * 0.5f` persis |
| **Arah kelopak salah** | Sudut `angleDirection` terbalik | Ingat: Y layar positif ke bawah |

---

## Kesimpulan

Program 8 mendemonstrasikan solusi elegan untuk menggambar Vesica Piscis tanpa artefak visual yang tidak diinginkan. Dengan menggunakan perhitungan trigonometri untuk menentukan posisi pusat lingkaran pembentuk, kita dapat:

1. Menempatkan titik pangkal kelopak tepat di pusat layar
2. Menghilangkan lingkaran tengah yang berlebihan
3. Menciptakan perpotongan garis yang presisi dan bersih

---

**Referensi:**

* Bresenham, J. E. (1977). *"A linear algorithm for incremental digital display of circular arcs"*.
* Lawlor, R. (1982). *"Sacred Geometry: Philosophy and Practice"*.
* Lundy, M. (2001). *"Sacred Geometry"*.
