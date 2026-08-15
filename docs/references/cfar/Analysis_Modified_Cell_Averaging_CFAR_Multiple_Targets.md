---
title: "Analysis of Some Modified Cell-Averaging CFAR Processors in Multiple-Target Situations"
short_title: "SO-CFAR Processor Analysis"
authors:
  - "L. M. Weiss"
year: 1982
publication: "IEEE Transactions on Aerospace and Electronic Systems"
publisher: "IEEE"
doi: "10.1109/TAES.1982.309210"
arxiv: null
site_url: "https://doi.org/10.1109/TAES.1982.309210"
pdf_url: null
peer_reviewed: true
paper_type: "journal"
primary_topic: "CFAR"
tags:
  - SO-CFAR
  - smallest-of
  - multi-target-resolution
manar_relevance:
  - passive-rf
  - FMCW-radar
relevance_score: 9
---
# Analysis of Some Modified Cell-Averaging CFAR Processors in Multiple-Target Situations

## Citation

L. M. Weiss. "Analysis of Some Modified Cell-Averaging CFAR Processors in Multiple-Target Situations." *IEEE Trans. AES*, 1982. DOI: 10.1109/TAES.1982.309210.

## Abstract / Paper Summary

Evaluates Smallest-Of CFAR (SO-CFAR) which selects the minimum of leading/lagging half-windows, preventing target masking when an interfering target is isolated to one half-window.

## Why This Matters for MANAR

Provides insight into resolving closely spaced victim radio emitters.

## Core Architecture / Method

```
Leading S1, Lagging S2 -> Min(S1, S2) -> Threshold Z
```

## Detailed Technical Description

### Detailed Technical Description
Analytical derivation of SO-CFAR performance with 1 to 2 interfering targets.

## Key Equations / Algorithms

\[ Z = \min\left(\sum X_{1,i}, \sum X_{2,i}\right) \]

## Key Parameters / Measurements

| Parameter | Value | Context |
|---|---:|---|
| Half Window M | 16 | Cells per half |

## Useful Findings for MANAR

- PAPER FINDING: SO-CFAR retains sensitivity when interfering targets fall in one half-window.

## MANAR Design Questions This Paper Helps Answer

- Can SO-CFAR be combined into a hybrid CFAR logic for MANAR?

## What This Paper Does NOT Establish

- Fails to regulate false alarms when leaving high-clutter areas.

## References Worth Following

- Gandhi & Kassam (1988).

