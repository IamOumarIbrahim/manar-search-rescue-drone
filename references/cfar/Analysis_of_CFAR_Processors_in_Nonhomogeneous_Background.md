---
title: "Analysis of CFAR Processors in Nonhomogeneous Background"
short_title: "Analysis of CFAR Processors in Nonhomogeneous Background"
authors:
  - "Prashant Gandhi"
  - "Saleem A. Kassam"
year: 1988
publication: "IEEE Transactions on Aerospace and Electronic Systems"
publisher: "IEEE"
doi: "10.1109/7.7073"
arxiv: null
site_url: "https://doi.org/10.1109/7.7073"
pdf_url: null
peer_reviewed: true
paper_type: "journal"
primary_topic: "CFAR"
tags:
  - CA-CFAR
  - OS-CFAR
  - GO-CFAR
  - SO-CFAR
  - nonhomogeneous-clutter
manar_relevance:
  - passive-rf
  - FMCW-radar
relevance_score: 10
---
# Analysis of CFAR Processors in Nonhomogeneous Background

## Citation

P. Gandhi and S. A. Kassam. "Analysis of CFAR Processors in Nonhomogeneous Background." *IEEE Trans. AES*, 1988. DOI: 10.1109/7.7073.

## Abstract / Paper Summary

Comparative analysis of CA, OS, GO, and SO CFAR processors under clutter step boundaries and multiple interfering targets.

## Why This Matters for MANAR

Provides MANAR engineers with a rigorous baseline trade-off matrix across all standard CFAR detector variants.

## Core Architecture / Method

```
N Cells -> CA / GO / SO / OS Processor -> Threshold Z -> Compare vs CUT Y
```

## Detailed Technical Description

### Detailed Technical Description
Derives exact closed-form detection and false alarm equations for four major CFAR variants.

## Key Equations / Algorithms

\[ Z_{GO} = \max(S_1, S_2), \quad Z_{SO} = \min(S_1, S_2) \]

## Key Parameters / Measurements

| Parameter | Value | Context |
|---|---:|---|
| Half Window M | 16 | Cells per side |
| CA Loss | 0 dB | Homogeneous baseline |

## Useful Findings for MANAR

- PAPER FINDING: OS-CFAR provides the best overall compromise across clutter edges and multi-target clutter.

## MANAR Design Questions This Paper Helps Answer

- Which CFAR variant fits MANAR radar altimetry vs passive RF?

## What This Paper Does NOT Establish

- Does not address 2D FFT range-Doppler processing overhead.

## References Worth Following

- Rohling, H. (1983) IEEE Trans. AES.

