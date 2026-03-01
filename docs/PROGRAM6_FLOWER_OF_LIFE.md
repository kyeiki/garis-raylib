# Program 6 — Flower of Life Pattern

## Konsep Matematika

**Flower of Life** adalah pola geometri sakral yang telah digunakan selama ribuan tahun di berbagai budaya. Pola ini terdiri dari banyak lingkaran yang saling beririsan dengan proporsi matematis yang sempurna.

### Struktur Dasar

Flower of Life dibangun dari:
1. **1 lingkaran pusat** — sebagai titik awal
2. **6 lingkaran kelopak** — berpusat di tepi lingkaran pusat, membentuk pola bunga
3. **Lingkaran tambahan** — dapat diperluas secara tak terbatas

### Prinsip Geometri

Kunci dari Flower of Life adalah **jarak antar pusat lingkaran = radius**.

```
        ● Utara (0, -R)
        │
        │
●───────●───────● Barat (-R, 0)   Timur (R, 0)
        │
        │
        ● Selatan (0, +R)
```

Setiap lingkaran kelopak beririsan dengan lingkaran pusat tepat di titik tengah jari-jari.

---

## Koordinat 6 Kelopak Utama

### Sistem Koordinat Kartesian

Dengan pusat di (0, 0) dan radius R:

| Arah | Sudut | Offset X | Offset Y | Rumus |
|------|-------|----------|----------|-------|
| Utara | 90° | 0 | -R | (0, -R) |
| Timur Laut | 30° | +R×cos(30°) | -R×sin(30°) | (+R×0.866, -R×0.5) |
| Tenggara | 330° | +R×cos(30°) | +R×sin(30°) | (+R×0.866, +R×0.5) |
| Selatan | 270° | 0 | +R | (0, +R) |
| Barat Daya | 210° | -R×cos(30°) | +R×sin(30°) | (-R×0.866, +R×0.5) |
| Barat Laut | 150° | -R×cos(30°) | -R×sin(30°) | (-R×0.866, -R×0.5) |

### Visualisasi Sudut

```
        90° (Utara)
          ●
          │
150° ●────┼────● 30° (Timur Laut)
          │
180° ─────●───── 0°/360°
          │
210° ●────┼────● 330° (Tenggara)
          │
          ●
       270° (Selatan)
```

---

## Perhitungan Detail

### Konstanta Trigonometri

```
cos(30°) = cos(π/6) ≈ 0.866025
sin(30°) = sin(π/6) = 0.5
```

### Contoh dengan R = 114 piksel

| Kelopak | Pusat X | Pusat Y | Perhitungan |
|---------|---------|---------|-------------|
| Utara | 500 | 216 | cy - R = 330 - 114 |
| Selatan | 500 | 444 | cy + R = 330 + 114 |
| Timur | 614 | 330 | cx + R = 500 + 114 |
| Barat | 386 | 330 | cx - R = 500 - 114 |
| Timur Laut | 599 | 273 | cx + R×0.866, cy - R/2 |
| Barat Daya | 401 | 387 | cx - R×0.866, cy + R/2 |
| Barat Laut | 401 | 273 | cx - R×0.866, cy - R/2 |
| Tenggara | 599 | 387 | cx + R×0.866, cy + R/2 |

---

## Algoritma Menggambar

### Langkah-langkah

1. **Tentukan pusat** (cx, cy) dan radius R
2. **Gambar lingkaran pusat** dengan `Midcircle(cx, cy, R, color)`
3. **Gambar 6 kelopak utama**:
   - Utara: `Midcircle(cx, cy - R, R, color)`
   - Selatan: `Midcircle(cx, cy + R, R, color)`
   - Timur: `Midcircle(cx + R, cy, R, color)`
   - Barat: `Midcircle(cx - R, cy, R, color)`
   - Timur Laut: `Midcircle(cx + R×0.866, cy - R/2, R, color)`
   - Barat Daya: `Midcircle(cx - R×0.866, cy + R/2, R, color)`
4. **(Opsional) Tambahkan kelopak diagonal lainnya**

---

## Implementasi C

```c
// File: src/screens/program6.c

void DrawProgram6(void) {
    // Setup koordinat
    int cx = SCREEN_W / 2;
    int cy = SCREEN_H / 2 + 30;
    int R = 114;  // Radius = 3 satuan kartesian (3 × 38)

    // 1. Lingkaran Pusat
    Midcircle(cx, cy, R, RAYWHITE);

    // 2. Kelopak Utama (4 arah mata angin)
    Midcircle(cx, cy - R, R, RED);      // Utara
    Midcircle(cx, cy + R, R, BLUE);     // Selatan
    Midcircle(cx + R, cy, R, GREEN);    // Timur
    Midcircle(cx - R, cy, R, ORANGE);   // Barat

    // 3. Kelopak Diagonal (4 arah tambahan)
    int dx = (int)(R * 0.866);  // R × cos(30°)
    int dy = R / 2;              // R × sin(30°)
    
    Midcircle(cx + dx, cy - dy, R, YELLOW);   // Timur Laut
    Midcircle(cx - dx, cy + dy, R, YELLOW);   // Barat Daya
    Midcircle(cx - dx, cy - dy, R, MAGENTA);  // Barat Laut
    Midcircle(cx + dx, cy + dy, R, MAGENTA);  // Tenggara
}
```

---

## Hubungan dengan Midpoint Circle

Program6 menggunakan fungsi [`Midcircle()`](../src/algo/midcircle.c) dari Program5:

```c
void Midcircle(int centerX, int centerY, int radius, Color color);
```

### Mengapa Midcircle Cocok?

1. **Integer-only** — Semua perhitungan koordinat menggunakan integer
2. **8-way symmetry** — Lingkaran sempurna tanpa floating-point
3. **Efisien** — O(r) kompleksitas, cocok untuk multiple circles
4. **Akurat** — Tidak ada rounding error dari operasi float

---

## Variasi Flower of Life

### 1. Seed of Life (7 Lingkaran)

Versi paling sederhana dengan hanya:
- 1 lingkaran pusat
- 6 kelopak mengelilingi

```
     ●  ●  ●
      \ | /
     ●──●──●
      / | \
     ●  ●  ●
```

### 2. Flower of Life Penuh

Ekspansi lebih lanjut dengan lingkaran tambahan di luar:
- 19 lingkaran total
- Membentuk pola "bunga" yang lebih kompleks

### 3. Fruit of Life

13 lingkaran dari Flower of Life yang saling bersentuhan:
- Basis untuk Metatron's Cube
- Geometri sakral lanjutan

---

## Aplikasi di Program6

Program6 menampilkan:

### Lingkaran yang Digambar

| No | Warna | Pusat (Kartesian) | Radius |
|----|-------|-------------------|--------|
| 1 | Putih | (0, 0) | 3 |
| 2 | Merah | (0, -3) | 3 |
| 3 | Biru | (0, +3) | 3 |
| 4 | Hijau | (+3, 0) | 3 |
| 5 | Orange | (-3, 0) | 3 |
| 6 | Kuning | (+2.6, -1.5) | 3 |
| 7 | Kuning | (-2.6, +1.5) | 3 |
| 8 | Magenta | (-2.6, -1.5) | 3 |
| 9 | Magenta | (+2.6, +1.5) | 3 |

### Konversi ke Piksel Layar

```
screen_x = originX + cart_x × tickStep
screen_y = originY - cart_y × tickStep

Dengan:
- originX = 500 (tengah layar)
- originY = 380 (sedikit di bawah tengah)
- tickStep = 38 piksel per satuan
```

---

## Kompleksitas

### Time Complexity

- **Per lingkaran:** O(r) — sebanding dengan radius
- **Total (9 lingkaran):** O(9 × r) = O(r)

### Space Complexity

- **O(1)** — hanya variabel lokal untuk koordinat

---

## Makana Geometri Sakral

### Sejarah

Flower of Life ditemukan di:
- Kuil Osiris, Abydos, Mesir (~10.500 SM)
- Sinagoge kuno di Galilea
- Kuil Buddha di India
- Gereja-gereja abad pertengahan di Eropa

### Simbolisme

1. **Kesatuan** — Semua lingkaran terhubung
2. **Keseimbangan** — Simetri sempurna 6 arah
3. **Kreativitas** — Basis untuk pola geometri lain
4. **Kehidupan** — Mewakili siklus dan pertumbuhan

---

## Perbandingan dengan Pola Lain

| Pola | Jumlah Lingkaran | Kompleksitas |
|------|------------------|--------------|
| Seed of Life | 7 | Rendah |
| Flower of Life | 19+ | Sedang |
| Fruit of Life | 13 | Sedang |
| Metatron's Cube | 13 + garis | Tinggi |
| **Program6** | 9 | Sedang |

---

## Ekstensi yang Mungkin

### 1. Animasi

```c
// Animasi kelopak muncul satu per satu
for (int i = 0; i < 6; i++) {
    DrawKelopak(i, color[i]);
    WaitTime(0.5);
}
```

### 2. Interaktif

```c
// Radius berubah sesuai posisi mouse
int R = (mouseX - cx) / 2;
if (R < 50) R = 50;
if (R > 200) R = 200;
```

### 3. Warna Gradient

```c
// Warna berubah berdasarkan sudut
Color GetAngleColor(int angle) {
    float hue = angle / 360.0f;
    return ColorFromHSV(hue, 0.8f, 1.0f);
}
```

---

## Referensi

- Melchizedek, D. (1999). "The Ancient Secret of the Flower of Life"
- Lawlor, R. (1982). "Sacred Geometry: Philosophy and Practice"
- Schneider, M. S. (1994). "A Beginner's Guide to Constructing the Universe"
- Lundy, M. (2001). "Sacred Geometry"
