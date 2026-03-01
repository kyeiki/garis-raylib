# Modul 1 — Algoritma Midpoint Circle

**Mata Kuliah:** Grafika Komputer  
**Program:** Program 5  
**Tingkat:** Pemula–Menengah

---

## 🎯 Tujuan Pembelajaran

Setelah menyelesaikan modul ini, mahasiswa mampu:

1. Menjelaskan mengapa algoritma Midpoint Circle lebih efisien daripada metode trigonometri.
2. Memahami konsep **8-way symmetry** pada lingkaran dan manfaatnya.
3. Menurunkan **decision parameter** `d` dari persamaan lingkaran.
4. Mengimplementasikan algoritma Midpoint Circle dalam bahasa C.
5. Melakukan **perhitungan manual** iterasi algoritma untuk radius kecil.

---

## 1. Latar Belakang — Mengapa Tidak Pakai `sin` / `cos`?

Cara paling naif untuk menggambar lingkaran adalah iterasi sudut:

```c
// ❌ Metode naif — TIDAK efisien
for (float angle = 0; angle < 2*PI; angle += 0.01f) {
    int x = cx + (int)(r * cosf(angle));
    int y = cy + (int)(r * sinf(angle));
    DrawPixel(x, y, color);
}
```

**Masalah metode ini:**

| Masalah | Penjelasan |
|---|---|
| **Lambat** | `sin` dan `cos` adalah operasi FPU yang berat |
| **Piksel bolong** | Besar sudut yang tetap bisa melompati piksel pada bagian tertentu |
| **Bergantung FPU** | Tidak bisa dijalankan pada prosesor tanpa unit floating-point |

**Solusi:** Algoritma Midpoint Circle (Bresenham Circle) menggunakan **hanya operasi integer** (penjumlahan dan pengurangan) sehingga jauh lebih cepat dan portabel.

---

## 2. Konsep Matematika

### 2.1 Persamaan Lingkaran

Lingkaran dengan pusat $(c_x, c_y)$ dan jari-jari $r$ memenuhi:

$$
(x - c_x)^2 + (y - c_y)^2 = r^2
$$

Bila kita tempatkan pusat di koordinat $(0, 0)$:

$$
f(x, y) = x^2 + y^2 - r^2 = 0
$$

Fungsi $f(x, y)$ ini digunakan sebagai **uji posisi titik**:
- $f < 0$ → titik di **dalam** lingkaran
- $f > 0$ → titik di **luar** lingkaran
- $f = 0$ → titik tepat **pada** lingkaran

### 2.2 8-Way Symmetry

Lingkaran bersifat simetri terhadap 4 sumbu (horizontal, vertikal, dan dua diagonal). Artinya, cukup hitung **1/8 bagian** (satu oktan), kemudian 7 titik lainnya didapat secara gratis dari pencerminan:

```
Dari satu titik (x, y), kita dapatkan 8 titik sekaligus:

   (-x, y)  |  (x, y)
   (-y, x)  |  (y, x)
   (-y,-x)  |  (y,-x)
   (-x,-y)  |  (x,-y)
```

Dit diterjemahkan ke koordinat layar sebagai:

```c
DrawPixel(cx + x, cy + y);  // Oktan 1
DrawPixel(cx + y, cy + x);  // Oktan 2
DrawPixel(cx + y, cy - x);  // Oktan 3
DrawPixel(cx + x, cy - y);  // Oktan 4
DrawPixel(cx - x, cy - y);  // Oktan 5
DrawPixel(cx - y, cy - x);  // Oktan 6
DrawPixel(cx - y, cy + x);  // Oktan 7
DrawPixel(cx - x, cy + y);  // Oktan 8
```

---

## 3. Penurunan Decision Parameter

Algoritma berjalan dari $x = 0$ hingga $x = y$ (oktan pertama). Di setiap langkah, ia memilih antara dua piksel kandidat:

- **E (East):** $(x+1, y)$ — tetap di tinggi yang sama
- **SE (South-East):** $(x+1, y-1)$ — turun satu tingkat

Untuk memilih piksel mana yang **lebih dekat ke lingkaran ideal**, kita evaluasi titik tengah (midpoint) antara E dan SE: $(x+1,\ y - 0.5)$.

Kita masukkan ke fungsi $f$:

$$
d = f(x+1,\ y-0.5) = (x+1)^2 + (y-0.5)^2 - r^2
$$

**Inisialisasi** saat $x=0,\ y=r$:

$$
d_0 = f(1,\ r-0.5) = 1 + r^2 - r + 0.25 - r^2 = 1.25 - r
$$

Untuk menghindari bilangan desimal, kita gunakan $d = 3 - 2r$ (dikalikan 4 dan dibulatkan):

$$
d_0 = 3 - 2r
$$

**Update decision parameter:**

| Kondisi | Pilih | Update `d` |
|---|---|---|
| $d < 0$ | E (kanan) | $d \mathrel{+}= 4x + 6$ |
| $d \geq 0$ | SE (kanan-bawah) | $d \mathrel{+}= 4(x - y) + 10$, lalu $y \mathrel{-}= 1$ |

---

## 4. Pseudocode

```
Fungsi MidpointCircle(cx, cy, r):
    x ← 0
    y ← r
    d ← 3 - 2 × r

    Selama (y ≥ x):
        Plot8Titik(cx, cy, x, y)

        Jika d < 0:
            d ← d + 4×x + 6
        Jika tidak:
            d ← d + 4×(x - y) + 10
            y ← y - 1

        x ← x + 1
```

---

## 5. Contoh Perhitungan Manual — r = 5

**Inisialisasi:** $x=0,\ y=5,\ d = 3 - 2 \times 5 = -7$

| Langkah | $x$ | $y$ | $d$ | Kondisi | Aksi | d baru | y baru |
|---|---|---|---|---|---|---|---|
| 0 | 0 | 5 | −7 | $d < 0$ | E | $-7 + 0 + 6 = -1$ | 5 |
| 1 | 1 | 5 | −1 | $d < 0$ | E | $-1 + 4 + 6 = 9$ | 5 |
| 2 | 2 | 5 | 9 | $d \geq 0$ | SE | $9 + 4(2-5) + 10 = 7$ | 4 |
| 3 | 3 | 4 | 7 | $d \geq 0$ | SE | $7 + 4(3-4) + 10 = 13$ | 3 |
| 4 | 4 | 3 | — | **BERHENTI** ($x \geq y$) | — | — | — |

**Titik yang diplot (offset dari pusat):**

| Langkah | Titik-titik yang diplot |
|---|---|
| 0 | $(0,5), (5,0), (0,-5), (-5,0)$ |
| 1 | $(1,5),(5,1),(5,-1),(1,-5),(-1,-5),(-5,-1),(-5,1),(-1,5)$ |
| 2 | $(2,4),(4,2),(4,-2),(2,-4),(-2,-4),(-4,-2),(-4,2),(-2,4)$ |
| 3 | $(3,3),(3,-3),(-3,-3),(-3,3)$ (4 titik unik karena overlap diagonal) |

---

## 6. Implementasi C

```c
// File: src/algo/midcircle.c

/* ─── HELPER: Plot 8 titik simetri sekaligus ─── */
static void PlotSymmetryPoints(int cx, int cy, int x, int y, Color color) {
    DrawPixel(cx + x, cy + y, color); // Oktan 1 (kanan-bawah)
    DrawPixel(cx + y, cy + x, color); // Oktan 2
    DrawPixel(cx + y, cy - x, color); // Oktan 3
    DrawPixel(cx + x, cy - y, color); // Oktan 4 (kanan-atas)
    DrawPixel(cx - x, cy - y, color); // Oktan 5 (kiri-atas)
    DrawPixel(cx - y, cy - x, color); // Oktan 6
    DrawPixel(cx - y, cy + x, color); // Oktan 7
    DrawPixel(cx - x, cy + y, color); // Oktan 8 (kiri-bawah)
}

/* ─── FUNGSI UTAMA: Midpoint Circle ─── */
void Midcircle(int centerX, int centerY, int radius, Color color) {
    // Kasus khusus: radius 0 → hanya satu titik
    if (radius <= 0) {
        DrawPixel(centerX, centerY, color);
        return;
    }

    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;  // Decision parameter awal

    while (y >= x) {  // Iterasi satu oktan (x = 0 hingga x = y)
        PlotSymmetryPoints(centerX, centerY, x, y, color);

        if (d < 0) {
            // Pilih E (East): gerak ke kanan, y tetap
            d = d + 4 * x + 6;
        } else {
            // Pilih SE (South-East): gerak ke kanan-bawah
            d = d + 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}
```

### Catatan Penting Implementasi

> **Mengapa `while (y >= x)` bukan `while (x <= r)`?**  
> Kondisi $y \geq x$ menandai bahwa kita masih di oktan pertama. Ketika $x > y$, kita sudah melewati 45° dan titik-titik mulai duplikat dengan yang sudah diplot.

---

## 7. Variasi Algoritma

### 7.1 Lingkaran Berisi (Filled Circle)

Gambar garis horizontal di setiap baris $y$:

```c
void MidcircleFilled(int cx, int cy, int r, Color color) {
    int x = 0, y = r, d = 3 - 2 * r;
    while (y >= x) {
        // Isi dua baris horizontal untuk simpetri atas-bawah
        for (int i = cx - x; i <= cx + x; i++) {
            DrawPixel(i, cy + y, color);
            DrawPixel(i, cy - y, color);
        }
        for (int i = cx - y; i <= cx + y; i++) {
            DrawPixel(i, cy + x, color);
            DrawPixel(i, cy - x, color);
        }
        if (d < 0) { d += 4*x + 6; }
        else { d += 4*(x-y) + 10; y--; }
        x++;
    }
}
```

### 7.2 Lingkaran Tebal (Thick Circle)

Gambar beberapa lingkaran konsentris:

```c
void MidcircleThick(int cx, int cy, int r, int thickness, Color color) {
    int innerR = r - thickness / 2;
    int outerR = r + thickness / 2;
    for (int r_i = innerR; r_i <= outerR; r_i++) {
        Midcircle(cx, cy, r_i, color);  // Reuse fungsi utama
    }
}
```

---

## 8. Perbandingan Performa

| Metode | Operasi per Lingkaran | Membutuhkan FPU | Kompleksitas |
|---|---|---|---|
| Trigonometri (`sin/cos`) | $\sim 2r\pi$ kali FPU | ✅ Ya | $O(r)$ namun lambat |
| Persamaan ($\sqrt{r^2 - x^2}$) | $\sim r$ kali `sqrt` | ✅ Ya | $O(r)$ sedang |
| **Midpoint / Bresenham** | $\sim r/\sqrt{2}$ kali integer | ❌ Tidak | **$O(r)$ tercepat** |

---

## 9. Latihan Soal

**Soal 1 (Mudah):**  
Gambarkan lingkaran dengan pusat $(0,0)$ dan $r = 3$ menggunakan algoritma Midpoint Circle. Buat tabel iterasi lengkap dengan kolom $x$, $y$, $d$, kondisi lompatan, dan daftar titik yang diplot pada setiap langkah.

**Soal 2 (Menengah):**  
Jelaskan mengapa nilai $d$ diinisialisasi dengan $3 - 2r$ bukan $1.25 - r$. Tunjukkan langkah-langkah penurunannya secara matematis.

**Soal 3 (Tantangan):**  
Modifikasi fungsi `Midcircle` agar hanya menggambar **setengah lingkaran bagian atas** (oktan 1, 2, 7, 8 saja). Tuliskan kode C-nya dan jelaskan modifikasi apa yang diperlukan pada fungsi `PlotSymmetryPoints`.

---

## 📝 Ringkasan

| Konsep | Penjelasan Singkat |
|---|---|
| **Decision parameter** | Nilai integer $d$ yang menentukan arah piksel berikutnya |
| **8-way symmetry** | Satu kalkulasi → 8 piksel, hanya 1/8 lingkaran yang dihitung |
| **$d < 0$ → E** | Titik midpoint di dalam lingkaran, pilih E (kanan) |
| **$d \geq 0$ → SE** | Titik midpoint di luar lingkaran, pilih SE (kanan-bawah) dan $y$ turun |
| **Time complexity** | $O(r)$ — jumlah iterasi linier terhadap radius |
| **Space complexity** | $O(1)$ — hanya membutuhkan variabel lokal `x`, `y`, `d` |

---

**Referensi:**
- Bresenham, J. E. (1977). *"A linear algorithm for incremental digital display of circular arcs"*
- Foley, J. D., et al. (1990). *"Computer Graphics: Principles and Practice"*
- Hearn, D., & Baker, M. P. (2004). *"Computer Graphics with OpenGL"*
