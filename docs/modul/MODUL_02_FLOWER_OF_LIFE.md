# Modul 2 — Flower of Life

**Mata Kuliah:** Grafika Komputer  
**Program:** Program 6  
**Prasyarat:** Modul 1 (Algoritma Midpoint Circle)  
**Tingkat:** Pemula–Menengah

---

## 🎯 Tujuan Pembelajaran

Setelah menyelesaikan modul ini, mahasiswa mampu:

1. Menjelaskan struktur geometri dasar pola Flower of Life.
2. Menghitung koordinat 6 kelopak utama menggunakan konstanta trigonometri.
3. Menerapkan fungsi `Midcircle` dari Modul 1 untuk menggambar pola multi-lingkaran.
4. Melakukan konversi koordinat **kartesian → layar piksel**.
5. Memahami hubungan antara Seed of Life, Flower of Life, dan Fruit of Life.

---

## 1. Pengantar — Geometri Sakral

**Flower of Life** adalah pola geometri tertua di dunia, ditemukan di:
- **Mesir Kuno** (~10.500 SM) di Kuil Osiris, Abydos
- **Yunani Kuno** pada karya Pythagoras tentang rasio √3
- **Arsitektur Gotik** abad pertengahan sebagai motif dekoratif

Dari sudut pandang matematika, Flower of Life menarik karena ia dibangun dari **prinsip geometri yang sangat sederhana**: semua lingkaran memiliki **jari-jari yang sama** dan **jarak antar pusat = jari-jari**.

---

## 2. Geometri Dasar

### 2.1 Aturan Emas: Jarak Pusat = Jari-jari

Kunci dari seluruh pola ini adalah satu kondisi:

$$
\text{Jarak}(C_\text{pusat},\ C_\text{kelopak}) = R
$$

Di mana $R$ adalah jari-jari lingkaran. Artinya, **pusat setiap kelopak berada tepat di tepi lingkaran pusat**.

```
               C_utara
                 ●        ← Jarak dari C_pusat = R
                / \
        R     /   \  R
             /     \
C_barat ●──────●──────● C_timur
             \     /
        R     \   /  R
                \ /
                 ●
              C_selatan
```

Karena semua jarak = $R$, keenam kelopak membentuk **hexagon sempurna** mengelilingi lingkaran pusat.

### 2.2 Enam Posisi Kelopak

Pusat keenam kelopak terletak pada sudut-sudut hexagon di sekeliling lingkaran pusat:

| Kelopak | Sudut | Offset X | Offset Y |
|---|---|---|---|
| Utara | 90° | $0$ | $-R$ |
| Timur Laut | 30° | $+R \cos 30° = +R \cdot \frac{\sqrt{3}}{2}$ | $-R \sin 30° = -\frac{R}{2}$ |
| Tenggara | 330° | $+R \cos 30°$ | $+\frac{R}{2}$ |
| Selatan | 270° | $0$ | $+R$ |
| Barat Daya | 210° | $-R \cos 30°$ | $+\frac{R}{2}$ |
| Barat Laut | 150° | $-R \cos 30°$ | $-\frac{R}{2}$ |

### 2.3 Konstanta Trigonometri

Cukup dua konstanta yang perlu diingat:

$$
\cos 30° = \frac{\sqrt{3}}{2} \approx 0.866025
$$

$$
\sin 30° = \frac{1}{2} = 0.5
$$

Untuk komputer, kita sederhanakan:
```c
int dx = (int)(R * 0.866025f); // offset horizontal diagonal
int dy = R / 2;                 // offset vertikal diagonal
```

---

## 3. Konversi Koordinat: Kartesian → Layar

Sistem koordinat kartesian (matematika) dan sistem koordinat layar berbeda dalam arah sumbu Y:

| Sumbu | Kartesian | Layar |
|---|---|---|
| X | positif ke kanan | positif ke kanan |
| Y | positif ke **atas** | positif ke **bawah** |

Rumus konversi:

$$
\text{screen}_x = \text{originX} + \text{cart}_x \times \text{tickStep}
$$

$$
\text{screen}_y = \text{originY} - \text{cart}_y \times \text{tickStep}
$$

Pada Program 6:
- `originX = 500`, `originY = 380` (pusat layar)
- `tickStep = 38` piksel per satuan kartesian
- `R = 114` piksel = 3 satuan kartesian

---

## 4. Algoritma

### Langkah-langkah Menggambar

```
1. Tentukan (cx, cy) = pusat layar dan R = radius (piksel)
2. Gambar lingkaran pusat: Midcircle(cx, cy, R)
3. Gambar 4 kelopak sumbu kartesian:
   - Utara   : Midcircle(cx, cy - R, R)
   - Selatan : Midcircle(cx, cy + R, R)
   - Timur   : Midcircle(cx + R, cy, R)
   - Barat   : Midcircle(cx - R, cy, R)
4. Hitung dx = R × 0.866, dy = R / 2
5. Gambar 4 kelopak diagonal:
   - Timur Laut : Midcircle(cx + dx, cy - dy, R)
   - Tenggara   : Midcircle(cx + dx, cy + dy, R)
   - Barat Daya : Midcircle(cx - dx, cy + dy, R)
   - Barat Laut : Midcircle(cx - dx, cy - dy, R)
```

---

## 5. Implementasi C

```c
// File: src/screens/program6.c

void DrawProgram6(void) {
    /* ─── Konfigurasi dasar ─── */
    int cx = SCREEN_W / 2;       // Pusat layar X
    int cy = SCREEN_H / 2 + 30;  // Pusat layar Y (sedikit di bawah tengah)
    int R  = 114;                 // Radius = 3 satuan kartesian (3 × 38)

    /* ─── Offset diagonal: cos(30°) × R dan sin(30°) × R ─── */
    int dx = (int)(R * 0.866025f); // ≈ 99 piksel
    int dy = R / 2;                // = 57 piksel

    /* ─── 1. Lingkaran pusat ─── */
    Midcircle(cx, cy, R, RAYWHITE);

    /* ─── 2. Kelopak 4 arah mata angin ─── */
    Midcircle(cx,      cy - R, R, RED);    // Utara
    Midcircle(cx,      cy + R, R, BLUE);   // Selatan
    Midcircle(cx + R,  cy,     R, GREEN);  // Timur
    Midcircle(cx - R,  cy,     R, ORANGE); // Barat

    /* ─── 3. Kelopak 4 arah diagonal ─── */
    Midcircle(cx + dx, cy - dy, R, YELLOW);   // Timur Laut (NE)
    Midcircle(cx + dx, cy + dy, R, MAGENTA);  // Tenggara   (SE)
    Midcircle(cx - dx, cy + dy, R, YELLOW);   // Barat Daya (SW)
    Midcircle(cx - dx, cy - dy, R, MAGENTA);  // Barat Laut (NW)
}
```

### Hubungan dengan Modul 1

Seluruh fungsi `Midcircle()` yang digunakan adalah **fungsi yang sama** dari Modul 1. Program 6 hanya memanggil fungsi tersebut 9 kali dengan pusat berbeda — ini menunjukkan pentingnya membuat **fungsi yang dapat digunakan ulang (reusable)**.

---

## 6. Tabel Koordinat Semua 9 Lingkaran

Dengan $R = 114$, $dx = 99$, $dy = 57$ (dalam piksel layar, pusat $(500, 380)$):

| No | Nama | $C_x$ (piksel) | $C_y$ (piksel) | Warna |
|---|---|---|---|---|
| 1 | Pusat | 500 | 380 | Putih |
| 2 | Utara | 500 | 266 | Merah |
| 3 | Selatan | 500 | 494 | Biru |
| 4 | Timur | 614 | 380 | Hijau |
| 5 | Barat | 386 | 380 | Oranye |
| 6 | Timur Laut | 599 | 323 | Kuning |
| 7 | Tenggara | 599 | 437 | Magenta |
| 8 | Barat Daya | 401 | 437 | Kuning |
| 9 | Barat Laut | 401 | 323 | Magenta |

---

## 7. Hierarki Pola Geometri Sakral

Flower of Life merupakan bagian dari keluarga pola yang berhubungan:

```
Seed of Life (7 lingkaran)
        ↓ + 12 lingkaran tambahan
Flower of Life (19 lingkaran)
        ↓ pilih 13 lingkaran terseleksi
Fruit of Life (13 lingkaran)
        ↓ hubungkan dengan garis
Metatron's Cube
```

| Pola | Jumlah Lingkaran | Dipelajari di |
|---|---|---|
| **Seed of Life** | 7 | Program 6 (1 pusat + 6 kelopak) |
| Flower of Life | 19+ | Ekspansi Program 6 |
| Fruit of Life | 13 | — |
| Metatron's Cube | 13 + garis | — |

> Program 6 hanya menggambar **Seed of Life** yang diperluas (9 lingkaran). Untuk mendapat Flower of Life penuh (19 lingkaran), tambahkan lingkaran di posisi $2R$ dari pusat.

---

## 8. Kompleksitas

| Aspek | Nilai |
|---|---|
| **Jumlah pemanggilan** `Midcircle` | 9 kali |
| **Time complexity per lingkaran** | $O(R)$ |
| **Time complexity total** | $O(9R) = O(R)$ |
| **Space complexity** | $O(1)$ — hanya variabel lokal |

---

## 9. Latihan Soal

**Soal 1 (Mudah):**  
Dengan $R = 80$ piksel dan pusat layar di $(400, 300)$, hitung koordinat piksel $(C_x, C_y)$ untuk keenam kelopak utama (Utara, Selatan, Timur, Barat, Timur Laut, Barat Daya). Tunjukkan perhitungan untuk setiap arah.

**Soal 2 (Menengah):**  
Program 6 menggunakan `int dy = R / 2` untuk offset diagonal (pembagian integer). Jelaskan apakah ini menimbulkan kesalahan pembulatan yang signifikan, dan kapan kesalahan ini berdampak pada tampilan visual. Bandingkan dengan menggunakan `(int)(R * 0.5f)`.

**Soal 3 (Tantangan):**  
Modifikasi kode Program 6 agar pola Flower of Life **berputar perlahan** menggunakan parameter waktu dari Raylib (`GetTime()`). Semua 9 lingkaran harus berputar mengelilingi pusat dengan kecepatan 30° per detik. Tuliskan pseudocode-nya.

---

## 📝 Ringkasan

| Konsep | Penjelasan Singkat |
|---|---|
| **Aturan dasar** | Jarak antar pusat lingkaran = jari-jari ($R$) |
| **6 kelopak** | Terletak di sudut-sudut hexagon reguler |
| **cos 30° ≈ 0.866** | Digunakan untuk offset diagonal (arang X) |
| **sin 30° = 0.5** | Digunakan untuk offset diagonal (arah Y) |
| **Reusability** | Fungsi `Midcircle()` dari Modul 1 cukup dipanggil 9× |
| **Konversi Y** | Layar Y turun ke bawah ↔ Kartesian Y naik ke atas |

---

**Referensi:**
- Melchizedek, D. (1999). *"The Ancient Secret of the Flower of Life"*
- Lawlor, R. (1982). *"Sacred Geometry: Philosophy and Practice"*
- Schneider, M. S. (1994). *"A Beginner's Guide to Constructing the Universe"*
