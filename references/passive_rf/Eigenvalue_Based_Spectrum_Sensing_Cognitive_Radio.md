---
title: "Eigenvalue-Based Spectrum Sensing Algorithms for Cognitive Radio"
short_title: "Eigenvalue-Based Spectrum Sensing"
authors:
  - "Yonghong Zeng"
  - "Ying-Chang Liang"
year: 2009
publication: "IEEE Transactions on Communications"
publisher: "IEEE"
doi: "10.1109/TCOMM.2009.06.070402"
arxiv: null
site_url: "https://doi.org/10.1109/TCOMM.2009.06.070402"
pdf_url: null
peer_reviewed: true
paper_type: "journal"
primary_topic: "passive_rf"
tags:
  - eigenvalue-detection
  - covariance-matrix
  - random-matrix-theory
  - multi-antenna-sensing
manar_relevance:
  - passive-rf
  - anomaly-detection
relevance_score: 9
---
# Eigenvalue-Based Spectrum Sensing Algorithms for Cognitive Radio

## Citation

Y. Zeng and Y.-C. Liang. "Eigenvalue-Based Spectrum Sensing Algorithms for Cognitive Radio." *IEEE Trans. Comm.*, 2009. DOI: 10.1109/TCOMM.2009.06.070402.

## Abstract / Paper Summary

Proposes Maximum-Minimum Eigenvalue (MME) detection using sample covariance matrices of multi-antenna receivers, overcoming noise uncertainty SNR walls without requiring prior signal or noise information.

## Why This Matters for MANAR

Enables MANAR to detect weak RF emitters using dual-channel SDR receivers even when background noise power is uncertain.

## Core Architecture / Method

```
Multi-Antenna RF -> Covariance Matrix R -> Eigenvalues (lambda_max, lambda_min) -> Ratio T = lambda_max / lambda_min
```

## Detailed Technical Description

### Detailed Technical Description
Uses Tracy-Widom distributions from random matrix theory to set exact thresholds for the ratio of maximum to minimum sample eigenvalues.

## Key Equations / Algorithms

\[ T_{MME} = \frac{\lambda_{max}}{\lambda_{min}} > \gamma_{threshold} \]

## Key Parameters / Measurements

| Parameter | Value | Context |
|---|---:|---|
| Antenna Channels (M) | 2 - 4 | Receiver channels |
| Detection Limit | -20 dB SNR | Below SNR wall |

## Useful Findings for MANAR

- PAPER FINDING: MME detection is immune to noise uncertainty and operates effectively at low SNR.
- MANAR RELEVANCE: High-value candidate for MANAR multi-channel SDR receiver.

## MANAR Design Questions This Paper Helps Answer

- Does MANAR have sufficient compute for 2x2 covariance matrix eigenvalue decomposition on ARM?

## What This Paper Does NOT Establish

- Requires multi-channel synchronized SDR hardware.

## References Worth Following

- Tandra & Sahai (2008).

