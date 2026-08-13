# MANAR V1 System Requirements Baseline

**Document Title:** MANAR V1 System Requirements Baseline  
**Document Path:** `docs/engineering/MANAR_V1_REQUIREMENTS.md`  
**Project Phase:** Phase 2 — MANAR V1 Engineering Definition  
**Authority / Project Lead:** Oumar Ibrahim  
**Status:** **FINAL CONSOLIDATED SPECIFICATION DRAFT**

---

## 1. Purpose & Scope

### 1.1 Purpose
This specification establishes the functional, operational, performance, software, safety, and physical requirements for the **MANAR V1 Supervised-Autonomy Multisensor Search-and-Rescue System**. 

The primary objective of this document is to define **what MANAR V1 must accomplish before deciding exactly how it will be built**. This specification establishes system capabilities independently of specific commercial hardware components, sensor part numbers, or airframe selections.

### 1.2 Scope
This baseline applies to the complete MANAR V1 architecture, including:
- The deterministic C++ control core and runtime state database;
- Onboard perception, evidence fusion, and machine-learning processing;
- Multisensor payload interfaces (optical, thermal, radar, RF, acoustic, and navigation);
- Guidance, signaling, and search marker payloads;
- Operator interface command, telemetry, and human-in-the-loop authorization flows;
- System safety boundaries, failsafe triggers, and power distribution envelopes.

---

## 2. Requirement Classification Rules

Every requirement in this document is assigned exactly one status classification and uses specific normative syntax.

### 2.1 Status Classifications
1. **`LOCKED`**: Explicitly established by authoritative MANAR documentation (`README.md`, `ROADMAP.md`, `REFERENCES.md`, `BEHAVIOR_DECISIONS.md`) or direct owner decisions.
2. **`PROTOTYPE`**: Implemented in current prototype control software (`core/control.cpp`, `core/config.json`), but not yet formally ratified as a finalized MANAR V1 requirement.
3. **`PROPOSED`**: Supported by engineering feasibility analysis, but awaiting formal owner ratification.
4. **`TBD — OWNER DECISION`**: Open operational, capability, or architectural choice requiring an explicit decision by Oumar Ibrahim.
5. **`TBD — TEST / RESEARCH`**: Technical boundary requiring empirical bench testing, flight testing, or simulation validation.

### 2.2 Normative Language Rules
- **`LOCKED` Requirements:** Expressed using normative **`shall`** statements (e.g., *"MANAR V1 shall..."*).
- **`PROPOSED` Requirements:** Expressed using recommendation **`should`** statements (e.g., *"MANAR V1 should..."*).
- **`PROTOTYPE` Values:** Expressed descriptively (e.g., *"The current prototype implements..."*).
- **`TBD` Items:** Described objectively without declaring a specific choice (e.g., *"TBD — Owner must determine..."*).

---

## 3. Mission Requirements

| ID | Requirement Statement | Status | Source |
| :--- | :--- | :--- | :--- |
| **MIS-001** | MANAR V1 **shall** operate as a civilian, non-military supervised-autonomy search-and-rescue (SAR) aerial system. | **`LOCKED`** | [`README.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/README.md#L6) |
| **MIS-002** | MANAR V1 **shall** execute a canonical mission state lifecycle: Idle $\rightarrow$ Transit Enroute $\rightarrow$ Area Lawnmower Search $\rightarrow$ Candidate Detection $\rightarrow$ Human Authorization Gate $\rightarrow$ Target Verification $\rightarrow$ Rescue Guidance / Signaling $\rightarrow$ Return-to-Home (RTH) / Landing. | **`LOCKED`** | [`README.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/README.md), [`ROADMAP.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/ROADMAP.md) |
| **MIS-003** | MANAR V1 **shall** maintain exactly one active mission state at a time. Starting a new mission **shall** reset previous mission-specific state (`started`, `aborted`, `finished`, `rescueefound`, `enroute`, `returning`, `waitingforhelp`). | **`LOCKED`** | Owner Decision / `control.cpp` |
| **MIS-004** | MANAR V1 **shall** permit mission initiation (`startmission`) while the aircraft is grounded on the launch pad, prior to takeoff. | **`LOCKED`** | Owner Decision / `control.cpp` |
| **MIS-005** | Whether starting a new mission automatically clears or resets the active flight destination is **TBD — OWNER DECISION**. | **`TBD — OWNER DECISION`** | Phase 2 / Milestone 4 |
| **MIS-006** | MANAR V1 **should** support continuous search mission execution under day, night, and adverse low-visibility atmospheric conditions (dust, light smoke, haze). | **`PROPOSED`** | Version 3 Baseline |
| **MIS-007** | Continuous search mission endurance is **TBD — OWNER DECISION** and shall be established only after the airframe, battery chemistry, propulsion configuration, and payload mass are frozen. | **`TBD — OWNER DECISION`** | Version 3 Baseline |

---

## 4. Flight & Navigation Requirements

| ID | Requirement Statement | Status | Source |
| :--- | :--- | :--- | :--- |
| **FLT-001** | The current prototype implements four preset operational flight modes: **Quick** ($15.0\text{ m/s}$ default), **Active** ($5.0\text{ m/s}$ default), **Inspect** ($1.0\text{ m/s}$ default), and **Hover** ($0.0\text{ m/s}$ default). | **`PROTOTYPE`** | [`core/config.json`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/core/config.json#L8-L11) |
| **FLT-002** | MANAR V1 **shall** allow an operator to command arbitrary flight speeds ($0.0\text{ m/s}$ to `maximum_speed`) and flight altitudes ($0.0\text{ m}$ to `maximum_altitude`), overriding active flight mode presets. | **`LOCKED`** | Owner Decision / `control.cpp` |
| **FLT-003** | Relaunching flight during an active mission **shall** be permitted following a temporary flight stop or hover. | **`LOCKED`** | Owner Decision / `control.cpp` |
| **FLT-004** | Commanding a new flight destination while Return-to-Home (RTH) is active **shall** update the active navigation target and supersede the RTH trajectory. | **`LOCKED`** | Owner Decision / `control.cpp` |
| **NAV-001** | The current prototype implements a maximum speed clamp of $15.0\text{ m/s}$ (`maximum_speed`). | **`PROTOTYPE`** | [`core/config.json`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/core/config.json#L6) |
| **NAV-002** | The current prototype implements a maximum altitude ceiling clamp of $2000\text{ m AGL}$ (`maximum_altitude`). | **`PROTOTYPE`** | [`core/config.json`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/core/config.json#L5) |
| **NAV-003** | Operational search and target-verification altitudes are **TBD — OWNER DECISION** and shall be established from sensor resolution, mission needs, UAE airspace requirements, and the selected vision architecture. | **`TBD — OWNER DECISION`** | Version 3 Baseline |
| **NAV-004** | The current prototype evaluates waypoint arrival using a horizontal distance threshold of $5.0\text{ m}$ (`reached_radius`). | **`PROTOTYPE`** | [`core/config.json`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/core/config.json#L18) |
| **NAV-005** | The current prototype evaluates home base arrival during RTH using a horizontal distance threshold of $100.0\text{ m}$ (`home_arrival_radius`). | **`PROTOTYPE`** | [`core/config.json`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/core/config.json#L12) |
| **NAV-006** | Whether reaching the home arrival radius during RTH initiates an automatic landing versus a stationary hover at launch altitude is **TBD — OWNER DECISION**. | **`TBD — OWNER DECISION`** | Phase 2 / Milestone 4 |

---

## 5. Search Requirements

| ID | Requirement Statement | Status | Source |
| :--- | :--- | :--- | :--- |
| **SCH-001** | MANAR V1 **shall** perform automated lawnmower grid search navigation over operator-defined rectangular or polygonal search sector boundaries. | **`LOCKED`** | [`README.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/README.md), [`core/control.cpp`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/core/control.cpp#L718) |
| **SCH-002** | The current prototype implements a default lawnmower grid unit of $100\text{ m} \times 100\text{ m}$ with $20\text{ m}$ row spacing. | **`PROTOTYPE`** | [`core/config.json`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/core/config.json#L19-L21) |
| **SCH-003** | Search grid line spacing **should** adapt dynamically to the effective field-of-view (FOV) of the optical/thermal perception payload at the selected operational search altitude. | **`PROPOSED`** | Version 3 Baseline |
| **SCH-004** | Upon completing all generated lawnmower search waypoints without candidate detection or further required search action, MANAR V1 **shall** initiate Return-to-Home behavior. Mission completion semantics are handled separately upon home arrival. | **`LOCKED`** | [`core/control.cpp`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/core/control.cpp#L650-L667) |

---

## 6. Perception & Sensor Requirements

### 6.1 General Perception Framework
| ID | Requirement Statement | Status | Source |
| :--- | :--- | :--- | :--- |
| **PER-001** | MANAR V1 **shall** execute a multisensor detection and perception pipeline combining evidence from optical, thermal, radar, acoustic, and passive RF sensing modalities. | **`LOCKED`** | [`README.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/README.md#L6), [`REFERENCES.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/REFERENCES.md#L30) |
| **PER-002** | The perception pipeline **shall** generate structured candidate detection events (`candidate_id`, `latitude`, `longitude`, `confidence_score`, `sensor_modality_mask`) and pass them to the deterministic C++ control core without directly executing flight control maneuvers. | **`LOCKED`** | [`ROADMAP.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/ROADMAP.md#L181-L198) |

### 6.2 RGB / Visual Imaging
| ID | Requirement Statement | Status | Source |
| :--- | :--- | :--- | :--- |
| **VIS-001** | MANAR V1 **shall** provide daytime visual imaging for person detection, terrain assessment, and target verification. | **`LOCKED`** | [`README.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/README.md#L35) |
| **VIS-002** | The visual imaging payload **should** achieve a Ground Sample Distance (GSD) sufficient to resolve human target features at the target verification altitude ($\le 2.0\text{ cm/pixel}$). | **`PROPOSED`** | Version 3 Baseline |
| **VIS-003** | Whether visual verification is accomplished via a continuous optical zoom lens from search altitude versus descending to a low verification altitude with a fixed-focus camera is **TBD — OWNER DECISION**. | **`TBD — OWNER DECISION`** | Phase 2 Milestone 2 |

> *Engineering Note:* Candidate hardware providing optical zoom capabilities includes industrial block cameras (e.g., Sony FCB-EV9500L class). Integrated dual EO/IR pods (e.g., FLIR Hadron 640R class) provide lower mass but rely on digital zoom.

### 6.3 Thermal Imaging
| ID | Requirement Statement | Status | Source |
| :--- | :--- | :--- | :--- |
| **THM-001** | MANAR V1 **shall** provide Long-Wave Infrared (LWIR) thermal imaging for human heat signature detection across day and night operations. | **`LOCKED`** | [`README.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/README.md#L34) |
| **THM-002** | The thermal imaging payload **should** provide a uncooled microbolometer resolution of $\ge 640 \times 512$ pixels with a thermal sensitivity (NETD) of $\le 50\text{ mK}$. | **`PROPOSED`** | Version 3 Baseline |

> *Engineering Note:* Candidate thermal hardware includes standalone LWIR cores (e.g., FLIR Boson+ 640 class) or integrated dual-sensor modules.

### 6.4 Low-Light / IR Imaging
| ID | Requirement Statement | Status | Source |
| :--- | :--- | :--- | :--- |
| **LOW-001** | MANAR V1 **shall** provide low-light / Near-Infrared (NIR) imaging for nighttime visual confirmation under ambient moon and starlight illumination. | **`LOCKED`** | [`README.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/README.md#L36) |
| **LOW-002** | The low-light imaging payload **should** maintain spectral sensitivity across the $400\text{ nm} - 1200\text{ nm}$ NIR band. | **`PROPOSED`** | Version 3 Baseline |

### 6.5 Radar
| ID | Requirement Statement | Status | Source |
| :--- | :--- | :--- | :--- |
| **RAD-001** | MANAR V1 **shall** incorporate Frequency-Modulated Continuous-Wave (FMCW) radar sensing to detect presence, range, macro-motion, and micro-motion. | **`LOCKED`** | [`README.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/README.md#L37) |
| **RAD-002** | Radar micro-motion algorithms **should** function as an evidence contributor to multisensor fusion during stationary hover/inspection modes, rather than serving as a single-sensor conclusive respiration detector during forward flight. | **`PROPOSED`** | Version 3 Baseline |
| **RAD-003** | MANAR V1 **should** incorporate radar altimetry or another direct ground-clearance sensing method for terrain-relative altitude measurement. | **`PROPOSED`** | Version 3 Baseline |
| **RAD-004** | Resolving the potential RF frequency collision between the 24 GHz FMCW search radar and 24 GHz radar altimeter (Option A: TDM; Option B: Optical Altimeter; Option C: 60GHz/77GHz Search Radar; Option D: Unified Multimode Radar) is **TBD — OWNER DECISION**. | **`TBD — OWNER DECISION`** | Phase 2 Milestone 2 |

### 6.6 Passive RF
| ID | Requirement Statement | Status | Source |
| :--- | :--- | :--- | :--- |
| **RF-001** | MANAR V1 **shall** incorporate passive RF sensing to detect and correlate emissions from mobile devices (cellular uplinks, Wi-Fi, Bluetooth). | **`LOCKED`** | [`README.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/README.md#L39) |
| **RF-002** | Selection of the passive RF capability architecture (Level RF-A: Energy Presence; Level RF-B: RSSI Spatial Mapping; Level RF-C: Switched Bearing; Level RF-D: Coherent AoA Direction Finding) is **TBD — OWNER DECISION**. | **`TBD — OWNER DECISION`** | Phase 2 Milestone 2 |

```text
PASSIVE RF CAPABILITY LEVEL DEFINITION BLOCK:

- LEVEL RF-A (Energy / Presence Detection):
  * Capability: Detects RF energy in sector; zero directional data. Low SWaP (~155g, 1.5W).

- LEVEL RF-B (RSSI Spatial Correlation Mapping):
  * Capability: Measures signal strength along flight path; C++ core builds georeferenced RSSI heat map. Moderate SWaP (~360g, 3.5W).

- LEVEL RF-C (Switched Directional Bearing):
  * Capability: Measures coarse left/right directional bias during hover. Moderate SWaP (~550g, 5.0W).

- LEVEL RF-D (Coherent Angle-of-Arrival / Beamforming):
  * Capability: Computes real-time phase-locked bearing vector in a single hover. High SWaP (~1.02 kg, 11.0W).
```

### 6.7 Acoustic
| ID | Requirement Statement | Status | Source |
| :--- | :--- | :--- | :--- |
| **ACO-001** | MANAR V1 **shall** incorporate a public address speaker to broadcast voice prompts and audio instructions to victims. | **`LOCKED`** | [`README.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/README.md#L38) |
| **ACO-002** | MANAR V1 **shall** incorporate a far-field microphone array for acoustic listening and direction finding. | **`LOCKED`** | [`README.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/README.md#L38) |
| **ACO-003** | Selection of the acoustic operational flight strategy to overcome rotor wash noise (Option A: Hover with DSP filtering; Option B: Low-RPM Acoustic Dip flight mode; Option C: Temporary Ground Touchdown) is **TBD — OWNER DECISION**. | **`TBD — OWNER DECISION`** | Phase 2 Milestone 2 |

---

## 7. Guidance & Signaling Requirements

| ID | Requirement Statement | Status | Source |
| :--- | :--- | :--- | :--- |
| **SIG-001** | MANAR V1 **shall** incorporate an amber beacon for $360^\circ$ visual location alerting to rescue personnel. | **`LOCKED`** | [`README.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/README.md#L41) |
| **SIG-002** | MANAR V1 **shall** incorporate a white strobe for directional visual guidance to ground victims and search teams. | **`LOCKED`** | [`README.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/README.md#L42) |
| **SIG-003** | MANAR V1 **shall** incorporate a downward spotlight for close-range terrain illumination during night inspection and landing. | **`LOCKED`** | [`README.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/README.md#L43) |
| **SIG-004** | MANAR V1 **shall** incorporate heliograph mirrors for passive daylight solar signal reflection. | **`LOCKED`** | [`README.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/README.md#L44) |
| **SIG-005** | MANAR V1 **shall** incorporate a smoke marker concept for location and local surface wind vector marking. | **`LOCKED`** | [`README.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/README.md#L45) |
| **SIG-006** | Physical realization of the heliograph system (for example, fixed passive mirror geometry versus an actively oriented solar-reflection mechanism) is **TBD — OWNER DECISION**. Retro-reflective material may be evaluated separately as a visibility aid but does not replace the solar-reflection heliograph requirement. | **`TBD — OWNER DECISION`** | Phase 2 Milestone 2 |
| **SIG-007** | Whether MANAR V1 retains an actual airborne smoke/wind-marking capability and, if retained, how it is safely realized is **TBD — OWNER DECISION**. Replacing smoke with a non-smoke visual marker would constitute a revision of `SIG-005`, not merely an alternative smoke-marker implementation. | **`TBD — OWNER DECISION`** | Phase 2 Milestone 2 |

---

## 8. Human Authorization Requirements

MANAR V1 enforces explicit human authorization gates for consequential rescue decisions and operational state changes.

### 8.1 Action Authorization Matrix

| ID | Operational Action | Execution Classification | Control Core Behavior | Status |
| :--- | :--- | :--- | :--- | :--- |
| **HUM-001** | Mission Initialization (`startmission`) | **Operator-Commanded** | Requires explicit command submit. | **`LOCKED`** |
| **HUM-002** | Aircraft Takeoff / Launch (`launch`) | **Operator-Confirmed** | Awaits operator takeoff confirmation. | **`LOCKED`** |
| **HUM-003** | Lawnmower Search Navigation | **Automatic (Autonomous)** | Autonomous waypoint navigation. | **`LOCKED`** |
| **HUM-004** | Candidate Perception Detection | **Automatic (Telemetry)** | Transmits candidate alert to GUI. | **`LOCKED`** |
| **HUM-005** | Candidate Verification Approach | **Operator-Confirmed** | Pauses grid search; awaits operator authorization to approach. | **`LOCKED`** |
| **HUM-006** | Rescuee Found Determination | **Operator-Confirmed** | Requires human review of evidence before setting `rescueefound`. | **`LOCKED`** |
| **HUM-007** | Amber Beacon Activation | **TBD — OWNER DECISION** | Direct command in code; extra gate is unresolved. | **`TBD — OWNER DECISION`** |
| **HUM-008** | White Strobe Activation | **TBD — OWNER DECISION** | Direct command in code; extra gate is unresolved. | **`TBD — OWNER DECISION`** |
| **HUM-009** | Downward Spotlight Activation | **TBD — OWNER DECISION** | Direct command in code; extra gate is unresolved. | **`TBD — OWNER DECISION`** |
| **HUM-010** | Smoke Marker Deployment | **TBD — OWNER DECISION** | Direct command in code; extra gate is unresolved. | **`TBD — OWNER DECISION`** |
| **HUM-011** | Rescue Coordinate Handoff | **Operator-Commanded** | Transmits confirmed GPS coordinates to ground SAR teams. | **`LOCKED`** |
| **HUM-012** | Return-to-Home (RTH) | **Operator / Auto-Safety** | Triggered by command OR auto battery/failsafe threshold. | **`LOCKED`** |
| **HUM-013** | Mission Abort (`abortmission`) | **Operator-Commanded** | Cancels mission and initiates RTH behavior. | **`LOCKED`** |
| **HUM-014** | Flight Stop / Safe Landing (`stopflight`) | **Operator / Auto-Safety** | Triggers safe deceleration, vertical descent, and motor shutdown without automatically aborting the mission. | **`LOCKED`** |

---

## 9. Deterministic Control Requirements

| ID | Requirement Statement | Status | Source |
| :--- | :--- | :--- | :--- |
| **CTL-001** | The deterministic C++ control core **shall** serve as the sole authoritative owner of system runtime state (`runtime.json`). Subsystem modules, perception processes, and external GUI clients **shall NOT** directly mutate state. | **`LOCKED`** | [`ROADMAP.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/ROADMAP.md#L11-L26) |
| **CTL-002** | `STOP_FLIGHT` commands **shall** reduce flight speed to $0.0\text{ m/s}$, initiate safe vertical descent, set flight mode to `Stall`, and clear active enroute state without corrupting overall mission history or automatically aborting the mission. | **`LOCKED`** | Owner Decision / `control.cpp` |
| **CTL-003** | Aborting a mission (`abortmission`) **shall** set `aborted = true`, set `returning = true`, clear enroute state, and initiate Return-to-Home navigation. | **`LOCKED`** | Owner Decision / `control.cpp` |
| **CTL-004** | If `abortmission` is commanded while the aircraft is landed on the ground, whether the control core automatically relaunches the aircraft to fly home (Option A) versus logging `aborted = true` and awaiting an explicit `launch` command (Option B) is **TBD — OWNER DECISION**. | **`TBD — OWNER DECISION`** | Phase 2 / Milestone 4 |
| **CTL-005** | Confirming a rescuee (`rescueefound = true`) **shall** set `waitingforhelp = true`, cease active search navigation, and preserve target location telemetry. | **`LOCKED`** | Owner Decision / `control.cpp` |
| **CTL-006** | Whether target confirmation (`rescueefound = true`) triggers an immediate safe landing at current coordinates (prototype behavior) versus a stationary hover or low-altitude beaconing mode is **TBD — OWNER DECISION**. | **`TBD — OWNER DECISION`** | Phase 2 / Milestone 4 |
| **CTL-007** | Destination coordinates **shall** remain updatable while the aircraft is in `waitingforhelp` state. | **`LOCKED`** | Owner Decision / `control.cpp` |

---

## 10. Command & Runtime Requirements

| ID | Requirement Statement | Status | Source |
| :--- | :--- | :--- | :--- |
| **CMD-001** | Every operator command **shall** include a unique Command ID. The control core **shall** validate commands completely and produce a final `SUCCESS` or `REJECTED` result for every identifiable request. A command ID that has reached a durable final result **shall not** be executed again. Crash/interruption behavior **shall** be explicitly defined so uncertain execution is never silently reported as success. | **`LOCKED`** | [`ROADMAP.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/ROADMAP.md#L30-L47) |
| **CMD-002** | The C++ control process **shall** write a control heartbeat timestamp to `runtime.json` at a minimum frequency of $1.0\text{ Hz}$. | **`LOCKED`** | [`ROADMAP.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/ROADMAP.md#L63) |
| **CMD-003** | State persistence to `runtime.json` **shall** utilize an atomic temporary file write and replace strategy to prevent partial snapshot reads. | **`LOCKED`** | [`ROADMAP.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/ROADMAP.md#L59) |

---

## 11. Communications / External Interface Requirements

| ID | Requirement Statement | Status | Source |
| :--- | :--- | :--- | :--- |
| **COM-001** | External operator GUI and perception clients **shall** communicate with the deterministic C++ control core through a documented interface that preserves control-core state authority. | **`LOCKED`** | [`ROADMAP.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/ROADMAP.md#L143-L160) |
| **COM-002** | Selection of the final production transport mechanism (for example, file-based IPC, local sockets, WebSockets, or HTTP) is **TBD — OWNER DECISION**, informed by engineering evaluation during Roadmap Milestone 8. | **`TBD — OWNER DECISION`** | Roadmap Milestone 8 |

---

## 12. Safety & Failsafe Requirements

| ID | Requirement Statement | Status | Source |
| :--- | :--- | :--- | :--- |
| **SAF-001** | MANAR V1 **shall** continuously monitor battery state of charge (SoC) during active operations. | **`LOCKED`** | [`core/control.cpp`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/core/control.cpp#L754-L784) |
| **SAF-002** | The current prototype uses `battery_warning` ($30\%$ default) as the threshold for issuing a low-battery alert to the operator interface. | **`PROTOTYPE`** | [`core/config.json`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/core/config.json#L13) |
| **SAF-003** | The current prototype uses `battery_rth_warning` ($20\%$ default) as the threshold for issuing an RTH recommendation alert and transmitting location telemetry. | **`PROTOTYPE`** | [`core/config.json`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/core/config.json#L14) |
| **SAF-004** | The current prototype uses `battery_emergency_rth` ($10\%$ default) to trigger mandatory RTH logic, with the existing `rescueefound` exception preserved as prototype behavior. | **`PROTOTYPE`** | [`core/control.cpp`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/core/control.cpp#L756-L762) |
| **SAF-005** | The current prototype uses `battery_emergency_land` ($5\%$ default) to trigger emergency-coordinate transmission and immediate safe landing logic. | **`PROTOTYPE`** | [`core/control.cpp`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/core/control.cpp#L763-L770) |
| **SAF-006** | MANAR V1 **should** support an autonomous low-level failsafe response to sustained command-link loss, with the timeout and exact RTH behavior remaining subject to engineering validation. | **`PROPOSED`** | Version 3 Baseline |
| **SAF-007** | Low-level flight stabilization, motor mixing, attitude control, and primary emergency failsafes **should** be managed by a dedicated flight controller independently of companion compute processes. | **`PROPOSED`** | Version 3 Baseline |

---

## 13. Physical & Power Requirements

| ID | Requirement Statement | Status | Source |
| :--- | :--- | :--- | :--- |
| **PWR-001** | The onboard Power Distribution Unit (PDU) **should** supply regulated DC power rails (e.g., $5\text{V}$, $12\text{V}$, $24\text{V}$) derived from the main propulsion battery bus. | **`PROPOSED`** | Version 3 Baseline |
| **PWR-002** | The PDU **should** be continuously rated for $\sim 57.8\text{ W}$ normal search power and fused to accommodate a $\sim 209.4\text{ W}$ peak instantaneous electrical load without voltage sag. | **`PROPOSED`** | Version 3 Baseline |
| **PHY-001** | The total airborne payload mass (sensors, compute, avionics, gimbals, wiring, mounts) **should** not exceed a design limit of $4.30\text{ kg}$ (Best current design target: $3.85\text{ kg}$). | **`PROPOSED`** | Version 3 Baseline |
| **PHY-002** | Selection of the host aircraft airframe class (Quad-copter, Hexa-copter, Octo-copter, or Hybrid VTOL Fixed-Wing) is **TBD — OWNER DECISION**. | **`TBD — OWNER DECISION`** | Phase 2 Milestone 6 |

---

## 14. Environmental Requirements

| ID | Requirement Statement | Status | Source |
| :--- | :--- | :--- | :--- |
| **ENV-001** | Avionics enclosures and sensor housings **should** achieve an environmental protection rating of $\ge \text{IP54}$ for operation in light rain and dust. | **`PROPOSED`** | Version 3 Baseline |
| **ENV-002** | Payload electronics and cooling systems **should** operate reliably across an ambient temperature range of $-10^\circ\text{C}$ to $+45^\circ\text{C}$. | **`PROPOSED`** | Version 3 Baseline |

---

## 15. Prototype Values Not Yet Accepted as V1 Requirements

The following values exist in prototype code (`core/config.json`), but are explicitly **not** ratified as final physical engineering requirements:

1. **`maximum_altitude: 2000`**: Prototype software clamp. Operational search altitude ceiling is **`TBD — OWNER DECISION`**.
2. **`maximum_speed: 15`**: Prototype simulation speed limit. Real maximum speed depends on propulsion thrust and airframe drag.
3. **`launch_altitude: 10`**: Default simulation climb target. Real launch altitude will adapt to local obstacle clearance height.
4. **Preset Speeds (`quick_speed: 15`, `active_speed: 5`, `inspect_speed: 1`, `hover_speed: 0`)**: Default software preset values for simulation movement testing.
5. **Battery SoC Thresholds ($30\%, 20\%, 10\%, 5\%$)**: Prototype simulation battery triggers. Real safety thresholds will be calibrated to battery voltage discharge curves under load.
6. **Search Grid Unit ($100\text{m} \times 100\text{m}$, $20\text{m}$ spacing)**: Default test pattern generator constants.

---

## 16. Requirement Conflicts

| Conflict ID | Conflicting Requirements | Nature of Conflict | Requirement to Clarify / Resolve |
| :--- | :--- | :--- | :--- |
| **RC-01** | `RAD-001` (Search Radar) vs. `RAD-003` (Radar Altimeter) | Candidate search-radar and radar-altimeter implementations may occupy the same $24.0-24.25\text{ GHz}$ ISM band, creating mutual-interference risk. | Resolve through `RAD-004` without preselecting a frequency-separation strategy. |
| **RC-02** | `ACO-002` (Microphone Array) vs. Multirotor Rotor Wash Noise | Multirotor rotor wash may severely reduce acoustic signal-to-noise ratio during listening. | Resolve through `ACO-003` and empirical acoustic testing; do not preselect hover, low-RPM dip, or touchdown. |
| **RC-03** | `RF-002` (Passive RF capability) vs. `PHY-001` (Payload Mass Target) | Higher-capability coherent direction-finding architectures may impose substantially greater mass, power, antenna, and compute requirements than RSSI-based approaches. | Resolve through `RF-002`; do not preselect a passive-RF capability level. |
| **RC-04** | `VIS-003` (Verification Architecture) vs. `PHY-001` (Payload Mass Target) | Long-range optical verification can require substantially more optical and gimbal mass than a lighter fixed-focus architecture that descends for verification. | Resolve through `VIS-003` together with `NAV-003`; do not preselect a camera architecture. |
| **RC-05** | `SIG-005` (Smoke Marker) vs. Environmental / Operational Safety | Some physical smoke-generation or deployment methods may introduce fire, environmental, or regulatory hazards. | Resolve through `SIG-007`: retain a safe smoke/wind marker implementation, or explicitly revise `SIG-005` if the capability itself is removed. |
| **RC-06** | `NAV-003` (Search Altitude) vs. `VIS-002` (Target Ground Sample Distance) | Increasing search altitude improves search swath but reduces target spatial resolution and may introduce regulatory constraints. | Determine `NAV-003` jointly with `VIS-002`/`VIS-003`; current altitude ranges remain engineering recommendations, not accepted requirements. |

---

## 17. Requirements Traceability Matrix

| ID | Requirement Summary | Status | Source Path | Verification Method | Depends On |
| :--- | :--- | :--- | :--- | :--- | :--- |
| `MIS-001` | Civilian SAR Scope | **`LOCKED`** | [`README.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/README.md#L6) | Inspection | None |
| `MIS-002` | Canonical Mission Lifecycle | **`LOCKED`** | [`README.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/README.md) | Software Test | `CTL-001` |
| `MIS-003` | Single Active Mission & Reset | **`LOCKED`** | [`core/control.cpp`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/core/control.cpp#L733) | Software Test | `CTL-001` |
| `NAV-003` | Operational Altitude Ceiling | **`TBD`** | Version 3 Baseline | Owner Decision / Flight Test| GCAA Approval |
| `PER-001` | Multisensor Perception Engine| **`LOCKED`** | [`REFERENCES.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/REFERENCES.md#L30) | Bench Test / Sim | `PER-002` |
| `VIS-003` | Vision Camera Architecture | **`TBD`** | Version 3 Baseline | Owner Decision / Calculation| `PHY-001` |
| `RAD-004` | Radar Frequency Overlap Fix | **`TBD`** | Version 3 Baseline | Owner Decision / Bench Test | `RAD-001` |
| `RF-002` | Passive RF Capability Level | **`TBD`** | Version 3 Baseline | Owner Decision / Flight Test| `PHY-001` |
| `ACO-003` | Listening Flight Strategy | **`TBD`** | Version 3 Baseline | Owner Decision / Flight Test| `ACO-002` |
| `HUM-005` | Verification Approach Gate | **`LOCKED`** | [`README.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/README.md) | Software Test | `CTL-001` |
| `CTL-001` | Deterministic State Ownership| **`LOCKED`** | [`ROADMAP.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/ROADMAP.md#L11) | Software Test | None |
| `CTL-004` | Grounded Abort Behavior | **`TBD`** | Version 3 Baseline | Owner Decision / Software Test| `CTL-003` |
| `SAF-004` | Mandatory Emergency RTH | **`PROTOTYPE`**| [`core/control.cpp`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/core/control.cpp#L756) | Software Test / Sim | `SAF-001` |
| `PWR-002` | PDU Peak Load Rating (209W) | **`PROPOSED`** | Version 3 Baseline | Bench Test | `PWR-001` |
| `PHY-001` | Payload Mass Limit (4.30kg) | **`PROPOSED`** | Version 3 Baseline | Calculation / Weighing | `PHY-002` |
| `PHY-002` | Airframe Class Selection | **`TBD`** | Version 3 Baseline | Owner Decision | `PHY-001` |

---

## 18. Owner Decision Queue

The owner decision queue is ordered by physical and operational dependency. Mission capability choices precede subsystem architecture selections, which determine payload mass before airframe selection.

```text
  PHASE 2 (CURRENT): CAPABILITY & HARDWARE BOUNDARIES
  [1. Passive RF Level]
  [2. Vision Architecture] ---> [3. Search/Verification Altitudes]
  [4. Radar / Altimeter Strategy]
  [5. Signaling / Smoke Realization]
                │
                v
  [6. Payload Mass & Power Freeze] ---> [7. Airframe Selection]
                                             │
                                             v
                                [8. Propulsion / Battery Sizing]

  DETERMINISTIC / OPERATIONAL DECISIONS
  [9. Acoustic Flight Strategy]
  [10. Destination Reset]
  [11. Grounded Abort]
  [12. Target Confirmation Response]
  [13. Component Authorization Gates]
```

| Order | Decision | Type | Why Now? | Depends On | Unlocks | Milestone / Phase |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **1** | **Passive RF Capability Level (`RF-002`)** | Capability | Determines the level of RF localization information MANAR requires and materially affects RF subsystem SWaP. | None | RF architecture and payload budget | Phase 2 / Milestone 2 |
| **2** | **Vision Verification Architecture (`VIS-003`)** | Capability / Operational | Determines whether MANAR requires long-range optical verification or may descend for close verification. | None | Camera architecture, payload budget, altitude decision | Phase 2 / Milestone 2 |
| **3** | **Operational Altitudes (`NAV-003`)** | Operational | Establishes search/verification altitude from mission needs, sensor resolution, airspace constraints, and Decision #2. | Decision #2 | Search-grid geometry and optical requirements | Phase 2 / Milestone 2 |
| **4** | **Radar / Altimeter Strategy (`RAD-004`)** | Implementation | Resolves potential interference and determines radar/ground-clearance implementation. | Radar and ground-clearance capability requirements | Radar architecture and payload budget | Phase 2 / Milestone 2 |
| **5** | **Signaling & Smoke Realization (`SIG-006`, `SIG-007`)** | Capability / Implementation | Determines the physical realization and mass of heliograph and smoke/wind-marking functions, including whether `SIG-005` itself requires revision. | None | Signaling architecture and payload budget | Phase 2 / Milestone 2 |
| **6** | **Freeze Payload Mass & Power Envelopes** | Calculation | Recalculate the credible airborne payload mass and electrical envelope after payload-defining decisions are resolved. | Decisions #1, #2, #4, #5 | Airframe selection | Phase 2 / Milestone 2 |
| **7** | **Airframe Class Selection (`PHY-002`)** | Physical Configuration | Select the airframe class using the frozen payload envelope and mission hover/range requirements. | Decision #6 | Propulsion, battery sizing, 3D model | Phase 2 / Milestone 6 |
| **8** | **Propulsion & Battery Sizing** | Implementation | Size motors, propellers, ESCs, battery voltage/capacity, and reserves for the selected airframe and payload. | Decision #7 | Simulated aircraft configuration freeze and endurance model | Phase 2 / Milestone 6 |
| **9** | **Acoustic Flight Strategy (`ACO-003`)** | Operational | Determines whether MANAR needs special flight behavior for useful acoustic listening. | Acoustic test evidence | C++ control behavior and microphone integration | Roadmap Milestone 4 + later validation |
| **10** | **Destination Reset on New Mission (`MIS-005`)** | Software Behavior | Establishes whether `startmission` resets active destination coordinates. | None | C++ runtime state logic | Roadmap Milestone 4 |
| **11** | **Grounded Abort Behavior (`CTL-004`)** | Software Behavior | Resolves automatic relaunch for RTH versus setting abort/RTH state and waiting for explicit `LAUNCH_DRONE`. | None | C++ flight/mission behavior | Roadmap Milestone 4 |
| **12** | **Target Confirmation Response (`CTL-006`)** | Software Behavior | Resolves safe landing versus stationary hover/other explicitly defined behavior after `rescueefound = true`. | None | C++ mission-state logic | Roadmap Milestone 4 |
| **13** | **Component Authorization Gates (`HUM-007`-`HUM-010`)** | Operational / Safety | Resolves whether beacon, strobe, spotlight, and smoke require additional confirmation beyond a direct operator command. | None | Command API and future GUI authorization flow | Roadmap Milestone 4 / GUI phase |

---

## 19. Engineering Targets Still PROPOSED

The following engineering budget items remain **`PROPOSED` engineering targets** until candidate hardware integration and airframe selection are frozen in Phase 2 Milestone 6:

1. **`PHY-001`**: Total airborne payload mass design limit ($\le 4.30\text{ kg}$, best estimate $3.85\text{ kg}$).
2. **`PWR-001`**: PDU regulated voltage rails ($5\text{V}$, $12\text{V}$, $24\text{V}$ DC).
3. **`PWR-002`**: Continuous search power rating ($\sim 57.8\text{ W}$) and peak fused load rating ($\sim 209.4\text{ W}$).
4. **`ENV-001` & `ENV-002`**: Ingress protection ($\ge \text{IP54}$) and operating temperature range ($-10^\circ\text{C}$ to $+45^\circ\text{C}$).
5. **`RAD-002`**: Radar micro-motion evidence contribution to multisensor fusion during hover.
6. **`ACO-003`**: Public address speaker output target ($\ge 115\text{ dB SPL}$ @ $1\text{m}$).
7. **`SAF-006` & `SAF-007`**: Failsafe link loss timing and flight controller hardware independence.

---

## 20. Requirements Safe to Lock

The following normative requirements are fully supported by baseline documentation, owner behavioral decisions, or deterministic code logic, and are **safe to lock immediately**:

1. **`MIS-001`**: Civilian non-military supervised-autonomy SAR system scope.
2. **`MIS-002`**: Canonical 8-stage mission lifecycle.
3. **`MIS-003`**: Single active mission state ownership and previous mission flag resets.
4. **`MIS-004`**: Mission initiation (`startmission`) permission prior to takeoff launch.
5. **`FLT-002`**: Operator speed and altitude override capability over preset modes.
6. **`FLT-003`**: Relaunch permission during an active mission.
7. **`FLT-004`**: Destination override capability during active Return-to-Home.
8. **`SCH-001`**: Automated lawnmower grid search pattern navigation.
9. **`SCH-004`**: Automatic Return-to-Home initiation upon grid search completion.
10. **`PER-001`**: Multisensor evidence fusion approach (optical, thermal, radar, acoustic, RF).
11. **`PER-002`**: Structured perception event telemetry interface (`candidate_id`, `coordinates`, `confidence`).
12. **`VIS-001`, `THM-001`, `LOW-001`**: RGB, thermal LWIR, and low-light NIR sensing capabilities.
13. **`RAD-001`, `RF-001`, `ACO-001`, `ACO-002`**: Radar FMCW, passive RF, speaker, and mic array capabilities.
14. **`SIG-001` to `SIG-005`**: Amber beacon, white strobe, downward spotlight, heliograph, and smoke marker payload requirements.
15. **`HUM-001` to `HUM-006` & `HUM-011` to `HUM-014`**: Established execution/authorization classifications for mission start, launch, autonomous search, candidate telemetry, candidate approach, rescuee determination, coordinate handoff, RTH, abort, and flight stop / safe landing.
16. **`CTL-001`**: Sole authoritative runtime state ownership held by C++ control core.
17. **`CTL-002`**: `STOP_FLIGHT` execution logic ($0.0\text{ m/s}$ speed, safe vertical descent, `Stall` mode).
18. **`CTL-003`**: Mission abort execution logic (`aborted = true`, `returning = true`, route home).
19. **`CTL-005`**: Target confirmation state logic (`waitingforhelp = true`, cease active search).
20. **`CTL-007`**: Destination updatability while in `waitingforhelp` state.
21. **`CMD-001` to `CMD-003`**: Command ID validation, explicit final command results with replay prevention for durably finalized IDs, heartbeat publication ($1\text{ Hz}$), and atomic state writes.
22. **`COM-001`**: External client communication through a documented interface preserving control-core state authority.
23. **`SAF-001`**: Continuous battery state of charge monitoring.
