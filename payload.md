# MANAR Payload & System Specification Matrix

### Executive Summary Baseline

- **Current Itemized Payload Mass:** current candidate reference configuration: `2,981.7 g`
- **Normal Search Payload Power:** `37.23 W` *(continuous main bus draw)*
- **Peak Payload Power:** `190.49 W` *(instantaneous main bus draw)*
- **Unresolved Major Hardware Decisions:** `6 Open Engineering Decisions`

---

## A. Payload Component Matrix

*(Note: Items sorted by **Datasheet Verification** status (`[V]` Verified $\rightarrow$ `[E]` Estimated). "Datasheet Verified" indicates component specs are verified from official datasheets, NOT that the hardware is owner-approved for MANAR.)*

| Component / Subsystem | Category | Mass (g) | Continuous Power (W) | Peak Power (W) | Datasheet Verification | MANAR Integration Status | Research Citation |
| :--- | :--- | :---: | :---: | :---: | :---: | :---: | :--- |
| **Sony FCB-EV9500L Block Camera** | Visual / Day RGB | 439.0 | 8.00 | 12.00 | ![Verified](https://img.shields.io/badge/V-Verified-2ea44f) | ![Candidate](https://img.shields.io/badge/Status-Candidate%20(Arch%20A)-0052cc) | [Research L195](assets/Docs/engineering/MANAR_V1_ENGINEERING_RESEARCH_V3.md#L195) |
| **Ainstein US-D1 Radar Altimeter** | Flight Altimetry | 110.0 | 2.00 | 3.00 | ![Verified](https://img.shields.io/badge/V-Verified-2ea44f) | ![Baseline](https://img.shields.io/badge/Status-Engineering%20Baseline-2ea44f) | [Research L204](assets/Docs/engineering/MANAR_V1_ENGINEERING_RESEARCH_V3.md#L204) |
| **Holybro Pixhawk 6X Autopilot** | Flight Avionics | 88.0 | 2.00 | 2.50 | ![Verified](https://img.shields.io/badge/V-Verified-2ea44f) | ![Baseline](https://img.shields.io/badge/Status-Engineering%20Baseline-2ea44f) | [Research L202](assets/Docs/engineering/MANAR_V1_ENGINEERING_RESEARCH_V3.md#L202) |
| **Firehouse Arc V Strobe + Amber LED** | Visual Signaling | 55.0 | 6.50 | 12.00 | ![Verified](https://img.shields.io/badge/V-Verified-2ea44f) | ![Baseline](https://img.shields.io/badge/Status-Engineering%20Baseline-2ea44f) | [Research L206](assets/Docs/engineering/MANAR_V1_ENGINEERING_RESEARCH_V3.md#L206) |
| **FLIR Boson+ 640 Thermal Core** | Visual / Thermal IR | 7.5 | 3.20 | 4.00 | ![Verified](https://img.shields.io/badge/V-Verified-2ea44f) | ![Candidate](https://img.shields.io/badge/Status-Candidate%20(Arch%20A)-0052cc) | [Research L196](assets/Docs/engineering/MANAR_V1_ENGINEERING_RESEARCH_V3.md#L196) |
| **Seeed ReSpeaker v2.0 Mic Array** | Acoustic Sensing | 50.2 | 1.50 | 2.50 | ![Verified](https://img.shields.io/badge/V-Verified-2ea44f) | ![Baseline](https://img.shields.io/badge/Status-Engineering%20Baseline-2ea44f) | [Research L201](assets/Docs/engineering/MANAR_V1_ENGINEERING_RESEARCH_V3.md#L201) |
| **TI IWR6843 60 GHz mmWave Radar** | Search Radar | 45.0 | 4.50 | 6.00 | ![Verified](https://img.shields.io/badge/V-Verified-2ea44f) | ![Candidate](https://img.shields.io/badge/Status-Candidate%20(Option%20C)-0052cc) | [Research L199](assets/Docs/engineering/MANAR_V1_ENGINEERING_RESEARCH_V3.md#L199) |
| **Holybro ARK GPS CAN Module** | Flight Navigation | 33.0 | 0.65 | 0.90 | ![Verified](https://img.shields.io/badge/V-Verified-2ea44f) | ![Baseline](https://img.shields.io/badge/Status-Engineering%20Baseline-2ea44f) | [Research L203](assets/Docs/engineering/MANAR_V1_ENGINEERING_RESEARCH_V3.md#L203) |
| **NVIDIA Jetson Orin NX Module** | Perception Compute | 29.0 | 15.00 | 25.00 | ![Verified](https://img.shields.io/badge/V-Verified-2ea44f) | ![Baseline](https://img.shields.io/badge/Status-Engineering%20Baseline-2ea44f) | [Research L205](assets/Docs/engineering/MANAR_V1_ENGINEERING_RESEARCH_V3.md#L205) |
| **3-Axis Gyro EO/IR Gimbal & Dampers** | Mount / Gyro | 450.0 | 4.00 | 8.00 | ![Estimated](https://img.shields.io/badge/E-Estimated-708090) | ![Candidate](https://img.shields.io/badge/Status-Candidate%20(Arch%20A)-0052cc) | [Research L213](assets/Docs/engineering/MANAR_V1_ENGINEERING_RESEARCH_V3.md#L213) |
| **Open 15W PA Horn Speaker & Amp** | Acoustic Output | 280.0 | 0.00 | 35.00 | ![Estimated](https://img.shields.io/badge/E-Estimated-708090) | ![Candidate](https://img.shields.io/badge/Status-Candidate-0052cc) | [Research L208](assets/Docs/engineering/MANAR_V1_ENGINEERING_RESEARCH_V3.md#L208) |
| **60W High-Intensity LED Spotlight** | Scene Illumination | 180.0 | 0.00 | 60.00 | ![Estimated](https://img.shields.io/badge/E-Estimated-708090) | ![Candidate](https://img.shields.io/badge/Status-Candidate-0052cc) | [Research L209](assets/Docs/engineering/MANAR_V1_ENGINEERING_RESEARCH_V3.md#L209) |
| **Wiring Harness & Connectors** | Electrical Harness | 180.0 | 0.00 | 0.00 | ![Estimated](https://img.shields.io/badge/E-Estimated-708090) | ![Estimated](https://img.shields.io/badge/Status-Estimated-708090) | [Research L219](assets/Docs/engineering/MANAR_V1_ENGINEERING_RESEARCH_V3.md#L219) |
| **Smoke Canister Drop Housing & Servo** | Drop Mechanism | 175.0 | 0.50 | 5.00 | ![Estimated](https://img.shields.io/badge/E-Estimated-708090) | ![Decision Required](https://img.shields.io/badge/Status-Decision%20Required-dfb317) | [Research L217](assets/Docs/engineering/MANAR_V1_ENGINEERING_RESEARCH_V3.md#L217) |
| **Structural Mounting Plates & Rails** | Frame Mounts | 140.0 | 0.00 | 0.00 | ![Estimated](https://img.shields.io/badge/E-Estimated-708090) | ![Estimated](https://img.shields.io/badge/Status-Estimated-708090) | [Research L218](assets/Docs/engineering/MANAR_V1_ENGINEERING_RESEARCH_V3.md#L218) |
| **Level 1 Passive RF Receiver (Reference SDR Class)** | Passive RF Sensing | 120.0 | 1.50 | TBD | ![Estimated](https://img.shields.io/badge/E-Estimated-708090) | ![Approved TBD](https://img.shields.io/badge/Status-Approved%20%7C%20Hardware%20TBD-2ea44f) | [Research L89](assets/Docs/engineering/MANAR_V1_ENGINEERING_RESEARCH_V3.md#L89) |
| **Jetson Aluminum Heatsink & Fan** | Thermal Cooling | 110.0 | 0.00 | 1.50 | ![Estimated](https://img.shields.io/badge/E-Estimated-708090) | ![Baseline](https://img.shields.io/badge/Status-Engineering%20Baseline-2ea44f) | [Research L212](assets/Docs/engineering/MANAR_V1_ENGINEERING_RESEARCH_V3.md#L212) |
| **Fixed Camera Brackets & Enclosures** | Camera Mounts | 95.0 | 0.00 | 0.00 | ![Estimated](https://img.shields.io/badge/E-Estimated-708090) | ![Estimated](https://img.shields.io/badge/Status-Estimated-708090) | [Research L216](assets/Docs/engineering/MANAR_V1_ENGINEERING_RESEARCH_V3.md#L216) |
| **Jetson Industrial Carrier Board** | Compute Carrier | 90.0 | 2.00 | 4.00 | ![Estimated](https://img.shields.io/badge/E-Estimated-708090) | ![Baseline](https://img.shields.io/badge/Status-Engineering%20Baseline-2ea44f) | [Research L211](assets/Docs/engineering/MANAR_V1_ENGINEERING_RESEARCH_V3.md#L211) |
| **Spotlight / PA 24V Step-Up BEC** | Power Converter | 85.0 | 1.50 | 21.95 | ![Estimated](https://img.shields.io/badge/E-Estimated-708090) | ![Baseline](https://img.shields.io/badge/Status-Engineering%20Baseline-2ea44f) | [Research L215](assets/Docs/engineering/MANAR_V1_ENGINEERING_RESEARCH_V3.md#L215) |
| **LVDS-to-USB3 Decoder Board** | Video Interface | 45.0 | 1.50 | 2.50 | ![Estimated](https://img.shields.io/badge/E-Estimated-708090) | ![Candidate](https://img.shields.io/badge/Status-Candidate%20(Arch%20A)-0052cc) | [Research L210](assets/Docs/engineering/MANAR_V1_ENGINEERING_RESEARCH_V3.md#L210) |
| **FLIR Germanium Lens (19mm)** | Optical Lens | 45.0 | 0.00 | 0.00 | ![Estimated](https://img.shields.io/badge/E-Estimated-708090) | ![Candidate](https://img.shields.io/badge/Status-Candidate%20(Arch%20A)-0052cc) | [Research L197](assets/Docs/engineering/MANAR_V1_ENGINEERING_RESEARCH_V3.md#L197) |
| **Conformal Coating & IP Seals** | Weatherproofing | 40.0 | 0.00 | 0.00 | ![Estimated](https://img.shields.io/badge/E-Estimated-708090) | ![Estimated](https://img.shields.io/badge/Status-Estimated-708090) | [Research L220](assets/Docs/engineering/MANAR_V1_ENGINEERING_RESEARCH_V3.md#L220) |
| **SiOnyx Black Silicon OEM Board** | Visual / Low-Light IR | 35.0 | 2.50 | 3.50 | ![Estimated](https://img.shields.io/badge/E-Estimated-708090) | ![Candidate](https://img.shields.io/badge/Status-Candidate-0052cc) | [Research L198](assets/Docs/engineering/MANAR_V1_ENGINEERING_RESEARCH_V3.md#L198) |
| **Heliograph Reflectors (SOLAS Array)** | Passive Signaling | 35.0 | 0.00 | 0.00 | ![Estimated](https://img.shields.io/badge/E-Estimated-708090) | ![Decision Required](https://img.shields.io/badge/Status-Decision%20Required-dfb317) | [Research L207](assets/Docs/engineering/MANAR_V1_ENGINEERING_RESEARCH_V3.md#L207) |
| **Level 1 Passive RF Antenna (Multiband)** | Passive RF Antenna | 35.0 | 0.00 | 0.00 | ![Estimated](https://img.shields.io/badge/E-Estimated-708090) | ![Approved TBD](https://img.shields.io/badge/Status-Approved%20%7C%20Hardware%20TBD-2ea44f) | [Research L89](assets/Docs/engineering/MANAR_V1_ENGINEERING_RESEARCH_V3.md#L89) |

---

## B. Mass Budget

### Itemized Layer Subtotals

1. **Bare Hardware Subtotal:**
   - Sum of components (Sony camera 439g + FLIR Boson 7.5g + Lens 45g + SiOnyx 35g + Radar 45g + Level 1 RF Receiver ~120g + Level 1 Antenna ~35g + Mic array 50.2g + Pixhawk 88g + GPS 33g + Altimeter 110g + Orin NX 29g + Strobes 55g + Reflectors 35g + PA Horn 280g + Spotlight 180g):
   - **`1,586.7 g`** *(1.587 kg)*

2. **Integration & Structural Subtotal:**
   - Sum of components (Decoder 45g + Carrier board 90g + Heatsink 110g + Gimbal 450g + BEC 85g + Brackets 95g + Smoke housing 175g + Carbon rails 140g + Wiring 180g + Weatherproofing 40g):
   - **`1,395.0 g`** *(1.395 kg)*

3. **Total Directly Itemized Payload Mass:**
   - Bare Hardware (`1,586.7 g`) + Integration & Structure (`1,395.0 g`) = **`2,981.7 g`** *(~2.982 kg)*

4. **Candidate Reference Mass & Final Payload Envelope:**
   - **current candidate reference configuration: 2,981.7 g**
   - *Explanation:* This figure is the direct sum of the currently itemized candidate hardware components (reflecting the owner-approved Level 1 passive RF hardware selection), not a pre-established mass target to design toward.
   - **Final payload mass range: TBD after architecture selection.**

---

## C. Power Budget

### Operational Power States & PDU Rail Loss Model

Industrial DC-DC power converter efficiency is modeled at $\eta = 85\%$ average across rails ($P_{\text{loss}} = P_{\text{out}} \times [1/\eta - 1] \approx 17.65\%$ added heat loss).

| Operational Load State | Delivered Component Load | PDU Converter Heat Loss | Main Bus Power Draw | Operating Scenario / Description |
| :--- | :---: | :---: | :---: | :--- |
| **Idle / Baseline Power** | 12.15 W | 1.63 W | **13.78 W** | Avionics active + Compute standby (7W TDP) + Level 1 RF standby |
| **Normal Search Power** | 33.85 W | 4.38 W | **37.23 W** | Full sensor array active + Level 1 RF (~1.5W) + Orin NX (15W TDP) |
| **Target Verification Power** | 62.85 W | 9.51 W | **72.36 W** | Orin NX (25W TDP) + Active optical zoom verification |
| **Peak Simultaneous Load** | 162.90 W | 27.59 W | **190.49 W** | 60W Spotlight + 35W PA Horn + Strobes + Level 1 RF + Orin NX 25W |

> **Compute Note:** Incremental compute power for CFAR and optional spatial RSSI mapping: TBD through implementation measurement; expected to be small relative to the existing onboard compute budget.

---

## D. Candidate Reference Aircraft Configuration

> [!WARNING]
> **Status:** `CANDIDATE REFERENCE CONFIGURATION / UNAPPROVED ASSUMPTION`  
> Airframe architecture is **OWNER DECISION REQUIRED** ([Research L404](assets/Docs/engineering/MANAR_V1_ENGINEERING_RESEARCH_V3.md#L404), U-01). The parameters below are provisional assumptions used for feasibility modeling, NOT finalized decisions.

| System Parameter | Provisional Baseline Value | Status / Authority | Engineering Role |
| :--- | :---: | :---: | :--- |
| **Candidate Airframe Class** | Hexacopter (6x motors/ESCs) | `PROVISIONAL ASSUMPTION` | Reference airframe model for flight dynamics |
| **Estimated Airframe Mass** | ~4.15 kg | `PROVISIONAL ASSUMPTION` | Carbon frame, motors, ESCs & props allowance |
| **Estimated Battery Mass** | ~4.50 kg | `PROVISIONAL ASSUMPTION` | 6S 1,243 Wh high-density battery pack |
| **Candidate MTOW Target** | ~11.63 kg | `PROVISIONAL ASSUMPTION` | Sum of candidate payload (2.98 kg) + airframe + battery |

### Flight Endurance Analysis & Limits

- **Simple Energy Arithmetic:** Payload normal search draw (37.23 W) consumes **18.6 Wh** over 30 minutes (only **~1.5%** of an assumed 1,243 Wh battery pack).
- **Realistic Flight Endurance:** **`TBD / REQUIRES PROPULSION SIZING & SIMULATION`**
- *Explanation:* Total flight time is overwhelmingly dominated by motor propulsion power draw (~800 W – 1,200 W hover draw). Motor efficiency, propeller sizing, aerodynamic drag, usable battery capacity, and flight controller power limits are not finalized and require propulsion simulation.

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
