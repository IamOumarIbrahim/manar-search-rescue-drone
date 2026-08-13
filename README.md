<div align="center">

  <img src="assets/Images/Branding/GitHub README Header.png" alt="MANAR Search & Rescue Drone Header" width="100%" />

  # MANAR / منار
  **Supervised-Autonomy Multisensor Search-and-Rescue System**

  [![License: Proprietary](https://img.shields.io/badge/License-Proprietary-yellow.svg)](LICENSE.md)
  [![C++17](https://img.shields.io/badge/C%2B%2B-17-00599C.svg?logo=c%2B%2B)](core/)
  [![JSON](https://img.shields.io/badge/JSON-Runtime%20%26%20Config-000000.svg?logo=json&logoColor=white)](core/)
  [![Landing Page](https://img.shields.io/badge/Website-Live-brightgreen)](https://iamoumaribrahim.github.io/manar-search-rescue-drone/)

</div>

> **PROPRIETARY CORE · SELECTED OPEN-SOURCE COMPONENTS**
>
> MANAR is independently created and owned by **Oumar Ibrahim**.
> This repository contains selected demonstrations, documentation, and portfolio materials only.

### Component Overview

<p align="center">
  <img
    src="assets/Images/Branding/System Component Overview.png"
    alt="MANAR multisensor search-and-rescue system component overview"
    width="900"
  />
  <br />
  <em>MANAR Multisensor System Component Overview</em>
</p>

| Component              | Primary role                       |
| ---------------------- | ---------------------------------- |
| **Thermal**            | Person/heat detection              |
| **RGB/day**            | Daytime detection/verification     |
| **Low-light/IR**       | Night visual confirmation          |
| **24 GHz FMCW**        | Presence, range, motion, breathing |
| **Speaker + mic**      | Prompt, listen, direction finding  |
| **Passive RF**         | Detect/correlate device emissions  |
| **Amber beacon**       | 360° visual alert                  |
| **White strobe**       | Directional visual guidance        |
| **Downward spotlight** | Close-range illumination           |
| **Heliograph mirrors** | Passive daylight signaling         |
| **Smoke marker**       | Location/wind marking              |

## Current Status
> **Current phase:** Developing JSON runtime state database and configuration.

<p align="center">
  <img src="assets/Videos/demo.gif" alt="MANAR Demo" width="900">
</p>


## Development Roadmap

Development is tracked in [ROADMAP.md](ROADMAP.md).

## Project Milestones

Development is phase-locked: later phases depend on the core requirements of the previous phase being completed. Detailed engineering tasks are tracked in [`ROADMAP.md`](ROADMAP.md).

### Phase 0 — Foundation & Initial Prototype

- [x] Initial ideation & system specification
- [x] GitHub repository setup & project licensing
- [x] Brand visual identity & asset guidelines
- [x] Scope definition & operational constraint hardening
- [x] Prototype web dashboard (v1.0) & landing page
- [x] Deterministic C++ control system design
- [x] JSON runtime configuration & `setup.exe` utility
- [x] Decouple terminal interface from control process

### Phase 1 — Deterministic Core Hardening

- [ ] Establish authoritative runtime & state ownership
- [ ] Implement reliable command lifecycle & acknowledgment
- [ ] Harden JSON, runtime persistence & file handling
- [ ] Stabilize deterministic drone & mission state behavior
- [ ] Resolve contradictory mission, flight & RTH transitions

### Phase 2 — MANAR V1 Engineering Definition

- [ ] Define MANAR V1 engineering & mission requirements
- [ ] Research and select candidate hardware components
- [ ] Establish payload mass, power & interface budgets
- [ ] Validate aircraft physical feasibility & propulsion requirements
- [ ] Define component, power & communication architecture
- [ ] Freeze MANAR V1 simulated aircraft configuration

### Phase 3 — Flight & Mission Simulation

- [ ] Build deterministic C++ component subsystem
- [ ] Implement continuous movement simulation
- [ ] Implement dynamic battery & safety behavior
- [ ] Expand search, navigation & mission execution
- [ ] Model simulated sensor behavior
- [ ] Validate lawnmower search, RTH, abort & mission-completion behavior
- [ ] Create dimensionally grounded 3D drone model in Blender

### Phase 4 — External Interface & Operator Control

- [ ] Define C++ external command & telemetry interface
- [ ] Select and implement local transport architecture
- [ ] Develop TypeScript / React Operator GUI
- [ ] Integrate Operator GUI with C++ Control Core
- [ ] Implement live telemetry, command feedback & control heartbeat
- [ ] Add map, route & mission-state visualization

### Phase 5 — Perception & Supervised Autonomy

- [ ] Define structured perception-event interface
- [ ] Develop Python / PyTorch perception pipeline
- [ ] Develop multisensor detection & fusion logic
- [ ] Integrate Python perception with C++ Control Core
- [ ] Integrate perception events with Operator GUI
- [ ] Implement candidate verification & human-authorization flow
- [ ] Train, evaluate & validate perception models

### Phase 6 — System Validation

- [ ] Perform end-to-end simulated mission testing
- [ ] Validate nominal search-and-rescue mission flow
- [ ] Validate abort, RTH, low-battery & failure scenarios
- [ ] Validate C++ / GUI / ML integration
- [ ] Resolve critical system defects before release

### Phase 7 — Documentation & Public Release

- [ ] Complete LaTeX report, presentation & technical documentation
- [ ] Finalize repository architecture & setup documentation
- [ ] Produce final demonstration & portfolio material
- [ ] Final public launch
- [ ] Repository maintenance

## Project Structure

```text
## Project Structure

```text
manar-search-rescue-drone/
│
├── core/                       # Deterministic C++ control system
│   ├── control.cpp             # Persistent control process & system authority
│   ├── terminal.cpp            # Operator CLI & command producer
│   ├── setup.cpp               # Persistent configuration setup utility
│   ├── runtime_template.json   # Reference schema for runtime state
│   └── nlohmann/
│       └── json.hpp            # JSON library
│
├── webdashboardv1/             # Prototype operator dashboard (v1.0)
│   ├── dashboard.html
│   ├── script.js
│   └── style.css
│
├── manar-landing-page/         # MANAR project website
│   ├── index.html
│   ├── script.js
│   ├── styles.css
│   └── assets/
│       ├── Audio/
│       └── Images/
│           ├── Branding/
│           └── patterns/
│
├── assets/                     # Project media & design material
│   ├── Images/
│   │   └── Branding/
│   ├── Videos/
│   │   ├── Showcase footage/
│   │   └── Training videos/
│   └── Temp/
│
├── .github/
│   └── workflows/
│       └── deploy-pages.yml
│
├── README.md
├── REFERENCES.md
├── ROADMAP.md                  # Phase-ordered development roadmap
├── SECURITY.md
├── LICENSE.md
└── .gitignore
```


## Ownership and License

Copyright © 2026 Oumar Ibrahim. All rights reserved.

Unless explicitly stated otherwise, all materials in this repository are
proprietary and governed by the MANAR Proprietary Software and Materials License.

Selected files, components, or directories may be released under separate
open-source licenses. Any such license applies only to the material explicitly
identified as being covered by it.

See the [LICENSE](LICENSE) and any applicable file or directory license notices for
the complete terms.
