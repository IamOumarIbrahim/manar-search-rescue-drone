---
title: "Toward a Fully Autonomous UAV: Research Platform for Indoor and Outdoor Urban Search and Rescue"
short_title: "Autonomous Urban Search & Rescue UAV Platform"
authors:
  - "Teodor Tomic"
  - "Markus Wimmer"
  - "Korbinian Schmid"
  - "Michael Pitz"
  - "Alain Piccini"
year: 2012
publication: "IEEE Robotics & Automation Magazine"
publisher: "IEEE"
doi: "10.1109/MRA.2012.2206473"
arxiv: null
site_url: "https://doi.org/10.1109/MRA.2012.2206473"
pdf_url: null
peer_reviewed: true
paper_type: "journal"
primary_topic: "uav_sar"
tags:
  - autonomous-UAV
  - SAR-platform
  - system-architecture
  - fail-safe-flight
manar_relevance:
  - uav-sar
  - navigation
  - payload-swap
relevance_score: 10
---
# Toward a Fully Autonomous UAV: Research Platform for Indoor and Outdoor Urban Search and Rescue

## Citation

T. Tomic et al. "Toward a Fully Autonomous UAV: Research Platform for Indoor and Outdoor Urban Search and Rescue." *IEEE RAM*, 2012. DOI: 10.1109/MRA.2012.2206473.

## Abstract / Paper Summary

Presents the full hardware and software architecture of an autonomous search and rescue multirotor system, detailing sensor payload integration, onboard compute, state estimation, and emergency fail-safe state machines.

## Why This Matters for MANAR

Provides MANAR with an end-to-end blueprint for deterministic C++ flight control and payload software integration.

## Core Architecture / Method

```
Sensors (IMU, Laser, Vision) -> State Estimator -> C++ Flight Controller -> Payload Computer -> Ground Station Telemetry
```

## Detailed Technical Description

### Detailed Technical Description
Details system architecture separating low-level flight control (C++) from high-level perception and mission planning.

## Key Equations / Algorithms

\[ \mathbf{x}_{k+1} = \mathbf{f}(\mathbf{x}_k, \mathbf{u}_k) + \mathbf{w}_k \]

## Key Parameters / Measurements

| Parameter | Value | Context |
|---|---:|---|
| Payload Capacity | 1.2 kg | Multi-sensor payload weight |
| Onboard Compute | Intel Core i7 | High-level perception CPU |

## Useful Findings for MANAR

- PAPER FINDING: Rigid architectural separation between low-level flight stabilization and high-level perception prevents perception crashes from causing drone crashes.
- MANAR RELEVANCE: Directly mirrors MANAR's architectural split between C++ control and Python/TS perception/GUI layers.

## MANAR Design Questions This Paper Helps Answer

- How should MANAR structure shared memory IPC between C++ flight control and Python ML layers?

## What This Paper Does NOT Establish

- Pre-dates modern ARM Jetson edge AI platforms.

## References Worth Following

- Goodrich et al. (2008).

