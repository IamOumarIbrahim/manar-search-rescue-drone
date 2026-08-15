---
title: "The Unmanned Aerial Vehicle Benchmark: Object Detection and Tracking"
short_title: "UAVDT Object Detection & Tracking Benchmark"
authors:
  - "Dawei Du"
  - "Yuankai Qi"
  - "Hongyang Yu"
  - "Yifan Yang"
  - "Longyin Wen"
  - "Geng Lu"
  - "Siwei Lyu"
  - "Qingming Huang"
year: 2018
publication: "European Conference on Computer Vision (ECCV 2018)"
publisher: "Springer"
doi: "10.1007/978-3-030-01249-6_23"
arxiv: null
site_url: "https://doi.org/10.1007/978-3-030-01249-6_23"
pdf_url: null
peer_reviewed: true
paper_type: "conference"
primary_topic: "rgb_low_light_vision"
tags:
  - UAVDT-benchmark
  - UAV-tracking
  - aerial-object-detection
  - flight-altitude-impact
manar_relevance:
  - rgb-low-light-vision
  - target-detection
relevance_score: 9
---
# The Unmanned Aerial Vehicle Benchmark: Object Detection and Tracking

## Citation

D. Du et al. "The Unmanned Aerial Vehicle Benchmark: Object Detection and Tracking." *ECCV*, 2018. DOI: 10.1007/978-3-030-01249-6_23.

## Abstract / Paper Summary

Presents the UAVDT dataset focusing on aerial object detection and multi-object tracking under complex UAV motion, camera tilt, weather conditions, and occlusion.

## Why This Matters for MANAR

Provides quantitative analysis of how UAV flight parameters (altitude, velocity, gimbal angle) affect detection accuracy.

## Core Architecture / Method

```
UAV Video Frame -> Motion-Compensated Detector -> Multi-Hypothesis Association -> Tracked Target Trajectory
```

## Detailed Technical Description

### Detailed Technical Description
Contains 80,000 frames with detailed flight metadata (altitude, illumination, occlusion level).

## Key Equations / Algorithms

\[ MOTA = 1 - \frac{\sum (FN_t + FP_t + IDSW_t)}{\sum GT_t} \]

## Key Parameters / Measurements

| Parameter | Value | Context |
|---|---:|---|
| Total Frames | 80,000 frames | Video tracking dataset |
| Flight Conditions | Day, Night, Fog | Varied weather |

## Useful Findings for MANAR

- PAPER FINDING: High UAV speed and camera tilt increase tracking ID switches by over 40%.
- MANAR RELEVANCE: Emphasizes the need for velocity-aware tracking filters in MANAR perception.

## MANAR Design Questions This Paper Helps Answer

- How should MANAR's target tracker handle target occlusions during low-altitude passes?

## What This Paper Does NOT Establish

- Primarily focused on vehicle targets rather than wilderness search victims.

## References Worth Following

- Zhu et al. (2021).

