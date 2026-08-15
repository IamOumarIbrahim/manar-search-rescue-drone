---
title: "Edge CA-CFAR Data Reduction for Bandwidth-Efficient Real-Time Wideband Spectrum Sensing on Low-Cost SDRs"
short_title: "Edge CA-CFAR Spectrum Sensing on SDR"
authors:
  - "Alexander Tittel"
  - "Michael Cronauer"
  - "Jens Zumbrägel"
  - "Norbert Wehn"
year: 2024
publication: "MDPI Sensors"
publisher: "MDPI"
doi: "10.3390/s24010123"
arxiv: null
site_url: "https://doi.org/10.3390/s24010123"
pdf_url: "https://www.mdpi.com/1424-8220/24/1/123/pdf"
peer_reviewed: true
paper_type: "journal"
primary_topic: "CFAR"
tags:
  - CA-CFAR
  - spectrum-sensing
  - SDR
  - edge-computing
manar_relevance:
  - passive-rf
  - payload-swap
relevance_score: 9
---
# Edge CA-CFAR Data Reduction for Bandwidth-Efficient Real-Time Wideband Spectrum Sensing on Low-Cost SDRs

## Citation

A. Tittel et al. "Edge CA-CFAR Data Reduction for Bandwidth-Efficient Real-Time Wideband Spectrum Sensing on Low-Cost SDRs." *MDPI Sensors*, 2024. DOI: 10.3390/s24010123.

## Abstract / Paper Summary

Demonstrates real-time CA-CFAR spectrum sensing on low-cost SDR/FPGA hardware, compressing 100 MHz RF bandwidth by > 90% at the edge before transferring anomaly metrics to host.

## Why This Matters for MANAR

Directly addresses MANAR's SWaP and data bus saturation constraints during wideband RF monitoring.

## Core Architecture / Method

```
100 MHz RF -> SDR Front-End -> FFT -> FPGA CA-CFAR -> >90% Reduced Anomaly Stream
```

## Detailed Technical Description

### Detailed Technical Description
Edge implementation on Zynq UltraScale+ and ADALM-PLUTO platforms for wideband signal detection.

## Key Equations / Algorithms

\[ Z = T \cdot \frac{1}{N} \sum_{i=1}^N P(f_i) \]

## Key Parameters / Measurements

| Parameter | Value | Context |
|---|---:|---|
| Processed Bandwidth | 100 MHz | Real-time RF bandwidth |
| Data Reduction | > 90% | Bus payload reduction |

## Useful Findings for MANAR

- PAPER FINDING: Performing CFAR at the SDR edge reduces host data transfers by over 90%.

## MANAR Design Questions This Paper Helps Answer

- Can MANAR deploy streaming CFAR inside the SDR FPGA pipeline?

## What This Paper Does NOT Establish

- Tested on CA-CFAR rather than OS-CFAR.

## References Worth Following

- Yucek & Arslan (2009).

