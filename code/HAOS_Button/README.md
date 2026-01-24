# HAOS Button: Ultra-Low Power BLE Light Switch

An ultra-low-power, CR2032-battery-operated Bluetooth Low Energy (BLE) light switch designed specifically for **Home Assistant**. This project utilizes the **BTHome V2** protocol to provide long battery life and "no-pairing" automatic discovery.

## 1. Brief Overview
The HAOS Button is a custom-engineered remote for Home Assistant. Instead of maintaining a power-hungry active connection, it utilizes BLE Advertising to broadcast state changes.
*   **Hardware:** nRF52832 SoC.
*   **Power:** Powered by a single **CR2032 coin cell**.
*   **Protocol:** BTHome V2 (non-encrypted) for instant integration with HAOS.
*   **Form Factor:** Designed to fit behind existing wall plates or as a standalone remote. Small enough to be on a key chain. 

## 2. Installation & VS Code Setup
To compile and flash this project, your development environment must be configured for **nRF Connect SDK (NCS) v3.x.x (2026)**.

### Software Prerequisites
1.  **nRF Connect Desktop:** Install the [nRF Connect Desktop tool](https://www.nordicsemi.com).
2.  **Toolchain:** Use the **Toolchain Manager** within nRF Connect Desktop to install **SDK v3.2.1** (or the latest stable 2026 release).
3.  **VS Code Extensions:**
    *   **nRF Connect for VS Code Extension Pack** (Essential).
    *   **C/C++ Extension** (by Microsoft).

### Building the Project
1.  Open the `code/HAOS_Button` folder in VS Code.
2.  In the nRF Connect Sidebar, click **Add Build Configuration**.
3.  **Board Target:** Select `nrf52dk/nrf52832`.
4.  **Configuration:** Ensure `prj.conf` is selected.
5.  Click **Build Configuration**. 
    *   *Note: Use **Pristine Build** (Refresh icon) if you modify `app.overlay` or `prj.conf`.*

## 3. Hardware Specifications & Key Assumptions
This project is mapped to the custom HAOS PCB layout.

### GPIO Mapping
The hardware utilizes the following pinout as defined in the `app.overlay`:

| Component | GPIO Pin | Logic | Description |
| :--- | :--- | :--- | :--- |
| **Red LED** | `P0.06` | Active Low | Status/Error Indicator |
| **White LED** | `P0.07` | Active Low | Secondary Indicator |
| **Green LED** | `P0.08` | Active Low | Success/Heartbeat Indicator |
| **Up Button** | `P0.11` | Pull-Up | Shorts to GND when pressed |
| **Down Button** | `P0.12` | Pull-Up | Shorts to GND when pressed |
| **Reset/Button**| `P0.21` | Pull-Up | Hardware Reset / Input |

### Oscillator & Crystal Frequencies
To maintain BLE timing accuracy and achieve micro-ampere sleep currents, the following crystal assumptions are made:
*   **High-Frequency Clock (HFCLK):** 32 MHz Crystal (Required for BLE radio operation).
*   **Low-Frequency Clock (LFCLK):** 32.768 kHz Crystal (Required for accurate sleep-interval timing).
*   **DCDC Converter:** Enabled in `prj.conf` to reduce peak current draw from the CR2032 during radio bursts.

### Programming Interface (J6 Header)
*   **Connector:** Tag-Connect TC2030-IDC-NL (6-pin).
*   **Programmer:** SEGGER J-Link (e.g., EDU Mini).
*   **Power Note:** The programmer does **not** provide power via the J6 header. The board must be powered by the CR2032 or an external 3V supply during flashing.

## 4. Debugging & Output
Debugging is handled via **SWO (Single Wire Output)** on the J6 header to avoid the power overhead of a traditional UART.
*   **Frequency:** 2,000,000 Hz.
*   **Tools:** View logs using the **SEGGER J-Link SWO Viewer**.

## 5. Home Assistant Integration
1.  Navigate to **Settings > Devices & Services** in your HAOS instance.
2.  Press any button on the device to begin advertising.
3.  Home Assistant will automatically discover the device as a **BTHome** integration.
4.  Once configured, buttons will appear as event entities for use in Automations.

### Notes
2026-01-24 As of this date, the baord has not be manufactured or tested. This is still a draft version. 