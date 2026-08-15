---
title: "A Survey of Spectrum Sensing Algorithms for Cognitive Radio Applications"
short_title: "Spectrum Sensing Survey"
authors:
  - "Tevfik Yucek"
  - "Huseyin Arslan"
year: 2009
publication: "IEEE Communications Surveys & Tutorials"
publisher: "IEEE"
doi: "10.1109/SURV.2009.090109"
arxiv: null
site_url: "https://doi.org/10.1109/SURV.2009.090109"
pdf_url: null
peer_reviewed: true
paper_type: "journal"
primary_topic: "passive_rf"
tags:
  - spectrum-sensing
  - matched-filter
  - cyclostationary-detection
  - energy-detection
manar_relevance:
  - passive-rf
  - anomaly-detection
relevance_score: 10
---
# A Survey of Spectrum Sensing Algorithms for Cognitive Radio Applications

## Citation

T. Yucek and H. Arslan. "A Survey of Spectrum Sensing Algorithms for Cognitive Radio Applications." *IEEE Comm. Surveys & Tutorials*, 2009. DOI: 10.1109/SURV.2009.090109.

## Abstract / Paper Summary

Comprehensive survey comparing energy detection, matched filtering, cyclostationary feature detection, and eigenvalue-based sensing across sensitivity, complexity, and noise uncertainty.

## Why This Matters for MANAR

Provides MANAR with an authoritative trade-off taxonomy for passive RF sensing payload design.

## Core Architecture / Method

```
RF Input -> Feature Extraction (Energy / Cyclostationary / Eigenvalue) -> Decision Logic
```

## Detailed Technical Description

### Detailed Technical Description
Evaluates detection probability vs SNR trade-offs for spectrum sensing techniques under noise floor variations.

## Key Equations / Algorithms

\[ P_d = Q\left( \frac{\lambda - E[V|H_1]}{\sqrt{Var[V|H_1]}} \right) \]

## Key Parameters / Measurements

| Parameter | Value | Context |
|---|---:|---|
| Energy Detector Sensitivity | -15 dB SNR | Standard noise limit |
| Cyclostationary Sensitivity | -25 dB SNR | Requires known cyclic prefix |

## Useful Findings for MANAR

- PAPER FINDING: Energy detection has lowest computational complexity; cyclostationary feature detection is robust below SNR wall.

## MANAR Design Questions This Paper Helps Answer

- Which sensing algorithm should MANAR implement for emergency cellular signal detection?

## What This Paper Does NOT Establish

- Focuses on static cognitive radio nodes rather than highly dynamic airborne UAV platforms.

## References Worth Following

- Urkowitz, H. (1967).

