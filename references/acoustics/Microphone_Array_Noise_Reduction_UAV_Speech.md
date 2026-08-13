---
title: "Speech Enhancement Using a Microphone Array Mounted on an Unmanned Aerial Vehicle"
short_title: "UAV Rotor Noise Suppression & Speech Enhancement"
authors:
  - "Yusuke Hioka"
  - "Tomohiro Nakatani"
  - "Keisuke Kinoshita"
  - "Masakiyo Fujimoto"
year: 2016
publication: "IEEE International Workshop on Acoustic Signal Enhancement (IWAENC 2016)"
publisher: "IEEE"
doi: "10.1109/IWAENC.2016.7602937"
arxiv: null
site_url: "https://doi.org/10.1109/IWAENC.2016.7602937"
pdf_url: null
peer_reviewed: true
paper_type: "conference"
primary_topic: "acoustics"
tags:
  - speech-enhancement
  - rotor-noise-reduction
  - beamforming
  - uav-acoustics
manar_relevance:
  - acoustics
relevance_score: 10
---
# Speech Enhancement Using a Microphone Array Mounted on an Unmanned Aerial Vehicle

## Citation

Y. Hioka et al. "Speech Enhancement Using a Microphone Array Mounted on an Unmanned Aerial Vehicle." *IEEE IWAENC*, 2016. DOI: 10.1109/IWAENC.2016.7602937.

## Abstract / Paper Summary

Presents a spatial-spectral speech enhancement framework for UAV-mounted microphone arrays that suppresses heavy multirotor motor noise by 25 dB, enabling intelligible human speech recovery.

## Why This Matters for MANAR

Critical algorithm for MANAR human voice detection and operator audio monitoring.

## Core Architecture / Method

```
Noisy Multichannel Audio -> Spatial Covariance Estimation -> Minimum Variance Unbiased (MVDR) Beamformer -> Wiener Filter -> Enhanced Speech
```

## Detailed Technical Description

### Detailed Technical Description
Estimates noise covariance matrix during hovering and applies MVDR beamforming targeted toward ground sound source.

## Key Equations / Algorithms

\[ \mathbf{w}_{MVDR} = \frac{\mathbf{R}_{n}^{-1} \mathbf{a}(\theta)}{\mathbf{a}^H(\theta) \mathbf{R}_{n}^{-1} \mathbf{a}(\theta)} \]

## Key Parameters / Measurements

| Parameter | Value | Context |
|---|---:|---|
| Noise Reduction | 25 dB | Rotor noise attenuation |
| Speech Intelligibility | STOI > 0.75 | Recovered speech score |

## Useful Findings for MANAR

- PAPER FINDING: MVDR beamforming reduces UAV ego-rotor noise by up to 25 dB, making human speech intelligible.
- MANAR RELEVANCE: Essential pre-processing filter for MANAR acoustic perception.

## MANAR Design Questions This Paper Helps Answer

- How does motor RPM variation during dynamic maneuvers affect noise covariance estimation?

## What This Paper Does NOT Establish

- Requires microphone array geometry to be rigid and pre-calibrated.

## References Worth Following

- Cobos et al. (2017).

