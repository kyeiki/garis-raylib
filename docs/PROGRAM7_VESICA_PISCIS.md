
# Program 7 — Animated Vesica Piscis

## Konsep Matematika

**Vesica Piscis** (Latin untuk "kantung ikan") adalah bentuk lensa yang dibentuk oleh irisan dua lingkaran dengan radius yang sama, di mana pusat setiap lingkaran terletak persis pada keliling lingkaran lainnya.



### Persamaan Geometri

Secara umum, untuk membentuk satu kelopak Vesica Piscis yang proporsional di dalam program, persamaan dua lingkaran yang digunakan adalah:

```text
Lingkaran 1: (x - cx1)² + (y - cy1)² = r²
Lingkaran 2: (x - cx2)² + (y - cy2)² = r²

```

**Syarat Utama:** Jarak antara pusat `(cx1, cy1)` dan `(cx2, cy2)` harus tepat sama dengan jari-jari `r`.

### Proporsi Vesica Piscis

**Rasio tinggi : lebar = √3 : 1 ≈ 1.732 : 1**

---

## Struktur File dan Include

### Header Files

```c
#include "src/screens/program7.h"    // Deklarasi DrawProgram7()
#include "src/algo/midcircle.h"     // Algoritma Midpoint Circle (Midcircle)
#include "src/algo/bresenham.h"     // Algoritma Bresenham untuk garis
#include "src/ui/back_button.h"     // Komponen UI: Tombol kembali
#include "src/ui/cartesian.h"       // Grid kartesian (DrawCartesianGrid)
#include "src/ui/primitives.h"      // Primitif UI (DrawCoordInfo, dll)
#include "coords.h"                 // Sistem koordinat (G_OriginX, G_OriginY, G_TickStep)
#include "screen_type.h"            // Enum ScreenType untuk navigasi
#include "raylib.h"                 // Library grafis Raylib
#include <math.h>                   // Fungsi matematika (sin, cos, atan2, fmod)
#include <stdio.h>                  // sprintf untuk formatting teks

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

Fungsi ini menggambar sebagian dari lingkaran (busur) berdasarkan rentang sudut menggunakan aritmatika *integer* yang dioptimasi.

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
2. Deteksi apakah busur adalah lingkaran penuh (360°/2π) untuk optimasi *bypass* logika sudut.
3. Normalisasi sudut `startAngle` dan `endAngle` ke rentang `[0, 2π]`.
4. Iterasi menggunakan Midpoint Circle Algorithm:
* Hitung 8 titik simetri untuk setiap koordinat `(x, y)`.
* Untuk setiap titik simetri, hitung sudut posisinya menggunakan `atan2f()`.
* Normalisasi sudut titik ke rentang `[0, 2π]`.
* Periksa apakah sudut berada dalam rentang busur (menangani *wrap-around*).
* Jika lulus filter sudut, gambar piksel tersebut.


5. Update *decision parameter* dan lanjutkan iterasi.

#### Normalisasi Sudut & Penanganan Wrap-Around

**Mengapa fmodf krusial?** Fungsi `atan2f` mengembalikan nilai dari $-π$ hingga $+π$. Sementara kalkulasi pembentukan busur (misal `angle + PI + halfArc`) bisa menghasilkan nilai lebih besar dari $2π$. `fmodf` memaksa semua nilai masuk ke dalam lingkaran standar $[0, 2π]$ agar logika komparasi piksel tidak meleset.

```c
// Normalisasi ke rentang [0, 2π]
float normStart = fmodf(startAngle, 2.0f * PI);
if (normStart < 0) normStart += 2.0f * PI;

// Kasus 1: Busur normal (tidak melewati 0°)
if (normStart <= normEnd) {
    inArc = (normPoint >= normStart && normPoint <= normEnd);
}
// Kasus 2: Busur melewati 0° (misal dari 300° menyambung ke 60°)
else {
    inArc = (normPoint >= normStart || normPoint <= normEnd);
}

```

---

### 2. DrawVesicaPiscisAnim — Animasi Geometris Vesica Piscis

Fungsi ini menggambar dua busur yang membentuk Vesica Piscis dengan animasi pemotongan sudut secara geometris dari lingkaran penuh hingga menjadi kelopak.

```c
static void DrawVesicaPiscisAnim(int cx1, int cy1, int cx2, int cy2, 
                                  int r, float animProgress, Color col)

```

#### Algoritma Kunci: Dynamic Arc Cutting

```c
// Hitung sudut orientasi dari cx1 ke cx2
float angleToCenter = atan2f(dy, dx);

// Animasi Geometris:
// animProgress = 0.0 → halfArc = 180° (π)   → Lingkaran Penuh (360°)
// animProgress = 1.0 → halfArc = 60° (π/3)  → Vesica Piscis (120°)

float targetHalfArc = PI / 3.0f;  // 60 derajat (bentuk final)
float currentHalfArc = PI - (animProgress * (PI - targetHalfArc));

```

#### Perhitungan Pemotongan Busur

```c
// Lingkaran 1: Busur menghadap ke cx2
float start1 = angleToCenter - currentHalfArc;
float end1   = angleToCenter + currentHalfArc;
DrawArcFromAngle(cx1, cy1, r, start1, end1, col);

// Lingkaran 2: Busur menghadap ke cx1 (berlawanan arah + 180°)
float start2 = angleToCenter + PI - currentHalfArc;
float end2   = angleToCenter + PI + currentHalfArc;
DrawArcFromAngle(cx2, cy2, r, start2, end2, col);

```

---

## Fase Animasi

### Pemetaan Progress (Ping-Pong)

Animasi berjalan secara bolak-balik (*ping-pong*) dalam siklus 6 detik. Waktu 3 detik pertama digunakan untuk transisi $0 \rightarrow 1$, dan 3 detik berikutnya untuk kembali $1 \rightarrow 0$.

```c
// Progress loop: 0 → 1 → 0 (ping-pong)
float t = fmodf(animTime, cycleTime) / cycleTime;
float progress = (t < 0.5f) ? t * 2.0f : (1.0f - t) * 2.0f;

// Pemetaan ke fase geometris
if (progress <= 0.3f) {
    geomAnimProgress = 0.0f;  // Fase 1: Tahan lingkaran penuh (360°)
} else if (progress >= 0.7f) {
    geomAnimProgress = 1.0f;  // Fase 3: Tahan Vesica Piscis (120°)
} else {
    geomAnimProgress = (progress - 0.3f) / 0.4f;  // Fase 2: Transisi pemotongan
}

```

### Timeline Animasi (6 detik per siklus)

| Waktu | Progress (`progress`) | Fase Animasi | Deskripsi Visual |
| --- | --- | --- | --- |
| 0.0s - 0.9s | 0% - 30% | Fase 1 | Lingkaran penuh ditahan stabil |
| 0.9s - 2.1s | 30% - 70% | Fase 2 | Pemotongan busur bergerak dinamis |
| 2.1s - 3.0s | 70% - 100% | Fase 3 | Vesica Piscis murni ditahan stabil |
| 3.0s - 6.0s | 100% - 0% | Reverse | Animasi berbalik memulihkan lingkaran |

---

## Konfigurasi Program

### Posisi dan Pemetaan Kelopak Mata Angin

```c
int cx = SCREEN_W / 2;      // Pusat layar X
int cy = SCREEN_H / 2 + 30; // Pusat layar Y (sedikit di bawah)
int R = 114;                // Radius

// 4 pasangan Vesica Piscis tegak lurus pada sumbu
pairs[] = {
    { cx, cy, cx, cy - R, RED,    "Utara" },     // Pusat ke Utara
    { cx, cy, cx, cy + R, BLUE,   "Selatan" },   // Pusat ke Selatan
    { cx, cy, cx + R, cy, GREEN,  "Timur" },     // Pusat ke Timur
    { cx, cy, cx - R, cy, ORANGE, "Barat" },     // Pusat ke Barat
};

```

## Makna Geometri Sakral

Sejarah dari proporsi geometri fundamental.

### Sejarah Penggunaan Vesica Piscis

| Periode | Budaya | Signifikansi |
| --- | --- | --- |
| 2000 SM | Mesir Kuno | Proporsi dalam arsitektur sakral dan simbol dewi Isis. |
| 500 SM | Yunani Kuno | Studi rasio matematika oleh Pythagoras, akar kuadrat dari 3. |
| Kekristenan Awal | Romawi/Eropa | Digunakan sebagai simbol *Ichthys* (Ikan) dan *Mandorla* (Aura Christ). |
| Abad Pertengahan | Arsitektur Gotik | Rasio utama dalam desain lengkungan pintu dan jendela katedral. |

### Simbolisme & Matematika

1. **Irisan:** Tempat pertemuan dua entitas atau dunia (spiritual & material).
2. **Rasio $\sqrt{3}$:** Proporsi keseimbangan yang sempurna.
3. **Dasar Geometri Kompleks:** Bentuk ini adalah langkah pertama (*Seed of Life*) dalam mengkonstruksi *Flower of Life* dan kubus Metatron.
4. **Luas:** Luas area irisan Vesica Piscis dihitung dengan rumus:
$Luas = 2 \times r^2 \times (\frac{\pi}{3} - \frac{\sqrt{3}}{4}) \approx 1.2284 \times r^2$

---

## Kompleksitas Algoritma

| Fungsi | Time Complexity | Keterangan |
| --- | --- | --- |
| `DrawArcFromAngle` | $O(R)$ | Iterasi sebanding dengan nilai radius (Bresenham logic). |
| `DrawVesicaPiscisAnim` | $O(2R)$ | Memanggil dua operasi busur per satu bentuk kelopak. |
| `DrawProgram7` | $O(8R)$ | Menggambar 4 kelopak $\times$ 2 busur pada setiap iterasi *frame*. |

*Space Complexity:* **$O(1)$** — Tidak ada alokasi memori dinamis (*heap*), sepenuhnya berjalan efisien di memori lokal (*stack*).

---

## Troubleshooting & Debugging

| Kasus | Kemungkinan Penyebab | Solusi |
| --- | --- | --- |
| **Busur berlubang / tidak lengkap** | Logika normalisasi sudut gagal mendeteksi nilai negatif atau *overflow* radian. | Pastikan implementasi `fmodf` dan blok pengecekan kondisional (*wrap-around*) sudah diterapkan sesuai kode utama. |
| **FPS Drop / Flicker** | Fungsi `DrawArcFromAngle` mengeksekusi trigonometri berat padahal busur adalah $360^\circ$ penuh. | Gunakan deteksi `isFullCircle` untuk melakukan *bypass* langsung ke `DrawPixel`. |
| **Bentuk tidak proporsional** | Jarak `cx1, cy1` ke `cx2, cy2` tidak sama persis dengan variabel radius `R`. | Periksa kembali array struktur koordinat pusat. |

---

**Referensi Akademis:**

* Bresenham, J. E. (1977). *"A linear algorithm for incremental digital display of circular arcs"*.
* Lawlor, R. (1982). *"Sacred Geometry: Philosophy and Practice"*.

```

```