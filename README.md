# PKB — Wireless Split Mechanical Keyboard

[![View PCB on KiCanvas](https://hack.club/pcb-badge)](https://kicanvas.org/?repo=https://github.com/huzi-ctrl/split-keyboard/tree/main/kicad)
[![Build & Release ZMK Firmware](https://github.com/huzi-ctrl/zmk-config-split-keyboard/actions/workflows/build.yml/badge.svg)](https://github.com/huzi-ctrl/zmk-config-split-keyboard/actions/workflows/build.yml)
[![Latest Firmware Release](https://img.shields.io/github/v/release/huzi-ctrl/zmk-config-split-keyboard?label=Download%20Latest%20Firmware)](https://github.com/huzi-ctrl/zmk-config-split-keyboard/releases/latest)

**PKB (Printed KeyBoard)** is a custom 42-key wireless split mechanical keyboard designed from scratch in KiCad 10 and SolidWorks, powered by dual **Seeed Studio XIAO nRF52840** microcontrollers running **ZMK Firmware**.

---

## Why I Made It & How It Works

### Why I Built PKB
Traditional keyboards force your wrists to angle inward and make your fingers stretch across 6+ rows to reach numbers, symbols, and navigation keys. I wanted to design a compact, fully wireless split keyboard where:
1. Each half can be positioned at shoulder width for natural ergonomics.
2. Every letter, symbol, bracket, arrow key, F-key, and Numpad key is within **one row of the home row** using 3 tap-dance layers (**126 effective key bindings** across 42 physical switches).
3. Both halves fit on a **single panelized PCB** connected by mouse-bite breakaway tabs (`BO1`–`BO3`) to make fabrication and assembly clean and cost-effective.

### How It Works
- **Microcontroller & Wireless Split (`Seeed Studio XIAO nRF52840`)**: Each half runs a XIAO nRF52840 (`U1`/`U2`). The left half acts as the central controller (connecting to up to 4 devices over Bluetooth Low Energy or USB-C), while the right half pairs wirelessly over BLE to the left half.
- **I2C GPIO Expansion (`PCF8574AP`)**: Because the XIAO nRF52840 has only 11 GPIO pins, the 5 matrix columns (`COL0`–`COL4`) are driven directly by the XIAO (`D1`–`D5`), while the 5 matrix rows (`ROW0`–`ROW4`) are read through an **NXP PCF8574AP** 8-bit I2C I/O expander (`U3`/`U4`) on a shared I2C bus (`SCL` on `D9`, `SDA` on `D10`).
- **Dual 128×32 OLED Status Displays (`J3`/`J4`)**: Each half includes an I2C SSD1306 128×32 OLED display showing live battery percentage (`%`), active layer name (`QWERTY`, `CODE / NAV`, `NUM / FN`), and USB/Bluetooth profile status, with automatic 30-second idle blanking.
- **Per-Key RGB Underglow (`42× SK6812MINI`) with High-Side MOSFET Power Cut (`Q1`–`Q4`)**: Each half features 21 addressable SK6812MINI LEDs driven by hardware SPI (`D0`) and powered from `VBAT` through an NMOS+PMOS (`Q1`/`Q3`, `Q2`/`Q4`) high-side load switch (`D8`) so LED quiescent draw can be completely cut off to preserve battery life.

---

## PCB & 3D Renders

[![View PCB on KiCanvas](https://hack.club/pcb-badge)](https://kicanvas.org/?repo=https://github.com/huzi-ctrl/split-keyboard/tree/main/kicad)

### Full Panelized PCB Layout (KiCad)
![Full PCB Layout](images/pcb_full.png)

### 3D PCB Render
![3D PCB Render](images/pcb_3d.png)

---

## Schematics

Both the left and right halves share a modular hierarchical schematic (`side.kicad_sch`):

### Left Half Schematic (`/left/`)
![Left Half Schematic](images/left_schem.png)

### Right Half Schematic (`/right/`)
![Right Half Schematic](images/right_schem.png)

---

## CAD & Enclosure Design

Custom 3D-printed enclosures modeled in SolidWorks with integrated battery compartment and cable routing channel (available in `.STEP`, `.SLDPRT`, and `.3MF` formats inside [`CAD/`](CAD/)):

| Left Half Enclosure (`left_cad.png`) | Right Half Enclosure (`right_cad.png`) |
| :---: | :---: |
| ![Left Enclosure CAD](images/left_cad.png) | ![Right Enclosure CAD](images/right_cad.png) |

---

## Hardware Pinout

| Function | XIAO nRF52840 Pin | Peripheral / Notes |
| :--- | :--- | :--- |
| **`DLED` (RGB Data)** | `D0` (`P0.02`) | 21× SK6812MINI addressable RGB LEDs per half (`&spi2` MOSI via 330 $\Omega$ resistor) |
| **`COL0` – `COL4`** | `D1` – `D5` (`P0.03`, `P0.28`, `P0.29`, `P0.04`, `P0.05`) | Matrix column outputs (`GPIO_ACTIVE_LOW`, `col2row`) |
| **`INT` (Expander Interrupt)** | `D6` (`P1.11`) | Connected to `PCF8574AP` `/INT` (Pin 13) |
| **`VLED` Power Switch** | `D8` (`P1.13`) | Drives NMOS $\rightarrow$ PMOS high-side switch for `VLED` (`ext-power`) |
| **`SCL` (I2C Clock)** | `D9` (`P1.14`) | `i2c0` (`&xiao_i2c`) with 4.7 k$\Omega$ pull-up to `+3V3` |
| **`SDA` (I2C Data)** | `D10` (`P1.15`) | `i2c0` (`&xiao_i2c`) with 4.7 k$\Omega$ pull-up to `+3V3` |
| **`ROW0` – `ROW4`** | `PCF8574AP` `P0` – `P4` | I2C I/O Expander at address `0x38` (`A0=A1=A2=GND`) |
| **OLED Display (`J3`/`J4`)** | I2C (`SCL`/`SDA`) | 128×32 SSD1306 OLED at address `0x3C` |

---

## Firmware & Keymap (`ZMK`)

Pre-built `.uf2` firmware binaries are automatically compiled by GitHub Actions and published to [**GitHub Releases**](https://github.com/huzi-ctrl/zmk-config-split-keyboard/releases/latest):
- **Left Half (Central):** `pkb_left-seeeduino_xiao_ble-zmk.uf2`
- **Right Half (Peripheral):** `pkb_right-seeeduino_xiao_ble-zmk.uf2`

### Tap-Dance Thumb Keys
- **Left Thumb**: `1 Tap` = **Space** | `2 Taps` = Toggle **Layer 1 (`CODE / NAV`)**
- **Right Thumb**: `1 Tap` = **Backspace** | `2 Taps` = Toggle **Layer 2 (`NUM / FN`)**

### Layer 0: `QWERTY` (Base)
```text
+-----------------------------------------+   +-----------------------------------------+
|   Q   |   W   |   E   |   R   |   T     |   |   Y   |   U   |   I   |   O   |   P     |
|   A   |   S   |   D   |   F   |   G     |   |   H   |   J   |   K   |   L   |   ;     |
|   Z   |   X   |   C   |   V   |   B     |   |   N   |   M   |   ,   |   .   |   /     |
| LCTRL | LSHFT | LALT  | LGUI  |  ESC    |   | RGUI  | RALT  | RSHFT | RCTRL | ENTER   |
+-------------------------------+---------+   +---------+-------------------------------+
                                | SPC/NAV |   | BSP/NUM |
                                +---------+   +---------+
```

### Layer 1: `CODE / NAV` (Double-Tap Left Thumb)
```text
+-----------------------------------------+   +-----------------------------------------+
|   `   |   [   |   ]   |   -   |   =     |   | PG_UP | HOME  |  UP   |  END  |  DEL    |
|   ~   |   {   |   }   |   _   |   +     |   | PG_DN | LEFT  | DOWN  | RIGHT |  ---    |
|  ---  |   \   |   |   |   <   |   >     |   |  ---  |  ---  |  ---  |  ---  |  ---    |
|  ---  |  ---  |  ---  |  ---  |  ---    |   |  ---  |  ---  |  ---  |  ---  |  ---    |
+-------------------------------+---------+   +---------+-------------------------------+
                                | SPC/NAV |   |   ---   |
                                +---------+   +---------+
```

### Layer 2: `NUM / FN` (Double-Tap Right Thumb)
```text
+-----------------------------------------+   +-----------------------------------------+
|  F1   |  F2   |  F3   |  F4   |  F5     |   |   /   |   7   |   8   |   9   |   -     |
|  F6   |  F7   |  F8   |  F9   |  F10    |   |   *   |   4   |   5   |   6   |   +     |
|  F11  |  F12  |RGB_TOG|EP_TOG |OUT_TOG  |   |   0   |   1   |   2   |   3   |   .     |
| BT 1  | BT 2  | BT 3  | BT 4  | BT_CLR  |   |  ---  |  ---  |  ---  |  ---  |  ---    |
+-------------------------------+---------+   +---------+-------------------------------+
                                |   ---   |   | BSP/NUM |
                                +---------+   +---------+
```

---

## Repository Structure

```text
├── CAD/
│   ├── Keyboard.step          # Full 3D assembly in STEP format
│   ├── 3mf/                   # Printable 3MF files (left.3MF, right.3MF)
│   └── Raw/                   # Native SolidWorks part files (left.SLDPRT, right.SLDPRT)
├── kicad/
│   ├── Keyboard.kicad_pro     # KiCad 10 project file
│   ├── Keyboard.kicad_sch     # Top-level schematic sheet
│   ├── side.kicad_sch         # Left/Right modular keyboard half schematic
│   └── Keyboard.kicad_pcb     # Panelized 2-layer PCB layout
├── firmware/
│   ├── boards/shields/pkb/    # ZMK shield definition, devicetree, and Kconfig
│   ├── config/                # Keymap and ZMK configuration
│   └── build.yaml             # GitHub Actions build matrix
├── images/                    # PCB, schematic, and CAD screenshots
└── production.zip             # Ready-to-manufacture Gerber, Drill, and CPL files
```
