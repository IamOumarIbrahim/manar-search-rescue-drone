---
title: "Radar CFAR Thresholding in Clutter and Multiple Target Situations"
short_title: "Radar CFAR Thresholding in Clutter"
authors:
  - "Hermann Rohling"
year: 1983
publication: "IEEE Transactions on Aerospace and Electronic Systems"
publisher: "IEEE"
doi: "10.1109/TAES.1983.309350"
arxiv: null
site_url: "https://doi.org/10.1109/TAES.1983.309350"
pdf_url: null
peer_reviewed: true
paper_type: "journal"
primary_topic: "CFAR"
tags:
  - OS-CFAR
  - adaptive-thresholding
  - clutter-edge
  - multiple-targets
manar_relevance:
  - passive-rf
  - FMCW-radar
relevance_score: 10
---
# Radar CFAR Thresholding in Clutter and Multiple Target Situations

## Citation

Hermann Rohling. "Radar CFAR Thresholding in Clutter and Multiple Target Situations." *IEEE Trans. AES*, 1983. DOI: 10.1109/TAES.1983.309350.

## Abstract / Paper Summary

This foundational paper introduces Ordered Statistic CFAR (OS-CFAR) to solve target masking and clutter edge false alarms associated with CA-CFAR by rank-ordering N reference cell power samples and picking the k-th sample.

## Why This Matters for MANAR

Crucial for MANAR passive RF and radar anomaly detection near forest boundaries and multi-emitter target clusters.

## Core Architecture / Method

```
N Cells -> Sort X_{(1)}...X_{(N)} -> Pick X_{(k)} -> Scale T -> Compare vs CUT Y
```

## Detailed Technical Description

### Detailed Technical Description
Evaluates OS-CFAR rank selection k=3N/4 analytically and via Monte Carlo simulation under nonhomogeneous background noise.

## Key Equations / Algorithms

\[ P_{fa} = k \binom{N}{k} \sum_{m=0}^{N-k} \binom{N-k}{m} \frac{(-1)^m}{T + k + m} \]

## Key Parameters / Measurements

| Parameter | Value | Context |
|---|---:|---|
| Window Size N | 32 | Reference cells |
| Rank k | 24 | 3N/4 choice |
| Pfa | 1e-6 | Design target |

## Useful Findings for MANAR

- PAPER FINDING: OS-CFAR handles up to N-k interfering targets without target masking.
- MANAR RELEVANCE: Essential for RF anomaly detection in complex SAR environments.

## MANAR Design Questions This Paper Helps Answer

- Should MANAR use OS-CFAR for passive RF processing?

## What This Paper Does NOT Establish

- Does not evaluate non-Rayleigh K-distribution radar clutter.

## References Worth Following

- Finn & Johnson (1968) RCA Review.

