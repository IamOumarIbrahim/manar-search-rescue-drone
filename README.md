<div align="center">

  <img src="assets/Images/Branding/GitHub README Header.png" alt="MANAR Search & Rescue Drone Header" width="100%" />

# MANAR / منار

**Supervised-Autonomy Multisensor Search-and-Rescue System**

[![License: Proprietary](https://img.shields.io/badge/License-Proprietary-yellow.svg)](LICENSE.md)
[![C++17](https://img.shields.io/badge/C%2B%2B-17-00599C.svg?logo=c%2B%2B)](core/)
[![TypeScript](https://img.shields.io/badge/TypeScript-Operator%20GUI-3178C6.svg?logo=typescript\&logoColor=white)](gui/)
[![React](https://img.shields.io/badge/React-Operator%20Interface-61DAFB.svg?logo=react\&logoColor=black)](gui/)
[![WebSocket](https://img.shields.io/badge/WebSocket-Communication-010101.svg)](core/)
[![JSON](https://img.shields.io/badge/JSON-Runtime%20%26%20Messaging-000000.svg?logo=json\&logoColor=white)](core/)
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
> **Current phase:** Defining engineering requirements.

<p align="center">
  <img src="assets/Videos/demo.gif" alt="MANAR Demo" width="900">
</p>

## Closed Design Decisions

| Area | Decision |
| --- | --- |
| **Architecture** | React + TypeScript GUI, JSON/WebSocket comms, C++ control core. |
| **State authority** | C++ control core owns runtime state. |
| **Mission model** | One active mission with operator-controlled launch, abort, landing and RTH. |
| **Operator interface** | Live flight, mission, detection and imaging data. |
| **Multisensor design** | Imaging, FMCW radar, passive RF and audio sensing. |
| **Passive RF** | Attention trigger only; not independent rescuee confirmation. |
| **Human supervision** | Autonomy handles detection; critical control remains supervised. |
| **Future ML** | Python + PyTorch for perception and sensor fusion. |

## Project Milestones


- [x] Initial ideation & system specification
- [x] GitHub repository setup & project licensing
- [x] Brand visual identity & asset guidelines
- [x] Scope definition & operational constraint hardening
- [x] Prototype web dashboard (v1.0) & landing page
- [x] Deterministic C++ control system design
- [x] JSON runtime configuration & `setup.exe` utility
- [x] Decouple terminal interface from control logic

- [ ] Define MANAR V1 engineering & mission requirements
- [ ] Research and select candidate hardware components
- [ ] Establish payload mass, power & interface budgets
- [ ] Validate aircraft physical feasibility & propulsion requirements
- [ ] Freeze MANAR V1 simulated aircraft configuration
- [ ] Define component, power & communication architecture
- [ ] Build deterministic C++ component subsystem
- [ ] Expand deterministic flight & mission simulation engine
- [ ] Model aircraft flight dynamics & simulated sensor behavior
- [ ] Create dimensionally grounded 3D drone model in Blender
- [ ] Develop TypeScript / React Operator GUI
- [ ] Integrate TypeScript GUI with C++ Control Core
- [ ] Develop Python / PyTorch perception pipeline
- [ ] Develop multisensor detection & fusion logic
- [ ] Integrate Python ML pipeline with C++ Control Core & GUI
- [ ] Train, evaluate & validate perception models
- [ ] Perform end-to-end simulated mission testing
- [ ] TBD
- [ ] Complete LaTeX report, presentation & technical documentation
- [ ] Produce final demonstration & portfolio material
- [ ] Final public launch & repository maintenance


## Project Structure

```text
manar-search-rescue-drone/
│
├── core/                                  # Deterministic C++ control system
│   ├── control.cpp                        # Control process & runtime authority
│   ├── terminal.cpp                       # Operator CLI & command interface
│   ├── setup.cpp                          # Persistent configuration utility
│   ├── runtime_template.json              # Reference runtime-state schema
│   └── nlohmann/
│       └── json.hpp                       # JSON library
│
├── assets/
│   ├── Docs/                              # Engineering & development documentation
│   │   ├── Decisions/
│   │   │   ├── BEHAVIOR_DECISIONS.md
│   │   │   └── MANAR_V1_PASSIVE_RF_DECISIONS.md
│   │   ├── development/
│   │   │   └── M1_M4_IMPLEMENTATION_MAP.md
│   │   └── engineering/
│   │       ├── MANAR_V1_ENGINEERING_RESEARCH_V3.md
│   │       └── MANAR_V1_REQUIREMENTS.md
│   ├── Images/
│   │   └── Branding/
│   ├── Videos/
│   └── Temp/
│
├── references/                            # Research & technical references
│   ├── README.md
│   ├── REFERENCES.md
│   ├── acoustics/
│   ├── cfar/
│   ├── fmcw_radar/
│   ├── multisensor_fusion/
│   ├── navigation/
│   ├── passive_rf/
│   ├── rgb_low_light_vision/
│   ├── target_detection/
│   ├── thermal_vision/
│   └── uav_sar/
│
├── manar-landing-page/                    # Public project website
│   ├── index.html
│   ├── script.js
│   ├── styles.css
│   └── assets/
│
├── webdashboardv1/                        # Prototype dashboard v1.0
│   ├── dashboard.html
│   ├── script.js
│   └── style.css
│
├── .github/
│   └── workflows/
│       └── deploy-pages.yml
│
├── MANAR.md                               # MANAR behavioral/system decisions
├── payload.md                             # Payload definition
├── ROADMAP.md                             # Development roadmap
├── README.md
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

