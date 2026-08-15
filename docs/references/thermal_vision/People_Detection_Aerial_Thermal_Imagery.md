---
title: "People Detection from Aerial Thermal Imagery"
short_title: "Aerial Thermal People Detection"
authors:
  - "Jan Portmann"
  - "Stefan Lynen"
  - "Margarita Chli"
  - "Roland Siegwart"
year: 2014
publication: "IEEE International Conference on Robotics and Automation (ICRA 2014)"
publisher: "IEEE"
doi: "10.1109/ICRA.2014.6907147"
arxiv: null
site_url: "https://doi.org/10.1109/ICRA.2014.6907147"
pdf_url: null
peer_reviewed: true
paper_type: "conference"
primary_topic: "thermal_vision"
tags:
  - aerial-thermal-detection
  - UAV-person-detection
  - hotspot-segmentation
  - background-subtraction
manar_relevance:
  - thermal-vision
  - uav-sar
relevance_score: 10
---
# People Detection from Aerial Thermal Imagery

## Citation

J. Portmann et al. "People Detection from Aerial Thermal Imagery." *IEEE ICRA*, 2014. DOI: 10.1109/ICRA.2014.6907147.

## Abstract / Paper Summary

Evaluates human detection from UAV aerial thermal imagery flown over outdoor terrain at altitudes from 20 m to 80 m, combining local thermal contrast segmentation with HoG/SVM classification.

## Why This Matters for MANAR

Provides realistic detection metrics and thermal contrast limitations for UAV SAR operations.

## Core Architecture / Method

```
Thermal Video -> Background Stabilization -> Hotspot Blob Candidate Extraction -> HoG Feature Classifier -> Target Track
```

## Detailed Technical Description

### Detailed Technical Description
Evaluates pixels-on-target vs altitude (at 50 m altitude, a standing human occupies ~ 12x18 pixels on 320x240 thermal sensor).

## Key Equations / Algorithms

\[ N_{pixels} = \frac{h_{human} \cdot w_{human}}{z_{alt}^2 \cdot \tan(\text{FOV}_x) \cdot \tan(\text{FOV}_y)} \cdot N_{total} \]

## Key Parameters / Measurements

| Parameter | Value | Context |
|---|---:|---|
| Flight Altitudes | 20, 50, 80 m | UAV test flights |
| Pixels on Target (50m) | ~ 15x20 px | 320x240 thermal resolution |

## Useful Findings for MANAR

- PAPER FINDING: Thermal human detection reliability degrades rapidly above 60 m altitude due to low pixels-on-target.
- MANAR RELEVANCE: Establishes operational altitude ceiling guidelines for MANAR thermal search missions.

## MANAR Design Questions This Paper Helps Answer

- What thermal resolution (e.g. 640x512 vs 320x256) is required for MANAR to search effectively at 50m AGL?

## What This Paper Does NOT Establish

- Solar reflections off rocks and soil create false positive thermal hotspots during mid-day.

## References Worth Following

- Rudol & Doherty (2008).

