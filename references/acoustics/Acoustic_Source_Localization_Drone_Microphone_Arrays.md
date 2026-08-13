---
title: "Acoustic Source Localization Using Drone Embedded Microphone Arrays"
short_title: "Drone-Embedded Acoustic Source Localization"
authors:
  - "Maximo Cobos"
  - "Jose J. Lopez"
  - "Javier Gomez"
  - "Antonio M. Vidal"
year: 2017
publication: "IEEE/ACM Transactions on Audio, Speech, and Language Processing"
publisher: "IEEE"
doi: "10.1109/TASLP.2017.2661706"
arxiv: null
site_url: "https://doi.org/10.1109/TASLP.2017.2661706"
pdf_url: null
peer_reviewed: true
paper_type: "journal"
primary_topic: "acoustics"
tags:
  - acoustic-localization
  - microphone-array
  - beamforming
  - drone-noise-suppression
manar_relevance:
  - acoustics
  - target-detection
relevance_score: 10
---
# Acoustic Source Localization Using Drone Embedded Microphone Arrays

## Citation

M. Cobos et al. "Acoustic Source Localization Using Drone Embedded Microphone Arrays." *IEEE/ACM TASLP*, 2017. DOI: 10.1109/TASLP.2017.2661706.

## Abstract / Paper Summary

Investigates sound source localization (human screams, whistles) from microphone arrays mounted on multirotor drones, introducing generalized cross-correlation with phase transform (GCC-PHAT) robust to ego-rotor noise.

## Why This Matters for MANAR

Provides MANAR with acoustic array beamforming and rotor noise suppression algorithms for victim voice localization.

## Core Architecture / Method

```
Microphone Array Signals -> Rotor Frequency Notch Filtering -> GCC-PHAT TDOA Estimation -> Steered Response Power (SRP-PHAT) -> Direction of Arrival (DOA)
```

## Detailed Technical Description

### Detailed Technical Description
Uses 8-element circular microphone array mounted below drone motors, filtering motor fundamental and harmonic tones.

## Key Equations / Algorithms

\[ R_{12}^{PHAT}(\tau) = \int_{-\infty}^{\infty} \frac{X_1(f) X_2^*(f)}{|X_1(f) X_2^*(f)|} e^{j 2 \pi f \tau} df \]

## Key Parameters / Measurements

| Parameter | Value | Context |
|---|---:|---|
| DOA Accuracy | < 6 degrees | Outdoor flight test |
| Array Radius | 15 cm | 8-channel circular geometry |

## Useful Findings for MANAR

- PAPER FINDING: GCC-PHAT beamforming combined with spatial notch filtering enables acoustic direction finding under active rotor noise.
- MANAR RELEVANCE: Core algorithm for MANAR acoustic sensing payload.

## MANAR Design Questions This Paper Helps Answer

- Can MANAR perform 8-channel 48 kHz audio sampling and FFT beamforming on ARM SoC in real time?

## What This Paper Does NOT Establish

- Severe wind turbulence degrades acoustic TDOA estimation above 10 m/s flight speed.

## References Worth Following

- Hioka et al. (2016).

