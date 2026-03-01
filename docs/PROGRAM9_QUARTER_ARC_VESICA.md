# Program 9 — Vesica Piscis (4 Arah Mata Angin via Pergeseran Jari-Jari)

## Konsep Utama

Program 9 menggambar **4 bentuk Vesica Piscis** menghadap tepat ke 4 arah mata angin (Utara, Selatan, Timur, Barat), semua **ujung dalam bertemu di titik pusat (0,0)**. Perbedaan mendasar dari Program 8 adalah cara konstruksinya:

> **Ide inti:** Alih-alih menghitung posisi pangkal kelopak dari *titik ujung*, Program 9 menentukan **titik tengah lensa** terlebih dahulu, lalu menggeser dua pusat lingkaran masing-masing sejauh **r/2** ke arah berlawanan.

---

## Konsep Matematika

### Pergeseran Titik Pusat (Center Shift)

Diberikan titik tengah lensa `(mx, my)` dan jari-jari `r`:

```
C1 = (mx − r/2 · cos(angle),  my − r/2 · sin(angle))
C2 = (mx + r/2 · cos(angle),  my + r/2 · sin(angle))
```

| Variabel | Makna |
|---|---|
| `(mx, my)` | Titik tengah (pusat gravitasi) lensa |
| `r` | Jari-jari lingkaran pembentuk busur |
| `angle` | Arah sumbu C1→C2 (bukan arah long-axis lensa) |
| `C1`, `C2` | Pusat lingkaran pembentuk dua busur |

### Mengapa Jarak C1–C2 = r?

```
Jarak C1-C2 = |C2 - C1|
            = |(mx + r/2) - (mx - r/2)|
            = |r/2 + r/2|
            = r
```

Ini membentuk **segitiga sama sisi** antara C1, C2, dan kedua titik ujung lensa, karena:
- Jarak C1→ujung = r  (jari-jari busur)
- Jarak C2→ujung = r  (jari-jari busur)
- Jarak C1→C2   = r  (hasil pergeseran)

→ Sudut di setiap titik segitiga = **60°**, sehingga busur yang dibutuhkan = **120°** (2 × 60°).

### Diagram Geometri

```
              Ujung Atas (tip)
                   *
                 ╱   ╲
               ╱       ╲
             ╱    60°    ╲
   r       ╱               ╲   r
         C1 ─────────────── C2
         ←─── r/2 ──● ── r/2 ───→
               (mx, my)
             ╲               ╱
               ╲    60°    ╱
                 ╲       ╱
                   ╲   ╱
              Ujung Bawah (tip)
```

### Titik Ujung Lensa (Tips)

Ujung-ujung lensa berada **tegak lurus** terhadap sumbu C1–C2, pada jarak:

```
h = r × √3/2 ≈ r × 0.8660254
```

Koordinat tip:
```
tip_atas  = (mx + h·cos(angle + π/2),  my + h·sin(angle + π/2))
tip_bawah = (mx + h·cos(angle − π/2),  my + h·sin(angle − π/2))
```

---

## Perhitungan Sudut Busur

Setiap busur menggambar **120°** (π/3 radian di setiap sisi dari arah tengahnya):

```c
// Busur dari C1 — menghadap ke C2 (arah = angle)
float start1 = angle - PI/3.0f;   // -60° dari arah tengah
float end1   = angle + PI/3.0f;   // +60° dari arah tengah

// Busur dari C2 — menghadap ke C1 (arah = angle + PI)
float start2 = (angle + PI) - PI/3.0f;
float end2   = (angle + PI) + PI/3.0f;
```

| Busur | Pusat | Menghadap | Lebar |
|---|---|---|---|
| Busur 1 | C1 | Ke arah C2 | 120° |
| Busur 2 | C2 | Ke arah C1 | 120° |

---

## Struktur File dan Include

```c
#include "src/screens/program9.h"    // Deklarasi DrawProgram9()
#include "src/algo/midcircle.h"      // Algoritma Midpoint Circle (Midcircle)
#include "src/algo/bresenham.h"      // Algoritma Bresenham untuk garis
#include "src/ui/back_button.h"      // Komponen UI: Tombol kembali
#include "src/ui/cartesian.h"        // Grid kartesian (DrawCartesianGrid)
#include "src/ui/primitives.h"       // Primitif UI (DrawCoordInfo, dll)
#include "coords.h"                  // Sistem koordinat (G_OriginX, G_OriginY, G_TickStep)
#include "screen_type.h"             // Enum ScreenType untuk navigasi
#include "raylib.h"                  // Library grafis Raylib
#include <math.h>                    // Fungsi matematika (sin, cos, atan2, fmod)
```

---

## Algoritma Utama

### 1. DrawQuarterArc — Menggambar Busur dengan Filter Sudut

Menggunakan algoritma **Midpoint Circle** dengan pengecekan sudut per-piksel.

```c
static void DrawQuarterArc(int cx, int cy, int r, float startAngle, float endAngle, Color col)
```

#### Parameter

| Parameter | Tipe | Deskripsi |
|---|---|---|
| `cx, cy` | int | Pusat lingkaran (koordinat layar) |
| `r` | int | Radius lingkaran |
| `startAngle` | float | Sudut awal busur (radian, sistem layar) |
| `endAngle` | float | Sudut akhir busur (radian) |
| `col` | Color | Warna busur |

#### Cara Kerja

```
Untuk setiap (x, y) dari Midpoint Circle Algorithm:
  1. Hitung 8 titik simetri: (±x, ±y) dan (±y, ±x)
  2. Untuk tiap titik simetri:
       a. Hitung sudutnya dengan atan2f(py, px)
       b. Normalisasi sudut ke [0, 2π]
       c. Periksa apakah sudut ada dalam [normStart, normEnd]
          (tangani wrap-around jika normStart > normEnd)
       d. Jika ya → DrawPixel()
  3. Update decision parameter d:
       Jika d < 0: d += 4x + 6
       Jika d ≥ 0: d += 4(x-y) + 10, y--
  4. x++, ulangi hingga y < x
```

### 2. DrawLens — Menggambar Satu Lensa Vesica Piscis

```c
static void DrawLens(int mx, int my, int r, float angle, Color col1, Color col2)
```

#### Parameter

| Parameter | Tipe | Deskripsi |
|---|---|---|
| `mx, my` | int | Titik tengah lensa (pusat bounding box) |
| `r` | int | Jari-jari lingkaran pembentuk |
| `angle` | float | Arah sumbu pendek lensa (arah C1→C2, bukan long-axis) |
| `col1` | Color | Warna busur dari C1 |
| `col2` | Color | Warna busur dari C2 |

#### Implementasi

```c
static void DrawLens(int mx, int my, int r, float angle, Color col1, Color col2) {
    float halfR = r * 0.5f;

    // Geser C1 dan C2 sejauh r/2 dari titik tengah
    float c1x = mx - cosf(angle) * halfR;
    float c1y = my - sinf(angle) * halfR;
    float c2x = mx + cosf(angle) * halfR;
    float c2y = my + sinf(angle) * halfR;

    // Busur C1 (menghadap ke C2)
    float start1 = angle - PI / 3.0f;
    float end1   = angle + PI / 3.0f;

    // Busur C2 (menghadap ke C1)
    float start2 = angle + PI - PI / 3.0f;
    float end2   = angle + PI + PI / 3.0f;

    DrawQuarterArc((int)roundf(c1x), (int)roundf(c1y), r, start1, end1, col1);
    DrawQuarterArc((int)roundf(c2x), (int)roundf(c2y), r, start2, end2, col2);
}
```

---

## Konfigurasi 4 Arah Mata Angin

### Parameter Program

```c
int cx = SCREEN_W / 2;       // Pusat layar X
int cy = SCREEN_H / 2 + 30;  // Pusat layar Y
int R  = 114;                 // Jari-jari busur

float h = R * 0.8660254f;     // R × √3/2 = jarak pusat ke ujung lensa
```

### Tabel Posisi 4 Lensa

| Arah | Pusat Lensa (mx, my) | angle | Warna | Alasan |
|---|---|---|---|---|
| **Utara** | `(cx, cy − h)` | `0` | Merah | angle=0 → C1-C2 horizontal → tips vertikal = lensa tegak |
| **Selatan** | `(cx, cy + h)` | `0` | Biru | Sama, tapi di bawah pusat |
| **Timur** | `(cx + h, cy)` | `π/2` | Hijau | angle=π/2 → C1-C2 vertikal → tips horizontal = lensa rebah |
| **Barat** | `(cx − h, cy)` | `π/2` | Oranye | Sama, tapi di kiri pusat |

### Mengapa Offset = h = R × √3/2?

Ujung dalam setiap lensa berjarak `h` dari pusat lensa, ke arah tegak lurus sumbu angle. Agar semua ujung dalam **bertemu tepat di `(cx, cy)`**:

```
Lensa UTARA (angle=0, pusat di (cx, cy-h)):
  tip_bawah = (cx, cy−h + h) = (cx, cy)  ✓

Lensa SELATAN (angle=0, pusat di (cx, cy+h)):
  tip_atas  = (cx, cy+h − h) = (cx, cy)  ✓

Lensa TIMUR (angle=π/2, pusat di (cx+h, cy)):
  tip_kiri  = (cx+h − h, cy) = (cx, cy)  ✓

Lensa BARAT (angle=π/2, pusat di (cx-h, cy)):
  tip_kanan = (cx−h + h, cy) = (cx, cy)  ✓
```

### Diagram Layout

```
                    ● (ujung luar UTARA)
                   ╱╲
                  ╱  ╲  Merah
                 ╱    ╲
                ╱      ╲
               (cx,cy-h)   ← pusat lensa Utara
                ╲      ╱
      (Barat)    ╲    ╱    (Timur)
       ● ─────────(0,0)──────── ●
        ╲      ╱  ╲   ╱       ╱
  Oranye ╲    ╱    ╲ ╱       ╱ Hijau
          ╲  ╱      ╲      ╱
           ╲╱    ╱───╲   ╱
               (cx,cy+h)   ← pusat lensa Selatan
                ╲      ╱
                 ╲    ╱  Biru
                  ╲  ╱
                   ╲╱
                    ● (ujung luar SELATAN)
```

---

## Hubungan angle vs Long Axis Lensa

Ini adalah keunikan implementasi yang perlu diperhatikan:

| `angle` | Arah C1−C2 | Long axis (ujung lensa) | Orientasi visual |
|---|---|---|---|
| `0` | Horizontal | **Vertikal** | Lensa tegak (U/S) |
| `π/2` | Vertikal | **Horizontal** | Lensa rebah (T/B) |
| `π/4` | 45° | 135° | Lensa diagonal ↗ |

> **Catatan:** `angle` mendefinisikan arah *sumbu pendek* (antara dua pusat C1–C2), bukan arah *long axis*. Long axis selalu **tegak lurus** terhadap `angle`.

---

## Kompleksitas Algoritma

| Fungsi | Time Complexity | Keterangan |
|---|---|---|
| `DrawQuarterArc` | O(R) | Iterasi Midpoint Circle = R langkah |
| `DrawLens` | O(2R) | Dua busur per lensa |
| `DrawProgram9` | O(8R) | 4 lensa × 2 busur |

**Space Complexity:** O(1) — Tidak ada alokasi memori dinamis, sepenuhnya berjalan di stack.

---

## Proporsi Vesica Piscis

| Dimensi | Rumus | Nilai (R=114) |
|---|---|---|
| Jarak C1–C2 | `r` | 114 px |
| Tinggi lensa (tip-to-tip) | `r × √3 ≈ 1.732r` | 197 px |
| Lebar lensa (di tengah) | `r` | 114 px |
| Rasio tinggi:lebar | `√3 : 1` | 1.732 : 1 |
| Jarak pusat ke ujung | `r × √3/2` | 98.7 px |

---

## Troubleshooting

| Kasus | Penyebab | Solusi |
|---|---|---|
| **Ujung lensa tidak bertemu di (0,0)** | Offset `h` salah | Pastikan `h = R * 0.8660254` |
| **Busur tampak berlubang** | Normalisasi sudut gagal | Tangani wrap-around saat `normStart > normEnd` |
| **Lensa terlihat miring** | `angle` salah untuk orientasi yang diinginkan | Ingat: `angle=0` → lensa vertikal, `angle=π/2` → lensa horizontal |
| **Ujung tidak berpotongan** | `start/end` busur terlalu sempit | Gunakan persis ± `PI/3` (60°) dari arah tengah |

---

## Kesimpulan

Program 9 membuktikan bahwa Vesica Piscis dapat dibangun dari prinsip yang lebih sederhana secara konseptual:

1. Tentukan **titik tengah** lensa, bukan titik pangkal
2. **Geser** dua pusat lingkaran masing-masing sejauh `r/2`
3. Jarak pergeseran total = `r` → segitiga sama sisi otomatis terbentuk
4. Gambar busur **120°** dari setiap pusat ke arah lainnya
5. Keempat lensa bertemu sempurna di titik pusat karena offset = `r × √3/2`

---

**Referensi:**

* Bresenham, J. E. (1977). *"A linear algorithm for incremental digital display of circular arcs"*.
* Weisstein, E. W. *"Vesica Piscis"*. MathWorld — A Wolfram Web Resource.
* Lawlor, R. (1982). *"Sacred Geometry: Philosophy and Practice"*.
