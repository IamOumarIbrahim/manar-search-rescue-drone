---
title: "A Survey on Optimized Execution of Deep Learning Models on Edge Devices"
short_title: "Edge AI Deep Learning Sizing & Optimization"
authors:
  - "Sparsh Mittal"
year: 2019
publication: "IEEE Transactions on Neural Networks and Learning Systems"
publisher: "IEEE"
doi: "10.1109/TNNLS.2019.2902261"
arxiv: null
site_url: "https://doi.org/10.1109/TNNLS.2019.2902261"
pdf_url: null
peer_reviewed: true
paper_type: "journal"
primary_topic: "payload_swap"
tags:
  - edge-AI
  - model-quantization
  - TensorRT
  - Jetson-optimization
  - SWaP-compute
manar_relevance:
  - payload-swap
  - target-detection
relevance_score: 10
---
# A Survey on Optimized Execution of Deep Learning Models on Edge Devices

## Citation

Sparsh Mittal. "A Survey on Optimized Execution of Deep Learning Models on Edge Devices." *IEEE Trans. TNNLS*, 2019. DOI: 10.1109/TNNLS.2019.2902261.

## Abstract / Paper Summary

Comprehensive survey of deep learning optimization techniques (INT8/FP16 quantization, TensorRT compilation, layer pruning, knowledge distillation) for edge compute hardware like NVIDIA Jetson SoCs.

## Why This Matters for MANAR

Provides MANAR with exact engineering strategies to fit thermal/RGB deep learning models onto onboard Jetson compute within strict power limits.

## Core Architecture / Method

```
PyTorch Model -> FP16 / INT8 Calibration -> TensorRT Engine Optimization -> Execution on Jetson Orin SoC
```

## Detailed Technical Description

### Detailed Technical Description
Evaluates speedups and power savings across INT8 quantization (2-4x speedup, 50% memory reduction with < 1% mAP loss).

## Key Equations / Algorithms

\[ Speedup = \frac{\text{Latency}_{FP32}}{\text{Latency}_{INT8}}, \quad \text{Energy\_Saved} = 1 - \frac{P_{INT8} \cdot t_{INT8}}{P_{FP32} \cdot t_{FP32}} \]

## Key Parameters / Measurements

| Parameter | Value | Context |
|---|---:|---|
| INT8 Speedup | 2.5x - 3.8x | NVIDIA Jetson GPU |
| Power Reduction | ~ 40% | Compute power savings |

## Useful Findings for MANAR

- PAPER FINDING: INT8 quantization yields up to 3.8x speedup and 40% power reduction on Jetson edge platforms with negligible accuracy loss.
- MANAR RELEVANCE: Essential technique for running MANAR's multi-modal vision models on Jetson Orin Nano.

## MANAR Design Questions This Paper Helps Answer

- What INT8 calibration dataset should MANAR use for thermal IR camera quantization?

## What This Paper Does NOT Establish

- Quantization requires careful calibration to avoid accuracy degradation in low-contrast thermal channels.

## References Worth Following

- Boubin et al. (2020).

