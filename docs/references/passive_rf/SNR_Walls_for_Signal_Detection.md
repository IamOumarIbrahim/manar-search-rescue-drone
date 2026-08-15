---
title: "SNR Walls for Signal Detection"
short_title: "SNR Walls for Signal Detection"
authors:
  - "Rahul Tandra"
  - "Anant Sahai"
year: 2008
publication: "IEEE Journal of Selected Topics in Signal Processing"
publisher: "IEEE"
doi: "10.1109/JSTSP.2007.914879"
arxiv: null
site_url: "https://doi.org/10.1109/JSTSP.2007.914879"
pdf_url: null
peer_reviewed: true
paper_type: "journal"
primary_topic: "passive_rf"
tags:
  - SNR-wall
  - noise-uncertainty
  - passive-detection-limits
  - energy-detection
manar_relevance:
  - passive-rf
  - anomaly-detection
relevance_score: 10
---
# SNR Walls for Signal Detection

## Citation

R. Tandra and A. Sahai. "SNR Walls for Signal Detection." *IEEE JSTSP*, 2008. DOI: 10.1109/JSTSP.2007.914879.

## Abstract / Paper Summary

Proves mathematically that under noise uncertainty, there exists a fundamental Signal-to-Noise Ratio (SNR) threshold ('SNR Wall') below which energy detection cannot achieve robust detection regardless of integration time.

## Why This Matters for MANAR

Establishes physical limits for MANAR's passive RF payload sensitivity in noisy wilderness environments.

## Core Architecture / Method

```
Noise Uncertainty B (dB) ---> SNR Wall Calculation: SNR_{wall} = (B^2 - 1) / B
```

## Detailed Technical Description

### Detailed Technical Description
Derives SNR wall equations for energy detection and feature detection under bounded noise uncertainty.

## Key Equations / Algorithms

\[ SNR_{wall} = \frac{B^2 - 1}{B}, \quad B = 10^{\Delta/10} \]

## Key Parameters / Measurements

| Parameter | Value | Context |
|---|---:|---|
| Noise Uncertainty (Delta) | 1 dB | Typical SDR calibration drift |
| SNR Wall | -6 dB | Detection floor limit |

## Useful Findings for MANAR

- PAPER FINDING: Increasing integration time T cannot overcome SNR walls caused by noise calibration uncertainty.
- MANAR RELEVANCE: MANAR must implement dynamic online noise calibration or multi-antenna feature detection to operate below -10 dB SNR.

## MANAR Design Questions This Paper Helps Answer

- How frequently must MANAR calibrate its RF noise floor during flight?

## What This Paper Does NOT Establish

- Does not evaluate spatial correlation from multi-element antenna arrays.

## References Worth Following

- Urkowitz, H. (1967).

