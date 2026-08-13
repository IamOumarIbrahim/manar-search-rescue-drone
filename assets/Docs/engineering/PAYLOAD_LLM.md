# MANAR Payload & System Specification Matrix

### Executive Summary Baseline

- **Current Itemized Payload Mass:** `2,936.7 g` *(2.937 kg)* — derived from 26 itemized physical & electrical component specifications.
- **Normal Search Payload Power:** `47.20 W` delivered component load (`55.53 W` main bus draw including converter thermal losses).
- **Target Verification Payload Power:** `63.90 W` delivered component load (`75.18 W` main bus draw including converter thermal losses).
- **Peak Payload Power:** `178.90 W` delivered component load (`210.47 W` instantaneous main bus draw including converter thermal losses).
- **Unresolved Major Hardware Decisions:** `6 Open Engineering Decisions` requiring owner resolution.

---

## A. Payload Component Matrix

*(Note: Matrix is divided into two linked tables to preserve visual clarity and prevent excessive horizontal scrolling. Verification Labels: `[V]` Official Manufacturer Verified, `[D]` Distributor / Config / Assembly Verified, `[E]` Derived Engineering Estimate, `[U]` Unknown. "Verified" indicates component specifications are anchored to official technical datasheets, NOT that the hardware is owner-approved for MANAR.)*

### Table A1: Physical & Electrical Specifications

| Component / Model | Manufacturer | Role / Subsystem | Dimensions ($W \times L \times H$ mm) | Mass (g) | Input Voltage | Cont. Power (W) | Peak Power (W) | Operating Temp (°C) |
| :--- | :--- | :--- | :---: | ---: | :--- | ---: | ---: | :---: |
| **Sony FCB-EV9500L Block Camera** | Sony Corporation | Visual / Day RGB Zoom | $56.0 \times 64.0 \times 125.0$ | 456.0 | 7.0V – 12.0V DC | 6.10 | 7.80 | -5 to +60 |
| **FLIR Boson+ 640 Thermal Core** | Teledyne FLIR | Visual / Thermal IR Core | $21.0 \times 21.0 \times 11.0$ | 7.5 | 3.3V DC ($\pm 5\%$) | 0.50 – 1.55 | 3.20 | -40 to +80 |
| **FLIR Germanium 19mm Lens** | Teledyne FLIR | Thermal Optical Lens | $\varnothing 28.0 \times 24.5$ | 45.0 | Passive (N/A) | 0.00 | 0.00 | -40 to +80 |
| **SiOnyx Black Silicon OEM Board** | SiOnyx Inc. | Visual / Low-Light IR | $30.0 \times 20.0 \times 15.0$ | 35.0 | 1.8V / 2.8V / 5.0V DC | 1.50 | 3.50 | -20 to +50 |
| **TI IWR6843 60 GHz mmWave Radar** | Texas Instruments | Search Radar | $50.0 \times 40.0 \times 12.0$ | 45.0 | 3.3V / 5.0V DC | 2.50 | 6.00 | -40 to +105 (Die) / -20 to +60 (EVM) |
| **Level 1 Passive RF Receiver** | RTL-SDR.com / GSG | Passive RF Sensing | $115.0 \times 27.0 \times 13.0$ | 45.0 | 5.0V DC (USB) | 1.35 | 1.50 | -10 to +60 |
| **Level 1 Passive RF Antenna** | Taoglas Ltd | Passive RF Antenna | $\varnothing 13.0 \times 168.0$ | 35.0 | Passive (N/A) | 0.00 | 0.00 | -40 to +85 |
| **Seeed ReSpeaker v2.0 Mic Array** | Seeed Studio | Acoustic Sensing | $\varnothing 70.0 \times 11.0$ | 50.2 | 5.0V DC (USB) | 1.00 | 2.50 | 0 to +70 |
| **Holybro Pixhawk 6X Autopilot Assembly** | Holybro | Flight Avionics | $52.4 \times 103.4 \times 16.7$ | 88.0 | 4.75V – 5.25V DC | 2.00 | 2.50 | -40 to +85 |
| **Holybro ARK GPS CAN Module** | Holybro / ARK | Flight Navigation | $50.0 \times 50.0 \times 10.0$ | 33.0 | 4.75V – 5.25V DC | 0.65 | 0.90 | -40 to +85 |
| **Ainstein US-D1 Radar Altimeter** | Ainstein Inc. | Flight Altimetry | $108.0 \times 79.0 \times 20.0$ | 110.0 | 5.0V – 13.0V DC | 2.00 | 3.00 | -20 to +65 |
| **NVIDIA Jetson Orin NX 16GB** | NVIDIA Corporation | Perception Compute | $69.6 \times 45.0 \times 6.0$ | 29.0 | 5.0V – 20.0V DC | 15.00 | 25.00 | -25 to +80 |
| **Firehouse Arc V Strobe Array** | Firehouse Tech | Visual Signaling | $38.0 \times 25.0 \times 13.0$ | 55.0 | Internal 250mAh LiPo | 6.50 | 12.00 | -10 to +50 |
| **Heliograph SOLAS Reflectors** | 3M Safety / Custom | Passive Signaling | $150.0 \times 100.0 \times 2.0$ | 35.0 | Passive (N/A) | 0.00 | 0.00 | -40 to +80 |
| **Open 15W PA Horn Transducer** | Dayton Audio / Custom | Acoustic Transducer | $\varnothing 120.0 \times 130.0$ | 240.0 | Passive Audio Drive | 0.00 | 0.00 | -20 to +60 |
| **CZI GL60 Mini 60W Spotlight** | CZI Technology | Scene Illumination | $135.0 \times 87.0 \times 82.0$ | 240.0 | 24.0V DC | 0.00 | 60.00 | -20 to +50 |
| **TWIGA USB3 Neo Decoder Board** | TWIGA Solutions | Video Interface | $48.0 \times 45.0 \times 11.0$ | 13.0 | 6.0V – 12.0V DC | 1.50 | 2.50 | 0 to +60 |
| **Connect Tech Hadron Carrier** | Connect Tech Inc. | Compute Carrier | $82.6 \times 58.8 \times 16.0$ | 60.0 | 9.0V – 60.0V DC | 1.50 | 4.00 | -40 to +85 |
| **Jetson Active Heatsink & Fan** | Waveshare / NVIDIA | Thermal Cooling | $60.0 \times 45.0 \times 28.0$ | 110.0 | 5.0V DC (PWM) | 0.00 | 1.50 | -20 to +70 |
| **TPA3116D2 Audio Amplifier Board** | Texas Instruments / Custom | Acoustic Driver | $45.0 \times 35.0 \times 15.0$ | 40.0 | 12.0V – 24.0V DC | 0.00 | 35.00 | -20 to +60 |
| **3-Axis Gyro EO/IR Gimbal** | Gremsy / Custom | Mount / Gyro | $140.0 \times 120.0 \times 150.0$ | 450.0 | 12.0V – 24.0V DC | 4.00 | 8.00 | -15 to +50 |
| **Spotlight / PA 24V Step-Up BEC** | Pololu / Vicor | Power Converter | $55.0 \times 35.0 \times 18.0$ | 85.0 | 18.0V – 26.0V DC | 1.50 | 21.95* | -20 to +75 |
| **Fixed Camera Brackets** | Custom Aluminum | Camera Mounts | Derived Custom | 95.0 | Passive (N/A) | 0.00 | 0.00 | -40 to +100 |
| **Smoke Canister Drop Housing** | Custom Composite | Drop Mechanism | $120.0 \times 65.0 \times 55.0$ | 175.0 | 5.0V – 6.0V DC | 0.50 | 5.00 | -10 to +60 |
| **Structural Mount Plates & Rails** | Custom Carbon Fiber | Frame Mounts | $250.0 \times 120.0 \times 15.0$ | 140.0 | Passive (N/A) | 0.00 | 0.00 | -50 to +120 |
| **Wiring Harness & Connectors** | Molex / Amphenol | Electrical Harness | Custom (4.5m run) | 180.0 | Up to 60V DC | 0.00 | 0.00 | -40 to +105 |
| **Conformal Coating & IP Seals** | MG Chemicals | Weatherproofing | Thin-film / Gaskets | 40.0 | Passive (N/A) | 0.00 | 0.00 | -50 to +150 |

*\*Note: 21.95W peak entry for the 24V BEC represents internal conversion thermal loss ($\eta_{\text{BEC}} = 82\%$) during 95W 24V peak output, which is accounted for within the overall PDU rail loss model.*

---

### Table A2: Integration & Procurement Specifications

| Component / Model | Data / Control Interface | Approx. Unit Price | Verification | MANAR Status | Datasheet / Technical Source |
| :--- | :--- | :--- | :---: | :--- | :--- |
| **Sony FCB-EV9500L Block Camera** | Digital LVDS video, VISCA UART control | \$1,200 – \$1,800 USD | ![Verified](https://img.shields.io/badge/V-Verified-2ea44f) | ![Candidate](https://img.shields.io/badge/Status-Candidate%20(Arch%20A)-0052cc) | [Sony FCB-EV9500L Tech Manual](https://pro.sony/en_02/products/camera-block-fcb-series/fcb-ev9500l) |
| **FLIR Boson+ 640 Thermal Core** | USB / CMOS / MIPI video; UART / I2C | \$4,000 – \$5,500 USD | ![Distributor](https://img.shields.io/badge/D-Distributor-dfb317) | ![Candidate](https://img.shields.io/badge/Status-Candidate%20(Arch%20A)-0052cc) | [Teledyne FLIR Boson+ Datasheet](https://www.flir.com/products/boson-plus/) |
| **FLIR Germanium 19mm Lens** | Threaded M12 / Bayonet mount | \$800 – \$1,200 USD | ![Verified](https://img.shields.io/badge/V-Verified-2ea44f) | ![Candidate](https://img.shields.io/badge/Status-Candidate%20(Arch%20A)-0052cc) | [Teledyne FLIR Boson Lens Catalog](https://www.flir.com/products/boson-plus/) |
| **SiOnyx Black Silicon OEM Board** | MIPI CSI-2 (8-lane), I2C / Serial | \$600 – \$1,000 USD | ![Distributor](https://img.shields.io/badge/D-Distributor-dfb317) | ![Candidate](https://img.shields.io/badge/Status-Candidate-0052cc) | [SiOnyx XQE/XRB Specification](https://www.sionyx.com/pages/oem) |
| **TI IWR6843 60 GHz mmWave Radar** | USB, UART, SPI, CAN-FD, 60-pin ADC | \$179 – \$220 USD | ![Verified](https://img.shields.io/badge/V-Verified-2ea44f) | ![Candidate](https://img.shields.io/badge/Status-Candidate%20(Option%20C)-0052cc) | [TI IWR6843 Datasheet SWRS219](https://www.ti.com/lit/ds/symlink/iwr6843.pdf) |
| **Level 1 Passive RF Receiver** | USB 2.0 (Micro-B/Type-C), SMA Female | \$30 – \$40 USD (RTL) | ![Verified](https://img.shields.io/badge/V-Verified-2ea44f) | ![Approved TBD](https://img.shields.io/badge/Status-Approved%20%7C%20Hardware%20TBD-2ea44f) | [RTL-SDR v4 Manual](https://www.rtl-sdr.com/v4/) |
| **Level 1 Passive RF Antenna** | SMA Male (50 $\Omega$ impedance) | \$25 – \$45 USD | ![Distributor](https://img.shields.io/badge/D-Distributor-dfb317) | ![Approved TBD](https://img.shields.io/badge/Status-Approved%20%7C%20Hardware%20TBD-2ea44f) | [Taoglas TG.30.8113 Datasheet](https://www.taoglas.com/product/tg-30-5g-4g-wideband-antenna/) |
| **Seeed ReSpeaker v2.0 Mic Array** | USB 2.0 (UAC 1.0), 3.5mm Jack, I2S | \$69 USD | ![Verified](https://img.shields.io/badge/V-Verified-2ea44f) | ![Baseline](https://img.shields.io/badge/Status-Engineering%20Baseline-2ea44f) | [Seeed ReSpeaker v2.0 Wiki](https://wiki.seeedstudio.com/ReSpeaker_Mic_Array_v2.0/) |
| **Holybro Pixhawk 6X Autopilot Assembly** | Ethernet, CAN x3, UART x8, SPI, I2C | \$230 – \$290 USD | ![Distributor](https://img.shields.io/badge/D-Distributor-dfb317) | ![Baseline](https://img.shields.io/badge/Status-Engineering%20Baseline-2ea44f) | [Holybro Pixhawk 6X Manual](https://docs.holybro.com/autopilot/pixhawk-6x) |
| **Holybro ARK GPS CAN Module** | CAN bus (DroneCAN / PX4) | \$89 USD | ![Verified](https://img.shields.io/badge/V-Verified-2ea44f) | ![Baseline](https://img.shields.io/badge/Status-Engineering%20Baseline-2ea44f) | [Holybro ARK GPS Datasheet](https://docs.holybro.com/gps/ark-gps) |
| **Ainstein US-D1 Radar Altimeter** | CAN / UART (Molex 50579204) | \$499 USD | ![Verified](https://img.shields.io/badge/V-Verified-2ea44f) | ![Baseline](https://img.shields.io/badge/Status-Engineering%20Baseline-2ea44f) | [Ainstein US-D1 User Manual](https://ainstein.ai/us-d1/) |
| **NVIDIA Jetson Orin NX 16GB** | 260-pin SO-DIMM (PCIe, MIPI, USB 3.2) | \$999 USD (1KU) | ![Verified](https://img.shields.io/badge/V-Verified-2ea44f) | ![Baseline](https://img.shields.io/badge/Status-Engineering%20Baseline-2ea44f) | [NVIDIA Orin NX Data Sheet](https://developer.nvidia.com/embedded/jetson-orin-nx) |
| **Firehouse Arc V Strobe Array** | Micro-USB / USB-C Charge Port | \$120 – \$180 USD (4-pack) | ![Verified](https://img.shields.io/badge/V-Verified-2ea44f) | ![Baseline](https://img.shields.io/badge/Status-Engineering%20Baseline-2ea44f) | [Firehouse Arc V Spec](https://firehousetechnology.com/store/p101/Arc_V.html) |
| **Heliograph SOLAS Reflectors** | Passive Optical Reflection | \$15 – \$30 USD | ![Estimated](https://img.shields.io/badge/E-Estimated-708090) | ![Decision Required](https://img.shields.io/badge/Status-Decision%20Required-dfb317) | [3M SOLAS Sheet Specs](https://www.3m.com) |
| **Open 15W PA Horn Transducer** | 2-pin Audio Leads | \$50 – \$80 USD | ![Estimated](https://img.shields.io/badge/E-Estimated-708090) | ![Candidate](https://img.shields.io/badge/Status-Candidate-0052cc) | Derived Horn Transducer Spec |
| **CZI GL60 Mini 60W Spotlight** | CAN bus, PWM, DJI PSDK | \$1,100 – \$1,800 USD | ![Verified](https://img.shields.io/badge/V-Verified-2ea44f) | ![Candidate](https://img.shields.io/badge/Status-Candidate-0052cc) | [CZI GL60 Mini Specification](https://www.czitouch.com/en/product/gl60mini) |
| **TWIGA USB3 Neo Decoder Board** | LVDS Input $\rightarrow$ USB 3.2 Gen 1 Out | \$350 – \$500 USD | ![Verified](https://img.shields.io/badge/V-Verified-2ea44f) | ![Candidate](https://img.shields.io/badge/Status-Candidate%20(Arch%20A)-0052cc) | [TWIGA USB3 Neo Specification](https://www.twiga-support.com/product/usb3-neo/) |
| **Connect Tech Hadron Carrier** | 2x GbE, 2x USB 3.1, 4x CSI, 2x CAN | \$350 – \$500 USD | ![Verified](https://img.shields.io/badge/V-Verified-2ea44f) | ![Baseline](https://img.shields.io/badge/Status-Engineering%20Baseline-2ea44f) | [Connect Tech Hadron Spec Sheet](https://connecttech.com/product/hadron-carrier-for-nvidia-jetson-orin-nx-orin-nano/) |
| **Jetson Active Heatsink & Fan** | 4-pin JST PWM Fan Header | \$18 – \$30 USD | ![Verified](https://img.shields.io/badge/V-Verified-2ea44f) | ![Baseline](https://img.shields.io/badge/Status-Engineering%20Baseline-2ea44f) | [Waveshare Orin NX Cooling Wiki](https://www.waveshare.com/jetson-orin-nx-fan.htm) |
| **TPA3116D2 Audio Amplifier Board** | 3.5mm / Differential Analog In, 2-pin Out | \$15 – \$40 USD | ![Estimated](https://img.shields.io/badge/E-Estimated-708090) | ![Candidate](https://img.shields.io/badge/Status-Candidate-0052cc) | Derived TPA3116D2 Board Spec |
| **3-Axis Gyro EO/IR Gimbal** | CAN / UART / PWM (Pixhawk S.BUS) | \$800 – \$1,400 USD | ![Estimated](https://img.shields.io/badge/E-Estimated-708090) | ![Candidate](https://img.shields.io/badge/Status-Candidate%20(Arch%20A)-0052cc) | [Gremsy Gimbal Spec Sheet](https://gremsy.com) |
| **Spotlight / PA 24V Step-Up BEC** | Screw Terminals / 14AWG Wire Pads | \$35 – \$60 USD | ![Estimated](https://img.shields.io/badge/E-Estimated-708090) | ![Baseline](https://img.shields.io/badge/Status-Engineering%20Baseline-2ea44f) | [Pololu 24V Step-Up Specs](https://www.pololu.com) |
| **Fixed Camera Brackets** | M3/M4 Fasteners & Dampers | \$50 – \$100 USD (Mfg) | ![Estimated](https://img.shields.io/badge/E-Estimated-708090) | ![Estimated](https://img.shields.io/badge/Status-Estimated-708090) | Derived Machined Aluminum Estimate |
| **Smoke Canister Drop Housing** | 1x PWM Servo Line | \$45 – \$75 USD | ![Estimated](https://img.shields.io/badge/E-Estimated-708090) | ![Decision Required](https://img.shields.io/badge/Status-Decision%20Required-dfb317) | Savox SW-0250MG & Composite Spec |
| **Structural Mount Plates & Rails** | 12mm/16mm Carbon Tube Clamps | \$60 – \$120 USD | ![Estimated](https://img.shields.io/badge/E-Estimated-708090) | ![Estimated](https://img.shields.io/badge/Status-Estimated-708090) | Derived 3K Carbon Fiber Plate Estimate |
| **Wiring Harness & Connectors** | JST-GH, Molex Micro-Fit, SMA, USB-C | \$75 – \$150 USD | ![Estimated](https://img.shields.io/badge/E-Estimated-708090) | ![Estimated](https://img.shields.io/badge/Status-Estimated-708090) | Derived AWG Wiring Copper Spec |
| **Conformal Coating & IP Seals** | Chemical / Mechanical Gasket Seal | \$25 – \$40 USD | ![Estimated](https://img.shields.io/badge/E-Estimated-708090) | ![Estimated](https://img.shields.io/badge/Status-Estimated-708090) | MG Chemicals 422B Coating Spec |

---

## B. Mass Budget

### Itemized Layer Subtotals

1. **Bare Hardware Subtotal:**
   - Sum of components (Sony FCB-EV9500L 456g + FLIR Boson+ 7.5g + FLIR 19mm Lens 45g + SiOnyx 35g + TI Radar 45g + RTL-SDR v4 45g + Level 1 Antenna 35g + Mic array 50.2g + Pixhawk FC Module 23g + GPS 33g + Radar Altimeter 110g + Orin NX 29g + Strobes 55g + Reflectors 35g + PA Horn Transducer 240g + CZI Spotlight 240g):
   - **`1,483.7 g`** *(1.484 kg)*

2. **Integration & Structural Subtotal:**
   - Sum of components (Pixhawk Baseboard & Housing 65g + TWIGA Decoder 13g + CTI Hadron Carrier 60g + Heatsink & Fan 110g + TPA3116D2 Amp Board 40g + Gimbal 450g + 24V BEC 85g + Brackets 95g + Smoke housing 175g + Carbon rails 140g + Wiring 180g + Weatherproofing 40g):
   - **`1,453.0 g`** *(1.453 kg)*

3. **Total Directly Itemized Payload Mass:**
   - Bare Hardware (`1,483.7 g`) + Integration & Structure (`1,453.0 g`) = **`2,936.7 g`** *(~2.937 kg)*

4. **Candidate Reference Mass & Final Payload Envelope:**
   - **Current candidate reference configuration: 2,936.7 g**
   - *Explanation:* This figure is the direct sum of the 26 itemized candidate components (reflecting official 456g Sony camera mass, Pixhawk 6X assembly breakdown, separated TPA3116D2 audio amplifier line item, TWIGA USB decoder, Connect Tech carrier, and CZI spotlight), not an arbitrary target.
   - **Final payload mass range: TBD after architecture selection (Architecture A vs B).**

---

## C. Power Budget

### Operational Power States & PDU Rail Loss Model

Industrial DC-DC power converter efficiency is modeled at $\eta = 85\%$ average across rails ($P_{\text{loss}} = P_{\text{out}} \times [1/\eta - 1] \approx 17.65\%$ added heat loss).

| Operational Load State | Delivered Component Load | PDU Converter Heat Loss | Main Bus Power Draw | Operating Scenario / Description |
| :--- | :---: | :---: | :---: | :--- |
| **Idle / Standby Power** | 17.10 W | 3.02 W | **20.12 W** | Avionics active + Compute standby (7W TDP) + Passive RF standby |
| **Normal Search Power** | 47.20 W | 8.33 W | **55.53 W** | Full sensor array active + Level 1 RF (1.35W) + Orin NX (15W TDP) |
| **Target Verification Power** | 63.90 W | 11.28 W | **75.18 W** | Orin NX (25W TDP) + Active optical zoom verification |
| **Peak Simultaneous Load** | 178.90 W | 31.57 W | **210.47 W** | 60W CZI Spotlight + 35W PA Horn Amp + Strobes + Level 1 RF + Orin NX 25W |

> **BEC Thermal Loss Note:** The 24V Step-Up Boost BEC operates at $\eta_{\text{BEC}} = 82\%$ efficiency to supply the 24V regulated rail (60W spotlight + 35W PA amp = 95W peak load). At 95W output, internal BEC thermal loss is $20.85\text{ W} - 21.95\text{ W}$, which is accounted for within the overall PDU rail loss model ($31.57\text{ W}$ heat loss at 210.47W main bus draw).

---

## D. Candidate Reference Aircraft Configuration & Integration Audit

> [!WARNING]
> **Status:** `CANDIDATE REFERENCE CONFIGURATION / UNAPPROVED ASSUMPTION`  
> Airframe architecture is **OWNER DECISION REQUIRED** ([Research L404](assets/Docs/engineering/MANAR_V1_ENGINEERING_RESEARCH_V3.md#L404), U-01). The parameters below are provisional assumptions used for feasibility modeling, NOT finalized decisions.

| System Parameter | Provisional Baseline Value | Status / Authority | Engineering Role |
| :--- | :---: | :---: | :--- |
| **Candidate Airframe Class** | Hexacopter (6x motors/ESCs) | `PROVISIONAL ASSUMPTION` | Reference airframe model for flight dynamics |
| **Estimated Airframe Mass** | ~4.15 kg | `PROVISIONAL ASSUMPTION` | Carbon frame, motors, ESCs & props allowance |
| **Estimated Battery Mass** | ~4.50 kg | `PROVISIONAL ASSUMPTION` | 6S 1,243 Wh high-density battery pack |
| **Candidate MTOW Target** | ~11.59 kg | `PROVISIONAL ASSUMPTION` | Sum of candidate payload (2.94 kg) + airframe + battery |

### Flight Endurance Analysis & Limits

- **Simple Energy Arithmetic:** Payload normal search draw (55.53 W bus draw) consumes **27.76 Wh** over 30 minutes (only **~2.2%** of an assumed 1,243 Wh battery pack).
- **Realistic Flight Endurance:** **`TBD / REQUIRES PROPULSION SIZING & SIMULATION`**
- *Explanation:* Total flight time is overwhelmingly dominated by motor propulsion power draw (~800 W – 1,200 W hover draw). Motor efficiency, propeller sizing, aerodynamic drag, usable battery capacity, and flight controller power limits are not finalized and require propulsion simulation.

---

### Discovered Hardware Inconsistencies & Integration Risks

During physical and electrical grounding against manufacturer documentation, the following critical engineering discrepancies were identified:

1. **Sony Mass Specification Correction:**
   - *Issue:* Primitive specifications listed the Sony FCB-EV9500L camera mass at 439 g. Official Sony Technical Specifications confirm mass is approximately 456 g.
   - *Resolution:* Updated Sony camera mass to 456.0 g, adjusting bare hardware subtotal to 1,483.7 g and total payload mass to 2,936.7 g.
2. **Pixhawk 6X Configuration & Housing Assembly:**
   - *Issue:* Holybro lists FC module (23 g) and standard baseboard (51 g) separately. The combined 88 g baseline figure includes 14 g for aluminum housing/standoffs.
   - *Resolution:* Explicitly itemized the assembly breakdown ($23\text{ g}$ FC Module in Bare Hardware, $65\text{ g}$ Baseboard & Housing in Integration), downgrading verification label to `[D]` (Assembly Verified).
3. **Audio Power Amplifier Line Item Separation:**
   - *Issue:* The 15W PA horn speaker was previously listed without an explicit amplifier line item in Table A1, causing bookkeeping confusion.
   - *Resolution:* Separated into two explicit line items: Open 15W PA Horn Transducer ($240.0\text{ g}$, Bare Hardware) and TPA3116D2 Audio Amplifier Board ($40.0\text{ g}$, Integration Component).
4. **BEC Boost Power Model Normalization:**
   - *Issue:* Boost BEC peak power entry ($21.95\text{ W}$) was confused with overall PDU loss.
   - *Resolution:* Clarified that 21.95W is the internal BEC thermal loss during 95W 24V peak output ($\eta = 82\%$), which is encompassed within the overall PDU rail loss model ($31.57\text{ W}$ heat loss at 210.47W bus draw).
5. **Sony LVDS Video Conversion Interface Required:**
   - *Issue:* The Sony FCB-EV9500L outputs digital LVDS video via a 30-pin KEL connector, requiring conversion to USB 3.2 UVC.
   - *Resolution:* Added the TWIGA USB3 Neo interface board (13.0 g, 1.5 W).

---

## E. Open Decisions

Before freezing the MANAR V1 aircraft and hardware architecture, the following 6 major decisions require explicit owner resolution:

> **RESOLVED DECISION:** MANAR V1 uses Level 1 passive RF presence-detection hardware. Position-tagged RSSI measurements may support optional software spatial mapping. Exact receiver and antenna hardware remain TBD (see [`MANAR_V1_PASSIVE_RF_DECISIONS.md`](MANAR_V1_PASSIVE_RF_DECISIONS.md)).

1. **Airframe Class Selection ([Research L460](assets/Docs/engineering/MANAR_V1_ENGINEERING_RESEARCH_V3.md#L460), U-01):** Select Quadcopter vs Hexacopter vs Hybrid VTOL based on hover inspection vs range requirements.
2. **Radar Frequency & Interference Strategy ([Research L462](assets/Docs/engineering/MANAR_V1_ENGINEERING_RESEARCH_V3.md#L462), U-03):** Confirm shifting search radar from 24 GHz to 60 GHz (TI IWR6843, Option C) to eliminate mutual interference with the 24 GHz Ainstein radar altimeter.
3. **Vision Payload Architecture ([Research L463](assets/Docs/engineering/MANAR_V1_ENGINEERING_RESEARCH_V3.md#L463), U-04):** Select Architecture A (Sony 30x Zoom + FLIR Boson+, 966g) vs Architecture B (Integrated FLIR Hadron 640R EO/IR pod, 310g, saving 656g).
4. **Smoke Marker Realization ([Research L464](assets/Docs/engineering/MANAR_V1_ENGINEERING_RESEARCH_V3.md#L464), U-08):** Select mechanical pyrotechnic drop canister (fire hazard risk) vs non-pyrotechnic electronic marker array.
5. **Operational Altitude Ceiling ([Research L465](assets/Docs/engineering/MANAR_V1_ENGINEERING_RESEARCH_V3.md#L465), U-10):** Set authoritative search altitude (50m–120m AGL) and inspection altitude (15m–30m AGL) compliant with UAE GCAA 122m ceiling.
6. **Heliograph Realization ([Research L451](assets/Docs/engineering/MANAR_V1_ENGINEERING_RESEARCH_V3.md#L451), U-07):** Determine practical solar-signaling mechanism. SOLAS retroreflective panels may be evaluated separately as supplemental ground-team visibility hardware.

---

## F. Hardware Research Status

The hardware grounding audit of the MANAR Payload & System Specification Matrix produced the following status metrics:

- **Subsystems Itemized:** 26 itemized payload subsystems & integration components.
- **Real Devices Researched:** 17 commercial products & components.
- **Fully Manufacturer-Verified Candidates `[V]`:** 13 candidates ($50.0\%$) backed by exact official technical datasheets.
- **Distributor / Config / Assembly Verified `[D]`:** 4 candidates ($15.4\%$) backed by distributor specs, configuration ranges, or assembly breakdowns (FLIR Boson+ 640, SiOnyx Black Silicon, Taoglas TG.30 Antenna, Holybro Pixhawk 6X Assembly).
- **Derived Engineering Estimates `[E]`:** 9 items ($34.6\%$) clearly marked as custom mechanical, wiring, or material estimates.
- **Components Lacking Finalized Bounding Dimensions:** 3 integration items (Fixed Camera Brackets [Derived Custom], Wiring Harness [Custom 4.5m run], Conformal Coating & Seals [Thin-film/Gaskets]).
- **Components with Missing Power Data:** 0 components (100% electrically bounded).
- **Components with Missing Temperature Data:** 0 components (100% environmentally bounded).
- **Components Requiring Manufacturer Quotation:** 3 items (Sony FCB-EV9500L, FLIR Boson+ 640 OEM, CTI Hadron Carrier Board).
- **Major Newly Discovered Integration Problems:** 5 critical issues identified & resolved (Sony 456g mass update, Pixhawk 6X assembly breakdown, audio amplifier line item separation, 24V step-up BEC thermal loss, spotlight weight correction).

### Highest-Priority Remaining Hardware Research Tasks

1. **Thermal Core OEM Interface Confirmation:** Contact Teledyne FLIR to confirm whether the Boson+ 640 MIPI CSI-2 flex tail can directly interface with CTI Hadron MIPI ports or requires a dedicated FLIR VPC accessory board.
2. **Propulsion Simulation Sizing:** Execute propulsion simulation (using real payload mass of 2.94 kg) to determine hover current draw, motor selection, and realistic flight endurance.
3. **Custom Wiring Harness Breakdown:** Draft a detailed 2D wiring schematic to convert the $180\text{ g}$ wiring harness estimate into individual wire gauge lengths and connector housing masses.
