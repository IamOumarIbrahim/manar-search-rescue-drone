---
title: "Vision-Controlled Micro Flying Robots: From System Design to Autonomous Navigation and Mapping in GPS-Denied Environments"
short_title: "Vision-Based Navigation in GPS-Denied Environments"
authors:
  - "Davide Scaramuzza"
  - "Michael Achtelik"
  - "Laurent Kneip"
  - "Stephan Weiss"
  - "Roland Siegwart"
year: 2014
publication: "IEEE Robotics & Automation Magazine"
publisher: "IEEE"
doi: "10.1109/MRA.2014.2322295"
arxiv: null
site_url: "https://doi.org/10.1109/MRA.2014.2322295"
pdf_url: null
peer_reviewed: true
paper_type: "journal"
primary_topic: "navigation"
tags:
  - visual-odometry
  - GPS-denied-navigation
  - MAV-autonomy
  - sensor-fusion-EKF
manar_relevance:
  - navigation
relevance_score: 10
---
# Vision-Controlled Micro Flying Robots: From System Design to Autonomous Navigation and Mapping in GPS-Denied Environments

## Citation

D. Scaramuzza et al. "Vision-Controlled Micro Flying Robots: From System Design to Autonomous Navigation and Mapping in GPS-Denied Environments." *IEEE RAM*, 2014. DOI: 10.1109/MRA.2014.2322295.

## Abstract / Paper Summary

Presents system design and visual-inertial odometry (VIO) algorithms for micro aerial vehicles navigating in GNSS-degraded or denied environments (dense forests, canyons, structures).

## Why This Matters for MANAR

Provides MANAR with fallback navigation architectures when GNSS signals are jammed or degraded.

## Core Architecture / Method

```
Monocular/Stereo Camera + High-Rate IMU -> Visual-Inertial EKF -> Position & Velocity Estimate -> Flight Controller
```

## Detailed Technical Description

### Detailed Technical Description
Fuses 100 Hz visual feature tracking with 500 Hz IMU integration for drift-free local state estimation.

## Key Equations / Algorithms

\[ \mathbf{x}_{VIO} = [\mathbf{p}_{w}^b, \mathbf{v}_{w}^b, \mathbf{q}_{w}^b, \mathbf{b}_a, \mathbf{b}_g]^T \]

## Key Parameters / Measurements

| Parameter | Value | Context |
|---|---:|---|
| Positional Drift | < 1% of distance | VIO trajectory error |
| Update Rate | 30 Hz Vision / 500 Hz IMU | Sensor sync rate |

## Useful Findings for MANAR

- PAPER FINDING: Tight visual-inertial coupling provides robust localization during temporary GNSS outages.
- MANAR RELEVANCE: Essential fallback navigation strategy for MANAR SAR flights in mountain valleys.

## MANAR Design Questions This Paper Helps Answer

- What camera baseline and FOV are optimal for MANAR visual-inertial odometry?

## What This Paper Does NOT Establish

- Requires texture-rich visual environments; fails over uniform water or snow.

## References Worth Following

- Tomic et al. (2012).

