<div align="center">

  <img src="assets/Images/Branding/GitHub README Header.png" alt="MANAR Search & Rescue Drone Header" width="100%" />

# MANAR / منار

**Supervised-Autonomy Multisensor Search-and-Rescue System**

[![License: Proprietary](https://img.shields.io/badge/License-Proprietary-yellow.svg)](LICENSE.md)
[![C++17](https://img.shields.io/badge/C%2B%2B-17-00599C.svg?logo=c%2B%2B)](core/)
[![JSON](https://img.shields.io/badge/JSON-Runtime%20%26%20Messaging-000000.svg?logo=json&logoColor=white)](core/)
[![Landing Page](https://img.shields.io/badge/Website-Live-brightgreen)](https://iamoumaribrahim.github.io/manar-search-rescue-drone/)

</div>

> **PROPRIETARY BY DEFAULT · SELECTED COMPONENTS MAY BE OPEN-SOURCED WHEN EXPLICITLY MARKED**
>
> MANAR is independently created and owned by **Oumar Ibrahim**.
> Unless explicitly licensed otherwise, all source code, engineering material,
> algorithms, documentation, and project assets in this repository are proprietary.

<h2 align="center">Component Overview</h2>

<p align="center">
  <img
    src="assets/Images/Branding/System Component Overview.png"
    alt="MANAR multisensor search-and-rescue system component overview"
    width="820"
  />
  <br />
  <em>MANAR Multisensor System Component Overview</em>
</p>

### Multisensor System Components

| Component | Primary Role |
| :--- | :--- |
| **Thermal** | Person/heat detection |
| **RGB/day** | Daytime detection/verification |
| **Low-light/IR** | Night visual confirmation |
| **24 GHz FMCW** | Presence, range, motion, breathing |
| **Speaker + mic** | Prompt, listen, direction finding |
| **Passive RF** | Detect/correlate device emissions |
| **Amber beacon** | 360° visual alert |
| **White strobe** | Directional visual guidance |
| **Downward spotlight** | Close-range illumination |
| **Heliograph mirrors** | Passive daylight signaling |
| **Smoke marker** | Location/wind marking |

### Repository Map

| Directory / File | Description |
| :--- | :--- |
| [`core/`](core/) | Deterministic C++ control system & flight state engine |
| [`perception/vision/`](perception/vision/) | Visual target detection, models (YOLO11n / D-FINE-N), and benchmark suite |
| [`perception/mamba/`](perception/mamba/) | Mamba SSM temporal multi-sensor fusion research |
| [`gui/`](gui/) | Operator Ground Control Station (GCS) staging |
| [`landing-page/`](landing-page/) | Project website |
| [`docs/`](docs/) | Technical paper, operator manual, and reference library |
| [`assets/`](assets/) | Shared branding, diagrams, and video evaluation datasets |
| [`SECURITY.md`](SECURITY.md) | Security policy |
| [`LICENSE.md`](LICENSE.md) | Licensing terms |

### Key Architectural Decisions

#### Implemented & Benchmarked Subsystems
| Subsystem / Decision | Validation & Rationale |
| :--- | :--- |
| **YOLO11n Edge Detector** | Empirically selected via multi-spectral benchmark (~29.2 ms CPU latency, ~1.73× faster than D-FINE-N, 0 false alarms on clutter, ~2.6M params) for real-time edge person detection. |
| **Greedy Route Heuristic** | Fast, deterministic $O(n^2)$ waypoint sequencing for multi-sector search planning with plan locking and operator override privilege. |
| **Deterministic C++ State Core** | State-locked 10 Hz flight state machine, structured JSON command arbitration, and battery-reserve Return-to-Launch (RTL) preemption. |

#### Planned & Investigated Architecture
| Area / Concept | Current Scope & Rationale |
| :--- | :--- |
| **Mamba SSM Temporal Fusion** | Investigating linear $O(L)$ selective state spaces for real-time temporal verification across multi-sensor feature windows as a candidate escalation gate. |
| **1D CNN Feature Projection** | Planned feature compressor to align 2D visual candidates into compact 1D embeddings alongside acoustic, radar, and RF anomaly streams. |
| **Two-Level Passive RF** | Proposed architecture: Level 1 CFAR anomaly triggering + Level 2 signal derivative trend tracking ($\Delta S_k$). |
| **Multi-Tiered Telemetry** | Planned adaptive transmission rate policy (1–2 Hz baseline, 5 Hz proximity, on-event escalation) to prevent RF link saturation. |

---

## Multimodal Sensing & Target Verification

<p align="center">
  <img
    src="assets/Images/Visualization/RGB, IR, and Thermal Multisensor Detection.png"
    alt="RGB, IR, and Thermal Multisensor Detection"
    width="650"
  />
  <br />
  <em>Synchronized Day RGB, Low-Light IR, and Long-Wave Thermal (LWIR) target verification across diurnal and environmental extremes.</em>
</p>

---

## Fig. 2. Conceptual 10-stage search, verification, and rescue lifecycle.

MANAR coordinates flight execution through a modular navigation hierarchy and deterministic mission logic.

<p align="center">
  <img
    src="assets/Images/Paper/fig2_conceptual_lifecycle.png"
    alt="Fig. 2. Conceptual 10-stage search, verification, and rescue lifecycle."
    width="480"
  />
  <br />
  <em>Fig. 2. Conceptual 10-stage search, verification, and rescue lifecycle.</em>
</p>

---

## Perception & Sensor-Fusion Pipeline

Synchronized feature vectors $\mathbf{x}_t \in \mathbb{R}^D$ sampled at $10\text{ Hz}$ across $20\text{--}50$ time steps ($2\text{--}5\text{ s}$ window) are evaluated by a Mamba State Space Model (SSM). Mamba acts strictly as an alert escalation filter (Reject candidate, Continue verification, or Alert operator), while final rescue determination is reserved exclusively for the human operator.

<p align="center">
  <img
    src="assets/Images/Visualization/CNN and Mamba Sensor-Fusion Architecture.png"
    alt="Fig. 5. Planned multisensor perception and Mamba hover verification pipeline"
    width="900"
  />
  <br />
  <em>Fig. 5. Planned multisensor perception and Mamba hover verification pipeline.</em>
</p>

---

## Fig. 3. Spatial geometry: Lawnmower sweep coverage pattern (left) and sequential greedy route progression with RTL return leg (right).

When operators specify multiple search sectors, MANAR executes an $O(n^2)$ greedy nearest-neighbor route optimizer to minimize travel transit distance, combined with systematic lawnmower sweeps.

<p align="center">
  <img
    src="assets/Images/Paper/fig3_lawnmower_and_route.png"
    alt="Fig. 3. Spatial geometry: Lawnmower sweep coverage pattern (left) and sequential greedy route progression with RTL return leg (right)."
    width="540"
  />
  <br />
  <em>Fig. 3. Spatial geometry: Lawnmower sweep coverage pattern (left) and sequential greedy route progression with RTL return leg (right).</em>
</p>

---

## System Demonstrations

> **Current status:** Hardware component sizing, React Operator GUI, and downstream multisensor fusion interface.

<div align="center">

### Autonomous Flight Core & Lawnmower Search
<img src="assets/Videos/demo.gif" alt="MANAR Autonomous Flight and Terminal Demo" width="900">
<br/><br/>

### YOLO11n Real-Time Target Detection
<img src="assets/Videos/demo2.gif" alt="MANAR Visual Detection Demo" width="900">

</div>

---

## Try It Yourself (Quick Start)

> **Requirements:** C++17 compliant compiler (`g++` / MinGW-w64, Clang, or MSVC) on Windows 10/11, Linux, or macOS.

### 1. Build the Core Applications

Open PowerShell or terminal in the repository root:

```powershell
cd core
mkdir -p build

g++ -std=c++17 apps/control.cpp system/shared.cpp system/flight.cpp system/components.cpp system/drone.cpp system/mission.cpp system/route_optimizer.cpp -I. -Ithird_party -o build/control.exe
g++ -std=c++17 apps/terminal.cpp -I. -Ithird_party -o build/terminal.exe
g++ -std=c++17 apps/setup.cpp -I. -Ithird_party -o build/setup.exe
```

### 2. Launch the Control Core & Operator Terminal

In **Terminal 1** (Flight Control Engine):
```powershell
cd core
.\build\control.exe
```

In **Terminal 2** (Interactive Operator Command Station):
```powershell
cd core
.\build\terminal.exe
```

> [!TIP]
> For a full mission simulation walkthrough (setting search waypoints, greedy route optimization, sensor toggles, and RTL failsafes) as well as pre-flight parameter configuration (`setup.exe`), see the **[Operator Guide](docs/OPERATOR_GUIDE.md)**.

---

## Project Milestones

- [x] **System Architecture & Safety Specification**: Conceptual 10-stage lifecycle, SWaP constraints, safety invariants, and modular repo hierarchy.
- [x] **Deterministic C++ Control Core (`core/`)**: 10 Hz flight state machine, greedy route optimizer, failsafe RTL preemption, JSON IPC, and interactive terminal.
- [x] **Edge Visual Perception & Benchmark (`perception/vision/`)**: Multi-spectral SAR video benchmark, locked YOLO11n ONNX inference engine (~29 ms CPU latency).
- [x] **Project Presentation & Website (`landing-page/`)**: Responsive project landing page, system diagrams, and public repository showcase.
- [ ] **Temporal Multi-Sensor Fusion (`perception/mamba/`)**: Mamba SSM sequence modeling across synchronized visual, radar, RF, and acoustic streams.
- [ ] **Modern Operator Ground Station (`gui/`)**: Real-time React / TypeScript web interface with geospatial mapping and live telemetry streaming.
- [ ] **Airframe Engineering & Hardware Validation**: Component mass/power budgets, 3D CAD modeling, and propulsion validation for 1-hour active search.
- [ ] **Academic Publication (`docs/paper/`)**: LaTeX technical paper, benchmark analysis, and formal dissemination.

---

## Ownership and License

Copyright © 2026 Oumar Ibrahim. All rights reserved.

Unless explicitly stated otherwise, all materials in this repository are
proprietary and governed by the MANAR Proprietary Software and Materials License.

Selected files, components, or directories may be released under separate
open-source licenses. Any such license applies only to the material explicitly
identified as being covered by it.

See the [LICENSE](LICENSE.md) and any applicable file or directory license notices for
the complete terms.
