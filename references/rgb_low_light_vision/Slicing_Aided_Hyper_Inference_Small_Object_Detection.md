---
title: "Slicing Aided Hyper Inference and Fine-Tuning for Small Object Detection"
short_title: "SAHI Small Object Aerial Inference"
authors:
  - "Fatih Cagatay Akyon"
  - "Sinan Onur Altinuc"
  - "Alptekin Temizel"
year: 2022
publication: "IEEE International Conference on Image Processing (ICIP 2022)"
publisher: "IEEE"
doi: "10.1109/ICIP46576.2022.9897990"
arxiv: null
site_url: "https://doi.org/10.1109/ICIP46576.2022.9897990"
pdf_url: null
peer_reviewed: true
paper_type: "conference"
primary_topic: "rgb_low_light_vision"
tags:
  - SAHI
  - slicing-inference
  - small-object-detection
  - aerial-image-processing
manar_relevance:
  - rgb-low-light-vision
  - target-detection
relevance_score: 10
---
# Slicing Aided Hyper Inference and Fine-Tuning for Small Object Detection

## Citation

F. C. Akyon et al. "Slicing Aided Hyper Inference and Fine-Tuning for Small Object Detection." *IEEE ICIP*, 2022. DOI: 10.1109/ICIP46576.2022.9897990.

## Abstract / Paper Summary

Introduces SAHI (Slicing Aided Hyper Inference), an open-source inference pipeline that slices high-resolution aerial images into overlapping sub-patches, runs object detection, and merges bounding boxes via NMS.

## Why This Matters for MANAR

Directly solves MANAR's small human target detection problem when flying at high altitudes with 4K RGB cameras.

## Core Architecture / Method

```
High-Res Aerial Image (4K) -> Overlapping Grid Slice (640x640) -> Parallel CNN Inference -> Global NMS Merge -> Bounding Boxes
```

## Detailed Technical Description

### Detailed Technical Description
Boosts small target AP by 6.8% on VisDrone and xView benchmarks without altering base detector weights.

## Key Equations / Algorithms

\[ B_{global} = NMS\left( \bigcup_{i=1}^K \{ T_i(B_{local,i}) \} \right) \]

## Key Parameters / Measurements

| Parameter | Value | Context |
|---|---:|---|
| mAP Boost | + 6.8% AP | VisDrone small object benchmark |
| Slice Size | 640x640 | Overlapping tile size |

## Useful Findings for MANAR

- PAPER FINDING: Slicing high-resolution aerial images before inference prevents small targets from disappearing during CNN downsampling.
- MANAR RELEVANCE: Critical inference architecture for MANAR high-resolution RGB payload.

## MANAR Design Questions This Paper Helps Answer

- What slice overlap ratio (e.g. 20% vs 30%) balances duplicate detection NMS overhead and border target loss?

## What This Paper Does NOT Establish

- Increases total GPU inference passes per frame proportionally to slice count.

## References Worth Following

- Zhu et al. (2021).

