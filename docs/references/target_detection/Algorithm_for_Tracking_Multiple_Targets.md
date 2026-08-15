---
title: "An Algorithm for Tracking Multiple Targets"
short_title: "Multiple Hypothesis Tracking (MHT)"
authors:
  - "Donald B. Reid"
year: 1979
publication: "IEEE Transactions on Automatic Control"
publisher: "IEEE"
doi: "10.1109/TAC.1979.1102177"
arxiv: null
site_url: "https://doi.org/10.1109/TAC.1979.1102177"
pdf_url: null
peer_reviewed: true
paper_type: "journal"
primary_topic: "target_detection"
tags:
  - MHT
  - multiple-hypothesis-tracking
  - target-confirmation
  - data-association
manar_relevance:
  - target-detection
  - multisensor-fusion
relevance_score: 10
---
# An Algorithm for Tracking Multiple Targets

## Citation

Donald B. Reid. "An Algorithm for Tracking Multiple Targets." *IEEE Trans. Autom. Control*, vol. AC-24, no. 6, pp. 843-854, Dec. 1979. DOI: 10.1109/TAC.1979.1102177.

## Abstract / Paper Summary

This foundational paper formulates Multiple Hypothesis Tracking (MHT), maintaining a tree of data association hypotheses over time to track multiple targets in heavy clutter.

## Why This Matters for MANAR

Provides MANAR with a probabilistic framework for multi-frame target confirmation and candidate persistence.

## Core Architecture / Method

```
Measurements -> Gating -> Form Association Hypotheses -> Kalman Filter Update -> Hypothesis Pruning -> Confirmed Tracks
```

## Detailed Technical Description

### Detailed Technical Description
Evaluates hypothesis probabilities based on spatial proximity, detection history, and false alarm rate.

## Key Equations / Algorithms

\[ P(H_i | Z^k) = \frac{1}{c} P(Z_k | H_i, Z^{k-1}) P(H_i | Z^{k-1}) \]

## Key Parameters / Measurements

| Parameter | Value | Context |
|---|---:|---|
| False Alarm Density | Clutter rate lambda | Poisson false alarm model |
| Track Confirmation | N-out-of-M rule | Persistence criterion |

## Useful Findings for MANAR

- PAPER FINDING: Deferring association decisions over multiple frames enables reliable target tracking in high false alarm environments.
- MANAR RELEVANCE: Core mathematical model for MANAR's multi-frame target confirmation engine.

## MANAR Design Questions This Paper Helps Answer

- How many hypothesis frames should MANAR maintain before pruning to prevent exponential memory growth?

## What This Paper Does NOT Establish

- Combinatorial explosion of hypotheses requires aggressive N-scan pruning.

## References Worth Following

- Bar-Shalom & Tse (1975).

