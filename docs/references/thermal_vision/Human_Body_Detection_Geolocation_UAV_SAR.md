---
title: "Human Body Detection and Geolocation for UAV Search and Rescue Missions"
short_title: "Thermal Human Detection & Geolocation UAV SAR"
authors:
  - "Piotr Rudol"
  - "Patrick Doherty"
year: 2008
publication: "IEEE Aerospace Conference (AERO 2008)"
publisher: "IEEE"
doi: "10.1109/AERO.2008.4526559"
arxiv: null
site_url: "https://doi.org/10.1109/AERO.2008.4526559"
pdf_url: null
peer_reviewed: true
paper_type: "conference"
primary_topic: "thermal_vision"
tags:
  - UAV-search-and-rescue
  - thermal-body-detection
  - geolocation
  - gimbal-target-tracking
manar_relevance:
  - thermal-vision
  - uav-sar
  - target-detection
relevance_score: 9
---
# Human Body Detection and Geolocation for UAV Search and Rescue Missions

## Citation

P. Rudol and P. Doherty. "Human Body Detection and Geolocation for UAV Search and Rescue Missions." *IEEE AERO*, 2008. DOI: 10.1109/AERO.2008.4526559.

## Abstract / Paper Summary

Presents an end-to-end UAV search and rescue pipeline that detects human bodies in thermal IR video, projects image coordinates into GPS coordinates using camera gimbal telemetry, and alerts human operators.

## Why This Matters for MANAR

Provides MANAR with a complete reference architecture for target geolocation from airborne thermal imagery.

## Core Architecture / Method

```
Thermal Camera -> Thermal Hotspot Detector -> Body Shape Filter -> Gimbal Projection Matrix -> GPS Target Location
```

## Detailed Technical Description

### Detailed Technical Description
Fuses thermal detection bounding box centroids with UAV pose (roll, pitch, yaw, GPS) to estimate ground target coordinates.

## Key Equations / Algorithms

\[ \mathbf{P}_{GPS} = \mathbf{P}_{UAV} + \mathbf{R}_{body}^{NED} \cdot \mathbf{R}_{cam}^{body} \cdot \mathbf{v}_{ray} \cdot d_{ground} \]

## Key Parameters / Measurements

| Parameter | Value | Context |
|---|---:|---|
| Geolocation Error | < 5 m | Tested at 30 m altitude |
| Processing Frame Rate | 15 fps | Onboard PC104 computer |

## Useful Findings for MANAR

- PAPER FINDING: Integrating gimbal pitch/yaw and UAV altitude enables sub-5m target geolocation accuracy.
- MANAR RELEVANCE: Direct template for MANAR C++ target geolocation estimator.

## MANAR Design Questions This Paper Helps Answer

- How does IMU gimbal drift impact target coordinate projection accuracy?

## What This Paper Does NOT Establish

- Tested on small static bodies in open field.

## References Worth Following

- Portmann et al. (2014).

