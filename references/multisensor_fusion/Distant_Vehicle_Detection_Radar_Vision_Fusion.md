---
title: "Distant Vehicle Detection Using Radar and Vision Fusion"
short_title: "Radar-Vision Feature & Decision Fusion"
authors:
  - "Simon Chadwick"
  - "Will Maddern"
  - "Paul Newman"
year: 2019
publication: "IEEE International Conference on Robotics and Automation (ICRA 2019)"
publisher: "IEEE"
doi: "10.1109/ICRA.2019.8794312"
arxiv: null
site_url: "https://doi.org/10.1109/ICRA.2019.8794312"
pdf_url: null
peer_reviewed: true
paper_type: "conference"
primary_topic: "multisensor_fusion"
tags:
  - radar-vision-fusion
  - distant-target-detection
  - multimodal-fusion
  - sensor-fusion
manar_relevance:
  - multisensor-fusion
  - fmcw-radar
  - rgb-low-light-vision
relevance_score: 9
---
# Distant Vehicle Detection Using Radar and Vision Fusion

## Citation

S. Chadwick et al. "Distant Vehicle Detection Using Radar and Vision Fusion." *IEEE ICRA*, 2019. DOI: 10.1109/ICRA.2019.8794312.

## Abstract / Paper Summary

Proposes a deep fusion architecture combining FMCW radar range-Doppler projections with RGB camera feature maps, demonstrating significant recall improvements for distant small targets.

## Why This Matters for MANAR

Demonstrates practical radar + vision fusion for long-range target confirmation in MANAR.

## Core Architecture / Method

```
Radar Point Cloud / Doppler Map -> Coordinate Projection -> RGB Image Feature Map -> Spatial Fusion Layer -> Target Box
```

## Detailed Technical Description

### Detailed Technical Description
Projects radar range-velocity detections into image pixel space, generating region proposals for distant targets.

## Key Equations / Algorithms

\[ \mathbf{u}_{img} = \mathbf{K} \cdot \mathbf{T}_{cam}^{radar} \cdot \mathbf{P}_{radar} \]

## Key Parameters / Measurements

| Parameter | Value | Context |
|---|---:|---|
| Recall Gain | + 18% | Distant targets (> 50m) |
| Fusion Method | Spatial Projection Layer | Cross-modal attention |

## Useful Findings for MANAR

- PAPER FINDING: Projecting radar range detections into camera space dramatically increases recall for distant targets.
- MANAR RELEVANCE: Direct blueprint for MANAR FMCW radar + vision candidate confirmation.

## MANAR Design Questions This Paper Helps Answer

- How should MANAR handle spatial registration errors caused by drone frame vibration?

## What This Paper Does NOT Establish

- Requires tight spatial camera-radar calibration.

## References Worth Following

- Liu et al. (2022).

