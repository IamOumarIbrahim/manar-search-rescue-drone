---
title: "Coverage Path Planning for Search and Rescue Drones in Complex Environments"
short_title: "Coverage Path Planning for SAR Drones"
authors:
  - "Marina Torres"
  - "David A. Pelta"
  - "Jose L. Verdegay"
year: 2016
publication: "IEEE Access"
publisher: "IEEE"
doi: "10.1109/ACCESS.2016.2618995"
arxiv: null
site_url: "https://doi.org/10.1109/ACCESS.2016.2618995"
pdf_url: null
peer_reviewed: true
paper_type: "journal"
primary_topic: "uav_sar"
tags:
  - coverage-path-planning
  - lawnmower-pattern
  - SAR-optimization
  - battery-aware-flight
manar_relevance:
  - uav-sar
  - navigation
relevance_score: 9
---
# Coverage Path Planning for Search and Rescue Drones in Complex Environments

## Citation

M. Torres et al. "Coverage Path Planning for Search and Rescue Drones in Complex Environments." *IEEE Access*, 2016. DOI: 10.1109/ACCESS.2016.2618995.

## Abstract / Paper Summary

Formulates coverage path planning (CPP) algorithms for search and rescue UAVs operating under strict battery endurance constraints, optimizing lawnmower and grid flight paths to maximize terrain coverage.

## Why This Matters for MANAR

Provides MANAR's mission planner with mathematical path optimization algorithms.

## Core Architecture / Method

```
Search Polygon -> Convex Decomposition -> Sweep Angle Optimization -> Lawnmower Waypoints -> Battery Constraint Check
```

## Detailed Technical Description

### Detailed Technical Description
Optimizes sweep orientation to minimize UAV turn count, reducing energy consumption by up to 18%.

## Key Equations / Algorithms

\[ N_{turns} = \left\lceil \frac{\text{Width}(\mathcal{P}, \theta)}{W_{FOV}} \right\rceil \]

## Key Parameters / Measurements

| Parameter | Value | Context |
|---|---:|---|
| Energy Saving | 12 - 18% | Turn optimization benefit |
| Coverage Efficiency | > 95% | Area coverage ratio |

## Useful Findings for MANAR

- PAPER FINDING: Aligning lawnmower sweeps along the longest polygon edge minimizes energy-expensive turns, extending search area.
- MANAR RELEVANCE: Directly applicable to MANAR's mission route generator in C++ control software.

## MANAR Design Questions This Paper Helps Answer

- Should MANAR dynamically re-plan coverage paths when unexpected wind conditions alter battery consumption?

## What This Paper Does NOT Establish

- Assumes flat or moderately terrain-following flight grids.

## References Worth Following

- Goodrich et al. (2008).

