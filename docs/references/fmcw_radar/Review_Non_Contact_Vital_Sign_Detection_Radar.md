---
title: "A Review on Recent Progress in Non-Contact Vital Sign Detection Using Radar Technology"
short_title: "Non-Contact Vital Sign Radar Review"
authors:
  - "Changzhi Li"
  - "Jenshan Lin"
  - "Victor Lubecke"
year: 2013
publication: "IEEE Transactions on Microwave Theory and Techniques"
publisher: "IEEE"
doi: "10.1109/TMTT.2013.2250298"
arxiv: null
site_url: "https://doi.org/10.1109/TMTT.2013.2250298"
pdf_url: null
peer_reviewed: true
paper_type: "journal"
primary_topic: "fmcw_radar"
tags:
  - vital-signs
  - breathing-detection
  - heartbeat-radar
  - non-contact-sensing
manar_relevance:
  - fmcw-radar
  - target-detection
relevance_score: 10
---
# A Review on Recent Progress in Non-Contact Vital Sign Detection Using Radar Technology

## Citation

C. Li et al. "A Review on Recent Progress in Non-Contact Vital Sign Detection Using Radar Technology." *IEEE Trans. MTT*, 2013. DOI: 10.1109/TMTT.2013.2250298.

## Abstract / Paper Summary

Comprehensive review of continuous-wave (CW) and FMCW radar techniques for detecting human respiratory (0.1 - 0.5 Hz) and cardiac (0.8 - 2.5 Hz) micro-motions through foliage and obscurants.

## Why This Matters for MANAR

Provides MANAR with phase demodulation and harmonic filtering algorithms for vital sign radar processing.

## Core Architecture / Method

```
FMCW Radar Phase Output -> Arctangent Demodulation -> Bandpass Filter (0.1-0.5 Hz) -> FFT -> Respiratory Rate Peak
```

## Detailed Technical Description

### Detailed Technical Description
Formulates phase shift demodulation $\Delta \phi(t) = \frac{4 \pi}{\lambda} x(t)$ for chest displacement extraction.

## Key Equations / Algorithms

\[ \Delta \phi(t) = \frac{4\pi}{\lambda} \left[ x_b(t) + x_h(t) \right] \]

## Key Parameters / Measurements

| Parameter | Value | Context |
|---|---:|---|
| Respiratory Frequency | 0.1 - 0.5 Hz | 12 - 30 breaths/min |
| Chest Displacement | 4 - 12 mm | Respiration movement |

## Useful Findings for MANAR

- PAPER FINDING: Arctangent phase demodulation provides superior linearity for millimeter chest displacement estimation.
- MANAR RELEVANCE: Core signal processing chain for MANAR mmWave victim respiration verification.

## MANAR Design Questions This Paper Helps Answer

- What carrier frequency (24 GHz vs 60 GHz) provides optimal SNR for airborne vital sign sensing?

## What This Paper Does NOT Establish

- Body orientation and clothing attenuate phase signals.

## References Worth Following

- Adib et al. (2015).

