---
title: "Multisensor Data Fusion"
short_title: "Robotics Multisensor Data Fusion"
authors:
  - "Hugh Durrant-Whyte"
  - "Thomas C. Henderson"
year: 2008
publication: "Springer Handbook of Robotics"
publisher: "Springer"
doi: "10.1007/978-3-540-30301-5_26"
arxiv: null
site_url: "https://doi.org/10.1007/978-3-540-30301-5_26"
pdf_url: null
peer_reviewed: true
paper_type: "book_chapter"
primary_topic: "multisensor_fusion"
tags:
  - bayesian-fusion
  - kalman-fusion
  - covariance-intersection
  - robotics-fusion
manar_relevance:
  - multisensor-fusion
relevance_score: 10
---
# Multisensor Data Fusion

## Citation

H. Durrant-Whyte and T. C. Henderson. "Multisensor Data Fusion." *Springer Handbook of Robotics*, pp. 585-610, 2008. DOI: 10.1007/978-3-540-30301-5_26.

## Abstract / Paper Summary

Presents the mathematical foundations of probabilistic multi-sensor data fusion for autonomous robotic systems, covering Bayesian estimation, Extended Kalman Filters, Information Filters, and Covariance Intersection.

## Why This Matters for MANAR

Provides MANAR with robust mathematical algorithms for fusing asynchronous multi-sensor telemetry.

## Core Architecture / Method

```
Sensor Measurements (Radar, Thermal, RF) -> Decentralized Information Filter -> Covariance Intersection -> Fused State
```

## Detailed Technical Description

### Detailed Technical Description
Formulates Covariance Intersection for fusing estimates with unknown cross-correlations.

## Key Equations / Algorithms

\[ \mathbf{P}^{-1} = \omega \mathbf{P}_1^{-1} + (1-\omega) \mathbf{P}_2^{-1}, \quad \mathbf{a} = \mathbf{P} \left( \omega \mathbf{P}_1^{-1} \mathbf{a}_1 + (1-\omega) \mathbf{P}_2^{-1} \mathbf{a}_2 \right) \]

## Key Parameters / Measurements

| Parameter | Value | Context |
|---|---:|---|
| Fusion Algorithm | Covariance Intersection | Unknown cross-correlation safe |
| Weight Parameter | omega in [0,1] | Convex optimization |

## Useful Findings for MANAR

- PAPER FINDING: Covariance Intersection guarantees consistent fusion bounds even when sensor noise correlations are unknown.
- MANAR RELEVANCE: Essential algorithm for fusing independent RF, radar, and vision candidate tracks.

## MANAR Design Questions This Paper Helps Answer

- Is Information Filtering or Covariance Intersection preferred for MANAR's asynchronous sensor architecture?

## What This Paper Does NOT Establish

- Focuses on estimation math rather than deep learning multimodal vision.

## References Worth Following

- Hall & Llinas (1997).

