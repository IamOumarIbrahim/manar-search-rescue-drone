---
title: "An Introduction to Multisensor Data Fusion"
short_title: "Introduction to Multisensor Data Fusion"
authors:
  - "David L. Hall"
  - "James Llinas"
year: 1997
publication: "Proceedings of the IEEE"
publisher: "IEEE"
doi: "10.1109/5.554205"
arxiv: null
site_url: "https://doi.org/10.1109/5.554205"
pdf_url: null
peer_reviewed: true
paper_type: "journal"
primary_topic: "multisensor_fusion"
tags:
  - JDL-fusion-model
  - data-fusion-taxonomy
  - sensor-level-fusion
  - decision-level-fusion
manar_relevance:
  - multisensor-fusion
relevance_score: 10
---
# An Introduction to Multisensor Data Fusion

## Citation

David L. Hall and James Llinas. "An Introduction to Multisensor Data Fusion." *Proc. IEEE*, vol. 85, no. 1, pp. 6-23, 1997. DOI: 10.1109/5.554205.

## Abstract / Paper Summary

This foundational survey defines the Joint Directors of Laboratories (JDL) multi-level data fusion model (Level 0 Signal, Level 1 Object, Level 2 Situation, Level 3 Threat, Level 4 Process Refinement) and categorizes fusion architectures.

## Why This Matters for MANAR

Provides MANAR with the master architectural taxonomy for sensor-level, feature-level, and decision-level fusion.

## Core Architecture / Method

```
Raw Sensors -> Level 0 Signal -> Level 1 Object Fusion -> Level 2 Situation Assessment -> Operator UI
```

## Detailed Technical Description

### Detailed Technical Description
Compares centralized, distributed, and hybrid data fusion topographies across processing latency and bandwidth.

## Key Equations / Algorithms

\[ P(H_i | Z_1, Z_2) = \frac{P(Z_1, Z_2 | H_i) P(H_i)}{\sum_j P(Z_1, Z_2 | H_j) P(H_j)} \]

## Key Parameters / Measurements

| Parameter | Value | Context |
|---|---:|---|
| Fusion Level 1 | Object Refinement | Kinematic tracking & identity |
| Fusion Level 2 | Situation Refinement | Contextual SAR assessment |

## Useful Findings for MANAR

- PAPER FINDING: Feature-level fusion balances bandwidth efficiency and detection performance.
- MANAR RELEVANCE: Core architectural reference for MANAR's Python sensor fusion manager.

## MANAR Design Questions This Paper Helps Answer

- Which JDL fusion level corresponds to MANAR's RF + thermal candidate confirmation layer?

## What This Paper Does NOT Establish

- General conceptual taxonomy rather than specific drone sensor implementations.

## References Worth Following

- Durrant-Whyte & Henderson (2008).

