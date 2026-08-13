---
title: "Micro-Doppler Effect in Radar: Phenomena, Model, and Applications"
short_title: "Micro-Doppler Effect in Radar"
authors:
  - "Victor C. Chen"
  - "Fayin Li"
  - "Soo-Chang Ho"
  - "Harry Wechsler"
year: 2006
publication: "IEEE Transactions on Aerospace and Electronic Systems"
publisher: "IEEE"
doi: "10.1109/TAES.2006.1642571"
arxiv: null
site_url: "https://doi.org/10.1109/TAES.2006.1642571"
pdf_url: null
peer_reviewed: true
paper_type: "journal"
primary_topic: "fmcw_radar"
tags:
  - micro-doppler
  - radar-human-detection
  - spectrogram
  - joint-time-frequency
manar_relevance:
  - fmcw-radar
  - target-detection
relevance_score: 10
---
# Micro-Doppler Effect in Radar: Phenomena, Model, and Applications

## Citation

V. C. Chen et al. "Micro-Doppler Effect in Radar: Phenomena, Model, and Applications." *IEEE Trans. AES*, 2006. DOI: 10.1109/TAES.2006.1642571.

## Abstract / Paper Summary

This foundational paper establishes the theoretical physics and signal processing models for radar micro-Doppler signatures caused by mechanical vibrations or internal human body motions (limbs, chest wall breathing).

## Why This Matters for MANAR

Provides MANAR with the mathematical foundation to distinguish human targets from inanimate radar clutter.

## Core Architecture / Method

```
Raw FMCW Radar Chirps -> Range FFT -> Doppler FFT -> Short-Time Fourier Transform (STFT) -> Micro-Doppler Feature Extraction
```

## Detailed Technical Description

### Detailed Technical Description
Formulates frequency modulation induced by target micro-motions $f_D(t) = \frac{2 f_0}{c} v(t)$ and derives time-frequency distributions.

## Key Equations / Algorithms

\[ f_{D,micro}(t) = \frac{2 f_0}{c} D_{chest} \omega_v \cos(\omega_v t) \]

## Key Parameters / Measurements

| Parameter | Value | Context |
|---|---:|---|
| Carrier Frequency (f0) | 24 / 60 GHz | FMCW radar band |
| Chest Displacement | 1 - 12 mm | Breathing micro-motion |

## Useful Findings for MANAR

- PAPER FINDING: Micro-Doppler signatures provide unique spectral velocity patterns capable of isolating human motion.
- MANAR RELEVANCE: Core algorithm for MANAR mmWave human confirmation.

## MANAR Design Questions This Paper Helps Answer

- What STFT window length optimizes micro-Doppler resolution vs update rate on MANAR?

## What This Paper Does NOT Establish

- Assumes stationary radar platform; airborne vibration creates severe interfering micro-Doppler clutter.

## References Worth Following

- Kim & Ling (2009).

