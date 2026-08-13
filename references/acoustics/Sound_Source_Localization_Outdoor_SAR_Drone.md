---
title: "An Acoustic Source Localization Method Using a Drone-Mounted Phased Microphone Array"
short_title: "Outdoor Drone Acoustic Victim Localization"
authors:
  - "Kotaro Hoshiba"
  - "Makoto Kumon"
  - "Kazuhiro Nakadai"
year: 2021
publication: "MDPI Drones"
publisher: "MDPI"
doi: "10.3390/drones5030075"
arxiv: null
site_url: "https://doi.org/10.3390/drones5030075"
pdf_url: "https://www.mdpi.com/2504-446X/5/3/75/pdf"
peer_reviewed: true
paper_type: "journal"
primary_topic: "acoustics"
tags:
  - acoustic-SAR
  - victim-voice-localization
  - phased-array
  - outdoor-drone-sound
manar_relevance:
  - acoustics
  - uav-sar
relevance_score: 9
---
# An Acoustic Source Localization Method Using a Drone-Mounted Phased Microphone Array

## Citation

K. Hoshiba et al. "An Acoustic Source Localization Method Using a Drone-Mounted Phased Microphone Array." *MDPI Drones*, 2021. DOI: 10.3390/drones5030075.

## Abstract / Paper Summary

Demonstrates outdoor victim voice and scream localization using a 16-channel phased microphone array mounted on a search and rescue drone hovering at 10-25 m altitude.

## Why This Matters for MANAR

Validates practical acoustic SAR detection ranges for human calls for help in outdoor environments.

## Core Architecture / Method

```
16-Channel Array -> MUSIC / SRP-PHAT Algorithm -> Ground Angle Estimation -> Intersect with Terrain Elevation -> GPS Target Coordinate
```

## Detailed Technical Description

### Detailed Technical Description
Evaluates detection probability for human shouting (80 dB SPL at 1 m) from 15 m hover altitude in 5 m/s wind.

## Key Equations / Algorithms

\[ P_{MUSIC}(\theta) = \frac{1}{\mathbf{a}^H(\theta) \mathbf{E}_n \mathbf{E}_n^H \mathbf{a}(\theta)} \]

## Key Parameters / Measurements

| Parameter | Value | Context |
|---|---:|---|
| Hover Altitude | 15 - 25 m | Tested operational altitude |
| Detection Range | 40 m | Ground human scream detection |

## Useful Findings for MANAR

- PAPER FINDING: Human screams (80 dB SPL) can be localized within 3 m accuracy from 15 m hover altitude.
- MANAR RELEVANCE: Confirms technical capability of MANAR acoustic payload under hover.

## MANAR Design Questions This Paper Helps Answer

- Must MANAR pause forward flight and hover to perform acoustic voice localization?

## What This Paper Does NOT Establish

- Wind speeds above 7 m/s severely degrade acoustic detection range.

## References Worth Following

- Cobos et al. (2017).

