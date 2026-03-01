# Program 5 — Algoritma Midpoint Circle

## Konsep Matematika

**Algoritma Midpoint Circle** (juga dikenal sebagai Bresenham Circle Algorithm) adalah algoritma untuk menggambar lingkaran menggunakan hanya operasi integer, tanpa perhitungan floating-point atau fungsi trigonometri.

### Persamaan Lingkaran

Lingkaran dengan pusat (cx, cy) dan radius r didefinisikan oleh:

```
(x - cx)² + (y - cy)² = r²
```

Atau jika pusat di (0, 0):

```
x² + y² = r²
```

### 8-Way Symmetry

Lingkaran memiliki simetri pada 8 bagian. Dengan menghitung satu titik (x, y), kita bisa mendapatkan 8 titik lainnya secara gratis:
```
(x, y)
(y, x)
(y, -x)
(x, -y)
(-x, -y)
(-y, -x)
(-y, x)
(-x, y)
```

**8 titik simetri:**
```
(cx + x, cy + y)  ← Oktan 1
(cx + y, cy + x)  ← Oktan 2
(cx + y, cy - x)  ← Oktan 3
(cx + x, cy - y)  ← Oktan 4
(cx - x, cy - y)  ← Oktan 5
(cx - y, cy - x)  ← Oktan 6
(cx - y, cy + x)  ← Oktan 7
(cx - x, cy + y)  ← Oktan 8
```

Dengan demikian, kita hanya perlu menghitung 1/8 lingkaran (oktan pertama), dan sisanya didapat dari simetri.

---

## Algoritma Midpoint

### Inisialisasi

```
x = 0
y = r
d = 3 - 2 × r    ← Decision parameter awal
```

### Iterasi

Untuk setiap langkah dari x = 0 hingga x ≥ y:

1. **Plot 8 titik simetri**
2. **Evaluasi decision parameter:**
   - Jika `d < 0`: Pilih titik **E (East)** → `d = d + 4×x + 6`
   - Jika `d ≥ 0`: Pilih titik **SE (South-East)** → `d = d + 4×(x - y) + 10` dan `y--`
3. **Increment:** `x++`

### Mengapa Decision Parameter Bekerja?

Decision parameter `d` mengukur jarak titik midpoint ke lingkaran ideal:

```
f(x, y) = x² + y² - r²

Jika f < 0: titik di dalam lingkaran
Jika f > 0: titik di luar lingkaran
Jika f = 0: titik tepat pada lingkaran
```

Pada setiap langkah, kita memilih antara:
- **E (East):** (x+1, y) — bergerak ke kanan
- **SE (South-East):** (x+1, y-1) — bergerak ke kanan-bawah

Midpoint antara E dan SE adalah di (x+1, y-0.5). Decision parameter membantu memilih titik mana yang lebih dekat ke lingkaran ideal.

---

## Contoh Perhitungan Lengkap — Lingkaran r = 5

**Inisialisasi:**
```
x = 0, y = 5
d = 3 - 2×5 = 3 - 10 = -7
```

**Tabel Iterasi:**

| Langkah | x | y | d | Kondisi | Aksi | 8 Titik (offset dari pusat) |
|---------|---|---|---|---------|------|----------------------------|
| 0 | 0 | 5 | -7 | d < 0 | E: d = -7 + 0 + 6 = -1 | (0,5), (5,0), (5,0), (0,-5), (0,-5), (-5,0), (-5,0), (0,5) |
| 1 | 1 | 5 | -1 | d < 0 | E: d = -1 + 4 + 6 = 9 | (1,5), (5,1), (5,-1), (1,-5), (-1,-5), (-5,-1), (-5,1), (-1,5) |
| 2 | 2 | 5 | 9 | d ≥ 0 | SE: d = 9 + 4(2-5) + 10 = 9 - 12 + 10 = 7, y=4 | (2,4), (4,2), (4,-2), (2,-4), (-2,-4), (-4,-2), (-4,2), (-2,4) |
| 3 | 3 | 4 | 7 | d ≥ 0 | SE: d = 7 + 4(3-4) + 10 = 7 - 4 + 10 = 13, y=3 | (3,3), (3,3), (3,-3), (3,-3), (-3,-3), (-3,-3), (-3,3), (-3,3) |
| 4 | 4 | 3 | - | - | **BERHENTI** (x ≥ y) | - |

**Total piksel unik:** 16 titik (beberapa titik overlap pada langkah 3)

---

## Implementasi C

```c
// File: src/algo/midcircle.c

// Helper: Plot 8 titik simetri sekaligus
static void PlotSymmetryPoints(int cx, int cy, int x, int y, Color color) {
    DrawPixel(cx + x, cy + y, color); // Oktan 1
    DrawPixel(cx + y, cy + x, color); // Oktan 2
    DrawPixel(cx + y, cy - x, color); // Oktan 3
    DrawPixel(cx + x, cy - y, color); // Oktan 4
    DrawPixel(cx - x, cy - y, color); // Oktan 5
    DrawPixel(cx - y, cy - x, color); // Oktan 6
    DrawPixel(cx - y, cy + x, color); // Oktan 7
    DrawPixel(cx - x, cy + y, color); // Oktan 8
}

void Midcircle(int centerX, int centerY, int radius, Color color) {
    if (radius <= 0) {
        DrawPixel(centerX, centerY, color);
        return;
    }
    
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;  // Decision parameter awal
    
    while (y >= x) {
        PlotSymmetryPoints(centerX, centerY, x, y, color);
        
        if (d < 0) {
            // Pilih titik E (East)
            d = d + 4 * x + 6;
        } else {
            // Pilih titik SE (South-East)
            d = d + 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}
```

---

## Variasi Lingkaran

### 1. Lingkaran Berisi (Filled Circle)

Menggambar garis horizontal untuk setiap baris y:

```c
void MidcircleFilled(int centerX, int centerY, int radius, Color color) {
    int x = 0, y = radius;
    int d = 3 - 2 * radius;
    
    while (y >= x) {
        // Gambar garis horizontal untuk mengisi
        for (int i = centerX - x; i <= centerX + x; i++) {
            DrawPixel(i, centerY + y, color);
            DrawPixel(i, centerY - y, color);
        }
        for (int i = centerX - y; i <= centerX + y; i++) {
            DrawPixel(i, centerY + x, color);
            DrawPixel(i, centerY - x, color);
        }
        
        if (d < 0) {
            d = d + 4 * x + 6;
        } else {
            d = d + 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}
```

### 2. Lingkaran Tebal (Thick Circle)

Menggambar beberapa lingkaran konsentris:

```c
void MidcircleThick(int centerX, int centerY, int radius, int thickness, Color color) {
    int innerR = radius - thickness / 2;
    int outerR = radius + thickness / 2;
    
    for (int r = innerR; r <= outerR; r++) {
        Midcircle(centerX, centerY, r, color);
    }
}
```

### 3. Lingkaran Putus-putus (Dashed Circle)

Menggunakan counter untuk mengatur pola dash dan gap:

```c
void MidcircleDashed(int centerX, int centerY, int radius, 
                     int dashLen, int gapLen, Color color) {
    int x = 0, y = radius;
    int d = 3 - 2 * radius;
    
    int counter = 0;
    int drawing = 1;
    int current_limit = dashLen;
    
    while (y >= x) {
        if (drawing) {
            // Plot 8 titik hanya jika dalam fase "draw"
            PlotSymmetryPoints(centerX, centerY, x, y, color);
        }
        
        counter += 8;
        if (counter >= current_limit) {
            counter = 0;
            drawing = !drawing;
            current_limit = drawing ? dashLen : gapLen;
        }
        
        if (d < 0) {
            d = d + 4 * x + 6;
        } else {
            d = d + 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}
```

---

## Perbandingan dengan Metode Lain

| Metode | Operasi | Akurasi | Kecepatan |
|--------|---------|---------|-----------|
| **Trigonometri** | sin, cos (float) | Tinggi | Lambat |
| **Persamaan Lingkaran** | sqrt (float) | Tinggi | Sedang |
| **Midpoint/Bresenham** | Integer only | Tinggi | **Cepat** |

### Mengapa Midpoint Lebih Efisien?

1. **Integer-only:** Tidak membutuhkan FPU (Floating-Point Unit)
2. **8-way symmetry:** Hanya 1/8 perhitungan diperlukan
3. **Incremental:** Hanya penjumlahan dan pengurangan per langkah
4. **No multiplication/division:** Operasi berat dihindari dalam loop

---

## Kompleksitas

- **Time Complexity:** O(r) — jumlah iterasi sebanding dengan radius
- **Space Complexity:** O(1) — hanya beberapa variabel integer

---

## Aplikasi di Program 5

Program 5 menampilkan:

1. **Grid Kartesian** — digambar menggunakan algoritma Bresenham yang sudah ada
2. **4 Lingkaran** dengan posisi dan radius berbeda:
   - Lingkaran 1: pusat (0, 0), r = 5 (merah)
   - Lingkaran 2: pusat (-4, 3), r = 3 (biru)
   - Lingkaran 3: pusat (5, -2), r = 4 (kuning)
   - Lingkaran 4: pusat (0, 5), r = 2 (magenta)

3. **Konversi koordinat** Kartesian ke layar menggunakan:
   ```
   screen_x = originX + cart_x × tickStep
   screen_y = originY - cart_y × tickStep
   ```

---

## Referensi

- Bresenham, J. E. (1977). "A linear algorithm for incremental digital display of circular arcs"
- Foley, J. D., et al. (1990). "Computer Graphics: Principles and Practice"
- Hearn, D., & Baker, M. P. (2004). "Computer Graphics with OpenGL"
