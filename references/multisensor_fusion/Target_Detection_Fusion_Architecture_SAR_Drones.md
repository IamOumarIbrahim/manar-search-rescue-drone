---
title: "Multimodal Target Detection and Fusion Architecture for Search and Rescue Drones"
short_title: "Multimodal SAR Drone Detection Fusion Architecture"
authors:
  - "Jun Liu"
  - "Xiaoming Wang"
  - "Chen Zhang"
  - "Lei Dong"
year: 2022
publication: "IEEE Access"
publisher: "IEEE"
doi: "10.1109/ACCESS.2022.3168234"
arxiv: null
site_url: "https://doi.org/10.1109/ACCESS.2022.3168234"
pdf_url: null
peer_reviewed: true
paper_type: "journal"
primary_topic: "multisensor_fusion"
tags:
  - multimodal-SAR-fusion
  - thermal-RGB-RF-fusion
  - decision-fusion
  - UAV-target-confirmation
manar_relevance:
  - multisensor-fusion
  - uav-sar
relevance_score: 10
---
# Multimodal Target Detection and Fusion Architecture for Search and Rescue Drones

## Citation

J. Liu et al. "Multimodal Target Detection and Fusion Architecture for Search and Rescue Drones." *IEEE Access*, 2022. DOI: 10.1109/ACCESS.2022.3168234.

## Abstract / Paper Summary

Presents a complete multi-modal target detection and fusion system designed specifically for Search and Rescue UAVs, integrating thermal vision, RGB imagery, and passive RF anomaly scores using a Bayesian belief network.

## Why This Matters for MANAR

Provides MANAR with an exact operational reference architecture for multi-sensor target aggregation.

## Core Architecture / Method

```
Thermal Detector + RGB Detector + RF Detector -> Confidence Aggregation -> Bayesian Belief Network -> Supervised Operator Alert
```

## Detailed Technical Description

### Detailed Technical Description
Combines spatial bounding box IoU with confidence score fusion, reducing false positives by 84% in wilderness SAR tests.

## Key Equations / Algorithms

\[ P(Target | S_{th}, S_{rgb}, S_{rf}) = \frac{\prod_{i} P(S_i | Target) P(Target)}{P(S_{th}, S_{rgb}, S_{rf})} \]

## Key Parameters / Measurements

| Parameter | Value | Context |
|---|---:|---|
| False Positive Reduction | 84% | Wilderness flight tests |
| Overall Precision | 94.2% | Multimodal fusion precision |

## Useful Findings for MANAR

- PAPER FINDING: Multi-modal fusion across thermal, vision, and RF reduces false positive SAR alerts by 84%.
- MANAR RELEVANCE: Directly validates MANAR's core architectural thesis of multisensor fusion.

## MANAR Design Questions This Paper Helps Answer

- What confidence aggregation thresholds should MANAR assign to unconfirmed single-sensor detections?

## What This Paper Does NOT Establish

- System tested with simulated RF beacons alongside real thermal cameras.

## References Worth Following

- Hall & Llinas (1997).

