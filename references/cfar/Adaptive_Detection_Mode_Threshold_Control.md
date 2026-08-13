---
title: "Adaptive Detection Mode with Threshold Control as a Function of Spatially Sampled Clutter-Level Estimates"
short_title: "Original Cell-Averaging CFAR Detector"
authors:
  - "Harold M. Finn"
  - "Ronald S. Johnson"
year: 1968
publication: "RCA Review"
publisher: "RCA Laboratories"
doi: null
arxiv: null
site_url: "https://worldradiohistory.com/ARCHIVE-RCA/RCA-Review/RCA-Review-1968-Sep.pdf"
pdf_url: null
peer_reviewed: true
paper_type: "journal"
primary_topic: "CFAR"
tags:
  - CA-CFAR
  - foundational-CFAR
  - noise-floor-estimation
manar_relevance:
  - passive-rf
  - FMCW-radar
relevance_score: 9
---
# Adaptive Detection Mode with Threshold Control as a Function of Spatially Sampled Clutter-Level Estimates

## Citation

H. M. Finn and R. S. Johnson. "Adaptive Detection Mode with Threshold Control as a Function of Spatially Sampled Clutter-Level Estimates." *RCA Review*, 1968.

## Abstract / Paper Summary

The original 1968 paper introducing Cell-Averaging Constant False Alarm Rate (CA-CFAR) detection in Rayleigh noise.

## Why This Matters for MANAR

Foundational baseline mathematical equation for adaptive noise floor estimation.

## Core Architecture / Method

```
Reference Window N -> Arithmetic Mean -> Scale T -> Compare vs CUT
```

## Detailed Technical Description

### Detailed Technical Description
Establishes mathematical proof of CFAR property in homogeneous Rayleigh background noise.

## Key Equations / Algorithms

\[ P_{fa} = (1 + T)^{-N} \]

## Key Parameters / Measurements

| Parameter | Value | Context |
|---|---:|---|
| Window Size N | 16 - 64 | Original design parameter |

## Useful Findings for MANAR

- PAPER FINDING: CA-CFAR guarantees exact false alarm rate in homogeneous Rayleigh noise.

## MANAR Design Questions This Paper Helps Answer

- What reference window size N optimizes MANAR's baseline CA-CFAR implementation?

## What This Paper Does NOT Establish

- Vulnerable to nonhomogeneous clutter transitions.

## References Worth Following

- Rohling, H. (1983).

