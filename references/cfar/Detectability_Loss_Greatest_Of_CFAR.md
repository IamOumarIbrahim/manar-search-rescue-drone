---
title: "Detectability Loss Due to "Greatest Of" Selection in a Cell-Averaging CFAR"
short_title: "Detectability Loss in GO-CFAR"
authors:
  - "V. Greg Hansen"
  - "J. H. Sawyers"
year: 1980
publication: "IEEE Transactions on Aerospace and Electronic Systems"
publisher: "IEEE"
doi: "10.1109/TAES.1980.308885"
arxiv: null
site_url: "https://doi.org/10.1109/TAES.1980.308885"
pdf_url: null
peer_reviewed: true
paper_type: "journal"
primary_topic: "CFAR"
tags:
  - GO-CFAR
  - clutter-transitions
  - false-alarm-control
manar_relevance:
  - FMCW-radar
  - navigation
relevance_score: 9
---
# Detectability Loss Due to "Greatest Of" Selection in a Cell-Averaging CFAR

## Citation

V. G. Hansen and J. H. Sawyers. "Detectability Loss Due to 'Greatest Of' Selection in a Cell-Averaging CFAR." *IEEE Trans. AES*, 1980. DOI: 10.1109/TAES.1980.308885.

## Abstract / Paper Summary

Analyzes the detectability loss of GO-CFAR in homogeneous background noise (~0.5 dB loss for N=32) while proving its effectiveness at bounding false alarm spikes when entering high clutter.

## Why This Matters for MANAR

Ideal reference for MANAR radar altimeter altitude tracking over sudden forest/water transitions.

## Core Architecture / Method

```
Leading Half M + Lagging Half M -> Max(S1, S2) -> Threshold Z
```

## Detailed Technical Description

### Detailed Technical Description
Evaluates GO-CFAR threshold bias and false alarm probability in homogeneous Rayleigh noise.

## Key Equations / Algorithms

\[ Z = \max\left(\sum_{i=1}^M X_{1,i}, \sum_{i=1}^M X_{2,i}\right) \]

## Key Parameters / Measurements

| Parameter | Value | Context |
|---|---:|---|
| SNR Loss | < 0.7 dB | Homogeneous noise loss |
| Window 2M | 32 | Reference cells |

## Useful Findings for MANAR

- PAPER FINDING: GO-CFAR bounds false alarm spikes at clutter entries with minimal SNR loss.

## MANAR Design Questions This Paper Helps Answer

- Is GO-CFAR optimal for MANAR terrain-following radar altimetry?

## What This Paper Does NOT Establish

- Does not resolve target masking under multiple target reflections.

## References Worth Following

- Gandhi & Kassam (1988).

