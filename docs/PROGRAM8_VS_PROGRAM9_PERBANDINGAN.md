# Perbandingan Program 8 vs Program 9 — Dua Pendekatan Vesica Piscis

Kedua program menghasilkan bentuk **Vesica Piscis** yang serupa secara visual, namun menggunakan pendekatan konstruksi yang **berbeda secara fundamental**. Memahami perbedaan ini penting untuk menguasai teknik pemodelan geometri pada grafika komputer.

---

## Ringkasan Perbedaan

| Aspek | Program 8 | Program 9 |
|---|---|---|
| **Nama pendekatan** | *Pangkal ke Ujung* (Base-to-Tip) | *Pergeseran Titik Jari-Jari* (Center Shift) |
| **Titik referensi utama** | Titik pangkal kelopak `(cx, cy)` | Titik tengah lensa `(mx, my)` |
| **Cara mencari C1, C2** | Dari titik tengah kelopak `PM`, geser tegak lurus ±r/2 | Dari titik tengah lensa, geser searah angle ±r/2 |
| **Arah pergeseran C1/C2** | **Tegak lurus** terhadap arah kelopak | **Searah** sumbu pendek lensa (angle) |
| **Orientasi visual** | Kelopak berpangkal di `(cx, cy)`, tumbuh keluar | Lensa simetris dengan pusat di `(mx, my)` |
| **Fungsi gambar busur** | `DrawArcFromAngle()` | `DrawQuarterArc()` (nama berbeda, logika sama) |
| **Jumlah fungsi helper** | 2 (`DrawArcFromAngle`, `DrawPetal`) | 2 (`DrawQuarterArc`, `DrawLens`) |
| **Animasi** | Tidak | Tidak |
| **Output** | 4 kelopak berbeda warna | 4 lensa berbeda warna |

---

## Perbedaan Cara Berpikir

### Program 8 — "Dari Pangkal, Tumbuh ke Ujung"

Program 8 bertanya: *"Di mana kelopak ini berakar, dan ke mana ia tumbuh?"*

```
Titik pangkal (cx, cy) sudah diketahui.
↓
Hitung titik tengah kelopak PM:
  PM = (cx, cy) + r·sqrt(3)/2 × arah_kelopak
↓
Dari PM, geser TEGAK LURUS ±r/2 untuk mendapat C1 dan C2
↓
Gambar busur dari C1 dan C2 yang saling berpotongan
```

Analoginya seperti **daun yang tumbuh dari batang** — kamu tahu di mana daunnya menempel, lalu menggambar bentuknya keluar.

### Program 9 — "Dari Tengah, Geser Keluar"

Program 9 bertanya: *"Di mana pusat lensa, dan seberapa jauh lingkaran-lingkaran pembentuknya?"*

```
Titik tengah lensa (mx, my) sudah diketahui.
↓
Geser C1 = (mx, my) − r/2 × arah_sumbu_pendek
Geser C2 = (mx, my) + r/2 × arah_sumbu_pendek
(jarak C1–C2 otomatis = r → segitiga sama sisi)
↓
Gambar busur 120° dari C1 dan C2 saling berhadapan
```

Analoginya seperti **lensa optik yang dipasang** — kamu tahu di mana pusatnya, lalu cari komponen pembentuknya.

---

## Perbandingan Kode Inti

### Program 8 — Fungsi DrawPetal

```c
static void DrawPetal(int cx, int cy, int r, float angleDirection, Color col) {
    // Titik referensi = PANGKAL (cx, cy)
    float h    = r * 0.8660254f;  // r * sqrt(3)/2
    float halfR = r * 0.5f;

    // Hitung titik tengah kelopak (PM) dari pangkal
    float pmX = cx + cosf(angleDirection) * h;
    float pmY = cy + sinf(angleDirection) * h;

    // Geser TEGAK LURUS terhadap arah kelopak (±90°)
    float c1X = pmX + cosf(angleDirection - PI/2.0f) * halfR;
    float c1Y = pmY + sinf(angleDirection - PI/2.0f) * halfR;
    float c2X = pmX + cosf(angleDirection + PI/2.0f) * halfR;
    float c2Y = pmY + sinf(angleDirection + PI/2.0f) * halfR;

    // Sudut busur dihitung dari arah C1→C2 dan C2→C1
    float dirC1toC2 = angleDirection + PI/2.0f;
    float start1 = dirC1toC2 - PI/3.0f;
    float end1   = dirC1toC2 + PI/3.0f;
    float dirC2toC1 = angleDirection - PI/2.0f;
    float start2 = dirC2toC1 - PI/3.0f;
    float end2   = dirC2toC1 + PI/3.0f;

    DrawArcFromAngle((int)roundf(c1X), (int)roundf(c1Y), r, start1, end1, col);
    DrawArcFromAngle((int)roundf(c2X), (int)roundf(c2Y), r, start2, end2, col);
}
```

### Program 9 — Fungsi DrawLens

```c
static void DrawLens(int mx, int my, int r, float angle, Color col1, Color col2) {
    // Titik referensi = TENGAH LENSA (mx, my)
    float halfR = r * 0.5f;

    // Geser SEARAH angle (bukan tegak lurus)
    float c1x = mx - cosf(angle) * halfR;
    float c1y = my - sinf(angle) * halfR;
    float c2x = mx + cosf(angle) * halfR;
    float c2y = my + sinf(angle) * halfR;

    // Sudut busur langsung dari angle (lebih sederhana)
    float start1 = angle - PI / 3.0f;
    float end1   = angle + PI / 3.0f;
    float start2 = angle + PI - PI / 3.0f;
    float end2   = angle + PI + PI / 3.0f;

    DrawQuarterArc((int)roundf(c1x), (int)roundf(c1y), r, start1, end1, col1);
    DrawQuarterArc((int)roundf(c2x), (int)roundf(c2y), r, start2, end2, col2);
}
```

### Perbedaan Kunci pada Kode

| Aspek | Program 8 (`DrawPetal`) | Program 9 (`DrawLens`) |
|---|---|---|
| **Input posisi** | Titik pangkal `(cx, cy)` | Titik tengah `(mx, my)` |
| **Langkah ekstra** | Hitung PM dulu, baru C1/C2 | Langsung hitung C1/C2 |
| **Arah pergeseran C1** | `angleDirection ± π/2` (tegak lurus) | `angle` (searah) |
| **Arah pergeseran C2** | `angleDirection ∓ π/2` (tegak lurus) | `-angle` (berlawanan) |
| **Formula sudut busur** | Dari `dirC1toC2` dan `dirC2toC1` | Langsung dari `angle` |
| **Warna** | 1 warna per kelopak | 2 warna terpisah (col1, col2) |

---

## Perbedaan Posisi Titik Referensi

Ini adalah perbedaan yang paling mempengaruhi cara *memanggil* fungsi:

### Memanggil dari Layout 4 Arah Mata Angin:

**Program 8** — Semua kelopak pangkalnya di SATU TITIK yang sama:
```c
int cx = SCREEN_W / 2, cy = SCREEN_H / 2 + 30;
int R = 114;

// Semua kelopak berpangkal di (cx, cy)
DrawPetal(cx, cy, R, -PI/2.0f, RED);    // Utara
DrawPetal(cx, cy, R,  PI/2.0f, BLUE);   // Selatan
DrawPetal(cx, cy, R,  0.0f,    GREEN);  // Timur
DrawPetal(cx, cy, R,  PI,      ORANGE); // Barat
//                    ^-- arah kelopak tumbuh
```

**Program 9** — Setiap lensa punya pusat yang BERBEDA, digeser sejauh h:
```c
int cx = SCREEN_W / 2, cy = SCREEN_H / 2 + 30;
int R = 114;
float h = R * 0.8660254f;  // R × √3/2

DrawLens(cx,      cy - h, R, 0.0f,    col_red,    col_orange); // Utara: pusat di atas
DrawLens(cx,      cy + h, R, 0.0f,    col_blue,   col_cyan);   // Selatan: pusat di bawah
DrawLens(cx + h,  cy,     R, PI/2.0f, col_green,  col_lime);   // Timur: pusat di kanan
DrawLens(cx - h,  cy,     R, PI/2.0f, col_orange, col_yellow); // Barat: pusat di kiri
//       ^---titik tengah lensa, bukan pangkal        ^-- sumbu pendek (bukan arah tumbuh)
```

---

## Kesetaraan Geometris (Equivalence Proof)

Meskipun cara berpikirnya berbeda, **hasil geometrisnya ekuivalen**. Buktinya:

Misalkan Program 8 menggambar kelopak ke arah Utara dari `(cx, cy)`:

```
PM (titik tengah kelopak) = (cx, cy − h)    [h = R·√3/2]
C1 = PM + R/2 · cos(−PI/2 − PI/2) = PM + R/2 · cos(-PI) = (cx − R/2, cy − h)
C2 = PM + R/2 · cos(−PI/2 + PI/2) = PM + R/2 · cos( 0 ) = (cx + R/2, cy − h)
```

Program 9 menggambar lensa Utara dari pusat `(cx, cy − h)` dengan angle=0:
```
C1 = (cx − R/2·cos(0), cy−h − R/2·sin(0)) = (cx − R/2, cy − h)
C2 = (cx + R/2·cos(0), cy−h + R/2·sin(0)) = (cx + R/2, cy − h)
```

**C1 dan C2 identik** → busur yang digambar pun identik. ✓

---

## Kapan Menggunakan Masing-masing?

| Situasi | Gunakan Program 8 | Gunakan Program 9 |
|---|---|---|
| **Kelopak bunga** yang bertemu di satu pangkal | ✅ Lebih natural | Bisa, tapi perlu offset manual |
| **Lensa optik / mata** yang perlu pusat presisi | Bisa, tapi perlu hitung balik | ✅ Lebih natural |
| **Menyusun pola melingkar** (seperti Flower of Life) | ✅ Pangkal langsung di tepi lingkaran | Lebih rumit |
| **Merotasi lensa arbitrarily** | Perlu berhati-hati dengan arah | ✅ Langsung pakai parameter `angle` |
| **Grid/layout** based (tiap sel punya lensa di tengahnya) | Perlu hitung offset | ✅ Langsung pakai `(mx, my)` |
| **Animasi morphing** ukuran (zoom lens) | Setara | ✅ Mudah, cukup ubah `r` |
| **2 warna berbeda** per busur dalam satu lensa | Perlu duplikasi fungsi | ✅ Sudah built-in (`col1`, `col2`) |

---

## Ringkasan Visual Perbedaan Arah Pergeseran

```
PROGRAM 8 — Pergeseran TEGAK LURUS arah kelopak:

  Arah kelopak = ke ATAS (angle = -PI/2)

  C1 ←─── PM ───→ C2      (C1 dan C2 digeser HORIZONTAL = tegak lurus ke atas)
         ↑
       (cx,cy)

─────────────────────────────────────────────────────────────────

PROGRAM 9 — Pergeseran SEARAH angle:

  angle = 0 (horizontal) untuk lensa Utara-Selatan

  C1 ←─────────────────→ C2   (C1 dan C2 digeser HORIZONTAL = searah angle=0)
              ↑
            (mx, my)  ← titik tengah lensa, bukan pangkal
```

---

## Kesimpulan

| | Program 8 | Program 9 |
|---|---|---|
| **Kekuatan** | Alami untuk pola bunga, pola sakral berbasis pangkal | Fleksibel untuk grid, lensa optis, rotasi bebas |
| **Kelemahan** | Perlu hitung posisi tengah lensa secara manual jika dibutuhkan | Arah `long axis` tegak lurus `angle` — bisa membingungkan |
| **Abstraksi** | *"Kelopak tumbuh dari titik ini"* | *"Lensa berpusat di sini, dibangun dari dua lingkaran menggeser"* |
| **Kompleksitas kode** | Sedikit lebih banyak (langkah PM) | Lebih ringkas (langsung ke C1/C2) |

Keduanya adalah pendekatan yang valid dan saling melengkapi — Program 8 lebih intuitif untuk pola radikal berbasis pangkal, sedangkan Program 9 lebih fleksibel untuk penempatan lensa di posisi sembarang.
