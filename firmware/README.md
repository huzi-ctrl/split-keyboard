# PKB — 42-Key Wireless Split Keyboard (ZMK Firmware)

ZMK firmware configuration for the **PKB** 40% (42-key) wireless split keyboard powered by the **Seeed Studio XIAO nRF52840** (`seeeduino_xiao_ble`).

---

## Hardware & Pinout Summary

Each half uses an identical PCB (`side.kicad_sch`) with the following connections:

| Function | XIAO nRF52840 Pin | Peripheral / Notes |
| :--- | :--- | :--- |
| **`DLED` (RGB Data)** | `D0` (`P0.02`) | 21× SK6812MINI addressable RGB LEDs (`&spi2` MOSI via 330 $\Omega$ resistor) |
| **`COL0` – `COL4`** | `D1` – `D5` (`P0.03`, `P0.28`, `P0.29`, `P0.04`, `P0.05`) | Matrix column outputs (`GPIO_ACTIVE_LOW`, `col2row`) |
| **`INT` (Expander Interrupt)** | `D6` (`P1.11`) | Connected to `U4` (`PCF8574AP`) `/INT` (Pin 13) |
| **`VLED` Power Switch** | `D8` (`P1.13`) | Drives `Q2` (NMOS) $\rightarrow$ `Q4` (PMOS) high-side switch for `VLED` (`ext-power`) |
| **`SCL` (I2C Clock)** | `D9` (`P1.14`) | Remapped `i2c0` (`&xiao_i2c`) with 4.7 k$\Omega$ pull-up to `+3V3` |
| **`SDA` (I2C Data)** | `D10` (`P1.15`) | Remapped `i2c0` (`&xiao_i2c`) with 4.7 k$\Omega$ pull-up to `+3V3` |
| **`ROW0` – `ROW4`** | `PCF8574AP` `P0` – `P4` | I2C I/O Expander (`U4`) at address `0x38` (`A0=A1=A2=GND`) |
| **OLED Display (`J4`)** | I2C (`SCL`/`SDA`) | 128×32 SSD1306 OLED at address `0x3C` |

> **Hardware Notes:**
> - **Matrix Diodes (`D1`–`D21`)**: Oriented with **Anode on `ROW`** and **Cathode on `COL`** (`ROW ->| COL`) so the `PCF8574AP`'s internal pull-ups on `ROW0`–`ROW4` are pulled low when the XIAO drives an active `COL` low.
> - **PCF8574 Address**: `PCF8574AP` (`A` variant) with `A0=A1=A2=GND` uses I2C address `0x38`. If using a non-`A` `PCF8574` chip, change `0x38` to `0x20` in `boards/shields/pkb/pkb.dtsi`.

---

## Keymap & Layers

Layer switching uses **Tap-Dance** on the two thumb keys:
- **Left Thumb**:
  - **Single Tap**: `Space`
  - **Double Tap**: Toggle **Layer 1 (`CODE / NAV`)** on/off
- **Right Thumb**:
  - **Single Tap**: `Backspace`
  - **Double Tap**: Toggle **Layer 2 (`NUM / FN`)** on/off

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
Includes F-keys, Numpad, **RGB Underglow toggle**, **External Power (`VLED`) toggle**, **USB/BLE output toggle**, and **Bluetooth profile controls**:
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

## 128×32 OLED Display

The OLED display acts as a live status dashboard:
- **Top-Left**: Output status (`USB` or `BT 1`–`4` connection state on the left half; split wireless link icon on the right half).
- **Top-Right**: Battery icon + **numeric battery percentage (`%`)**.
- **Bottom-Left**: Active layer name (`QWERTY`, `CODE / NAV`, `NUM / FN`).
- **Auto-Off**: Automatically blanks after **30 seconds** of inactivity to save battery and prevent OLED burn-in, and wakes immediately on any key press.

---

## How to Flash Firmware to the Keyboard

1. **Download the Built Firmware**:
   - Open the [**GitHub Actions**](https://github.com/huzi-ctrl/zmk-config-split-keyboard/actions) tab on GitHub.
   - Click the latest successful workflow run at the top.
   - Under **Artifacts**, click **`firmware`** to download `firmware.zip` and extract it:
     - `pkb_left-seeeduino_xiao_ble-zmk.uf2`
     - `pkb_right-seeeduino_xiao_ble-zmk.uf2`

2. **Flash the Left Half (Central)**:
   - Connect the **Left Half** XIAO to your computer over USB-C.
   - **Double-click the `RST` button** on the XIAO to enter UF2 bootloader mode (a USB drive named `XIAO-SENSE` will appear).
   - Drag and drop **`pkb_left-seeeduino_xiao_ble-zmk.uf2`** onto the USB drive. (The board will automatically reboot once finished.)

3. **Flash the Right Half (Peripheral)**:
   - Connect the **Right Half** XIAO to your computer over USB-C.
   - **Double-click the `RST` button** on the Right XIAO.
   - Drag and drop **`pkb_right-seeeduino_xiao_ble-zmk.uf2`** onto the USB drive.

4. **Power On & Pair**:
   - Turn on both halves. The right half will automatically connect to the left half.
   - Pair your computer/phone via Bluetooth to **`PKB`**.
