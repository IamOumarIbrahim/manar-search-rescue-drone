---
title: "Smart Homes That Monitor Breathing and Heart Rate"
short_title: "Vital-Radio FMCW Breathing Monitor"
authors:
  - "Fadel Adib"
  - "Hongzi Mao"
  - "Zachary Kabelac"
  - "Dina Katabi"
  - "Robert C. Miller"
year: 2015
publication: "Proceedings of the ACM Conference on Human Factors in Computing Systems (CHI '15)"
publisher: "ACM"
doi: "10.1145/2702123.2702200"
arxiv: null
site_url: "https://doi.org/10.1145/2702123.2702200"
pdf_url: null
peer_reviewed: true
paper_type: "conference"
primary_topic: "fmcw_radar"
tags:
  - FMCW-vital-signs
  - Vital-Radio
  - multi-person-breathing
  - range-Doppler-respiration
manar_relevance:
  - fmcw-radar
  - target-detection
relevance_score: 9
---
# Smart Homes That Monitor Breathing and Heart Rate

## Citation

F. Adib et al. "Smart Homes That Monitor Breathing and Heart Rate." *Proc. ACM CHI*, 2015. DOI: 10.1145/2702123.2702200.

## Abstract / Paper Summary

Presents Vital-Radio, an FMCW radar system that isolates multiple human targets at different range bins and measures their respiratory and cardiac rates simultaneously with > 99% accuracy.

## Why This Matters for MANAR

Demonstrates range-gated vital sign detection, enabling MANAR to isolate individual victims in distinct range resolution bins.

## Core Architecture / Method

```
FMCW Chirps -> Range FFT -> Range-Bin Selection -> Time-Series Phase Tracking -> FFT -> Breathing Metric
```

## Detailed Technical Description

### Detailed Technical Description
Uses 5.5 - 7.2 GHz FMCW radar with 1.7 GHz sweep bandwidth achieving 8.8 cm range resolution.

## Key Equations / Algorithms

\[ \Delta R = \frac{c}{2 B} = \frac{3 \times 10^8}{2 \times 1.7 \times 10^9} \approx 0.088\text{ m} \]

## Key Parameters / Measurements

| Parameter | Value | Context |
|---|---:|---|
| Sweep Bandwidth (B) | 1.7 GHz | FMCW sweep width |
| Range Resolution | 8.8 cm | Bin isolation width |
| Breathing Accuracy | 99.4% | Controlled test accuracy |

## Useful Findings for MANAR

- PAPER FINDING: High sweep bandwidth isolates multiple individuals into distinct range bins for independent vital sign tracking.
- MANAR RELEVANCE: Proves multi-target vital sign detection capability using FMCW range gating.

## MANAR Design Questions This Paper Helps Answer

- What FMCW bandwidth is feasible on MANAR's onboard radar payload?

## What This Paper Does NOT Establish

- Tested indoors with stationary radar and stationary subjects.

## References Worth Following

- Li et al. (2013).

