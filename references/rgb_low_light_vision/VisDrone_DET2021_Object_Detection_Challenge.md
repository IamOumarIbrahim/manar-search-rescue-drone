---
title: "VisDrone-DET2021: The Vision Meets Drone Object Detection Challenge Results"
short_title: "VisDrone Aerial Object Detection Benchmark"
authors:
  - "Pengfei Zhu"
  - "Longyin Wen"
  - "Dawei Du"
  - "Xiao Bian"
  - "Haibin Ling"
  - "Qinghua Hu"
  - "Jiayu Zheng"
year: 2021
publication: "IEEE/CVF International Conference on Computer Vision Workshops (ICCVW 2021)"
publisher: "IEEE"
doi: "10.1109/ICCVW54120.2021.00319"
arxiv: null
site_url: "https://doi.org/10.1109/ICCVW54120.2021.00319"
pdf_url: null
peer_reviewed: true
paper_type: "conference"
primary_topic: "rgb_low_light_vision"
tags:
  - VisDrone
  - UAV-object-detection
  - aerial-pedestrian-detection
  - small-target-benchmark
manar_relevance:
  - rgb-low-light-vision
  - target-detection
relevance_score: 10
---
# VisDrone-DET2021: The Vision Meets Drone Object Detection Challenge Results

## Citation

P. Zhu et al. "VisDrone-DET2021: The Vision Meets Drone Object Detection Challenge Results." *IEEE ICCVW*, 2021. DOI: 10.1109/ICCVW54120.2021.00319.

## Abstract / Paper Summary

Reports benchmarks and evaluations from the VisDrone-DET2021 dataset comprising 10,209 drone-captured images with over 2.6 million annotated small targets (pedestrians, vehicles) across diverse altitudes and angles.

## Why This Matters for MANAR

Primary training and evaluation dataset benchmark for MANAR aerial RGB human detection models.

## Core Architecture / Method

```
Drone RGB Image (High Res) -> Feature Pyramid Network -> Small Object Anchor Head -> Pedestrian Bounding Box
```

## Detailed Technical Description

### Detailed Technical Description
Evaluates top detector architectures (YOLOv5, ClusDet, SAHI) on small aerial targets (< 16x16 pixels).

## Key Equations / Algorithms

\[ mAP_{50} = \frac{1}{C} \sum_{c=1}^C AP_{50}(c) \]

## Key Parameters / Measurements

| Parameter | Value | Context |
|---|---:|---|
| Annotated Images | 10,209 images | Large aerial dataset |
| Small Target Ratio | > 60% | Objects < 32x32 pixels |

## Useful Findings for MANAR

- PAPER FINDING: Standard object detectors (Faster R-CNN, baseline YOLO) fail on drone imagery due to tiny target pixel size and oblique camera angles.
- MANAR RELEVANCE: Proves necessity of specialized small-object detection heads for MANAR aerial vision.

## MANAR Design Questions This Paper Helps Answer

- What input image resolution (e.g. 1280x1280 vs 640x640) balances mAP and inference FPS on Jetson Orin?

## What This Paper Does NOT Establish

- RGB only; does not include thermal imagery.

## References Worth Following

- Du et al. (2018).

