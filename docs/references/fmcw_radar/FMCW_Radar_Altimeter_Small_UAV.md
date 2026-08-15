---
title: "Design and Evaluation of a 24 GHz FMCW Radar Altimeter for Small Unmanned Aerial Vehicles"
short_title: "24 GHz FMCW Radar Altimeter for MAVs"
authors:
  - "Young-Seok Kim"
  - "Jin-Kyu Park"
  - "Seung-Hyun Kong"
year: 2019
publication: "IEEE Transactions on Instrumentation and Measurement"
publisher: "IEEE"
doi: "10.1109/TIM.2018.2878621"
arxiv: null
site_url: "https://doi.org/10.1109/TIM.2018.2878621"
pdf_url: null
peer_reviewed: true
paper_type: "journal"
primary_topic: "fmcw_radar"
tags:
  - radar-altimeter
  - FMCW-altimetry
  - terrain-following
  - altitude-estimation
manar_relevance:
  - fmcw-radar
  - navigation
relevance_score: 9
---
# Design and Evaluation of a 24 GHz FMCW Radar Altimeter for Small Unmanned Aerial Vehicles

## Citation

Y.-S. Kim et al. "Design and Evaluation of a 24 GHz FMCW Radar Altimeter for Small Unmanned Aerial Vehicles." *IEEE Trans. Instrum. Meas.*, 2019. DOI: 10.1109/TIM.2018.2878621.

## Abstract / Paper Summary

Presents the hardware design and flight evaluation of a lightweight 24 GHz FMCW radar altimeter for UAV terrain-relative flight, demonstrating sub-10 cm altitude accuracy over foliage and grass.

## Why This Matters for MANAR

Provides MANAR with proven radar altimetry architectures for GNSS-degraded terrain-following flight.

## Core Architecture / Method

```
24 GHz FMCW Transmitter -> Ground Reflection -> Beat Frequency FFT Peak -> Kalman Filter -> Altitude AGL
```

## Detailed Technical Description

### Detailed Technical Description
Uses 250 MHz sweep bandwidth, 150 g payload mass, and 2.5 W power consumption.

## Key Equations / Algorithms

\[ h_{AGL} = \frac{c \cdot f_{beat}}{2 \cdot (B / T_{sweep})} \]

## Key Parameters / Measurements

| Parameter | Value | Context |
|---|---:|---|
| Altitude Accuracy | < 8 cm | 0 - 50 m AGL range |
| Payload Weight | 150 g | SWaP footprint |
| Power | 2.5 W | Power consumption |

## Useful Findings for MANAR

- PAPER FINDING: 24 GHz FMCW radar altimetry provides reliable AGL altitude over tree canopies where optical flow fails.
- MANAR RELEVANCE: Directly applicable to MANAR terrain-relative flight controller.

## MANAR Design Questions This Paper Helps Answer

- Should MANAR integrate a standalone 24 GHz altimeter or use a combined mmWave sensing payload?

## What This Paper Does NOT Establish

- Maximum tested altitude 60 m AGL.

## References Worth Following

- Scaramuzza et al. (2014).

