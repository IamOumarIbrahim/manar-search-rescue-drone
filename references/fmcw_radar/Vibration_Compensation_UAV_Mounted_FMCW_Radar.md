---
title: "Vibration Compensation for UAV-Mounted FMCW Radar Target Detection"
short_title: "Vibration Mitigation in Airborne FMCW Radar"
authors:
  - "Ali Mostajabi"
  - "Hamidreza Motahari"
  - "Mohammad Ghavami"
year: 2020
publication: "IEEE International Radar Conference (RADAR 2020)"
publisher: "IEEE"
doi: "10.1109/RADAR48443.2020.9114750"
arxiv: null
site_url: "https://doi.org/10.1109/RADAR48443.2020.9114750"
pdf_url: null
peer_reviewed: true
paper_type: "conference"
primary_topic: "fmcw_radar"
tags:
  - airborne-radar-vibration
  - vibration-compensation
  - IMU-radar-fusion
  - rotor-noise-mitigation
manar_relevance:
  - fmcw-radar
  - navigation
  - payload-swap
relevance_score: 10
---
# Vibration Compensation for UAV-Mounted FMCW Radar Target Detection

## Citation

A. Mostajabi et al. "Vibration Compensation for UAV-Mounted FMCW Radar Target Detection." *IEEE RadarConf*, 2020. DOI: 10.1109/RADAR48443.2020.9114750.

## Abstract / Paper Summary

Addresses the critical problem of UAV motor and rotor vibrations corrupting FMCW radar phase and micro-Doppler measurements. The authors propose an IMU-assisted phase compensation algorithm that cancels high-frequency platform displacement artifacts.

## Why This Matters for MANAR

CRITICAL for MANAR. Without vibration compensation, airborne FMCW radar cannot detect human micro-Doppler or breathing signals due to rotor vibration dominance.

## Core Architecture / Method

```
IMU High-Rate Displacement -> Phase Correction Matrix -> FMCW Radar Range-Phase Stream -> Cleaned Micro-Doppler Spectrogram
```

## Detailed Technical Description

### Detailed Technical Description
Fuses 1 kHz IMU accelerometer data with radar chirp phase to subtract platform vibration displacement $x_{uav}(t)$.

## Key Equations / Algorithms

\[ \phi_{corr}(t) = \phi_{raw}(t) - \frac{4\pi}{\lambda} x_{uav}(t) \]

## Key Parameters / Measurements

| Parameter | Value | Context |
|---|---:|---|
| Vibration Suppression | > 18 dB | Rotor frequency rejection |
| IMU Sampling Rate | 1000 Hz | Sync rate with radar chirps |

## Useful Findings for MANAR

- PAPER FINDING: IMU-assisted phase subtraction reduces rotor vibration phase noise by > 18 dB.
- MANAR RELEVANCE: Essential algorithm for enabling airborne radar sensing on MANAR multirotor.

## MANAR Design Questions This Paper Helps Answer

- Is MANAR's flight controller IMU sampling rate sufficient for high-frequency vibration cancellation?

## What This Paper Does NOT Establish

- Requires microsecond-level hardware synchronization between IMU and radar ADC.

## References Worth Following

- Chen et al. (2006).

