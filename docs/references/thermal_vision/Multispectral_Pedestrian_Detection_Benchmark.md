---
title: "Multispectral Pedestrian Detection: Benchmark Dataset and Baseline"
short_title: "KAIST Multispectral Pedestrian Benchmark"
authors:
  - "Soonmin Hwang"
  - "Jaesik Park"
  - "Namil Kim"
  - "Yukyung Choi"
  - "In So Kweon"
year: 2015
publication: "IEEE Conference on Computer Vision and Pattern Recognition (CVPR 2015)"
publisher: "IEEE"
doi: "10.1109/CVPR.2015.7298604"
arxiv: null
site_url: "https://doi.org/10.1109/CVPR.2015.7298604"
pdf_url: null
peer_reviewed: true
paper_type: "conference"
primary_topic: "thermal_vision"
tags:
  - multispectral-detection
  - thermal-RGB-fusion
  - KAIST-dataset
  - pedestrian-detection
manar_relevance:
  - thermal-vision
  - multisensor-fusion
relevance_score: 10
---
# Multispectral Pedestrian Detection: Benchmark Dataset and Baseline

## Citation

S. Hwang et al. "Multispectral Pedestrian Detection: Benchmark Dataset and Baseline." *IEEE CVPR*, 2015. DOI: 10.1109/CVPR.2015.7298604.

## Abstract / Paper Summary

Introduced the benchmark KAIST Multispectral Pedestrian Dataset containing synchronized RGB and LWIR (8-14 um) thermal image pairs captured during day and night, establishing baseline ACF and CNN fusion performance.

## Why This Matters for MANAR

Essential baseline benchmark for MANAR's thermal + RGB vision pipeline training and evaluation.

## Core Architecture / Method

```
RGB Image + LWIR Thermal Image -> Calibration & Registration -> Dual-Stream Feature Extractor -> Fusion Layer -> Bounding Box
```

## Detailed Technical Description

### Detailed Technical Description
Contains 95,000 thermal-color image pairs with 1,182 unique pedestrian annotations under day and night lighting.

## Key Equations / Algorithms

\[ Log\text{-}Miss\text{-}Rate = \exp\left( \frac{1}{9} \sum_{fppi} \log(1 - P_d) \right) \]

## Key Parameters / Measurements

| Parameter | Value | Context |
|---|---:|---|
| Thermal Band | LWIR (8-14 um) | FLIR A35 sensor |
| Resolution | 640x480 | RGB and LWIR resolution |

## Useful Findings for MANAR

- PAPER FINDING: Thermal imagery drastically improves detection accuracy at night, while RGB provides crucial spatial texture during day.
- MANAR RELEVANCE: Primary training and evaluation benchmark for MANAR multispectral vision.

## MANAR Design Questions This Paper Helps Answer

- How well do ground-based KAIST trained models generalize to top-down UAV aerial views?

## What This Paper Does NOT Establish

- Images captured from ground vehicle height rather than aerial UAV altitude.

## References Worth Following

- Li et al. (2019).

