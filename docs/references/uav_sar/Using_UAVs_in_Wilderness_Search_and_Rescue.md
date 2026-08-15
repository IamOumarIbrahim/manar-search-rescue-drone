---
title: "Using Unmanned Aerial Vehicles in Wilderness Search and Rescue"
short_title: "Wilderness Search & Rescue MAV System"
authors:
  - "Michael A. Goodrich"
  - "Benjamin P. Morse"
  - "Chris Engh"
  - "Joseph L. Cooper"
  - "Joshua M. Adams"
year: 2008
publication: "Journal of Field Robotics"
publisher: "Wiley"
doi: "10.1002/rob.20226"
arxiv: null
site_url: "https://doi.org/10.1002/rob.20226"
pdf_url: null
peer_reviewed: true
paper_type: "journal"
primary_topic: "uav_sar"
tags:
  - wilderness-SAR
  - human-in-the-loop
  - UAV-search-patterns
  - supervised-autonomy
manar_relevance:
  - uav-sar
  - target-detection
relevance_score: 10
---
# Using Unmanned Aerial Vehicles in Wilderness Search and Rescue

## Citation

M. A. Goodrich et al. "Using Unmanned Aerial Vehicles in Wilderness Search and Rescue." *Journal of Field Robotics*, 2008. DOI: 10.1002/rob.20226.

## Abstract / Paper Summary

Seminal field robotics paper detailing lessons learned from real-world wilderness search and rescue drone deployments, establishing human-in-the-loop workflow requirements, search pattern optimization, and operator cognitive load limits.

## Why This Matters for MANAR

Primary domain reference establishing MANAR's supervised autonomy philosophy and human authorization gates.

## Core Architecture / Method

```
Search Area Definition -> Lawnmower / Spiral Flight Path -> Sensor Payload Stream -> Human Operator Authorization Gate -> Target Verification
```

## Detailed Technical Description

### Detailed Technical Description
Field trials evaluating human visual search fatigue, video frame rates, coverage rates, and operator interface requirements.

## Key Equations / Algorithms

\[ Coverage\_Rate = v_{uav} \cdot 2 z_{alt} \tan(\text{FOV}/2) \]

## Key Parameters / Measurements

| Parameter | Value | Context |
|---|---:|---|
| Search Speed | 10 - 15 m/s | Typical SAR coverage speed |
| Flight Altitude | 40 - 100 m | Wilderness search altitude |

## Useful Findings for MANAR

- PAPER FINDING: Fully autonomous target declaration leads to high false alarm rates; human-in-the-loop authorization gates are essential for field SAR operations.
- MANAR RELEVANCE: Core architectural justification for MANAR's supervised autonomy and operator interface design.

## MANAR Design Questions This Paper Helps Answer

- What operator authorization interaction model minimizes decision latency while maintaining zero false target dispatches?

## What This Paper Does NOT Establish

- Uses early analog video links and manual flight control platforms.

## References Worth Following

- Tomic et al. (2012).

