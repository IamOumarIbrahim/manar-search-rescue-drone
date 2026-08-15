---
title: "UAV-Based Passive RF Sensing for Emergency Search and Rescue"
short_title: "UAV Passive RF Emergency Localization"
authors:
  - "Stefano Scanzio"
  - "Claudio Zunino"
  - "Adriano Valenzano"
year: 2021
publication: "IEEE Transactions on Industrial Informatics"
publisher: "IEEE"
doi: "10.1109/TII.2021.3054128"
arxiv: null
site_url: "https://doi.org/10.1109/TII.2021.3054128"
pdf_url: null
peer_reviewed: true
paper_type: "journal"
primary_topic: "passive_rf"
tags:
  - passive-rf-sensing
  - search-and-rescue
  - cellular-detection
  - wi-fi-localization
manar_relevance:
  - passive-rf
  - uav-sar
relevance_score: 9
---
# UAV-Based Passive RF Sensing for Emergency Search and Rescue

## Citation

S. Scanzio et al. "UAV-Based Passive RF Sensing for Emergency Search and Rescue." *IEEE Trans. Ind. Inf.*, 2021. DOI: 10.1109/TII.2021.3054128.

## Abstract / Paper Summary

Presents a complete UAV-integrated passive RF sensing payload for emergency victim localization via mobile phone and Wi-Fi emissions, demonstrating outdoor search operational results.

## Why This Matters for MANAR

Validates payload integration and detection ranges for airborne passive RF SAR.

## Core Architecture / Method

```
Embedded SDR Payload -> Passive Wi-Fi/Cellular Sniffer -> Signal Strength Spatial Map -> Target Geolocation
```

## Detailed Technical Description

### Detailed Technical Description
Experimental flight validation using lightweight SDR payload detecting Wi-Fi probes and cellular signals from altitudes up to 50 m.

## Key Equations / Algorithms

\[ d = 10^{\frac{P_0 - RSSI}{10 \eta}} \]

## Key Parameters / Measurements

| Parameter | Value | Context |
|---|---:|---|
| Flight Altitude | 30 - 50 m | Operational SAR flight |
| Detection Range | 120 m | Wi-Fi probe signal |

## Useful Findings for MANAR

- PAPER FINDING: Wi-Fi probe requests can be detected up to 120 m range from 30 m UAV altitude.
- MANAR RELEVANCE: Confirms technical feasibility of MANAR passive RF payload specs.

## MANAR Design Questions This Paper Helps Answer

- Can MANAR achieve similar RF detection ranges under dense forest canopy?

## What This Paper Does NOT Establish

- Field tests performed in open rural areas rather than dense mountain forests.

## References Worth Following

- Sallouha et al. (2019).

