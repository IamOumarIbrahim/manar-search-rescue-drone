---
title: "Human Activity Classification Based on Micro-Doppler Signatures Using Artificial Neural Networks"
short_title: "Human Micro-Doppler Activity Classification"
authors:
  - "Youngwook Kim"
  - "Hao Ling"
year: 2009
publication: "IEEE Transactions on Antennas and Propagation"
publisher: "IEEE"
doi: "10.1109/TAP.2009.2015813"
arxiv: null
site_url: "https://doi.org/10.1109/TAP.2009.2015813"
pdf_url: null
peer_reviewed: true
paper_type: "journal"
primary_topic: "fmcw_radar"
tags:
  - micro-doppler-classification
  - neural-network
  - human-radar-detection
manar_relevance:
  - fmcw-radar
  - target-detection
relevance_score: 9
---
# Human Activity Classification Based on Micro-Doppler Signatures Using Artificial Neural Networks

## Citation

Y. Kim and H. Ling. "Human Activity Classification Based on Micro-Doppler Signatures Using Artificial Neural Networks." *IEEE Trans. AP*, 2009. DOI: 10.1109/TAP.2009.2015813.

## Abstract / Paper Summary

Demonstrates automated classification of human activities (walking, crawling, breathing, limb movement) from Doppler radar spectrograms using neural networks with > 90% accuracy.

## Why This Matters for MANAR

Validates automated machine learning classification of micro-Doppler signatures for MANAR target confirmation.

## Core Architecture / Method

```
Radar Spectrogram -> Feature Extraction (Torso Velocity, Periodicity, Bandwidth) -> Neural Network Classifier -> Activity Label
```

## Detailed Technical Description

### Detailed Technical Description
Extracts 6 key physical features from radar STFT spectrograms for neural network classification.

## Key Equations / Algorithms

\[ B_{Doppler}(t) = f_{max}(t) - f_{min}(t) \]

## Key Parameters / Measurements

| Parameter | Value | Context |
|---|---:|---|
| Classification Accuracy | > 92% | 7 human activity classes |
| Radar Bandwidth | 1.5 GHz | High-resolution FMCW |

## Useful Findings for MANAR

- PAPER FINDING: Micro-Doppler feature extraction allows robust classification of human motion vs background.
- MANAR RELEVANCE: Directly applicable to MANAR C++ ML inference layer.

## MANAR Design Questions This Paper Helps Answer

- Can small embedded neural nets run micro-Doppler classification on Jetson Orin Nano in real time?

## What This Paper Does NOT Establish

- Evaluated using ground-mounted stationary radar.

## References Worth Following

- Chen et al. (2006).

