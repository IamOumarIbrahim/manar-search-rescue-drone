---
title: "Zero-Reference Deep Curve Estimation for Low-Light Image Enhancement"
short_title: "Zero-DCE Low-Light Image Enhancement"
authors:
  - "Chunle Guo"
  - "Chongyi Li"
  - "Jichang Guo"
  - "Chen Change Loy"
  - "Junhui Hou"
  - "Sam Kwong"
  - "Runmin Cong"
year: 2020
publication: "IEEE/CVF Conference on Computer Vision and Pattern Recognition (CVPR 2020)"
publisher: "IEEE"
doi: "10.1109/CVPR42600.2020.00185"
arxiv: null
site_url: "https://doi.org/10.1109/CVPR42600.2020.00185"
pdf_url: null
peer_reviewed: true
paper_type: "conference"
primary_topic: "rgb_low_light_vision"
tags:
  - Zero-DCE
  - low-light-enhancement
  - real-time-enhancement
  - zero-reference-learning
manar_relevance:
  - rgb-low-light-vision
  - payload-swap
relevance_score: 9
---
# Zero-Reference Deep Curve Estimation for Low-Light Image Enhancement

## Citation

C. Guo et al. "Zero-Reference Deep Curve Estimation for Low-Light Image Enhancement." *IEEE CVPR*, 2020. DOI: 10.1109/CVPR42600.2020.00185.

## Abstract / Paper Summary

Introduces Zero-DCE, a lightweight deep neural network that enhances low-light images in real time (> 500 fps on GPU) without requiring paired training data by estimating pixel-wise high-order curves.

## Why This Matters for MANAR

Enables MANAR to enhance low-light RGB camera feeds during twilight or night operations before running object detection.

## Core Architecture / Method

```
Low-Light RGB Image -> Lightweight CNN (Zero-DCE) -> Light-Enhancement Curves LE(x) -> Enhanced RGB Image
```

## Detailed Technical Description

### Detailed Technical Description
Uses a 7-layer CNN with only 79k parameters to iteratively adjust pixel dynamic range.

## Key Equations / Algorithms

\[ LE_n(x) = LE_{n-1}(x) + \alpha_n(x) LE_{n-1}(x)(1 - LE_{n-1}(x)) \]

## Key Parameters / Measurements

| Parameter | Value | Context |
|---|---:|---|
| Parameter Count | 79,000 params | Ultra-lightweight model |
| Inference Speed | > 500 fps | GPU throughput |

## Useful Findings for MANAR

- PAPER FINDING: Light enhancement curves preserve color spectrum and avoid noise over-amplification in low-light imagery.
- MANAR RELEVANCE: Perfect pre-processing block for MANAR low-light RGB search missions.

## MANAR Design Questions This Paper Helps Answer

- Can Zero-DCE run directly on Jetson Orin Nano at 60 fps alongside YOLO detector?

## What This Paper Does NOT Establish

- Extreme dark conditions still require active illumination or LWIR thermal sensing.

## References Worth Following

- Zhu et al. (2021).

