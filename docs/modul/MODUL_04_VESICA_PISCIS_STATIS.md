# Modul 4 — Vesica Piscis Statis (Pendekatan Pangkal ke Ujung)

**Mata Kuliah:** Grafika Komputer  
**Program:** Program 8  
**Prasyarat:** Modul 3 (Animasi Vesica Piscis)  
**Tingkat:** Menengah

---

## 🎯 Tujuan Pembelajaran

Setelah menyelesaikan modul ini, mahasiswa mampu:

1. Mengidentifikasi **masalah artefak visual** pada pendekatan Program 7 dan menjelaskan penyebabnya.
2. Menerapkan pendekatan **"pangkal ke ujung"** (*Base-to-Tip*) untuk menggambar kelopak Vesica Piscis.
3. Menghitung posisi pusat lingkaran pembentuk ($C_1$, $C_2$) menggunakan trigonometri dari titik pangkal.
4. Mengimplementasikan fungsi `DrawPetal` yang bersih tanpa artefak.
5. Menyusun 4 kelopak yang bertemu tepat di satu titik pusat.

---

## 1. Masalah di Program 7 — Artefak Lingkaran Tengah

Pada Program 7, kita menggambar dua lingkaran penuh lalu memotong busurnya. Ini menghasilkan **masalah visual**: ketika dua pasang lingkaran berbeda diletakkan dengan salah satu pusatnya di titik yang sama, **lingkaran-lingkaran itu saling menimpa** dan menghasilkan garis tambahan di titik tengah.

```
Program 7: Dua pasang (Utara-Selatan):

    C_utara (pusat lingkaran utara)
       ●───── Vesica Utara
   (cx, cy) ← TITIK INI adalah pusat lingkaran
       ●───── Vesica Selatan
    C_selatan

Masalah: busur dari C_utara dan C_selatan sama-sama
         melewati (cx, cy) dan menghasilkan piksel ganda.
```

**Program 8 memperbaiki ini** dengan pendekatan yang berbeda: alih-alih satu titik pusat bersama, ia menghitung posisi pusat tiap pasang lingkaran secara presisi dari **titik pangkal kelopak**.

---

## 2. Pendekatan "Pangkal ke Ujung" — Konsep

### 2.1 Cara Berpikir

Program 8 bertanya: *"Di mana kelopak ini berakar, dan ke mana arahnya tumbuh?"*

```
Langkah konstruksi:

(cx, cy) = Titik pangkal (tempat kelopak berakar)
     ↓
Hitung PM = Titik tengah kelopak
  PM = (cx, cy) + h × arah_kelopak
  (di mana h = R × √3/2 = tinggi setengah kelopak)
     ↓
Dari PM, geser ±R/2 ke arah TEGAK LURUS arah kelopak
  → Dapat C1 dan C2
     ↓
Gambar busur 120° dari C1 dan C2
```

### 2.2 Mengapa Tegak Lurus?

Dalam geometri Vesica Piscis, garis yang menghubungkan $C_1–C_2$ selalu **tegak lurus** terhadap sumbu panjang (arah tumbuh) kelopak. Karena kelopak tumbuh ke arah `angleDirection`, maka $C_1$ dan $C_2$ digeser ke arah `angleDirection ± 90°`.

### 2.3 Diagram Geometri

```
                    Ujung kelopak (tip)
                          ▲
                         / \
                        /   \
                       / 60° \
                      /       \
                    C1────PM────C2    ← PM = Titik tengah kelopak
                      \       /        C1, C2 digeser ±R/2 dari PM
                       \     /         tegak lurus arah kelopak
                        \   /
                         \ /
               (cx, cy) = ● = Titik pangkal (akar kelopak)
```

---

## 3. Penurunan Rumus

### 3.1 Hitung Titik Tengah Kelopak (PM)

Titik pangkal adalah `(cx, cy)` dan kelopak tumbuh ke arah `angleDirection`:

$$
PM_x = cx + \cos(\text{angle}) \times h
$$
$$
PM_y = cy + \sin(\text{angle}) \times h
$$

Di mana:

$$
h = R \times \frac{\sqrt{3}}{2} \approx R \times 0.8660254
$$

> **Perhatian koordinat layar:** Sumbu Y layar positif ke bawah. Fungsi trigonometri `sinf`/`cosf` menggunakan sistem standar matematika, tetapi karena kita bekerja di koordinat layar, arah "atas" (sudut $-\pi/2$) akan menggerakkan ke atas layar.

### 3.2 Hitung Pusat C1 dan C2

$C_1$ dan $C_2$ digeser dari PM ke arah tegak lurus ($\pm 90°$ dari `angleDirection`):

$$
C_1 = \left(PM_x + \cos\left(\text{angle} - \frac{\pi}{2}\right) \cdot \frac{R}{2},\ PM_y + \sin\left(\text{angle} - \frac{\pi}{2}\right) \cdot \frac{R}{2}\right)
$$

$$
C_2 = \left(PM_x + \cos\left(\text{angle} + \frac{\pi}{2}\right) \cdot \frac{R}{2},\ PM_y + \sin\left(\text{angle} + \frac{\pi}{2}\right) \cdot \frac{R}{2}\right)
$$

Perhatikan bahwa $\cos(\theta + 90°) = -\sin(\theta)$ dan $\sin(\theta + 90°) = \cos(\theta)$, sehingga:

$$
C_1 = (PM_x + \sin(\text{angle}) \cdot R/2,\ PM_y - \cos(\text{angle}) \cdot R/2)
$$
$$
C_2 = (PM_x - \sin(\text{angle}) \cdot R/2,\ PM_y + \cos(\text{angle}) \cdot R/2)
$$

### 3.3 Hitung Sudut Busur

Busur dari $C_1$ menghadap ke $C_2$ (maka sudutnya dari arah $C_1 \to C_2$):

$$
\text{dirC1toC2} = \text{angleDirection} + 90°
$$
$$
\text{start}_1 = \text{dirC1toC2} - 60°,\quad \text{end}_1 = \text{dirC1toC2} + 60°
$$

Busur dari $C_2$ menghadap ke $C_1$ (berlawanan arah):

$$
\text{dirC2toC1} = \text{angleDirection} - 90°
$$
$$
\text{start}_2 = \text{dirC2toC1} - 60°,\quad \text{end}_2 = \text{dirC2toC1} + 60°
$$

---

## 4. Implementasi C

### 4.1 Fungsi `DrawPetal`

```c
// File: src/screens/program8.c

static void DrawPetal(int cx, int cy, int r, float angleDirection, Color col) {
    /* ─── Langkah 1: Hitung titik tengah kelopak (PM) ─── */
    float h    = r * 0.8660254f; // R × √3/2 = jarak pangkal ke PM
    float halfR = r * 0.5f;      // R/2      = jarak PM ke C1/C2

    float pmX = cx + cosf(angleDirection) * h;
    float pmY = cy + sinf(angleDirection) * h;

    /* ─── Langkah 2: Geser ke kiri dan kanan (tegak lurus) ─── */
    float c1X = pmX + cosf(angleDirection - PI/2.0f) * halfR;
    float c1Y = pmY + sinf(angleDirection - PI/2.0f) * halfR;

    float c2X = pmX + cosf(angleDirection + PI/2.0f) * halfR;
    float c2Y = pmY + sinf(angleDirection + PI/2.0f) * halfR;

    /* ─── Langkah 3: Hitung rentang busur ─── */
    // Busur C1: menghadap ke arah C2 (tegak lurus + 90° dari arah kelopak)
    float dirC1toC2 = angleDirection + PI/2.0f;
    float start1    = dirC1toC2 - PI/3.0f;  // -60°
    float end1      = dirC1toC2 + PI/3.0f;  // +60°

    // Busur C2: menghadap ke arah C1 (tegak lurus - 90° dari arah kelopak)
    float dirC2toC1 = angleDirection - PI/2.0f;
    float start2    = dirC2toC1 - PI/3.0f;
    float end2      = dirC2toC1 + PI/3.0f;

    /* ─── Langkah 4: Gambar kedua busur ─── */
    DrawArcFromAngle((int)roundf(c1X), (int)roundf(c1Y), r, start1, end1, col);
    DrawArcFromAngle((int)roundf(c2X), (int)roundf(c2Y), r, start2, end2, col);
}
```

### 4.2 Pemanggilan 4 Kelopak

```c
void DrawProgram8(void) {
    int cx = SCREEN_W / 2;
    int cy = SCREEN_H / 2 + 30;
    int R = 114;

    // Semua kelopak berpangkal di titik yang SAMA (cx, cy)
    DrawPetal(cx, cy, R, -PI/2.0f, RED);    // Menunjuk ke Utara
    DrawPetal(cx, cy, R,  PI/2.0f, BLUE);   // Menunjuk ke Selatan
    DrawPetal(cx, cy, R,  0.0f,    GREEN);  // Menunjuk ke Timur
    DrawPetal(cx, cy, R,  PI,      ORANGE); // Menunjuk ke Barat
}
```

### 4.3 Tabel Arah Kelopak

| Kelopak | `angleDirection` | Visual |
|---|---|---|
| Utara | $-\pi/2$ | Tumbuh ke atas (Y layar turun) |
| Selatan | $+\pi/2$ | Tumbuh ke bawah |
| Timur | $0$ | Tumbuh ke kanan |
| Barat | $\pi$ | Tumbuh ke kiri |

> **Catatan:** Karena Y layar positif ke bawah, sudut $-\pi/2$ adalah arah "atas" pada layar.

---

## 5. Perbandingan Program 7 vs Program 8

| Aspek | Program 7 | Program 8 |
|---|---|---|
| **Artefak visual** | Ada lingkaran berlebih di titik temu | ✅ Tidak ada |
| **Titik temu kelopak** | Tidak presisi | ✅ Tepat di satu titik |
| **Animasi** | ✅ Ya | Tidak (statis) |
| **Kompleksitas kode** | Lebih sederhana | Sedikit lebih kompleks |
| **Titik referensi** | Pusat masing-masing lingkaran | Titik pangkal semua kelopak |

---

## 6. Kompleksitas

| Fungsi | Time Complexity |
|---|---|
| `DrawArcFromAngle` | $O(R)$ |
| `DrawPetal` | $O(2R)$ |
| `DrawProgram8` | $O(8R)$ — 4 kelopak × 2 busur |

**Space:** $O(1)$ — hanya variabel lokal.

---

## 7. Latihan Soal

**Soal 1 (Mudah):**  
Untuk kelopak yang tumbuh ke arah **Timur Laut** ($\text{angle} = -\pi/4$) dengan $R = 100$ dan pangkal di $(300, 300)$:
  - Hitung koordinat titik tengah kelopak PM.
  - Hitung koordinat $C_1$ dan $C_2$.
  - Tentukan rentang sudut busur dari $C_1$ dan $C_2$.

**Soal 2 (Menengah):**  
Jelaskan mengapa `DrawPetal` menggunakan `roundf()` saat mengkonversi koordinat titik pusat ($C_1$, $C_2$) dari `float` ke `int`. Apa yang terjadi secara visual jika kita menggunakan `(int)` biasa (truncation) sebagai gantinya?

**Soal 3 (Tantangan):**  
Buat fungsi `DrawPetalAnim(int cx, int cy, int r, float angleDirection, float progress, Color col)` yang menggabungkan konsep animasi dari Program 7 ke dalam `DrawPetal` Program 8, sehingga kelopak tumbuh dari titik pangkal (`progress = 0`) hingga bentuk penuh (`progress = 1`).

---

## 📝 Ringkasan

| Konsep | Penjelasan Singkat |
|---|---|
| **Masalah P7** | Artefak piksel di titik temu karena busur lingkaran saling menimpa |
| **Solusi P8** | Hitung $C_1, C_2$ dari titik pangkal → tidak ada tumpang tindih |
| **PM** | Titik tengah kelopak: $\text{PM} = \text{pangkal} + h \cdot \hat{\text{angle}}$ |
| **$h = R\sqrt{3}/2$** | Jarak dari pangkal ke pusat kelopak |
| **Pergeseran tegak lurus** | $C_1, C_2$ digeser $\pm R/2$ tegak lurus arah kelopak |
| **Sudut busur** | Masing-masing 120° ($\pm 60°$ dari arah $C_1 \leftrightarrow C_2$) |

---

**Referensi:**
- Bresenham, J. E. (1977). *"A linear algorithm for incremental digital display of circular arcs"*
- Lawlor, R. (1982). *"Sacred Geometry: Philosophy and Practice"*
- Lundy, M. (2001). *"Sacred Geometry"*
