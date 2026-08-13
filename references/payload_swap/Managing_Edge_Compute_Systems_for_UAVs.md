---
title: "Managing Edge Compute Systems for Unmanned Aerial Vehicles"
short_title: "UAV Edge Compute & Power Resource Sizing"
authors:
  - "James G. Boubin"
  - "John Chomko"
  - "Christopher Stewart"
year: 2020
publication: "IEEE Internet Computing"
publisher: "IEEE"
doi: "10.1109/MIC.2020.2982855"
arxiv: null
site_url: "https://doi.org/10.1109/MIC.2020.2982855"
pdf_url: null
peer_reviewed: true
paper_type: "journal"
primary_topic: "payload_swap"
tags:
  - UAV-edge-compute
  - SWaP-power-budget
  - battery-endurance
  - dynamic-frequency-scaling
manar_relevance:
  - payload-swap
  - uav-sar
relevance_score: 9
---
# Managing Edge Compute Systems for Unmanned Aerial Vehicles

## Citation

J. G. Boubin et al. "Managing Edge Compute Systems for Unmanned Aerial Vehicles." *IEEE Internet Computing*, 2020. DOI: 10.1109/MIC.2020.2982855.

## Abstract / Paper Summary

Investigates the trade-offs between onboard UAV edge compute power consumption, perception frame rate, and flight endurance, proposing dynamic thermal and power throttling algorithms.

## Why This Matters for MANAR

Provides MANAR with energy budget models balancing onboard AI compute power against multirotor flight endurance.

## Core Architecture / Method

```
Battery Power Draw = P_motors(v, m_payload) + P_compute(clock_freq) -> Dynamic Power Governor
```

## Detailed Technical Description

### Detailed Technical Description
Measures flight endurance reduction per watt of compute power (10 W compute reduces flight time of 1.5 kg quadrotor by ~ 8%).

## Key Equations / Algorithms

\[ T_{flight} = \frac{E_{battery}}{P_{hover}(m_{base} + m_{payload}) + P_{compute}} \]

## Key Parameters / Measurements

| Parameter | Value | Context |
|---|---:|---|
| Compute Power Footprint | 7 - 15 W | Jetson Orin Nano draw |
| Flight Endurance Penalty | 6 - 10% | Per 10 W compute power |

## Useful Findings for MANAR

- PAPER FINDING: Onboard compute power draw (10-15W) reduces flight endurance by 6-10%; dynamic clock frequency scaling during transit extends flight time.
- MANAR RELEVANCE: Directly informs MANAR's SWaP power budget and dynamic payload power governor.

## MANAR Design Questions This Paper Helps Answer

- Should MANAR throttle Jetson GPU clock speeds during high-speed transit to target search area?

## What This Paper Does NOT Establish

- Evaluated on specific quadrotor testbeds under hover.

## References Worth Following

- Mittal, S. (2019).

