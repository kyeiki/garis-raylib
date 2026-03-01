# Modul 5 — Vesica Piscis: Pendekatan Pergeseran Titik Tengah

**Mata Kuliah:** Grafika Komputer  
**Program:** Program 9  
**Prasyarat:** Modul 4 (Vesica Piscis Statis)  
**Tingkat:** Menengah–Lanjut

---

## 🎯 Tujuan Pembelajaran

Setelah menyelesaikan modul ini, mahasiswa mampu:

1. Membedakan pendekatan **"pangkal ke ujung"** (Modul 4) dengan pendekatan **"pergeseran titik tengah"** (Modul 5).
2. Menjelaskan mengapa segitiga sama sisi terbentuk secara otomatis pada pendekatan Center Shift.
3. Mengimplementasikan fungsi `DrawLens` menggunakan titik tengah lensa sebagai referensi.
4. Menentukan posisi 4 lensa 4 arah mata angin agar ujung-ujungnya bertemu di satu titik.
5. Memilih pendekatan yang paling tepat untuk berbagai kasus pemakaian (*use case*).

---

## 1. Motivasi — Alternatif Cara Pandang

Program 8 berpikir dari titik **pangkal** lalu tumbuh keluar. Tapi kadang kita justru tahu **di mana pusat lensa** harus berada, misalnya:
- Meletakkan lensa di tengah sel grid
- Menempatkan lensa yang dirotasi pada sudut sembarang
- Membuat dua busur berwarna berbeda dalam satu lensa

Program 9 menjawab kebutuhan ini dengan bertanya: *"Di mana pusat lensa, dan seberapa jauh komponen pembentuknya?"*

---

## 2. Konsep Matematika

### 2.1 Titik Tengah Sebagai Referensi

Berbeda dengan Program 8 yang menggunakan **titik pangkal** $(cx, cy)$ sebagai referensi, Program 9 menggunakan **titik tengah lensa** $(m_x, m_y)$.

Dari titik tengah ini, dua pusat lingkaran $C_1$ dan $C_2$ digeser **searah** sumbu pendek lensa (parameter `angle`) sejauh $R/2$:

$$
C_1 = \left(m_x - \frac{R}{2}\cos(\text{angle}),\ m_y - \frac{R}{2}\sin(\text{angle})\right)
$$

$$
C_2 = \left(m_x + \frac{R}{2}\cos(\text{angle}),\ m_y + \frac{R}{2}\sin(\text{angle})\right)
$$

### 2.2 Jarak C1–C2 = R → Segitiga Sama Sisi

Dengan pergeseran di atas, jarak antara $C_1$ dan $C_2$ adalah:

$$
|C_1 - C_2| = \left|\left(-\frac{R}{2}\right) - \left(+\frac{R}{2}\right)\right| = R
$$

Karena jarak $C_1 \to C_2 = R$ dan jari-jari kedua lingkaran = $R$, maka:
- $|C_1 \to \text{ujung}| = R$ (jari-jari)
- $|C_2 \to \text{ujung}| = R$ (jari-jari)
- $|C_1 \to C_2| = R$ (pergeseran)

Ketiganya sama → **segitiga sama sisi** → setiap sudut = 60° → busur yang diperlukan = **120°** (2 × 60°).

### 2.3 Diagram Geometri

```
              Ujung Atas (tip)
                   *
                 ╱   ╲
               ╱       ╲       Semua sisi = R
             ╱    60°    ╲      → Segitiga sama sisi
            C1 ─────────── C2
            ←─ R/2 ─● ─ R/2 ─→
                  (mx, my)     ← Titik tengah lensa
            ╲               ╱
              ╲             ╱
                ╲         ╱
                  ╲     ╱
               Ujung Bawah (tip)
```

### 2.4 Arah `angle` vs Arah Visual Lensa

Ini adalah aspek yang paling mudah membingungkan:

> **`angle`** mendefinisikan arah **sumbu pendek** lensa (arah $C_1 \to C_2$),  
> **bukan** arah sumbu panjang (arah ujung ke ujung).

| `angle` | Arah $C_1$–$C_2$ | Arah long axis (ujung lensa) | Tampilan |
|---|---|---|---|
| $0$ | Horizontal | **Vertikal** | Lensa berdiri tegak |
| $\pi/2$ | Vertikal | **Horizontal** | Lensa berbaring mendatar |
| $\pi/4$ | Diagonal ↗ | Diagonal ↘ | Lensa miring 45° |

---

## 3. Implementasi C

### 3.1 Fungsi `DrawLens`

```c
// File: src/screens/program9.c
// DrawLens: Menggambar satu lensa Vesica Piscis
// (mx, my) = Titik TENGAH lensa (bukan titik pangkal!)
// angle    = Arah sumbu PENDEK lensa (bukan arah panjang)
// col1     = Warna busur dari C1
// col2     = Warna busur dari C2 (bisa berbeda!)

static void DrawLens(int mx, int my, int r,
                     float angle, Color col1, Color col2) {
    float halfR = r * 0.5f;

    /* ─── Geser C1 dan C2 dari titik tengah searah angle ─── */
    float c1x = mx - cosf(angle) * halfR;
    float c1y = my - sinf(angle) * halfR;
    float c2x = mx + cosf(angle) * halfR;
    float c2y = my + sinf(angle) * halfR;

    /* ─── Sudut busur C1: menghadap ke C2 (arah = angle) ─── */
    float start1 = angle - PI / 3.0f;   // angle - 60°
    float end1   = angle + PI / 3.0f;   // angle + 60°

    /* ─── Sudut busur C2: menghadap ke C1 (arah = angle + 180°) ─── */
    float start2 = angle + PI - PI / 3.0f;
    float end2   = angle + PI + PI / 3.0f;

    DrawQuarterArc((int)roundf(c1x), (int)roundf(c1y), r, start1, end1, col1);
    DrawQuarterArc((int)roundf(c2x), (int)roundf(c2y), r, start2, end2, col2);
}
```

### 3.2 Perbandingan Langsung dengan Program 8

| Aspek | Program 8 (`DrawPetal`) | Program 9 (`DrawLens`) |
|---|---|---|
| **Titik referensi input** | Pangkal `(cx, cy)` | Tengah lensa `(mx, my)` |
| **Langkah ekstra** | Hitung PM dulu → baru C1/C2 | Langsung hitung C1/C2 |
| **Arah pergeseran** | Tegak lurus `angleDirection` | Searah `angle` |
| **Warna** | 1 warna per kelopak | 2 warna terpisah per busur |
| **`angle = 0`** | Lensa horizontal (tumbuh ke kanan) | Lensa **vertikal** (C1-C2 horizontal) |

---

## 4. Konfigurasi 4 Lensa — 4 Arah Mata Angin

### 4.1 Menentukan Posisi Titik Tengah Lensa

Agar ujung dalam semua lensa bertemu tepat di `(cx, cy)`, setiap titik tengah lensa harus digeser sejauh $h = R\sqrt{3}/2$ dari pusat:

| Arah | Titik Tengah Lensa | `angle` | Alasan `angle` |
|---|---|---|---|
| **Utara** | $(cx,\ cy - h)$ | $0$ | `angle=0` → $C_1$–$C_2$ horizontal → lensa tegak vertikal |
| **Selatan** | $(cx,\ cy + h)$ | $0$ | Sama, di bawah pusat |
| **Timur** | $(cx + h,\ cy)$ | $\pi/2$ | `angle=π/2` → $C_1$–$C_2$ vertikal → lensa mendatar |
| **Barat** | $(cx - h,\ cy)$ | $\pi/2$ | Sama, di kiri pusat |

### 4.2 Bukti Ujung Bertemu di `(cx, cy)`

Ujung lensa berjarak $h$ dari titik tengah lensa, tegak lurus arah `angle`. Untuk lensa **Utara** (pusat di $(cx, cy-h)$, `angle=0`):

$$
\text{tip bawah} = (cx,\ cy - h + h) = (cx,\ cy) \checkmark
$$

Untuk lensa **Timur** (pusat di $(cx+h, cy)$, `angle=π/2`):

$$
\text{tip kiri} = (cx + h - h,\ cy) = (cx,\ cy) \checkmark
$$

### 4.3 Implementasi `DrawProgram9`

```c
void DrawProgram9(void) {
    int cx = SCREEN_W / 2;
    int cy = SCREEN_H / 2 + 30;
    int R  = 114;
    float h = R * 0.8660254f;   // R × √3/2 ≈ 98.7 piksel

    /* ─── 4 lensa, masing-masing punya titik tengah berbeda ─── */

    // Lensa UTARA: pusat di atas, C1-C2 horizontal (angle=0)
    DrawLens(cx,      (int)(cy - h), R, 0.0f,   RED,    ORANGE);

    // Lensa SELATAN: pusat di bawah, C1-C2 horizontal
    DrawLens(cx,      (int)(cy + h), R, 0.0f,   BLUE,   SKYBLUE);

    // Lensa TIMUR: pusat di kanan, C1-C2 vertikal (angle=π/2)
    DrawLens((int)(cx + h), cy,      R, PI/2.0f, GREEN,  LIME);

    // Lensa BARAT: pusat di kiri, C1-C2 vertikal
    DrawLens((int)(cx - h), cy,      R, PI/2.0f, ORANGE, YELLOW);
}
```

### 4.4 Diagram Layout 4 Lensa

```
                  ● (ujung luar Utara)
                 ╱╲
                ╱  ╲  RED/ORANGE
               ╱    ╲
          (cx, cy-h)       ← pusat lensa Utara
               ╲    ╱
               ╱    ╲
ORANGE ──────(cx,cy)──────── GREEN     ← titik temu semua ujung dalam
               ╲    ╱╲    ╱
  YELLOW        ╲  ╱  ╲  ╱  LIME
                ╲╱    ╱╲╲
           (cx, cy+h)      ← pusat lensa Selatan
               ╱    ╲
              ╱      ╲  BLUE/SKYBLUE
             ╱        ╲
            ● (ujung luar Selatan)
```

---

## 5. Kesetaraan Geometris dengan Program 8

Meskipun cara berpikirnya berbeda, **Program 8 dan Program 9 menghasilkan koordinat $C_1$, $C_2$ yang identik**:

**Program 8** untuk kelopak Utara dari `(cx, cy)`:
```
PM = (cx, cy - h)
C1 = PM + cos(-π/2 - π/2) × R/2 = (cx - R/2, cy - h)
C2 = PM + cos(-π/2 + π/2) × R/2 = (cx + R/2, cy - h)
```

**Program 9** untuk lensa Utara dari `(cx, cy - h)` dengan `angle = 0`:
```
C1 = (cx - R/2·cos(0), cy-h - R/2·sin(0)) = (cx - R/2, cy - h)
C2 = (cx + R/2·cos(0), cy-h + R/2·sin(0)) = (cx + R/2, cy - h)
```

**$C_1$ dan $C_2$ identik →** busur yang digambar pun identik. ✓

---

## 6. Kapan Menggunakan Masing-masing Pendekatan

| Situasi | Gunakan P8 | Gunakan P9 |
|---|---|---|
| **Kelopak bunga** berakar di satu titik | ✅ Natural | Perlu hitung offset |
| **Lensa optis** yang punya pusat presisi | Hitung balik | ✅ Natural |
| **Grid** dengan lensa di setiap sel | Perlu offset manual | ✅ Langsung pakai `(mx, my)` |
| **Rotasi bebas** di sembarang sudut | Hati-hati arah | ✅ Cukup ubah `angle` |
| **Dua warna berbeda** per busur | Perlu duplikasi | ✅ Built-in (`col1`, `col2`) |
| **Animasi morphing** ukuran | Setara | ✅ Cukup ubah `r` |

---

## 7. Proporsi Lensa

Ringkasan proporsi Vesica Piscis untuk $R = 114$ piksel:

| Dimensi | Rumus | Nilai |
|---|---|---|
| Jarak $C_1$–$C_2$ | $R$ | 114 px |
| Tinggi lensa (tip-to-tip) | $R\sqrt{3} \approx 1.732R$ | 197 px |
| Lebar lensa (di tengah) | $R$ | 114 px |
| Rasio tinggi : lebar | $\sqrt{3} : 1$ | $1.732 : 1$ |
| Jarak pusat ke ujung | $R\sqrt{3}/2$ | 98.7 px |
| Luas area irisan | $2R^2(\pi/3 - \sqrt{3}/4)$ | ≈ $1.228R^2$ |

---

## 8. Latihan Soal

**Soal 1 (Mudah):**  
Gambarkan tabel seperti di bagian 4.1 untuk menambahkan 4 lensa baru arah **diagonal** (Timur Laut, Tenggara, Barat Daya, Barat Laut). Tentukan koordinat titik tengah dan nilai `angle` yang dibutuhkan agar ujung dalam tiap lensa tetap bertemu di `(cx, cy)`.

**Soal 2 (Menengah):**  
Tunjukkan dengan kalkulasi matematis bahwa `DrawLens(mx, my, r, angle)` dari Program 9 dan `DrawPetal(cx, cy, r, angle)` dari Program 8 menghasilkan koordinat $C_1$ dan $C_2$ yang **sama persis** untuk kasus lensa Selatan (pusat di $(cx, cy+h)$, `angle=0`). Jelaskan setiap langkah.

**Soal 3 (Tantangan):**  
Program 9 menggunakan dua warna (`col1`, `col2`) untuk membedakan busur dari $C_1$ dan $C_2$ dalam satu lensa. Desain ulang `DrawProgram9` agar warna kedua busur dalam setiap lensa **berubah secara gradien** berdasarkan waktu (`GetTime()`), misalnya berputar melalui roda warna HSV. Tuliskan pseudocode-nya.

---

## 📝 Ringkasan

| Konsep | Penjelasan Singkat |
|---|---|
| **Titik referensi** | Titik **tengah** lensa $(m_x, m_y)$, bukan pangkal |
| **Pergeseran C1, C2** | Searah `angle`, masing-masing $\pm R/2$ dari tengah |
| **$|C_1-C_2| = R$** | Otomatis terbentuk segitiga sama sisi |
| **Busur 120°** | Karena sudut segitiga sama sisi = 60°, busur = 2 × 60° |
| **`angle = 0` → lensa tegak** | $C_1$–$C_2$ horizontal → ujung di atas–bawah |
| **`angle = π/2` → lensa mendatar** | $C_1$–$C_2$ vertikal → ujung di kiri–kanan |
| **Ekuivalen P8** | $C_1, C_2$ identik — hanya titik referensi berbeda |

---

## 📊 Perbandingan Semua Program (Ringkasan Modul 3–5)

| | **P7 (Animasi)** | **P8 (Statis P-to-T)** | **P9 (Center Shift)** |
|---|---|---|---|
| **Referensi** | Pusat lingkaran | Pangkal kelopak | Tengah lensa |
| **Animasi** | ✅ Ya | ❌ | ❌ |
| **Artefak** | Ada | ✅ Tidak ada | ✅ Tidak ada |
| **2 warna per lensa** | ❌ | ❌ | ✅ |
| **Fleksibel rotasi** | Sedang | Sedang | ✅ Tinggi |
| **Pola bunga (radial)** | Bisa | ✅ Natural | Bisa |
| **Pola grid** | Bisa | Bisa | ✅ Natural |

---

**Referensi:**
- Bresenham, J. E. (1977). *"A linear algorithm for incremental digital display of circular arcs"*
- Weisstein, E. W. *"Vesica Piscis"*. MathWorld — A Wolfram Web Resource
- Lawlor, R. (1982). *"Sacred Geometry: Philosophy and Practice"*
