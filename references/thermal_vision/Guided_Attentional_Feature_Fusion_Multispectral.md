---
title: "Guided Attentive Feature Fusion for Multispectral Pedestrian Detection"
short_title: "Guided Attentive Feature Fusion"
authors:
  - "Heng Zhang"
  - "Elisa Fromont"
  - "Sebastien Lefevre"
  - "Bruno Avignon"
year: 2021
publication: "IEEE Winter Conference on Applications of Computer Vision (WACV 2021)"
publisher: "IEEE"
doi: "10.1109/WACV48630.2021.00076"
arxiv: null
site_url: "https://doi.org/10.1109/WACV48630.2021.00076"
pdf_url: null
peer_reviewed: true
paper_type: "conference"
primary_topic: "thermal_vision"
tags:
  - attention-mechanism
  - feature-fusion
  - multispectral-detection
  - deep-learning
manar_relevance:
  - thermal-vision
  - multisensor-fusion
relevance_score: 9
---
# Guided Attentive Feature Fusion for Multispectral Pedestrian Detection

## Citation

H. Zhang et al. "Guided Attentive Feature Fusion for Multispectral Pedestrian Detection." *IEEE WACV*, 2021. DOI: 10.1109/WACV48630.2021.00076.

## Abstract / Paper Summary

Introduces Guided Attentive Feature Fusion (GAFF) for multispectral detection, using spatial and channel attention maps to guide feature combination between thermal and RGB CNN backbones.

## Why This Matters for MANAR

Provides state-of-the-art feature fusion modules for MANAR's deep learning perception engine.

## Core Architecture / Method

```
RGB CNN Stream + Thermal CNN Stream -> Spatial/Channel Attention Module -> Guided Feature Map -> Detection Head
```

## Detailed Technical Description

### Detailed Technical Description
Achieves 7.4% miss rate on KAIST benchmark by adaptively suppressing noisy channel features.

## Key Equations / Algorithms

\[ M_{spatial} = \sigma\left( f^{7\times 7}([AvgPool(F); MaxPool(F)]) \right) \]

## Key Parameters / Measurements

| Parameter | Value | Context |
|---|---:|---|
| KAIST Miss Rate | 7.41% | SOTA benchmark result |
| Inference Speed | 22 fps | NVIDIA RTX 2080 Ti |

## Useful Findings for MANAR

- PAPER FINDING: Spatial and channel attention guided fusion outperforms simple concatenation by over 3% mAP.
- MANAR RELEVANCE: High-performance architecture choice for MANAR ML perception model.

## MANAR Design Questions This Paper Helps Answer

- Can GAFF attention blocks be pruned for TensorRT deployment on Jetson Orin?

## What This Paper Does NOT Establish

- High parameter count requires quantization for edge deployment.

## References Worth Following

- Hwang et al. (2015).

