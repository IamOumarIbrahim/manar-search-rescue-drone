---
title: "Illumination-Aware Faster R-CNN for Robust Multispectral Pedestrian Detection"
short_title: "Illumination-Aware Multispectral Detector"
authors:
  - "Chenglong Li"
  - "Dan Song"
  - "Ruixin Tong"
  - "Ming-Hsuan Yang"
year: 2019
publication: "Pattern Recognition"
publisher: "Elsevier"
doi: "10.1016/j.patcog.2018.08.005"
arxiv: null
site_url: "https://doi.org/10.1016/j.patcog.2018.08.005"
pdf_url: null
peer_reviewed: true
paper_type: "journal"
primary_topic: "thermal_vision"
tags:
  - illumination-aware
  - multispectral-fusion
  - thermal-RGB-weighting
  - adaptive-fusion
manar_relevance:
  - thermal-vision
  - multisensor-fusion
relevance_score: 9
---
# Illumination-Aware Faster R-CNN for Robust Multispectral Pedestrian Detection

## Citation

C. Li et al. "Illumination-Aware Faster R-CNN for Robust Multispectral Pedestrian Detection." *Pattern Recognition*, 2019. DOI: 10.1016/j.patcog.2018.08.005.

## Abstract / Paper Summary

Proposes an illumination-aware deep network that dynamically computes illumination weights to adaptively weight RGB and thermal feature streams according to lighting conditions (e.g. giving higher weight to RGB in full daylight and higher weight to LWIR at night/twilight).

## Why This Matters for MANAR

Provides MANAR with an adaptive fusion mechanism for changing environmental light conditions.

## Core Architecture / Method

```
RGB + Thermal -> Illumination Sub-Network -> Weights (w_rgb, w_lwir) -> Weighted Feature Fusion -> Detection
```

## Detailed Technical Description

### Detailed Technical Description
Integrates an illumination estimation sub-network into Faster R-CNN, achieving a low miss rate of 9.5% on KAIST benchmark.

## Key Equations / Algorithms

\[ F_{fused} = w_{rgb}(I) \cdot F_{rgb} + w_{lwir}(I) \cdot F_{lwir} \]

## Key Parameters / Measurements

| Parameter | Value | Context |
|---|---:|---|
| Day Miss Rate | 9.2% | KAIST day benchmark |
| Night Miss Rate | 9.8% | KAIST night benchmark |

## Useful Findings for MANAR

- PAPER FINDING: Adaptive illumination weighting prevents poor RGB features from degrading thermal detection at night.
- MANAR RELEVANCE: Directly answers how MANAR should balance RGB and thermal vision across day/night operations.

## MANAR Design Questions This Paper Helps Answer

- Can the illumination weighting sub-network run efficiently within MANAR's TensorRT pipeline?

## What This Paper Does NOT Establish

- Requires calibrated RGB and thermal camera pairs with matching FOV.

## References Worth Following

- Hwang et al. (2015).

