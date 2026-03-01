# Modul 3 — Animasi Vesica Piscis

**Mata Kuliah:** Grafika Komputer  
**Program:** Program 7  
**Prasyarat:** Modul 1 (Midpoint Circle), Modul 2 (Flower of Life)  
**Tingkat:** Menengah

---

## 🎯 Tujuan Pembelajaran

Setelah menyelesaikan modul ini, mahasiswa mampu:

1. Mendefinisikan **Vesica Piscis** secara geometris beserta proporsi fundamentalnya ($\sqrt{3}:1$).
2. Mengimplementasikan fungsi **busur parsial** menggunakan filter sudut berbasis `atan2`.
3. Memahami teknik normalisasi sudut dan penanganan **wrap-around** pada rentang $[0, 2\pi]$.
4. Merancang animasi **ping-pong** yang merubah lingkaran penuh menjadi kelopak vesica piscis.
5. Mengidentifikasi signifikansi historis dan matematis proporsi $\sqrt{3}$.

---

## 1. Vesica Piscis — Definisi dan Proporsi

### 1.1 Definisi Geometris

**Vesica Piscis** (Latin: "kantung ikan") adalah daerah irisan antara dua lingkaran berjari-jari sama, di mana **pusat setiap lingkaran berada tepat pada keliling lingkaran lainnya**.

```
Syarat geometris:
  Radius C1 = Radius C2 = R
  Jarak(C1, C2) = R
```

Karena jarak antar pusat = radius, ketiga titik $C_1$, $C_2$, dan kedua ujung lensa membentuk **segitiga sama sisi**, sehingga setiap sudut = 60°.

### 1.2 Proporsi Fundamental

Ini adalah proporsi ajaib yang membuat Vesica Piscis unik:

$$
\frac{\text{Tinggi (panjang lensa)}}{\text{Lebar}} = \sqrt{3} \approx 1.732
$$

Dari sini diturunkan:
- **Busur yang dibutuhkan tiap lingkaran:** $120°$ ($= 2 \times 60°$)
- **Jarak dari pusat ke ujung:** $h = R \times \frac{\sqrt{3}}{2} \approx R \times 0.866$

### 1.3 Signifikansi Historis

| Periode | Penggunaan |
|---|---|
| 2000 SM (Mesir Kuno) | Proporsi arsitektur sakral, simbol dewi Isis |
| 500 SM (Yunani Kuno) | Studi rasio $\sqrt{3}$ oleh Pythagoras |
| Kekristenan Awal | Simbol *Ichthys* (ikan) dan *Mandorla* |
| Abad Pertengahan | Rasio desain lengkungan katedral Gotik |

---

## 2. Algoritma Busur Parsial (`DrawArcFromAngle`)

Untuk menggambar Vesica Piscis, kita tidak butuh lingkaran **penuh** — hanya busur 120°. Fungsi ini menggunakan **Midpoint Circle Algorithm** yang dimodifikasi dengna filter sudut per-piksel.

### 2.1 Alur Kerja

```
Untuk setiap (x, y) dari Midpoint Circle:
  1. Hasilkan 8 titik simetri
  2. Untuk tiap titik simetri (px, py):
       a. Hitung sudut: θ = atan2f(py - cy, px - cx)
       b. Normalisasi θ ke [0, 2π]
       c. Periksa apakah θ ada dalam [startAngle, endAngle]
       d. Jika ya → DrawPixel(px, py)
  3. Update decision parameter
```

### 2.2 Normalisasi Sudut dan Wrap-Around

**Masalah:** `atan2f` mengembalikan nilai dari $-\pi$ hingga $+\pi$  (bukan $0$ hingga $2\pi$), dan sudut busur kita bisa melewati titik $0°/360°$.

**Solusi normalisasi:**

```c
// Normalisasi ke rentang [0, 2π]
float norm = fmodf(sudut, 2.0f * PI);
if (norm < 0) norm += 2.0f * PI;
```

**Dua kasus pengecekan busur:**

```
Kasus 1: Busur NORMAL (tidak melewati titik 0°)
  normStart < normEnd
  → inArc jika: normStart ≤ θ ≤ normEnd

Contoh: busur dari 30° ke 150°
  (30° < 150°) → in arc jika 30° ≤ θ ≤ 150°

─────────────────────────────────────────────────────

Kasus 2: Busur WRAP-AROUND (melewati titik 0°)
  normStart > normEnd
  → inArc jika: θ ≥ normStart ATAU θ ≤ normEnd

Contoh: busur dari 300° ke 60° (melewati 0°)
  (300° > 60°) → in arc jika θ ≥ 300° ATAU θ ≤ 60°
```

### 2.3 Implementasi C

```c
static void DrawArcFromAngle(int cx, int cy, int r,
                              float startAngle, float endAngle, Color col) {
    if (r <= 0) return;

    /* ─── Deteksi lingkaran penuh (360°) ─── */
    float span = fabsf(endAngle - startAngle);
    int isFullCircle = (span >= 2.0f * PI - 0.001f);

    /* ─── Normalisasi sudut ke [0, 2π] ─── */
    float normStart = fmodf(startAngle, 2.0f * PI);
    if (normStart < 0) normStart += 2.0f * PI;
    float normEnd = fmodf(endAngle, 2.0f * PI);
    if (normEnd < 0) normEnd += 2.0f * PI;

    int x = 0, y = r, d = 3 - 2 * r;

    while (y >= x) {
        /* 8 titik simetri */
        int pts[8][2] = {
            {cx + x, cy + y}, {cx + y, cy + x},
            {cx + y, cy - x}, {cx + x, cy - y},
            {cx - x, cy - y}, {cx - y, cy - x},
            {cx - y, cy + x}, {cx - x, cy + y}
        };

        for (int i = 0; i < 8; i++) {
            if (isFullCircle) {
                /* Bypass filter sudut untuk lingkaran penuh */
                DrawPixel(pts[i][0], pts[i][1], col);
            } else {
                /* Filter sudut */
                float θ = atan2f(pts[i][1] - cy, pts[i][0] - cx);
                float normθ = fmodf(θ, 2.0f * PI);
                if (normθ < 0) normθ += 2.0f * PI;

                int inArc;
                if (normStart <= normEnd) {
                    inArc = (normθ >= normStart && normθ <= normEnd);
                } else { /* wrap-around */
                    inArc = (normθ >= normStart || normθ <= normEnd);
                }
                if (inArc) DrawPixel(pts[i][0], pts[i][1], col);
            }
        }

        if (d < 0) { d += 4*x + 6; }
        else { d += 4*(x-y) + 10; y--; }
        x++;
    }
}
```

> **Optimasi:** Pengecekan `isFullCircle` menghindari `atan2f` (operasi mahal) ketika menggambar lingkaran penuh di fase awal animasi.

---

## 3. Fungsi Animasi (`DrawVesicaPiscisAnim`)

### 3.1 Ide Dasar

Animasi bekerja dengan **menyempitkan busur secara perlahan**:

```
animProgress = 0.0 → halfArc = 180° → Lingkaran penuh (360°)
animProgress = 1.0 → halfArc =  60° → Vesica Piscis (120°)
```

Interpolasi linear:

$$
\text{halfArc}(t) = \pi - t \cdot (\pi - \frac{\pi}{3}) = \pi(1 - \frac{2t}{3})
$$

Di mana $t = \text{animProgress} \in [0, 1]$.

### 3.2 Implementasi

```c
static void DrawVesicaPiscisAnim(int cx1, int cy1, int cx2, int cy2,
                                  int r, float animProgress, Color col) {
    /* ─── Hitung arah dari C1 ke C2 ─── */
    int dx = cx2 - cx1, dy = cy2 - cy1;
    float angleToCenter = atan2f((float)dy, (float)dx);

    /* ─── Interpolasi halfArc: 180° → 60° ─── */
    float targetHalfArc  = PI / 3.0f;               // Target: 60°
    float currentHalfArc = PI - (animProgress * (PI - targetHalfArc));

    /* ─── Busur dari C1 (menghadap ke C2) ─── */
    float start1 = angleToCenter - currentHalfArc;
    float end1   = angleToCenter + currentHalfArc;
    DrawArcFromAngle(cx1, cy1, r, start1, end1, col);

    /* ─── Busur dari C2 (menghadap ke C1) ─── */
    float start2 = angleToCenter + PI - currentHalfArc;
    float end2   = angleToCenter + PI + currentHalfArc;
    DrawArcFromAngle(cx2, cy2, r, start2, end2, col);
}
```

---

## 4. Mekanisme Animasi Ping-Pong

### 4.1 Tiga Fase Animasi

Animasi satu siklus 6 detik dibagi menjadi 3 fase:

| Waktu | Progress `t` | Fase | Visual |
|---|---|---|---|
| 0.0s – 0.9s | 0% → 30% | **Fase 1: Tahan** | Dua lingkaran penuh |
| 0.9s – 2.1s | 30% → 70% | **Fase 2: Transisi** | Busur mengerut dari 360° → 120° |
| 2.1s – 3.0s | 70% → 100% | **Fase 3: Tahan** | Vesica Piscis final |
| 3.0s – 6.0s | 100% → 0% | **Reverse** | Kembali ke lingkaran penuh |

### 4.2 Kode Ping-Pong

```c
// Di dalam DrawProgram7(), dipanggil setiap frame:
float animTime = GetTime();
float cycleTime = 6.0f;  // satu siklus 6 detik

// t berjalan dari 0 → 1 → 0 (ping-pong)
float t = fmodf(animTime, cycleTime) / cycleTime;
float progress = (t < 0.5f) ? t * 2.0f : (1.0f - t) * 2.0f;

// Petakan ke tiga fase
float geomAnimProgress;
if (progress <= 0.3f) {
    geomAnimProgress = 0.0f;                      // Fase 1: tahan di lingkaran penuh
} else if (progress >= 0.7f) {
    geomAnimProgress = 1.0f;                      // Fase 3: tahan di vesica piscis
} else {
    geomAnimProgress = (progress - 0.3f) / 0.4f; // Fase 2: transisi
}
```

### 4.3 Visualisasi Progress

```
progress:     0────────0.3──────────────0.7──────────1
              │                          │           │
geomAnim:     0         0               1           1
              │                          │
Fase:     [1: Tahan]   [2: Transisi]   [3: Tahan]
Visual:  ●●  ──────→   )(  ──────→   ><
         (2 lingkaran penuh)    (kelopak vesica)
```

---

## 5. Konfigurasi 4 Pasang Pada Program 7

```c
int cx = SCREEN_W / 2;      // Pusat layar
int cy = SCREEN_H / 2 + 30;
int R  = 114;

// 4 pasang: pusat lingkaran yang berjarak R satu sama lain
struct { int x1,y1, x2,y2; Color col; } pairs[] = {
    { cx, cy,   cx,      cy - R, RED    },  // Pasang Utara
    { cx, cy,   cx,      cy + R, BLUE   },  // Pasang Selatan
    { cx, cy,   cx + R,  cy,     GREEN  },  // Pasang Timur
    { cx, cy,   cx - R,  cy,     ORANGE },  // Pasang Barat
};

for (int i = 0; i < 4; i++) {
    DrawVesicaPiscisAnim(pairs[i].x1, pairs[i].y1,
                         pairs[i].x2, pairs[i].y2,
                         R, geomAnimProgress, pairs[i].col);
}
```

---

## 6. Kompleksitas

| Fungsi | Time Complexity | Keterangan |
|---|---|---|
| `DrawArcFromAngle` | $O(R)$ | Iterasi Midpoint Circle dengan filter sudut |
| `DrawVesicaPiscisAnim` | $O(2R)$ | Dua busur per kelopak |
| `DrawProgram7` | $O(8R)$ | 4 pasang × 2 busur per frame |

**Space:** $O(1)$ — tidak ada alokasi heap.

---

## 7. Latihan Soal

**Soal 1 (Mudah):**  
Berapa nilai `halfArc` (dalam derajat) ketika `animProgress = 0.5f`? Tunjukkan perhitungannya menggunakan rumus interpolasi di bagian 3.1. Apa yang terlihat secara visual pada saat itu?

**Soal 2 (Menengah):**  
Jelaskan mengapa kita perlu menangani kasus *wrap-around* pada normalisasi sudut. Berikan contoh nyata busur yang dimulai dari 300° dan berakhir di 60° — titik piksel mana yang TIDAK terplot dengan benar jika kita tidak memeriksa kasus ini?

**Soal 3 (Tantangan):**  
Modifikasi fungsi animasi agar busur menggunakan **easing function** (misalnya *ease-in-out*) bukan interpolasi linear, sehingga animasi terasa lebih halus. Gunakan fungsi easing: $f(t) = t^2(3 - 2t)$ (smoothstep). Tuliskan kodenya.

---

## 📝 Ringkasan

| Konsep | Penjelasan Singkat |
|---|---|
| **Vesica Piscis** | Irisan dua lingkaran berjari-jari $R$, jarak pusat = $R$ |
| **Busur per lingkaran** | $120°$ (karena segitiga sama sisi: sudut = 60°) |
| **`DrawArcFromAngle`** | Midpoint Circle + filter `atan2` per piksel |
| **Normalisasi sudut** | `fmodf` + pengecekan negatif → rentang $[0, 2\pi]$ |
| **Wrap-around** | Busur melewati $0°$: `in OR` bukan `AND` |
| **Animasi ping-pong** | $t$ berjalan $0→1→0$ setiap 6 detik, dibagi 3 fase |

---

**Referensi:**
- Bresenham, J. E. (1977). *"A linear algorithm for incremental digital display of circular arcs"*
- Lawlor, R. (1982). *"Sacred Geometry: Philosophy and Practice"*
