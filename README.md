

# MANAR / منار

> **PROPRIETARY CORE · SELECTED OPEN-SOURCE COMPONENTS**
>
> MANAR is independently created and owned by **Oumar Ibrahim**.
> This repository contains selected demonstrations, documentation, and
> portfolio materials only.

## Project Overview

MANAR / منار is a supervised-autonomy, multisensor search-and-rescue
drone system designed to assist search teams while keeping consequential
decisions under human supervision.



## Landing Page

<a href="https://iamoumaribrahim.github.io/manar-search-rescue-drone/" target="_blank">MANAR: Supervisied autonomy for search and rescue</a>

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

## Established
<table>
  <tr>
    <td align="center" width="50%">
      <img
        src="assets\Temp\currentprogress3.png"
        alt="MANAR Version 1.0 Operator Dashboard"
        width="100%"
      />
      <br />
      <em>MANAR Human Control System — Terminal-Based Component Status Interface</em>
    </td>
    <td align="center" width="50%">
      <img
        src="assets\Temp\currentprogress4.png"
        alt="MANAR Human Control System"
        width="100%"
      />
      <br />
      <em>MANAR Human Control System — High-Level C++ Software Architecture</em>
    </td>
  </tr>
</table>
<p align="center">
  <img
    src="assets\Temp\Currentprogress2.png"
    alt="MANAR Human Control System — Terminal-Based Component Status Interface"
    width="900"
  />
  <br />
  <em>MANAR Version 1.0 Operator Dashboard — Multisensor Live Feed Interface</em>
</p>

## Project Milestones


> - [x] Initial Ideation
> - [x] GitHub Repository creation
> - [x] Initial Branding
> - [x] Project scope hardening
> - [x] Project limitations and constraints
> - [x] Basic web dashboard — v1.0
> - [x] Landing Page
> - [x] Initial deterministic control system design
> - [x] JSON Configuration and Setup exe
- [ ] Decouple terminal from control logic
- [ ] Build deterministic controls for components and drone
- [ ] Build the engine control system
- [ ] Build the component controls
- [ ] Test and design final drone configuration
- [ ] Model drone movement in Blender
- [ ] Build the TypeScript GUI
- [ ] Couple the TypeScript GUI with C++ control system
- [ ] Build the Python machine-learning algorithm
- [ ] Couple TypeScript GUI and C++
- [ ] Couple Python ML with TS and C++
- [ ] Test and train models
- [ ] System integration and end-to-end testing
- [ ] To be determined...
- [ ] LaTeX Report, Presentation and Full Documentation
- [ ] Final Publishing & LinkedIn Post
- [ ] Repository Maintenance

## Architecture

```mermaid
flowchart TD
    GUI["TypeScript / React<br/>Operator GUI"]
    CORE["C++<br/>Deterministic Control Core"]
    ML["Python / PyTorch<br/>Machine Learning"]

    GUI <-->|"HTTP / localhost"| CORE
    CORE <-->|"Detection / Sensor Data"| ML
```

**C++** owns system state and deterministic control.
**Python / PyTorch** handles machine learning and sensor analysis.
**TypeScript / React** provides the operator interface.


## Project Structure

```text
manar-search-rescue-drone/
│
├── core/                       # Deterministic control prototype
│   ├── config.json             # Persistent system configuration
│   ├── control.cpp             # Main control system
│   ├── setup.cpp               # Configuration setup utility
│   ├── logs.txt                # Runtime logs
│   └── nlohmann/
│       └── json.hpp            # JSON library
│
├── webdashboardv1/             # Prototype operator dashboard
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
├── assets/                     # Project media and design material
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
