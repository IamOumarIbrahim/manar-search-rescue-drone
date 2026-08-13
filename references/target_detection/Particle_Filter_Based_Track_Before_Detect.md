---
title: "Particle Filter Based Track-Before-Detect Algorithm"
short_title: "Particle Filter Track-Before-Detect (TBD)"
authors:
  - "Yvo Boers"
  - "J. N. Driessen"
year: 2004
publication: "IEEE Transactions on Aerospace and Electronic Systems"
publisher: "IEEE"
doi: "10.1109/TAES.2004.1310011"
arxiv: null
site_url: "https://doi.org/10.1109/TAES.2004.1310011"
pdf_url: null
peer_reviewed: true
paper_type: "journal"
primary_topic: "target_detection"
tags:
  - Track-Before-Detect
  - TBD
  - particle-filter
  - weak-target-tracking
manar_relevance:
  - target-detection
  - passive-rf
relevance_score: 9
---
# Particle Filter Based Track-Before-Detect Algorithm

## Citation

Y. Boers and J. N. Driessen. "Particle Filter Based Track-Before-Detect Algorithm." *IEEE Trans. AES*, 2004. DOI: 10.1109/TAES.2004.1310011.

## Abstract / Paper Summary

Formulates a Particle Filter Track-Before-Detect (PF-TBD) algorithm that processes un-thresholded raw sensor energy grids over time, accumulating weak target signals below the single-frame detection threshold.

## Why This Matters for MANAR

Enables MANAR to detect extremely weak RF or thermal signals by integrating energy along trajectory hypotheses over multiple frames.

## Core Architecture / Method

```
Raw Sensor Energy Grid (Unthresholded) -> Sequential Particle Sampling -> Energy Likelihood Weighting -> Track Declaration
```

## Detailed Technical Description

### Detailed Technical Description
Propagates 1000-5000 particles representing target position, velocity, and intensity state.

## Key Equations / Algorithms

\[ w_i^{(k)} = w_i^{(k-1)} \cdot \frac{p(z_k | x_{i,k})}{p(z_k | H_0)} \]

## Key Parameters / Measurements

| Parameter | Value | Context |
|---|---:|---|
| SNR Improvement | + 3 to 6 dB | Detection sensitivity gain |
| Particle Count | 2000 particles | Filter size |

## Useful Findings for MANAR

- PAPER FINDING: TBD achieves 3-6 dB higher sensitivity than threshold-first detection systems.
- MANAR RELEVANCE: High-value candidate for MANAR weak signal target confirmation.

## MANAR Design Questions This Paper Helps Answer

- Does MANAR have sufficient CPU/GPU capacity to run particle filtering on raw RF energy maps?

## What This Paper Does NOT Establish

- High computational load compared to conventional thresholding.

## References Worth Following

- Bar-Shalom & Tse (1975).

