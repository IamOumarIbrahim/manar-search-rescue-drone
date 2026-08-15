---
title: "Energy Detection of Unknown Deterministic Signals"
short_title: "Foundational Energy Detection"
authors:
  - "Harry Urkowitz"
year: 1967
publication: "Proceedings of the IEEE"
publisher: "IEEE"
doi: "10.1109/PROC.1967.5573"
arxiv: null
site_url: "https://doi.org/10.1109/PROC.1967.5573"
pdf_url: null
peer_reviewed: true
paper_type: "journal"
primary_topic: "passive_rf"
tags:
  - energy-detection
  - radiometer
  - chi-square-distribution
  - spectrum-sensing
manar_relevance:
  - passive-rf
  - anomaly-detection
relevance_score: 10
---
# Energy Detection of Unknown Deterministic Signals

## Citation

Harry Urkowitz. "Energy Detection of Unknown Deterministic Signals." *Proc. IEEE*, vol. 55, no. 4, pp. 523-531, 1967. DOI: 10.1109/PROC.1967.5573.

## Abstract / Paper Summary

This foundational paper formulates energy detection for unknown signals in Gaussian noise using square-law integration, proving test statistic follows central and non-central chi-square distributions.

## Why This Matters for MANAR

Forms the core theoretical foundation for MANAR's passive RF energy detection pipeline.

## Core Architecture / Method

```
RF Bandpass Filter -> Square-Law Integrator -> Compare vs Threshold lambda
```

## Detailed Technical Description

### Detailed Technical Description
Mathematical formulation of energy detector test statistic $V = \sum_{i=1}^{2TW} X_i^2$ over bandwidth W and integration time T.

## Key Equations / Algorithms

\[ V = \sum_{i=1}^{2TW} |X(i)|^2 \sim \begin{cases} \chi^2_{2TW} & H_0 \\ \chi^2_{2TW}(\gamma) & H_1 \end{cases} \]

## Key Parameters / Measurements

| Parameter | Value | Context |
|---|---:|---|
| Time-Bandwidth Product (TW) | 10 - 1000 | Integration samples |
| Noise Model | Zero-mean AWGN | Background noise assumption |

## Useful Findings for MANAR

- PAPER FINDING: Energy detection is optimal when signal structure is completely unknown.
- MANAR RELEVANCE: Essential baseline for MANAR passive RF anomaly detection.

## MANAR Design Questions This Paper Helps Answer

- What integration time T maximizes MANAR RF detection range?

## What This Paper Does NOT Establish

- Requires prior knowledge of background noise variance.

## References Worth Following

- Tandra & Sahai (2008).

