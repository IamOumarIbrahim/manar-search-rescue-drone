---
title: "Localization and Tracking of Airborne Emitting Targets Using UAV Swarms"
short_title: "Airborne RF Target Tracking UAV"
authors:
  - "Hazem Sallouha"
  - "Antonio Chiumento"
  - "Sofie Pollin"
year: 2019
publication: "IEEE Access"
publisher: "IEEE"
doi: "10.1109/ACCESS.2019.2944773"
arxiv: null
site_url: "https://doi.org/10.1109/ACCESS.2019.2944773"
pdf_url: null
peer_reviewed: true
paper_type: "journal"
primary_topic: "passive_rf"
tags:
  - RSSI-localization
  - airborne-rf-tracking
  - uav-swarm
  - emitter-geolocalization
manar_relevance:
  - passive-rf
  - target-detection
  - uav-sar
relevance_score: 9
---
# Localization and Tracking of Airborne Emitting Targets Using UAV Swarms

## Citation

H. Sallouha et al. "Localization and Tracking of Airborne Emitting Targets Using UAV Swarms." *IEEE Access*, 2019. DOI: 10.1109/ACCESS.2019.2944773.

## Abstract / Paper Summary

Investigates RSSI-based localization and tracking of ground/airborne RF emitters using flying UAV platforms, modeling air-to-ground path loss and flight trajectory optimization.

## Why This Matters for MANAR

Provides path loss models and spatial filtering architectures directly applicable to MANAR's RF victim localization module.

## Core Architecture / Method

```
UAV Trajectory -> Spatial RSSI Sampling -> Log-Distance Path Loss Model -> Unscented Kalman Filter -> Emitter Position
```

## Detailed Technical Description

### Detailed Technical Description
Combines spatial RSSI measurements along UAV flight path with UKF filtering to achieve sub-15 meter emitter position estimation.

## Key Equations / Algorithms

\[ RSSI(d) = P_t + G_t + G_r - 10 \eta \log_{10}(d) - X_{\sigma} \]

## Key Parameters / Measurements

| Parameter | Value | Context |
|---|---:|---|
| Localization Error | < 12 meters | Wilderness test scenario |
| Path Loss Exponent (eta) | 2.2 - 2.8 | Air-to-ground channel |

## Useful Findings for MANAR

- PAPER FINDING: Single or multi-UAV dynamic flight paths enable accurate RSSI-based emitter geolocalization within 3-5 flight passes.
- MANAR RELEVANCE: Directly supports MANAR's supervised autonomy RF search pattern.

## MANAR Design Questions This Paper Helps Answer

- How does altitude variation affect MANAR's RSSI path loss estimation accuracy?

## What This Paper Does NOT Establish

- Assumes continuous RF transmission rather than bursty cellular paging signals.

## References Worth Following

- Guvenc et al. (2018).

