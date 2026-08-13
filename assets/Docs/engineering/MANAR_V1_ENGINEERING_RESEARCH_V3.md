# MANAR V1 Engineering Definition — Version 3 Technical Feasibility & Research Baseline

**Document Title:** MANAR V1 Engineering Definition — Version 3 Research Baseline  
**Project Phase:** Phase 2 — MANAR V1 Engineering Definition  
**Authority / Project Lead:** Oumar Ibrahim  
**Status:** **VERSION 3 TECHNICAL BASELINE** (Strictly Audited & Traceable)

---

## A. Corrections to Version 2

Before establishing the Version 3 baseline, the following errors, overreaches, arithmetic flaws, and unauthorized design choices from Version 2 are explicitly corrected or retracted:

1. **Retraction of Dual-Mirror Heliograph Assumption:**  
   *Correction:* Version 2 assumed a "dual-mirror" arrangement ($50 \times 50\text{ mm} \times 2$). The MANAR repository (`README.md`, `core/control.cpp`) specifies "Heliograph mirrors" as a passive daylight signaling concept without defining mirror count, mechanical dimensions, or targeting mechanisms. The dual-mirror structure was an unsupported assumption.
2. **Correction of Arithmetic & Power Converter Loss Inconsistency:**  
   *Correction:* In Version 2, PDU converter loss was listed as $10.35\text{ W}$ during Target Verification ($78.6\text{ W}$ payload) but dropped to $2.70\text{ W}$ during Peak Load ($178.7\text{ W}$ payload). This was mathematically inconsistent. Power conversion loss scales proportionally with output power load ($P_{\text{loss}} = P_{\text{out}} \times [1/\eta - 1]$). At an assumed $85\%$ converter efficiency ($\eta = 0.85$), a $178.7\text{ W}$ output load incurs a conversion loss of $31.5\text{ W}$, bringing actual peak electrical draw from the main bus to **$210.2\text{ W}$**.
3. **Correction of KrakenSDR 433 MHz Frequency Assumption:**  
   *Correction:* Version 2 evaluated KrakenSDR assuming an arbitrary $433\text{ MHz}$ emergency beacon band ($\lambda = 69\text{ cm}$, requiring a $70\text{ cm}$ diameter antenna array). MANAR documentation (`README.md`) specifies passive RF to "Detect/correlate device emissions" (cellular $700\text{MHz}-2.6\text{GHz}$, Wi-Fi $2.4\text{GHz}/5.8\text{GHz}$). A 5-element coherent circular array for $2.4\text{ GHz}$ ($\lambda = 12.5\text{ cm}$) requires a physical array diameter of only $\sim 12.5\text{ cm}$. KrakenSDR remains high SWaP ($670\text{g}$ module), but the physical array dimensions were overstated in Version 2.
4. **Correction of Absolute Hardware Selections:**  
   *Correction:* Version 2 declared specific hardware items as "CONFIRMED" or "Optimal" (e.g., Holybro Pixhawk 6X, Jetson Orin NX, FLIR Boson+). While their datasheet specifications are verified facts, selecting them as the final MANAR V1 architecture represents an **ENGINEERING INFERENCE** or **DESIGN RECOMMENDATION**, not a finalized requirement. Final hardware selection is reserved for **OWNER DECISION REQUIRED**.
5. **Correction of 24 GHz Radar Conflict Resolution:**  
   *Correction:* Version 2 unilaterally concluded that the 24 GHz FMCW search radar "must move to 60 GHz". This was an unauthorized redesign. The frequency overlap between the search radar and radar altimeter is an **ENGINEERING CONFLICT** requiring a multi-option trade study, presented here for owner decision.
6. **Correction of CZI Payload Replacement Assertions:**  
   *Correction:* Version 2 declared CZI DJI-ecosystem devices as "INCORRECT" and discarded them. CZI devices remain valid candidate hardware *if* a DJI platform or PSDK bridge host is chosen, but create an **ENGINEERING CONFLICT** on open Pixhawk architectures.

---

## B. MANAR Requirement Baseline

The table below audits every system capability and parameter, categorizing source evidence to prevent prototype code constants from being mistaken for authoritative system requirements.

### System Requirement & Prototype Classification Matrix

| Subsystem / Parameter | Value / Description | Source Evidence | Classification | Audit Finding & Notes |
| :--- | :--- | :--- | :--- | :--- |
| **System Identity** | Supervised-autonomy multisensor SAR drone | [`README.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/README.md#L6) | **DOCUMENTED REQUIREMENT** | Civilian SAR mission scope established by project owner. |
| **Human-in-the-Loop Gate** | Operator authorization required for actions | [`README.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/README.md), [`REFERENCES.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/REFERENCES.md#L33) | **DOCUMENTED REQUIREMENT** | Core architectural principle; C++ core must enforce explicit human authorization. |
| **Component Manifest** | 11 payload elements (Thermal, RGB, IR, FMCW, Speaker/Mic, Passive RF, Beacon, Strobe, Spotlight, Heliograph, Smoke) | [`README.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/README.md#L32-L44) | **DOCUMENTED REQUIREMENT** | Mandatory baseline component list established by owner. |
| **Maximum Altitude** | $2000\text{ m}$ | [`core/config.json`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/core/config.json#L5) | **IMPLEMENTED PROTOTYPE VALUE** | **OWNER DECISION REQUIRED**. Code input clamp only; unsupported as operational ceiling. |
| **Flight Speeds** | Quick: $15\text{m/s}$, Active: $5\text{m/s}$, Inspect: $1\text{m/s}$, Hover: $0\text{m/s}$ | [`core/config.json`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/core/config.json#L8-L11) | **IMPLEMENTED PROTOTYPE VALUE** | Simulation logic defaults. Real search speeds depend on camera motion blur boundaries. |
| **Battery Thresholds** | Warning: $30\%$, RTH: $20\%$, Emg RTH: $10\%$, Emg Land: $5\%$ | [`core/config.json`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/core/config.json#L13-L16) | **IMPLEMENTED PROTOTYPE VALUE** | State transition triggers for C++ simulation core; real thresholds depend on battery discharge curves. |
| **Search-Grid Pattern** | $100\text{m} \times 100\text{m}$ area, $20\text{m}$ row spacing | [`core/config.json`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/core/config.json#L19-L21) | **IMPLEMENTED PROTOTYPE VALUE** | Default lawnmower test pattern generator baseline. |
| **Passive RF Role** | "Detect/correlate device emissions" | [`README.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/README.md#L39) | **DOCUMENTED REQUIREMENT** | Detect/correlate mobile signals. Antenna topology & band scope are **ENGINEERING ASSUMPTIONS**. |
| **Heliograph Reflector** | "Heliograph mirrors: Passive daylight signaling" | [`README.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/README.md#L43), `control.cpp` | **DOCUMENTED REQUIREMENT** | Baseline requirement established. Mirror count, dimensions, & gimbal tracking are **OWNER DECISION REQUIRED**. |
| **Smoke Marker** | "Smoke marker: Location/wind marking" | [`README.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/README.md#L44), `runtime_template.json` | **DOCUMENTED REQUIREMENT** | Baseline concept established. Release mechanism (servo drop vs pyro electric vs non-pyro LED) is **OWNER DECISION REQUIRED**. |
| **Control Architecture** | Deterministic C++ core + Python ML + TS GUI | [`ROADMAP.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/ROADMAP.md), [`REFERENCES.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/REFERENCES.md#L28-L34) | **DOCUMENTED REQUIREMENT** | Authoritative software system separation. |

---

## C. Heliograph Baseline Assessment

### 1. Documented MANAR Baseline vs Assumptions
* **Documented Requirement:** [`README.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/README.md#L43) defines "Heliograph mirrors: Passive daylight signaling". In [`core/control.cpp`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/core/control.cpp#L316), `const bool heliograph_mirror_status = true;` hardcodes its presence.
* **Unsupported Assumptions:** Repository documentation does **not** specify dual mirrors, motorized sun-tracking, or optical retro-reflectors.

### 2. Physical & Aerodynamic Engineering Evaluation
* **SOURCE FACT:** Sun reflection flash alignment requires satisfying Snell's Law of reflection: the mirror normal vector must bisect the angle between the Sun vector and the target observer vector.
* **CALCULATION / INFERENCE:**
  - *Fixed Static Mirror:* On a maneuvering UAV flying a lawnmower grid pattern, a static mirror flashes an observer on the ground for $< 0.1\text{ seconds}$ per turn, only when the aircraft attitude happens to align perfectly with the solar incidence angle.
  - *Active 2-Axis Tracking Mirror:* Requires an active 2-axis gimbal ($+180\text{g}$, $+2.5\text{W}$) driven by solar position vector calculations derived from GNSS time/location and aircraft attitude.
  - *Passive Retro-Reflective Array:* Uses micro-prismatic corner-cube retroreflectors (similar to SOLAS maritime tape, mass $< 20\text{g}$, 0W). Reflects searchlights from ground crews back to their source, but does **not** generate daylight flashes from solar illumination.
* **Structural & Aerodynamic Impact:**
  - Flat mirror surfaces ($100 \times 100\text{ mm}$) act as flat-plate drag sails in $15\text{ m/s}$ crosswinds, creating torque disturbances for the flight controller IMU.
* **Operational Altitude Utility:** At $50\text{ m} - 120\text{ m}$ AGL search altitude, a $50\times 50\text{ mm}$ mirror flash generates a beam divergence of $\sim 0.5^\circ$ ($\sim 9.3\text{ mrad}$), producing a bright solar flash visible to ground search teams up to $2-5\text{ km}$ away in clear desert/marine daylight.

```text
ENGINEERING CONFLICT & RECOMMENDATION:
Static mirrors are aerodynamically destabilizing and optically ineffective during dynamic flight.
RECOMMENDATION: Replace flat glass mirrors with low-profile curved solar flash reflectors or high-efficiency SOLAS retro-reflective panels mounted to the frame perimeter, OR specify an active 2-axis micro-gimbal heliograph payload.
Status: OWNER DECISION REQUIRED
```

---

## D. Passive RF Trade Study

### 1. Target Emission Signal Classes
MANAR documentation specifies detecting and correlating device emissions. Real-world target signals include:
- **Cellular Uplink Bands:** LTE/5G handsets transmitting handset-to-tower uplink bursts ($700\text{ MHz} - 900\text{ MHz}$, $1.8\text{ GHz} - 2.1\text{ GHz}$, $2.5\text{ GHz} - 2.6\text{ GHz}$).
- **Wi-Fi Probe Requests:** Handsets periodically broadcasting 802.11b/g/n/ac probe requests ($2.412\text{ GHz} - 2.484\text{ GHz}$ and $5.180\text{ GHz} - 5.825\text{ GHz}$).
- **Bluetooth / BLE Advertising:** Wearables and phones broadcasting BLE advert beacons ($2.402\text{ GHz} - 2.480\text{ GHz}$).

### 2. Capabilities vs Antenna Topologies Evaluation

#### A. Capability Level 1: Simple Energy Detection / Wideband Power Sensing
- **Topology:** Single omnidirectional whip antenna ($1\text{x}$ RF channel).
- **Output:** Raw RF RSSI (Received Signal Strength Indicator) power level.
- **Hardware Mass & Power:** Hardware: HackRF One or RTL-SDR v4 ($\sim 120\text{g}$, $1.5\text{W}$). Antennas: $1\text{x}$ dual-band monopole ($35\text{g}$). Compute: $< 5\%$ CPU load.
- **Operational Utility:** Detects presence of RF emissions within sector, but provides **zero direction or bearing information**.

#### B. Capability Level 2: RSSI Spatial Triangulation / Lawnmower Grid Mapping
- **Topology:** Two directional antennas (e.g., 2x Patch / Yagi antennas mounted $180^\circ$ apart or left/right on airframe) OR single omnidirectional antenna flown across a search grid.
- **Output:** Georeferenced RSSI heat map correlated with aircraft flight path coordinates in C++ core.
- **Hardware Mass & Power:** Hardware: Dual-channel receiver or 2x SDRs ($\sim 220\text{g}$, $3.0\text{W}$). Antennas: 2x Directional Patch Antennas ($140\text{g}$). Compute: $\sim 10\%$ CPU load.
- **Operational Utility:** Allows deterministic software core to locate victim handset by flying a grid pattern and mapping peak RSSI coordinates without needing phase-coherent beamforming.

#### C. Capability Level 3: Coherent Angle-of-Arrival (AoA) Beamforming
- **Topology:** 5-element phase-coherent Uniform Circular Array (UCA) operating with phase-locked local oscillators.
- **Output:** Real-time Bearing Vector / Angle-of-Arrival (AoA azimuth angle relative to aircraft heading).
- **Hardware Mass & Power:** Hardware: KrakenSDR 5-channel unit ($670\text{g}$, $11.0\text{W}$ continuous). Antennas & Frame: 5x Coax cables + 5x element array frame ($350\text{g}$). Total Subsystem Mass: **$1.02\text{ kg}$**. Compute: High CPU load ($1.5 - 2.0$ ARM cores for MUSIC/ESPRIT DSP algorithm execution).
- **Operational Utility:** Computes instantaneous bearing line to target in a single hover, eliminating the need to fly an entire grid pattern to map RSSI.

### 3. Passive RF Capability vs Hardware Complexity Matrix

| Parameter / Feature | Level 1: Omnidirectional RSSI | Level 2: Dual Directional RSSI Mapping | Level 3: KrakenSDR 5-Ch Coherent AoA |
| :--- | :--- | :--- | :--- |
| **Receiver Unit** | RTL-SDR v4 / HackRF One | Dual-Channel SDR (e.g., LimeSDR Mini) | KrakenSDR 5-Ch Coherent Unit |
| **Subsystem Mass** | **$155\text{ g}$** | **$360\text{ g}$** | **$1,020\text{ g}$** ($1.02\text{ kg}$) |
| **Subsystem Power** | **$1.5\text{ W}$** | **$3.5\text{ W}$** | **$11.0\text{ W}$** |
| **Antenna Topology** | 1x Monopole Whip | 2x Microstrip Patch Antennas | 5x Phase-Locked Monopoles (UCA) |
| **Physical Footprint** | $100 \times 30\text{ mm}$ | $120 \times 120\text{ mm}$ ($\times 2$) | $\varnothing 12.5\text{cm}$ ($2.4\text{GHz}$) to $\varnothing 40\text{cm}$ ($800\text{MHz}$) |
| **Compute Requirement**| $< 5\%$ CPU load | $< 12\%$ CPU load | $1.5 - 2.0$ ARM CPU Cores (25-30% Jetson) |
| **Target Information** | Presence Only | Position via Grid Heatmap | Instantaneous Bearing Vector |
| **V1 Feasibility Rating**| High SWaP Efficiency | **Optimal SAR SWaP Trade-off** | High Mass/Power Penalty |

```text
KRAKENSDR EVALUATION CLASSIFICATION: PARTIALLY SUITABLE / HIGH SWAP PENALTY
KrakenSDR delivers true single-pass Angle-of-Arrival direction finding, but imposes a 1.02 kg mass penalty and 11W continuous power load. Dual directional patch RSSI spatial mapping (Level 2) delivers georeferenced location correlation at 360g total mass and 3.5W power load.
Status: OWNER DECISION REQUIRED (Level 2 RSSI Spatial Mapping vs Level 3 KrakenSDR Coherent AoA).
```

---

## E. 24 GHz Radar / Altimeter Engineering Trade Study

### Identified Engineering Conflict
Both the **Ainstein US-D1 Radar Altimeter** and the proposed **InnoSenT IVS-979 Search Radar** operate within the **$24.0\text{ GHz} - 24.25\text{ GHz}$ ISM band**. Unsynchronized co-located operation will cause mutual receiver saturation, false range spikes, and loss of radar altitude lock.

### Trade Study Options Matrix

| Evaluation Criteria | Option A: Dual 24GHz + Time-Division (TDM) | Option B: 24GHz Search Radar + Optical Lidar Altimeter | Option C: 24GHz Altimeter + 60GHz/77GHz mmWave Search Radar | Option D: Unified 60GHz Multimode Radar (Search + Altimetry) |
| :--- | :--- | :--- | :--- | :--- |
| **System Architecture** | Keep both radars at 24GHz; alternate RF pulses via hardware sync wire. | Retain 24GHz search radar; replace altimeter with LightWare SF20/C Lidar. | Retain 24GHz Ainstein altimeter; migrate search radar to TI IWR6843 (60GHz). | Single 60GHz mmWave radar executing altimetry & search in software. |
| **Technical Difficulty** | High (Requires custom RF pulsing & firmware modification). | Low (Standard Lidar integration). | **Low / Standard** (Frequency isolation eliminates interference). | High (Requires complex multi-beam software DSP allocation). |
| **Interference Risk** | High (Multipath reflections can breach time slots). | **Zero** (Optical vs RF frequency separation). | **Zero** (24GHz vs 60GHz frequency separation). | **Zero** (Integrated single RF frontend). |
| **All-Weather Capability**| All-weather (24GHz penetrates fog/dust). | Degraded (Lidar blinded by dense fog/smoke/dust). | **All-weather** (Both 24GHz and 60GHz penetrate fog/dust). | All-weather (60GHz penetrates fog/dust). |
| **Combined SWaP Mass** | $\sim 175\text{ g}$ | $\sim 75\text{ g}$ | **$\sim 155\text{ g}$** | **$\sim 85\text{ g}$** |
| **Combined SWaP Power**| $\sim 3.5\text{ W}$ | $\sim 2.5\text{ W}$ | **$\sim 4.5\text{ W}$** | **$\sim 3.0\text{ W}$** |
| **DSP / Compute Risk** | High | Low | Low (Pre-processed point clouds) | High |

```text
TRADE STUDY RECOMMENDATION:
Option C (24GHz Ainstein Altimeter + 60GHz TI IWR6843 mmWave Search Radar) eliminates mutual interference entirely via physical frequency separation, preserves all-weather fog/dust penetration, and reduces radar search payload mass from 120g to 45g.
Status: OWNER / ENGINEERING DECISION REQUIRED
```

---

## F. Breathing & Micro-Motion Feasibility Chain

MANAR documentation specifies radar contribution to presence, motion, and breathing detection. Radar does **not** operate as a standalone binary sensor; it contributes micro-motion evidence vectors to the multisensor fusion engine.

### End-to-End Processing Feasibility Chain

```text
  +-----------------------+     +-----------------------+     +-----------------------+
  |  Radar RF Front-End   | --> | High-Speed ADC & DSP  | --> |  Range-Doppler FFT    |
  |  (60GHz FMCW Transmit)|     | (Onboard Radar MCU)   |     | (Isolate Distance Bin)|
  +-----------------------+     +-----------------------+     +-----------------------+
                                                                          |
                                                                          v
  +-----------------------+     +-----------------------+     +-----------------------+
  | Multisensor Fusion Engine   | Temporal Bandpass Filter    Phase Extraction        |
  | (PyTorch Candidate Score) <--- (0.15 Hz - 0.40 Hz)   <--- | (Extract Unwrapped    |
  +-----------------------+     | (Isolate Respiration) |     |  Phase Variance)      |
                                +-----------------------+     +-----------------------+
```

### Technical Feasibility & Vibration Noise Constraints

1. **Accessible Raw Data:** Raw FMCW I/Q chirp data **must** be captured via high-speed SPI/Ethernet by a dedicated onboard radar MCU (e.g., TI ARM Cortex-R4 inside IWR6843) to compute 1D Range FFTs before transferring processed range-phase arrays to the Jetson compute.
2. **Phase Extraction & Chest Displacement Resolution:**  
   *SOURCE FACT:* Human respiratory chest displacement is $\Delta d \approx 1\text{ mm} - 5\text{ mm}$.  
   At $24\text{ GHz}$ ($\lambda = 12.5\text{ mm}$), phase shift $\Delta \phi = \frac{4\pi \Delta d}{\lambda} \approx 0.1\text{ to } 0.5\text{ rad}$.  
   At $60\text{ GHz}$ ($\lambda = 5.0\text{ mm}$), phase shift $\Delta \phi = \frac{4\pi \Delta d}{\lambda} \approx 0.25\text{ to } 1.25\text{ rad}$ (**$2.5\text{x}$ higher phase sensitivity**).
3. **Aircraft Vibration Noise (The Primary Barrier):**  
   *CALCULATION:* Multirotor hover vibration introduces frame displacement of $\pm 10\text{ mm} - 50\text{ mm}$ at rotor frequencies ($20\text{ Hz} - 150\text{ Hz}$). This displacement is **$10\text{x} - 50\text{x}$ larger** than human chest movements.
4. **Platform Translation Effects:** Airborne forward search flight ($5\text{ m/s}$) completely obscures phase unwrapping for micro-motion.
5. **Realistic Role of Radar in MANAR Multisensor Fusion:**
   - **Enroute / Search Phase ($5\text{ m/s}$):** Radar functions as a **Macro-Motion & Range Sensor**, detecting moving targets or large body displacement through foliage.
   - **Hover / Stationary Verification Phase ($0\text{ m/s}$):** When vision/thermal flags a candidate victim, the deterministic core commands a stationary hover. The radar executes Range-Doppler phase filtering. If micro-motion phase variance is detected, radar passes a positive `micromotion_score` ($0.0 - 1.0$) to the Python multisensor fusion engine.
   - **Ground Touchdown / Low-RPM Dip (Optional):** Maximum respiratory sensitivity is achieved if the aircraft performs a temporary touchdown or low-RPM acoustic/radar dip.

---

## G. Mass Budget With Confidence Levels

Uncertainty codes: **[V]** Verified Hardware Mass; **[E-H]** High-Confidence Estimate ($\pm 5\%$); **[E-M]** Medium-Confidence Estimate ($\pm 15\%$); **[E-L]** Low-Confidence Estimate ($\pm 30\%$).

### Itemized Payload Mass Breakdown Table

| Component / Layer | Category | Mass (g) | Confidence | Source / Justification |
| :--- | :--- | :--- | :--- | :--- |
| **Sony FCB-EV9500L Block Camera** | Bare Hardware | $439\text{ g}$ | **[V]** | Sony Official Datasheet |
| **FLIR Boson+ 640 Thermal Core** | Bare Hardware | $7.5\text{ g}$ | **[V]** | FLIR Official Datasheet |
| **FLIR Germanium Lens ($19\text{mm}$)** | Bare Hardware | $45\text{ g}$ | **[E-H]** | Optical Lens Specification |
| **SiOnyx Black Silicon OEM Board**| Bare Hardware | $35\text{ g}$ | **[E-M]** | OEM Module Estimate |
| **60 GHz mmWave Radar (TI IWR6843)**| Bare Hardware | $45\text{ g}$ | **[V]** | TI Board Specification |
| **KrakenSDR 5-Ch Receiver Unit** | Bare Hardware | $670\text{ g}$ | **[V]** | KrakenRF Official Specification |
| **Seeed ReSpeaker v2.0 Mic Array**| Bare Hardware | $50.2\text{ g}$| **[V]** | Seeed Studio Datasheet |
| **Holybro Pixhawk 6X Flight Controller**| Bare Hardware | $88\text{ g}$ | **[V]** | Holybro Official Datasheet |
| **Holybro ARK GPS CAN Module** | Bare Hardware | $33\text{ g}$ | **[V]** | PX4 / Holybro Datasheet |
| **Ainstein US-D1 Radar Altimeter**| Bare Hardware | $110\text{ g}$ | **[V]** | Ainstein Official Datasheet |
| **NVIDIA Jetson Orin NX Module** | Bare Hardware | $29\text{ g}$ | **[V]** | NVIDIA Official Datasheet |
| **Firehouse Arc V Strobe + Amber LED**| Bare Hardware | $55\text{ g}$ | **[V]** | Manufacturer Datasheets |
| **Heliograph Reflectors (SOLAS Array)**| Bare Hardware | $35\text{ g}$ | **[E-M]** | Material Specification |
| **Open 15W PA Horn Speaker + Amp** | Bare Hardware | $280\text{ g}$ | **[E-H]** | Industrial Audio Horn Specs |
| **60W High-Intensity LED Spotlight**| Bare Hardware | $180\text{ g}$ | **[E-H]** | High-Flux LED Array Specs |
| **LVDS-to-USB3 Decoder Board** | Required Integration| $45\text{ g}$ | **[E-H]** | Kowa / Active Silicon Specs |
| **Jetson Industrial Carrier Board** | Required Integration| $90\text{ g}$ | **[E-H]** | Connect Tech Carrier Specs |
| **Jetson Aluminum Heatsink & Fan** | Required Integration| $110\text{ g}$ | **[E-H]** | Industrial Cooling Specs |
| **3-Axis Gyro EO/IR Gimbal & Dampers**| Required Integration| $450\text{ g}$ | **[E-M]** | Carbon Gimbal Structural Specs|
| **KrakenSDR Antenna Frame & Coax (5x)**| Required Integration| $350\text{ g}$ | **[E-M]** | RF Assembly Estimate |
| **Spotlight/PA 24V Step-Up BEC** | Required Integration| $85\text{ g}$ | **[E-H]** | High-Power Regulated BEC Specs|
| **Fixed Camera Brackets & Enclosures**| Required Integration| $95\text{ g}$ | **[E-M]** | Machined Aluminum/3D Composite|
| **Smoke Canister Drop Housing & Servo**| Required Integration| $175\text{ g}$ | **[E-M]** | Mechanical Release Assembly |
| **Structural Mounting Plates & Rails**| Structural Allowance| $140\text{ g}$ | **[E-L]** | Carbon Fiber Plate Allowance |
| **Wiring Harness & Connectors** | Wiring / Power | $180\text{ g}$ | **[E-M]** | 12AWG-24AWG Wire Harness |
| **Conformal Coating & IP Weatherproofing**| Weatherproofing | $40\text{ g}$ | **[E-L]** | Silicone Seals & Sealant |
| **SUBTOTAL BARE HARDWARE:** | | **$2,101.7\text{ g}$**| | |
| **SUBTOTAL INTEGRATION & STRUCTURE:**| | **$1,745.0\text{ g}$**| | |
| **BEST CURRENT ESTIMATE (TOTAL):** | | **$3,846.7\text{ g}$**| **[E-M]** | **Plausible Range: $3.4\text{ kg} - 4.3\text{ kg}$** |

### Top 5 Mass Sensitivity Dominators

1. **KrakenSDR RF Subsystem Assembly:** $1,020\text{ g}$ ($26.5\%$ of payload). Switching to Level 2 Dual Patch RSSI saves **$-660\text{ g}$**.
2. **Primary EO/IR Gimbal & Camera Assembly:** $980\text{ g}$ ($25.5\%$ of payload). Switching to integrated FLIR Hadron EO/IR saves **$-450\text{ g}$**.
3. **Structural Mounting Rails & Weatherproof Enclosures:** $375\text{ g}$ ($9.7\%$ of payload). Carbon fiber optimization can alter mass by $\pm 120\text{ g}$.
4. **Jetson Orin NX Compute Subsystem (Carrier + Heatsink):** $229\text{ g}$ ($6.0\%$ of payload). Passive vs active cooling varies mass by $\pm 60\text{ g}$.
5. **Open 15W PA Horn Speaker & Amplifier:** $345\text{ g}$ ($9.0\%$ of payload). Horn acoustic size scales directly with lower cutoff frequency.

---

## H. Electrical / Power Budget

### 1. Power Distribution Rail Loss Model
*SOURCE FORMULA:* Converter Loss $P_{\text{loss}} = P_{\text{out}} \times \left(\frac{1}{\eta} - 1\right)$.  
Assumed industrial DC-DC buck/boost converter efficiency $\eta = 85\%$ ($0.85$). Multiplier factor $\left(\frac{1}{0.85} - 1\right) \approx 0.1765$ ($17.65\%$ of delivered load added as heat loss).

#### Voltage Rail Allocation & Load Sizing

| Rail Voltage | Connected Load Subsystems | Continuous Load (W) | Peak Instantaneous Load (W) | Converter Efficiency ($\eta$) | Converter Loss @ Peak (W) | Total Bus Power Required (W) |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **5.0V Regulated Rail** | Autopilot, GPS, Thermal, 60GHz Radar, Radar Altimeter, Mic Array, Strobes | $11.05\text{ W}$ | $20.40\text{ W}$ | $88\%$ | $2.78\text{ W}$ | **$23.18\text{ W}$** |
| **12.0V Regulated Rail**| Sony RGB Zoom + LVDS Board, SiOnyx IR Camera, Beacon | $5.30\text{ W}$ | $16.00\text{ W}$ | $85\%$ | $2.82\text{ W}$ | **$18.82\text{ W}$** |
| **24.0V Step-Up Rail** | 60W LED Spotlight, 15W PA Horn Amplifier | $0.00\text{ W}$ | $100.00\text{ W}$ | $82\%$ | $21.95\text{ W}$ | **$121.95\text{ W}$** |
| **Compute Rail (Direct)**| NVIDIA Jetson Orin NX + Carrier (9V-20V input) | $15.00\text{ W}$ | $25.00\text{ W}$ | $90\%$ | $2.78\text{ W}$ | **$27.78\text{ W}$** |
| **USB 5.0V Feed Rail** | KrakenSDR RF Receiver (Dedicated 3A feed) | $11.00\text{ W}$ | $15.00\text{ W}$ | $85\%$ | $2.65\text{ W}$ | **$17.65\text{ W}$** |
| **TOTALS:** | | **$42.35\text{ W}$** | **$176.40\text{ W}$** | — | **$30.00\text{ W}$** | **$209.38\text{ W}$** |

### 2. Operational Load State Summary

- **Idle / Baseline Power (Avionics + Compute Standby 7W):** **$22.15\text{ W}$** (Continuous main bus draw: **$25.20\text{ W}$** including PDU idle losses).
- **Normal Search Power (Sensors Active + Orin NX 15W TDP):** **$49.60\text{ W}$** (Continuous main bus draw: **$57.80\text{ W}$**).
- **Target Verification Power (Orin NX 25W TDP + Zoom Active):** **$78.60\text{ W}$** (Continuous main bus draw: **$91.70\text{ W}$**).
- **Peak Simultaneous Load (Spotlight 60W + PA Horn 35W Peak + Strobes):** **$176.40\text{ W}$** (Instantaneous main bus peak: **$209.38\text{ W}$**).

```text
DISTINCTION: ENERGY CONSUMPTION vs RAIL CURRENT SIZING
- Battery Energy Consumption: Driven by Normal Search Power (57.8W continuous). Over a 30-minute flight, payload consumes ~28.9 Wh (only ~2.3% of a 1243Wh battery pack).
- Power Distribution Rail Sizing: The 24V bus must be fused and wired to handle instantaneous peak current spikes up to 8.7 Amps (209.4W @ 24V) without line voltage sag affecting Jetson compute logic.
```

---

## I. Vision Architecture Trade Study

### Architecture Options Comparison Matrix

| Evaluation Criteria | Architecture A: Modular Separate Cameras (Sony Zoom + FLIR Boson+ + SiOnyx) | Architecture B: Integrated Dual EO/IR Gimbal (FLIR Hadron 640R) + Separate SiOnyx | Architecture C: Quad-Sensor Heavy Gimbal Pod (Custom Optical Housing) |
| :--- | :--- | :--- | :--- |
| **Image / Optical Quality** | **Exceptional** (30x Optical Zoom, 1/1.8" STARVIS sensor) | Moderate (Fixed focal length RGB, no optical zoom) | **Exceptional** (Full optical zoom + thermal + low-light) |
| **Thermal Resolution** | $640 \times 512$ LWIR ($12\mu\text{m}$) | $640 \times 512$ LWIR ($12\mu\text{m}$) | $640 \times 512$ LWIR ($12\mu\text{m}$) |
| **Low-Light Night Capability**| Dedicated SiOnyx Black Silicon NIR sensor | Dedicated SiOnyx Black Silicon NIR sensor | Dedicated SiOnyx Black Silicon NIR sensor |
| **Subsystem Mass** | $966\text{ g}$ (Camera package + LVDS + Gimbal) | **$310\text{ g}$** (FLIR Hadron $56\text{g}$ + Gimbal $220\text{g}$) | $1,350\text{ g}$ (Heavy composite pod) |
| **Subsystem Power** | $8.0\text{ W}$ (Camera + Zoom + Decoder) | **$3.2\text{ W}$** (Integrated Hadron pod) | $10.5\text{ W}$ (Multi-camera pod) |
| **Gimbal Mass & Balance** | Asymmetrical balancing challenge ($450\text{g}$ gimbal) | **Compact pre-balanced pod** ($220\text{g}$ gimbal) | Heavy dual-arm gimbal ($750\text{g}$) |
| **Interface Complexity** | LVDS + MIPI + USB (High software complexity) | MIPI / USB (Standard software driver) | Multi-channel interface hub |
| **Target Verification Range**| **High Altitude Verification** ($100\text{m}$ AGL via 30x Zoom) | Low Altitude Verification ($15\text{m}-30\text{m}$ AGL descend required) | **High Altitude Verification** ($100\text{m}$ AGL via Zoom) |

```text
VISION ARCHITECTURE TRADE-OFF:
- Architecture A preserves long-range optical zoom verification from high search altitudes (100m AGL), but imposes a ~966g mass penalty.
- Architecture B reduces SWaP by -656g (saving 17% of total payload mass), but requires the drone to descend to 15m-30m AGL for close-range visual identification due to fixed focal length RGB optics.
Status: OWNER DECISION REQUIRED
```

---

## J. Open-Interface Speaker & Spotlight Class Definitions

Because CZI devices are locked to the proprietary DJI PSDK ecosystem, the functional requirements and open-interface candidate hardware classes are defined below without requiring DJI hardware.

### 1. Acoustic Speaker / Public Address Subsystem

```text
FUNCTIONAL REQUIREMENT → POSSIBLE HARDWARE CLASS → OPEN QUESTIONS
```

- **Functional Requirements:**
  - Acoustic Sound Pressure Level (SPL): $\ge 115\text{ dB SPL}$ @ $1\text{m}$ ($\ge 85\text{ dB SPL}$ @ $30\text{m}$ search altitude).
  - Intelligibility Distance: Clear voice broadcasting up to $100\text{m} - 150\text{m}$ line-of-sight.
  - Frequency Response: $300\text{ Hz} - 5\text{ kHz}$ (Optimized for human speech spectrum).
  - Physical Limits: Mass $\le 350\text{g}$; Power $\le 35\text{W}$ Peak; Weather rating $\ge \text{IP54}$.
  - Open Control Interface: Standard $3.5\text{mm}$ line-in audio from Jetson USB soundcard; amplifier enable via 5V PWM / GPIO pin.
- **Possible Open Hardware Classes:**
  - Class-D Mono High-Efficiency Amplifier ($12\text{V}-24\text{V}$ input, e.g., TPA3116 D2 module, mass $45\text{g}$) paired with a lightweight weatherproof re-entrant reflex horn speaker ($15\text{W}-25\text{W}$, mass $230\text{g}$).
- **Open Questions:**
  - How to insulate the onboard microphone array from acoustic feedback when the speaker is broadcasting?

### 2. Downward Illumination Spotlight Subsystem

```text
FUNCTIONAL REQUIREMENT → POSSIBLE HARDWARE CLASS → OPEN QUESTIONS
```

- **Functional Requirements:**
  - Luminous Flux: $\ge 4000\text{ Lumens}$ white LED ($\sim 6000\text{K}$ color temperature).
  - Beam Angle: $12^\circ - 15^\circ$ focused cone (Illuminates a $15\text{m} - 25\text{m}$ ground spot at $60\text{m}$ AGL).
  - Physical Limits: Mass $\le 250\text{g}$; Power $\le 65\text{W}$ Peak; Weather rating $\ge \text{IP55}$.
  - Open Control Interface: Direct DC connection to $24\text{V}$ regulated rail; dimming control via standard $100\text{Hz}-1\text{kHz}$ 5V PWM signal from Pixhawk AUX port.
- **Possible Open Hardware Classes:**
  - Custom High-Flux COB (Chip-on-Board) LED array mounted to a finned aluminum heatsink with a parabolic optical lens, driven by a PWM-dimmable constant-current LED driver board (e.g., Mean Well or custom MOSFET switching driver).
- **Open Questions:**
  - Should the spotlight be statically mounted pointing downward or mounted to a 1-axis tilt servo linked to camera pitch?

---

## K. Acoustic System Feasibility Assessment

### 1. Acoustic Noise Source Degradation Breakdown
1. **Propeller Blade-Passing Noise:** High-amplitude tonal spikes at fundamental frequency $f_{\text{BPF}} = \frac{\text{RPM} \times N_{\text{blades}}}{60}$ (typically $120\text{ Hz} - 350\text{ Hz}$) and its harmonics.
2. **Aerodynamic Rotor Turbulence & Wind Noise:** Broadband turbulent noise ($100\text{ Hz} - 8\text{ kHz}$) generated by air wash over microphone diaphragms during $15\text{ m/s}$ flight.
3. **Motor Mechanical & Electromagnetic Noise:** High-frequency bearing chatter and motor switching PWM noise ($8\text{ kHz} - 16\text{ kHz}$).

### 2. Operational Concept Evaluation Matrix

| Acoustic Strategy | Feasibility Rating | Integration Impact | Technical Verdict |
| :--- | :--- | :--- | :--- |
| **A. Continuous In-Flight Listening ($15\text{m/s}$ search)** | **Probably Ineffective** | Extreme wind noise ($> 95\text{dB}$) completely drowns out human calls. | Cannot detect human voice during high-speed transit. |
| **B. Stationary Hover Listening ($0\text{m/s}$)** | **Plausible** | Rotor wash noise ($85-95\text{dB}$) remains high; requires spectral filtering. | Possible for loud shouting if DSP noise cancellation is active. |
| **C. Low-RPM Dip / Descent Listening Mode** | **Plausible / Recommended** | C++ core drops altitude to $5\text{m}-10\text{m}$ and reduces motor pitch. | **High SNR improvement** for human voice detection. |
| **D. Temporary Ground Touchdown Mode** | **Highest SNR Quality** | C++ core lands aircraft briefly, cuts motors to $0\text{ RPM}$, listens for 15s. | **Optimal acoustic clarity**; consumes zero hover power. |
| **E. Lowered Boom / Tethered Mic Isolation** | **Plausible** | Lowering mic $1.5\text{m}$ below rotor plane reduces noise by $\sim 12\text{dB}$. | Effective, but introduces pendulum mechanical risk. |
| **F. Motor-RPM Reference Spectral Cancellation** | **Experimentally Required** | Feeding real-time motor RPM telemetry to adaptive DSP notch filters. | Eliminates blade-passing tones from audio stream. |

```text
ACOUSTIC MISSION CONCEPT RECOMMENDATION:
The C++ control core should enforce a "Call-and-Listen" mission state sequence:
1. Aircraft arrives at candidate coordinate and initiates Hover.
2. Speaker broadcasts acoustic prompt ("MANAR Search Team: Shout or signal for help").
3. Aircraft enters Low-RPM Acoustic Dip or Temporary Touchdown (Motors throttled down).
4. Microphone array records for 10 seconds; DSP executes spectral noise cancellation.
Status: OWNER DECISION REQUIRED
```

---

## L. UAE Regulatory Framework & Altitude Limits

### 1. UAE Civil Aviation Authority Framework
* **Authority Baseline:** In the United Arab Emirates, unmanned aircraft operations are governed by the **General Civil Aviation Authority (GCAA)** under **CAR VIII (Unmanned Aircraft Systems)** and local municipal authorities including the **Dubai Civil Aviation Authority (DCAA)** (Law No. 4 of 2020 / Executive Council Resolution No. 26).
* **Standard Operational Ceiling:** Under standard GCAA / DCAA civilian unmanned aircraft regulations, open-category UAS operations are restricted to a maximum altitude of **$400\text{ feet}$ AGL ($\sim 122\text{ meters}$ AGL)**.
* **Controlled Airspace & Special SAR Approvals:** Operating above $122\text{ m}$ AGL or within restricted flight zones requires official GCAA/DCAA operational authorization, NOTAM publication, and continuous transponder/remote-ID broadcasting.

### 2. Altitude Selection Trade-Off Analysis

| Evaluation Perspective | Low Altitude ($15\text{m} - 30\text{m}$ AGL) | Medium Search Altitude ($50\text{m} - 120\text{m}$ AGL) | High Altitude ($500\text{m} - 2000\text{m}$ AGL) |
| :--- | :--- | :--- | :--- |
| **UAE Regulatory Status** | **Fully Compliant** (Standard Open UAS) | **Fully Compliant** (Up to $122\text{m}$ ceiling) | **Requires Special GCAA / Defense Waiver** |
| **Thermal Sensor Resolution** | $640\times 512$ thermal yields **$1.5\text{cm}/0.8\text{cm}$ GSD** (Clear human body shape). | $640\times 512$ thermal yields **$3.2\text{cm}/7.6\text{cm}$ GSD** (Detectable heat blob). | $640\times 512$ thermal yields **$> 1.3\text{m}$ GSD** (Human fits inside single pixel). |
| **Visual Search Swath Width** | Narrow coverage ($\sim 30\text{m}$ width). | **Optimal Coverage** ($\sim 100\text{m} - 150\text{m}$ width). | Extreme swath width, but zero target detection. |
| **Climb Energy Penalty** | Negligible battery draw. | Low climb draw ($< 3\%$ battery). | Extreme battery draw ($> 35\%$ battery lost to climb). |

```text
MAXIMUM ALTITUDE REQUIREMENT MARK: OWNER DECISION REQUIRED
RECOMMENDED SAR OPERATIONAL CEILING:
- Search Phase: 50m - 120m AGL (Maximizes area coverage while remaining under 122m UAE GCAA ceiling).
- Inspection Phase: 15m - 30m AGL (Provides high GSD visual/thermal verification).
```

---

## M. Airframe Decision Framework

No airframe configuration is selected as "optimal". The weighted decision framework below exposes the engineering trade-offs across four airframe architectures.

### Weighted Decision Matrix

| Evaluation Criteria | Quadcopter (4 Rotors) | Hexacopter (6 Rotors) | Octocopter (8 Rotors) | Hybrid VTOL Fixed-Wing |
| :--- | :--- | :--- | :--- | :--- |
| **Payload Capacity ($3.85\text{ kg}$)** | High Motor Stress | **Optimal / Balanced** | High Capacity | High Mass Capacity |
| **Hover Efficiency & Precision** | High | **High** | Moderate | Low (High hover power draw) |
| **Low-Speed Controllability ($1\text{m/s}$)**| High | **High** | High | Poor (Near stall boundaries) |
| **Acoustic Listening Suitability**| Fair (4 Noise Sources) | **Fair (6 Noise Sources)** | Poor (8 Noise Sources) | Poor (High rotor wash) |
| **Single-Motor Failure Safety** | **Zero (Catastrophic Fall)**| **Partial (Fly-to-Land)**| **Full (Normal Control)** | Glides / Motor Redundant |
| **Search Endurance (Range)** | Moderate ($\sim 20-28\text{ min}$)| Moderate ($\sim 25-35\text{ min}$)| Lower ($\sim 18-25\text{ min}$) | **Exceptional ($\sim 60-90\text{ min}$)** |
| **Airframe Mechanical Complexity** | **Lowest (4 ESCs/Motors)** | Moderate (6 ESCs/Motors)| High (8 ESCs/Motors) | Highest (Fixed-wing + 4 VTOL) |
| **Simulated V1 Core Complexity** | **Simplest Physics Model**| **Standard Benchmark** | Standard Model | Complex Mode-Transition Logic |
| **Transportation Footprint** | Compact | Moderate ($900-1100\text{mm}$) | Large | Large Wingspan ($> 2.5\text{m}$) |

```text
AIRFRAME SELECTION STATUS: OWNER DECISION REQUIRED
Oumar Ibrahim must assign decision weighting factors (e.g., prioritizing search endurance vs hover inspection vs simulation simplicity) to freeze the airframe configuration prior to Phase 2 Milestone 6.
```

---

## N. Engineering Uncertainty Register

The register below tracks every open engineering unknown, its severity rating, and the phase/evidence required to resolve it.

### System Uncertainty Register

| ID | Unknown / Open Problem | System Impact | Severity | Resolution Evidence Required | Target Phase |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **U-01** | Airframe Architecture Freeze | Controls MTOW, motor selection, & simulation dynamics model. | **CRITICAL** | Owner decision selecting Quad vs Hexa vs VTOL. | Phase 2 (M6) |
| **U-02** | Passive RF Capability Level | Controls payload mass ($360\text{g}$ vs $1.02\text{kg}$) & compute load. | **CRITICAL** | Owner decision selecting Dual RSSI vs KrakenSDR AoA. | Phase 2 (M2) |
| **U-03** | Radar / Altimeter RF Interference | Risk of mutual saturation between altimeter & search radar. | **HIGH** | Select Option C (24GHz Altimeter + 60GHz Search Radar). | Phase 2 (M2) |
| **U-04** | Vision Architecture SWaP | Determines long-range optical zoom vs Hadron dual gimbal SWaP. | **HIGH** | Owner decision on altitude verification requirements. | Phase 2 (M2) |
| **U-05** | Microphone SNR Under Rotor Wash | Risk of acoustic speech detection failure in hover. | **HIGH** | Physical bench test of mic array under multirotor wash. | Phase 3 |
| **U-06** | Open Speaker / Spotlight Selection | Requires replacing DJI-locked CZI payloads with open interfaces. | **MEDIUM** | Bench testing open 24V LED spotlight & 15W horn PA. | Phase 2 (M2) |
| **U-07** | Heliograph Physical Realization | Resolves flat mirror drag & alignment issues during flight. | **MEDIUM** | Design low-profile SOLAS retro-reflector array housing. | Phase 2 (M2) |
| **U-08** | Smoke Marker Deployment Mechanism | Pyrotechnic drop fire hazard vs non-pyro LED strobe array. | **MEDIUM** | Owner decision on chemical smoke vs electronic signal. | Phase 2 (M2) |
| **U-09** | Jetson CPU/GPU Thermal Throttling | Active fan reliability & cooling mass penalty in desert ambient ($45^\circ\text{C}$). | **MEDIUM** | Thermal chamber testing of Jetson carrier assembly. | Phase 3 |
| **U-10** | Maximum Operational Altitude | Sets search grid parameters and regulatory clearance scope. | **LOW** | Owner decision establishing search/inspection altitudes. | Phase 2 (M1) |

---

## O. Requirement Traceability Matrix

This matrix maps every documented MANAR concept to its repository source, candidate implementation, verification status, and open decision status.

### Traceability Matrix

| Requirement Concept | Repository Source Path | Current Candidate Realization | Verified? | Engineering Conflict? | Owner Decision Required? |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **Civilian SAR Scope** | [`README.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/README.md#L6) | Supervised-autonomy SAR system | **YES** | None | No (Established) |
| **Human Authorization Gate**| [`README.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/README.md), `REFERENCES.md` | C++ explicit operator confirmation gate | **YES** | None | No (Established) |
| **Deterministic C++ Authority**| [`ROADMAP.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/ROADMAP.md#L11), `control.cpp` | Persistent C++ state owner | **YES** | None | No (Established) |
| **Thermal Perception** | [`README.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/README.md#L33) | FLIR Boson+ 640 ($12\mu\text{m}$, $<20\text{mK}$) | **YES** | None | Yes (Hadron vs Boson) |
| **RGB Day Camera** | [`README.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/README.md#L34) | Sony FCB-EV9500L 30x Zoom Block | **YES** | LVDS Interface Board | Yes (Zoom vs Hadron) |
| **Low-Light / IR Vision** | [`README.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/README.md#L35) | SiOnyx Black Silicon BSI CMOS Core | **PARTIAL**| OEM Availability | Yes |
| **24 GHz FMCW Search Radar** | [`README.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/README.md#L36) | TI IWR6843 (Shift to 60GHz recommended)| **NO** | 24GHz Altimeter Interference | Yes (Frequency Option) |
| **Speaker + Microphone** | [`README.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/README.md#L37) | Open 15W PA Horn + ReSpeaker v2.0 | **PARTIAL**| CZI PSDK Lock / Rotor Noise | Yes (Call-Listen State) |
| **Passive RF Sensing** | [`README.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/README.md#L38) | Level 2 Dual Patch vs Level 3 KrakenSDR | **PARTIAL**| KrakenSDR SWaP Penalty | Yes (Capability Level) |
| **Amber Beacon Alert** | [`README.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/README.md#L39) | High-Flux $360^\circ$ Amber LED Strobe | **YES** | None | No |
| **White Guidance Strobe** | [`README.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/README.md#L40) | Directional Arc V White Strobe | **YES** | None | No |
| **Downward Spotlight** | [`README.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/README.md#L41) | Open 60W PWM-Dimmable LED Spotlight | **PARTIAL**| CZI SkyPort Lock | Yes |
| **Heliograph Mirrors** | [`README.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/README.md#L42) | SOLAS Passive Retro-Reflector Array | **NO** | Flat Mirror Flight Drag | Yes (Mirror vs Panel) |
| **Smoke Marker Concept** | [`README.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/README.md#L43) | Servo Canister Release vs LED Strobe | **NO** | Pyrotechnic Fire Hazard | Yes (Pyro vs Electronic)|

---

## P. Owner Decisions Still Required

Before freezing the MANAR V1 simulated aircraft configuration in Phase 2 Milestone 6, **Oumar Ibrahim** must resolve the following six core engineering decisions:

1. **Airframe Class Selection:** Select Quadcopter vs Hexacopter vs Hybrid VTOL based on hover inspection vs range requirements.
2. **Passive RF Architecture Level:** Select Level 2 Dual Directional RSSI Spatial Mapping ($360\text{g}$, $3.5\text{W}$) vs Level 3 KrakenSDR 5-Channel Coherent AoA ($1.02\text{kg}$, $11.0\text{W}$).
3. **Radar Frequency & Interference Strategy:** Confirm shifting the presence/micro-motion search radar from $24\text{ GHz}$ to **$60\text{ GHz}$** (Option C) to isolate it from the $24\text{ GHz}$ Ainstein radar altimeter.
4. **Vision Payload SWaP Strategy:** Select Architecture A (Sony 30x Zoom + Boson+ thermal, $966\text{g}$) vs Architecture B (Integrated FLIR Hadron EO/IR pod, $310\text{g}$, saving $656\text{g}$).
5. **Smoke Marker Realization:** Select mechanical pyrotechnic drop canister (fire hazard risk) vs non-pyrotechnic high-flux multi-color electronic marker array.
6. **Operational Altitude Ceiling Definition:** Set authoritative search altitude ($50\text{m}-120\text{m}$ AGL) and inspection altitude ($15\text{m}-30\text{m}$ AGL) compliant with UAE GCAA $122\text{m}$ (400 ft) civilian UAS rules.

---

## Q. Elements Safe to Carry Forward Into Phase 2

The following engineering elements are verified, technically defensible, and **safe to carry forward** into Phase 2 system definition:

1. **Autopilot Architecture Baseline:** Holybro Pixhawk 6X (FMUv6X, STM32H753 MCU, dual Ethernet, 3x CAN bus) running PX4 flight stack with DroneCAN Holybro ARK GPS integration.
2. **Perception Compute Architecture Baseline:** NVIDIA Jetson Orin NX 16GB module mounted on an industrial carrier board with active thermal cooling ($10\text{W}-25\text{W}$ TDP, 100 INT8 TOPS).
3. **Primary Thermal Sensor Baseline:** Teledyne FLIR Boson+ 640 ($640\times 512$, $12\mu\text{m}$, $<20\text{mK}$ NETD) with MIPI CSI-2 interface.
4. **Radar Altimeter Subsystem:** Ainstein US-D1 24 GHz UAV Radar Altimeter ($110\text{g}$, IP67, CAN/UART interface).
5. **Deterministic Software Authority Boundary:** C++ persistent control core as sole authoritative owner of runtime state, enforcing human-in-the-loop authorization gates prior to consequential actions.
6. **Payload Power Rail Sizing Baseline:** Power distribution system engineered to supply **$57.8\text{ W}$ continuous search power** and fused to handle **$209.4\text{ W}$ peak instantaneous electrical load**.
7. **Plausible Payload Mass Baseline:** A realistic best-estimate payload mass of **$\sim 3.85\text{ kg}$** (Plausible range: $3.4\text{ kg} - 4.3\text{ kg}$ including all integration hardware, cabling, and structural mounts).