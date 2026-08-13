# MANAR v1.0 Design Specification

## Contents

1. [Purpose and Scope](#1-purpose-and-scope)
2. [Architecture and Requirements](#2-architecture-and-requirements)
3. [Guidance, Navigation, and Mission Logic](#3-guidance-navigation-and-mission-logic)
4. [Sensing and Signal Processing](#4-sensing-and-signal-processing)
5. [Multisensor Fusion and Alert Logic](#5-multisensor-fusion-and-alert-logic)
6. [Sensor and Guidance Performance](#6-sensor-and-guidance-performance)
7. [Rescue Guidance Systems](#7-rescue-guidance-systems)
8. [Operating and Authorization Rules](#8-operating-and-authorization-rules)
9. [Revision Summary](#9-revision-summary)
10. [Limitations](#10-limitations)

---

## 1. Purpose and Scope

MANAR is a proprietary multisensor search-and-rescue drone system developed and owned by **Oumar Ibrahim**, initially intended for localized deployment in the UAE.

MANAR uses **supervised autonomy**. Deterministic flight logic and machine-learning perception automate search and candidate inspection, while the human operator retains continuous override authority and makes the final **Probable rescuee** determination.

The system may automatically approach a persistent candidate within predefined safety limits. The operator may cancel the approach, pause the mission, command Return-to-Home (RTH), or take manual control at any time.

---

## 2. Architecture and Requirements

### 2.1 Supervised-Autonomy Model

| Layer | Responsibility |
|---|---|
| **Human operator** | Defines and supervises the mission, may override automation at any time, reviews alerts, makes the final Probable rescuee determination, and authorizes consequential guidance actions. |
| **Deterministic code** | Controls flight, navigation, safety, automatic candidate approach, mission states, sensor timing, signal processing, and failsafes. |
| **Machine learning** | Detects candidates and classifies sensor observations. Mamba is used only during hover verification to determine whether the operator should be alerted. ML never commands motors directly. |

### 2.2 System Requirements

| ID | Requirement |
|---|---|
| **SYS-R-01 — Communication-loss failsafe** | On command-link loss, enter a deterministic link-loss contingency. During a short configurable timeout, hold or continue only a verified safe route while attempting reconnection. If the link is not restored, initiate RTH. |
| **SYS-R-02 — Energy failsafe** | Continuously reserve sufficient return energy. Trigger RTH when the mission cannot safely continue; if safe return is no longer possible, perform a controlled emergency landing and preserve/transmit the latest coordinates when communications permit. |
| **SYS-R-03 — Position and location reporting** | Determine aircraft position primarily using GNSS. Continuously transmit position and flight-state telemetry while at least one telemetry link is available; otherwise continue logging the same data onboard until communications are restored. LTE/5G or satellite may provide redundant reporting where available. |

### 2.3 Design Targets

| ID | Target |
|---|---|
| **SYS-G-01 — High-altitude performance** | Autonomous takeoff, climb, and stable operation up to **2,000 m AMSL** with the designated rescue payload. |
| **SYS-G-02 — Mission endurance** | At least **1 hour of continuous active search** under the defined representative mission profile. The validation profile shall specify payload mass, nominal search altitude/speed, sensor duty cycles, expected hover-verification frequency, and environmental assumptions. |

### 2.4 Responsibility Allocation

**Human operator**
- Defines the search area, objective, launch point, route, altitude, geofence, and safety limits.
- Approves takeoff and may start, pause, abort, command RTH, or manually control the aircraft.
- Monitors flight status, map position, and sensor feeds.
- May cancel any automatic candidate approach or verification sequence.
- Reviews Mamba-triggered alerts and makes the final **Probable rescuee** determination.
- Approves beacon, strobe, smoke, and other consequential guidance actions.
- Shares confirmed coordinates with rescue teams and handles exceptional emergency decisions.

**Deterministic code**

| Area | Functions |
|---|---|
| **Flight and safety** | Search-path generation, waypoint navigation, geofencing, obstacle avoidance, altitude control, battery monitoring, automatic approach, RTH, fault detection, and sensor-health checks. |
| **Mission coordination** | Ambient-light mode switching, sensor synchronization, GNSS tagging, candidate persistence checks, approach/hover state transitions, and cross-camera target matching. |
| **Audio DSP** | SOS/message timing, listening windows, beamforming, direction estimation, and rotor-noise filtering using motor RPM. |
| **Passive RF DSP** | Frequency scanning, signal-strength measurement, and direction estimation from measurements at multiple aircraft positions. |
| **FMCW DSP** | Range/Doppler FFTs, range–Doppler maps, CFAR, range/velocity estimation, and phase-based micro-motion extraction. |

**Machine learning**

| Area | Functions |
|---|---|
| **Computer vision** | Detect and track standing, sitting, lying, motionless, partially buried, or partially visible people in RGB, low-light/IR, and thermal imagery; reject common false targets. |
| **Audio classification** | Detect speech, shouting, whistles, clapping, tapping, crying, and other human distress sounds while separating wind, animals, and rotor noise. |
| **Radar classification** | Classify extracted radar features as human motion, breathing-like micro-motion, animal motion, vegetation/debris, drone vibration, or unknown. |
| **RF classification** | Classify received activity as supported cellular, Wi-Fi, Bluetooth, drone telemetry, infrastructure, or unknown. |
| **Hover fusion (Mamba)** | Combine several seconds of synchronized hover observations and decide whether the candidate should be rejected, observed longer, or escalated to the operator. |

---

## 3. Guidance, Navigation, and Mission Logic

### 3.1 Guidance Architecture

A Pixhawk-class autopilot runs safety-critical flight controls.

```text
Operator defines mission
        ↓
Mission planner generates route
        ↓
Autopilot follows GNSS waypoints
        ↓
IMU + GNSS + compass + barometer stabilize the aircraft
        ↓
Radar altimeter maintains terrain-relative clearance
```

Machine-learning outputs are advisory inputs to mission logic and never direct motor commands.

### 3.2 Navigation Sensors and Data Sources

| Sensor / source | Function |
|---|---|
| **GNSS/GPS** | Global position and waypoint navigation |
| **IMU** | Attitude, acceleration, and short-term inertial motion estimation |
| **Compass** | Heading |
| **Barometer** | Approximate altitude |
| **Downward radar altimeter** | Terrain-relative height above snow, sand, or ground |
| **Forward/side obstacle radar** | Local obstacle detection in darkness and reduced visibility |
| **Terrain map** | Pre-mission and in-mission terrain awareness for slopes, ridges, and mountains |

> The obstacle/altimeter radars are navigation sensors. The **24 GHz FMCW rescue-sensing radar** is treated as a separate payload function unless a future implementation explicitly validates shared hardware.

### 3.3 Mission States

```text
TAKEOFF
→ CLIMB
→ SEARCH
→ CANDIDATE DETECTED
→ AUTOMATIC APPROACH
   └─ Operator UI: candidate approach active
→ HOVER AND VERIFY
→ MAMBA FUSION
    ├─ Reject candidate → RESUME SEARCH
    ├─ Continue verification → REMAIN IN HOVER
    └─ Alert operator
           ↓
       HUMAN REVIEW
       ├─ False positive → RESUME SEARCH
       ├─ Manual continued inspection → REMAIN IN HOVER
       └─ Probable rescuee → MARK / GUIDE
                                ↓
                      CONTINUE SEARCH / RTH
```

The operator may interrupt or override any state at any time.

### 3.4 Search Patterns

- **Open or relatively flat terrain:** use a **parallel-track (lawnmower) search pattern**. Track spacing depends on altitude, sensor field of view, overlap, terrain, visibility, and expected detection range.
- **Mountainous or steep terrain:** use terrain-aware contour-following routes rather than a fixed parallel-track pattern.

### 3.5 Candidate Detection, Approach, and Verification

Wide-area detectors create a candidate only after deterministic persistence checks.

```text
Candidate detected
Detector confidence: 82%
Bearing: 35° right
Estimated ground position: GNSS coordinate
```

The deterministic controller then:

1. Records the candidate position and supporting sensor evidence.
2. Verifies that approach is permitted by geofence, obstacle, terrain, wind, link, and battery constraints.
3. Automatically reduces speed and approaches the candidate.
4. Descends only to a predefined safe inspection altitude.
5. May perform a tightly bounded inspection orbit for positioning, then transitions to a stable hover.
6. Stabilizes and points imaging, FMCW, and acoustic sensors toward the candidate.
7. Starts Mamba inference only after stable hover is established.
8. Runs hover-specific verification and the call-and-listen sequence while Mamba is re-evaluated as new synchronized evidence becomes available.
9. Alerts the operator only when the Mamba escalation criterion is met.

The operator may cancel the automatic approach or take manual control at any point.

### 3.6 Communications and Failsafes

Use a dedicated encrypted command-and-control link as the primary control channel. Optional layers may include a separate video/data link, LTE/5G backup, satellite service for remote regions, and onboard mission storage.

#### Continuous Telemetry

MANAR shall continuously transmit low-bandwidth flight telemetry to the ground control station through the primary command-and-control link at a nominal **2–10 Hz** update rate while a telemetry link is available.

Telemetry shall include, at minimum:

- latitude and longitude;
- AMSL altitude and terrain-relative altitude when available;
- ground speed and heading;
- battery state and remaining-energy estimate;
- current mission state;
- command-link quality and health;
- RTH/failsafe status; and
- candidate coordinates when a candidate is being tracked.

Telemetry shall operate **independently of the high-bandwidth video/data stream** so that loss or degradation of live video does not by itself prevent position and flight-status reporting.

During total external link loss, MANAR shall continue recording position, aircraft state, mission state, and relevant health data to onboard storage. If communications are restored, current telemetry shall resume and retained mission logs shall remain available for post-mission review.

| Failure | Deterministic response |
|---|---|
| Brief control-link loss | Hold or continue only the verified safe route while reconnecting |
| Sustained control-link loss | RTH |
| GNSS degraded | Slow/hold using inertial attitude and local clearance sensing; do not claim absolute radar positioning; land safely if global navigation cannot be recovered |
| Battery reaches reserve | Stop searching and RTH |
| Dangerous wind | Abort, RTH, or land safely according to the energy and wind envelope |
| Rescue sensor failure | Disable affected sensing mode and notify operator |
| Obstacle detected | Stop, climb, or reroute within limits |
| Terrain clearance uncertain | Hold and request operator action or execute a safe contingency |
| Operator manual takeover | Immediately yield mission maneuvering to operator commands while preserving hard safety limits |

---

## 4. Sensing and Signal Processing

### 4.1 Imaging and Ambient-Light Switching

| Ambient light | Imaging mode |
|---:|---|
| Above **150 lux** | RGB + thermal |
| **50–150 lux** | RGB + low-light/IR + thermal |
| Below **50 lux** | Low-light/IR + thermal |

These are nominal thresholds. The implementation shall use **hysteresis and a minimum dwell time** to prevent rapid switching near 50 or 150 lux.

Training and evaluation data should cover:
- multiple altitudes and camera angles;
- morning, midday, sunset, and night;
- hot and cold desert terrain;
- varied clothing, body sizes, and poses;
- motionless, partially buried, and partially visible people;
- fog, dust, rain, haze, and other reduced-visibility conditions.

### 4.2 Acoustic Detection and Call-and-Listen

Use a **4–8 microphone digital MEMS array** mounted underneath the aircraft, as far as practical from the motors, behind wind protection, and on vibration-isolating mounts.

```text
4–8 MEMS microphones
        ↓
Wind + rotor-noise filtering
        ↓
Beamforming / direction estimation
        ↓
ML sound classifier
        ↓
Human-acoustic score + direction
```

- Speech-intelligibility range: approximately **300 Hz–5 kHz**.
- Practical processing band: **200 Hz–8 kHz**.
- Classes: speech/shouting, whistle, clapping, tapping, crying, animal, wind, rotor noise, and unknown.

**Wide-area search:** only short passive acoustic samples may be taken; these do not use the full rescue call cycle.

**Hover verification:** use the full prompt-and-listen sequence. During each **15 s** listening window:
- silence the speaker and buzzer;
- hold the aircraft as steadily as possible;
- record continuously;
- use known motor RPM as a reference for rotor-noise suppression;
- detect human acoustic events;
- estimate direction of arrival.

An acoustic observation is considered supporting evidence when:
- a human voice, whistle, clap, or tapping event is detected;
- confidence is at least **70%**; and
- the event occurs at least twice during the listening period.

#### SOS and Spoken Cycle

| Stage | Timing |
|---|---:|
| SOS buzzer | **8.4 s** |
| English + Arabic messages | **12 s** total |
| Silent listening | **15 s** |
| One complete cycle | **35.4 s** |
| Maximum hover verification | **Up to 3 cycles (106.2 s)** |

SOS pattern: 3 short (**0.4 s** each), 3 long (**1.2 s** each), and 3 short (**0.4 s** each), with **0.3 s gaps between signal elements**.

**English:** “Rescue drone. Shout, whistle, clap, or hit an object now.”

**Arabic:**  
> هذه طائرة إنقاذ. اصرخ، صفّر، صفّق، أو اضرب أي جسم الآن

One cycle is the default initial hover verification. Additional cycles, up to three total, are used only when verification remains inconclusive and the mission safety/energy budget permits.

Reference retained from the source:  
https://www.fema.gov/pdf/emergency/usr/appen_a.pdf

### 4.3 Passive RF Detection — v1.0 Scope

MANAR v1.0 limits passive RF monitoring to **frequencies at or below 6 GHz**.

| Source | v1.0 monitored range / bands | Role |
|---|---|---|
| **2G/3G/4G cellular** | Configured uplink bands within approximately **600 MHz–2.7 GHz** | Long-range supporting evidence |
| **5G sub-6** | Configured uplink bands **below 6 GHz** | Supporting evidence where local bands are supported |
| **Wi-Fi** | **2.4 GHz and 5 GHz** | Short/medium-range supporting evidence |
| **Bluetooth** | **2.4 GHz** | Close-range supporting evidence |
| **Wi-Fi above 6 GHz** | **Out of scope for v1.0** | Not monitored |

> The listed frequency ranges describe MANAR v1.0 implementation scope, not every frequency allocated to each standard worldwide.

**Receiver options**

| Option | Coverage | v1.0 role |
|---|---:|---|
| **HackRF One** | 1 MHz–6 GHz | Lower-cost proof-of-concept SDR |
| **USRP B205mini-i** | 70 MHz–6 GHz | Preferred higher-performance prototype SDR |
| **AD8318 RF detector** | 1 MHz–8 GHz | Optional auxiliary RF-power/RSSI detector only; not a substitute for the SDR classifier |

Use separate antenna coverage rather than one small broadband omnidirectional antenna:
- approximately **600 MHz–2.7 GHz** directional coverage for cellular;
- approximately **2.4–6 GHz** directional coverage for supported 5G/Wi-Fi/Bluetooth activity.

**Detection logic**
1. Scan configured **cellular uplink** bands within the supported range.
2. Separately scan configured **Wi-Fi/Bluetooth** bands.
3. Avoid treating strong known infrastructure/downlink energy as rescuee evidence.
4. Detect activity above the local noise baseline.
5. Classify the observation as supported cellular, Wi-Fi, Bluetooth, or unknown.
6. Record drone position, heading, received power, and classification confidence.
7. Compare measurements from several aircraft positions.
8. Estimate a likely bearing/region and use it only as supporting evidence.

### 4.4 FMCW Radar

The **24 GHz FMCW rescue radar** provides deterministic range, velocity, gross-motion, and micro-motion features.

With approximately **250 MHz sweep bandwidth**, nominal range resolution is about **60 cm**:

$$
\Delta R \approx \frac{c}{2B}
$$

Targets separated by less than this distance in range may be difficult to distinguish.

---

## 5. Multisensor Fusion and Alert Logic

### 5.1 Wide-Area vs Hover Processing

MANAR uses two distinct perception stages:

| Stage | Processing |
|---|---|
| **Wide-area search** | Per-sensor detectors/classifiers find persistent candidates. Mamba is not used. |
| **Hover verification** | Synchronized camera, radar, audio, RF, and aircraft-state observations are fused over time by Mamba. |

Wide-area candidate detection may trigger an **automatic approach** through deterministic mission logic. Mamba does not authorize or command the approach.

### 5.2 Per-Sensor Processing

```text
RGB      → CNN detector → boxes + confidence + compact 1D visual embedding
IR       → CNN detector → boxes + confidence + compact 1D visual embedding
Thermal  → CNN detector → boxes + confidence + compact 1D visual embedding
Radar    → range + motion + micro-motion features
Audio    → human-acoustic score + class scores + direction confidence
RF       → RF-presence score + class + bearing confidence
```

Targets are matched across cameras using geometry and bounding-box association before fusion.

### 5.3 Hover Feature Sequence

Each hover time step contains a fixed-format vector such as:

```text
[
  RGB_person_confidence,
  IR_person_confidence,
  thermal_person_confidence,
  RGB_embedding[],
  IR_embedding[],
  thermal_embedding[],
  target_x,
  target_y,
  radar_range,
  radar_motion_score,
  radar_micro_motion_score,
  human_acoustic_score,
  acoustic_direction_confidence,
  RF_presence_score,
  RF_bearing_confidence,
  drone_altitude,
  drone_speed,
  drone_attitude_motion,
  visibility_state
]
```

The visual embeddings are compact projected **1D feature vectors**, not raw 2D images.

### 5.4 Mamba Alert Gate

```text
20–50 consecutive hover feature vectors
                ↓
              Mamba
                ↓
Reject candidate
Continue verification
Alert operator
```

At **10 measurements/s**, 30 vectors represent a **3 s** observation window.

Mamba may be evaluated repeatedly throughout hover verification. Early evaluations use the sensors available at that moment; once a **15 s acoustic listening window** completes, updated acoustic evidence is included in subsequent fusion decisions. Mamba therefore does not need to wait for the full **35.4 s** call-and-listen cycle before producing an initial result.

**Mamba's role ends at alert escalation.** It does not declare a Probable rescuee and does not command the aircraft.

The human operator reviews the evidence and assigns one of the operational outcomes:

```text
False positive
Probable rescuee
Manual continued inspection
```

---

## 6. Sensor and Guidance Performance

**Rating:** 5 = Excellent · 4 = Strong · 3 = Conditional · 2 = Weak · 1 = Ineffective

> A high sensor score does not imply that the aircraft is safe to operate. Hail, dense wet snow, strong sand, and heavy rain may still require abort, emergency landing, or RTH.

### 6.1 Component Roles

| Component | Primary role |
|---|---|
| Thermal | Person and heat detection |
| RGB/day | Daytime detection and verification |
| Low-light/IR | Night visual confirmation |
| 24 GHz FMCW | Presence, range, motion, micro-motion |
| Speaker + mic | Prompt, listen, and estimate direction |
| Passive RF | Detect and correlate device emissions |
| Amber beacon | 360° visual attention cue |
| White strobe | Directional visual guidance |
| Downward spotlight | Close-range illumination and inspection |
| Heliograph mirrors | Three fixed passive daylight reflectors |
| Smoke marker | Daytime location and wind marking |

### 6.2 Lighting and Precipitation

| Component | Daylight | Night | Heavy rain | Wet snow | Hail |
|---|:---:|:---:|:---:|:---:|:---:|
| Thermal | 4 | 5 | 2 | 2 | 2 |
| RGB/day | 5 | 1 | 2 | 2 | 1 |
| Low-light/IR | 2 | 5 | 2 | 2 | 2 |
| 24 GHz FMCW | 5 | 5 | 4 | 3 | 3 |
| Speaker + mic | 4 | 4 | 2 | 2 | 1 |
| Passive RF | 5 | 5 | 4 | 3 | 4 |
| Amber beacon | 3 | 5 | 3 | 2 | 2 |
| White strobe | 3 | 5 | 3 | 2 | 2 |
| Downward spotlight | 1 | 5 | 2 | 2 | 1 |
| Heliograph mirrors | 4 | 1 | 1 | 1 | 1 |
| Smoke marker | 5 | 2 | 1 | 1 | 1 |

### 6.3 Visibility Obscurants

| Component | Haze | Fog | Dust | Moderate sand | Smoke |
|---|:---:|:---:|:---:|:---:|:---:|
| Thermal | 4 | 2 | 3 | 2 | 4 |
| RGB/day | 2 | 1 | 2 | 1 | 1 |
| Low-light/IR | 3 | 1 | 2 | 1 | 2 |
| 24 GHz FMCW | 5 | 5 | 4 | 3 | 5 |
| Speaker + mic | 4 | 4 | 3 | 2 | 3 |
| Passive RF | 5 | 5 | 5 | 4 | 5 |
| Amber beacon | 3 | 2 | 2 | 2 | 2 |
| White strobe | 3 | 1 | 2 | 1 | 2 |
| Downward spotlight | 3 | 1 | 2 | 1 | 1 |
| Heliograph mirrors | 2 | 1 | 1 | 1 | 1 |
| Smoke marker | 2 | 1 | 1 | 1 | 1 |

### 6.4 Scene and Platform Interference

| Component | Clutter | Drone motion | Moving vegetation/debris |
|---|:---:|:---:|:---:|
| Thermal | 3 | 3 | 3 |
| RGB/day | 3 | 2 | 2 |
| Low-light/IR | 3 | 2 | 2 |
| 24 GHz FMCW | 2 | 3 | 2 |
| Speaker + mic | 3 | 2 | 2 |
| Passive RF | 3 | 4 | 4 |
| Amber beacon | 2 | 4 | 3 |
| White strobe | 3 | 4 | 3 |
| Downward spotlight | 3 | 3 | 3 |
| Heliograph mirrors | 2 | 2 | 2 |
| Smoke marker | 3 | 4 | 2 |

### 6.5 Search-Phase Operation

| Component | Wide-area | Auto approach | Hover verify | Critical return |
|---|:---:|:---:|:---:|:---:|
| Thermal | 5 | 5 | 5 | 2 |
| RGB/day | 4 | 5 | 5 | 2 |
| Low-light/IR | 4 | 5 | 5 | 2 |
| 24 GHz FMCW | 4 | 5 | 5 | 1 |
| Speaker + mic | 2 | 2 | 5 | 1 |
| Passive RF | 4 | 4 | 4 | 1 |
| Amber beacon | 1 | 1 | 2 | 1 |
| White strobe | 1 | 1 | 2 | 1 |
| Downward spotlight | 1 | 2 | 5 | 1 |
| Heliograph mirrors | 1 | 1 | 2 | 1 |
| Smoke marker | 1 | 1 | 1 | 1 |

> Beacon, strobe, spotlight, and smoke remain inactive during automatic approach unless the operator explicitly activates them. The fixed heliograph mirrors are passive hardware and are not switched on or aimed. During hover verification, the operator may use the spotlight for inspection before Probable rescuee confirmation. A smoke marker that has already been deployed remains on the ground independently of aircraft power; this does not increase its **Critical return** aircraft-operation score.

---

## 7. Rescue Guidance Systems

### 7.1 Final v1.0 Visual-Guidance Configuration

- **Fixed 360° amber flashing beacon:** broad-area visual attention cue after operator authorization.
- **Directional white strobe:** high-intensity directional visual cue, nominally **2 Hz**, after operator authorization.
- **Downward white spotlight:** close-range visual inspection and illumination.
- **Three fixed heliograph-style mirrors:** passive daylight reflectors mounted at **40°**, **60°**, and **75° from vertical**, intended to bias reflections toward near-, medium-, and far-range geometries.
- **Smoke marker:** daytime location marking and approximate wind indication after explicit operator authorization and environmental safety checks.

### 7.2 Three Fixed Heliograph Mirrors

MANAR uses **three fixed, non-motorized heliograph-style mirrors** at different mounting angles:

| Mirror | Fixed mounting angle from vertical | Intended coverage bias |
|---|---:|---|
| **Mirror 1** | **40°** | Near-biased reflection |
| **Mirror 2** | **60°** | Medium-biased reflection |
| **Mirror 3** | **75°** | Far-biased reflection |

The mirror mounting angle is **not the same as the reflected-beam angle**. Actual reflection direction depends on solar azimuth/elevation, aircraft heading, pitch and roll, mirror orientation, and target geometry.

If **θ** is the actual reflected-beam angle from vertical, the idealized horizontal ground intercept from aircraft altitude **H** is:

$$
R = H\tan\theta
$$

Because **θ** varies with sun and aircraft geometry, MANAR v1.0 does **not** claim fixed ground distances for the 40°, 60°, and 75° mirror mounting angles.

The mirrors are passive and have no motorized aiming mechanism. They remain a **supplementary daylight guidance method** rather than the primary rescue-location signal.

---

## 8. Operating and Authorization Rules

### 8.1 Search-Phase Policy

| Phase | Operating policy |
|---|---|
| **Wide-area search** | Run thermal plus the lighting-appropriate camera; operate rescue FMCW at reduced rate/bursts; scan supported RF bands as power allows; use only short passive acoustic samples; keep guidance devices inactive. |
| **Candidate detected / automatic approach** | Require persistence, verify the safety envelope, record candidate coordinates, automatically approach, increase relevant sensor update rates, and focus processing on the candidate. Operator may cancel at any time. |
| **Hover verification** | Stabilize over/near the candidate, use full synchronized imaging/radar/audio/RF verification, perform the prompt-and-listen cycle, and run Mamba temporal fusion. The operator may activate the downward spotlight for inspection; beacon, strobe, and smoke remain off. The fixed heliograph mirrors remain passive. |
| **Operator alert** | Present synchronized evidence only when Mamba meets the alert criterion. Human decides whether the candidate is a false positive, requires manual continued inspection, or is a **Probable rescuee**. |
| **Confirmed guidance** | After human confirmation/authorization, use beacon, strobe, smoke, and other active guidance as appropriate. The heliograph mirrors remain passive and may provide incidental daylight reflections when geometry is favorable. |
| **Battery-critical return** | Preserve flight control, navigation, obstacle sensing, command link, logging, and RTH. Reduce or disable nonessential rescue sensing and guidance loads. |

### 8.2 Fusion and Authorization Rules

1. **Wide-area detection may automatically trigger approach.** The approach is executed only by deterministic flight logic within predefined safety constraints.
2. **The operator retains full override authority.** Automatic approach, hover verification, or any other mission state may be cancelled at any time.
3. **Mamba is a hover-stage alert gate only.** It determines whether sensor evidence is strong enough to alert the operator.
4. **Only the human operator assigns Probable rescuee status.**
5. **No single sensor should dominate confirmation.** Use multiple independent observations whenever possible.
6. **Radar detections in vegetation require caution.** Branches and debris can resemble body movement.
7. **RF is supporting evidence only.** A device may be abandoned, inside a vehicle, or separated from its owner.
8. **Thermal is supporting evidence only.** Animals, engines, rocks, fires, and recently occupied objects may produce similar signatures.
9. **Acoustic verification uses prompt-and-pause cycles.** Transmit briefly, then silence output and listen.
10. **Beacon, strobe, and smoke require operator authorization.** The downward spotlight may also be operator-activated during hover inspection before Probable rescuee confirmation. The heliograph mirrors are passive and require no activation.
11. **Smoke deployment uses the strictest authorization.** Prohibit it near fire-prone vegetation, fuel, enclosed spaces, crowds, active fires, poor-air-quality incidents, or unsuitable wind.
12. **Flight safety overrides rescue confidence.** Detection or fusion confidence never overrides airframe limits, icing risk, propulsion risk, link integrity, terrain clearance, or minimum return energy.

---

## 9. Revision Summary

| Area | Previous inconsistency | v1.0 resolution |
|---|---|---|
| **Approach authority** | Scope implied human approval was required before close approach, while mission logic approached automatically | Automatic deterministic approach is now intentional; operator can cancel or take over at any time |
| **Operator alert timing** | Operator was alerted before/around approach in some sections and after inspection in others | Final flow is **candidate → automatic approach → hover → Mamba → alert operator if warranted** |
| **Probable rescuee status** | Both Mamba and the human could appear to assign rescuee status | Mamba only gates alerts; **human alone assigns Probable rescuee** |
| **Mamba operating phase** | Mamba appeared to be general mission fusion | Mamba now runs **only during hover verification** |
| **Mamba alert outputs** | Included an undefined `Urgent alert` class | Simplified to **Reject candidate / Continue verification / Alert operator** |
| **ML flight authority** | Fusion/controller wording could imply ML influenced maneuvering directly | ML never commands motors; deterministic flight logic executes all maneuvers |
| **Human control** | Override authority was described generally but not tied to automatic approach | Explicitly states operator may cancel automatic approach or take manual control at any time |
| **RF upper limit** | 5G/Wi-Fi coverage extended beyond the 6 GHz limit of proposed SDRs | MANAR v1.0 RF scope is explicitly **≤6 GHz** |
| **Wi-Fi scope** | Included 6 GHz Wi-Fi despite ≤6 GHz receiver limit | v1.0 monitors **2.4 and 5 GHz Wi-Fi**; Wi-Fi above 6 GHz is out of scope |
| **RF scan logic** | “Phone uplink only” conflicted with Wi-Fi/Bluetooth detection | Cellular uplink scanning and Wi-Fi/Bluetooth scanning are now separate modes |
| **AD8318 role** | Presented alongside SDRs as though it could perform classification | Reclassified as an **optional RF-power/RSSI detector only** |
| **Endurance target** | “Continuous hovering and active search” mixed incompatible flight states | Revised to **≥1 hour continuous active search** |
| **GNSS vs telemetry** | GNSS was described as if it transmitted telemetry | GNSS determines position; command/data/LTE/satellite links report position |
| **GNSS-degraded behavior** | “IMU/radar positioning” implied unspecified absolute radar localization | Limited to short-term inertial/local-clearance support; no unsupported absolute radar-positioning claim |
| **Control-link loss** | Requirement implied immediate RTH while failsafe table allowed brief continuation | Added transient link-loss contingency followed by RTH after configurable timeout |
| **Continuous telemetry** | Exact telemetry contents, update rate, video independence, and link-loss logging were not explicitly defined | Added minimum telemetry fields, nominal **2–10 Hz** reporting, video-independent telemetry, and continuous onboard logging during total link loss |
| **Terrain map classification** | Terrain map was listed as a physical sensor | Section renamed **Navigation Sensors and Data Sources** |
| **Radar roles** | Obstacle radar and 24 GHz rescue FMCW could be interpreted as the same subsystem | They are explicitly separate unless shared hardware is later validated |
| **Ambient-light switching** | Exact thresholds could cause rapid camera-mode chatter | Added hysteresis and minimum dwell-time requirement |
| **Acoustic operation** | Wide-area listening and 15 s hover listening were conflated | Wide-area uses short passive samples; full 15 s listening occurs during hover verification |
| **Acoustic cycle duration** | “Short cycle” conflicted with three 35.4 s cycles | One **35.4 s** cycle is default; up to three (**106.2 s**) only if needed |
| **Fusion visual inputs** | Pooled visual embeddings were mentioned but absent from the feature vector | Compact 1D RGB/IR/thermal embeddings are explicitly included |
| **Audio/RF fusion inputs** | Classifier outputs did not match the Mamba vector | Replaced with consistent aggregate acoustic/RF scores and direction/bearing confidence |
| **Rating scale** | Numeric 1–5 tables lacked a definition | Restored the full 1–5 rating legend |
| **Downward spotlight** | Listed as hardware but omitted from performance matrices | Added to roles and all performance tables |
| **Smoke during RTH** | Critical-return score implied onboard smoke operation remained useful | Critical-return score changed to **1**; already-deployed ground marker is noted separately |
| **Beacon design** | Rotating vs fixed remained unresolved | Finalized as **fixed 360° amber flashing beacon** |
| **Mirror design** | Fixed, rotating, calculated-tilt, and three-mirror concepts conflicted | Finalized as **three fixed heliograph-style mirrors at 40°, 60°, and 75° from vertical** |
| **Mirror geometry** | Mirror mounting angles were incorrectly treated as reflected-beam angles | Retained the **40° / 60° / 75°** fixed mounting angles, but removed fixed 84 m / 173 m / 373 m coverage claims; actual reflection depends on solar and aircraft geometry |
| **Manual continued inspection** | Human outcome existed in fusion logic but not the mission-state diagram | Added **Manual continued inspection → Remain in hover** |
| **Approach awareness vs rescue alert** | Operator could cancel automatic approach but the UI notification timing was unclear | Added immediate **candidate approach active** mission-status notification; rescue-specific alert still occurs only after Mamba escalation |
| **Spotlight authorization** | Spotlight was scored as a hover-verification tool but also grouped with post-confirmation guidance | Explicitly allowed operator-controlled spotlight use during hover inspection before Probable rescuee confirmation |
| **Mamba/acoustic timing** | Mamba used 2–5 s feature windows while acoustic verification used 15 s listening windows | Mamba now re-evaluates throughout hover and incorporates acoustic evidence when it becomes available |
| **Telemetry during link loss** | “Continuous reporting” could imply transmission during total link loss | Clarified: transmit continuously while a link is available; otherwise log continuously onboard |

## 10. Limitations


### 10.1 Environmental Limitations

MANAR is not intended to guarantee normal search operation in all weather.

| Condition | Main limitation |
|---|---|
| **Heavy rain** | Optical degradation, acoustic masking, wet radar/optical surfaces, and aircraft weather limits |
| **Dense wet snow** | Whiteout, attenuation, icing, accumulation, and propulsion risk |
| **Hail** | Airframe, propeller, sensor-window, and payload damage; normal search should not continue |
| **Dense fog** | Severe RGB/IR degradation; thermal may also lose range |
| **Dust / sand** | Optical backscatter, abrasion, contamination, wind loading, and possible propulsion damage |
| **Smoke** | Optical obstruction, thermal confusion from heat sources, and hazardous operating environment |
| **Strong wind** | Reduced endurance, poor hover stability, acoustic degradation, and navigation risk |

Aircraft safety limits take priority over sensor capability.

### 10.2 Visual Detection Limitations

- RGB performance depends strongly on illumination and visibility.
- Low-light/IR imaging suffers from fog, dust, sand, smoke, and active-IR backscatter.
- Thermal imaging cannot see through solid obstacles.
- Hot terrain, animals, engines, fires, and heated objects can create thermal false positives.
- Clothing, burial, body orientation, and terrain occlusion can reduce person-detection confidence.
- Detection accuracy depends on the representativeness of the training dataset.

### 10.3 FMCW Radar Limitations

- The 24 GHz rescue radar cannot identify a person from range alone.
- Vegetation and debris may create misleading Doppler signatures.
- Drone motion and vibration can contaminate micro-motion measurements.
- Multipath in cluttered environments can create false or ambiguous returns.
- With approximately **250 MHz** sweep bandwidth, nominal range resolution is about **60 cm**.
- Breathing-like micro-motion should be treated as supporting evidence rather than definitive identification.

### 10.4 Passive RF Limitations

MANAR v1.0 passive RF monitoring is limited to **frequencies at or below 6 GHz**.

- Supported Wi-Fi scope is primarily **2.4 GHz and 5 GHz**.
- Wi-Fi operation above 6 GHz is out of scope for v1.0.
- RF detection requires the rescuee to carry an emitting or intermittently transmitting device.
- A detected device may be abandoned, inside a vehicle, or physically separated from its owner.
- Buildings, cliffs, vehicles, and metallic structures can create multipath and bearing errors.
- RF localization is approximate unless supported by measurements from multiple drone positions.
- Passive RF shall not be used as sole confirmation of a rescuee.

### 10.5 Acoustic Limitations

- Propellers, motors, airflow, and structural vibration generate strong self-noise.
- Wind, rain, sand, vegetation, traffic, animals, and machinery can mask human sounds.
- Acoustic direction estimates become unreliable in strong reverberation or multipath.
- A silent, unconscious, injured, or distant rescuee may produce no detectable acoustic response.
- Full acoustic verification is most useful during stable hover rather than forward search flight.

### 10.6 Mamba and Machine-Learning Limitations

- Mamba does not guarantee higher accuracy than specialized per-sensor models.
- Its value is temporal multisensor verification over several seconds of synchronized observations.
- Performance depends on training-data quality and similarity to real deployment conditions.
- Missing or corrupted sensor inputs may reduce fusion reliability.
- Mamba may reject a real rescuee or alert on a false candidate.
- Mamba does not assign **Probable rescuee** status and does not control flight.
- All model performance claims require validation on representative test data before deployment.

### 10.7 Guidance-System Limitations

- The fixed **360° amber beacon** is an attention cue, not a precise directional guide.
- The directional white strobe requires line of sight and is degraded by fog, dust, sand, smoke, snow, and rain.
- The downward spotlight is mainly useful for close-range night inspection.
- The three fixed heliograph-style mirrors at **40°**, **60°**, and **75° from vertical** are mounting-angle choices only; they do not directly define the reflected-beam angle.
- Heliograph reflections depend on solar position, aircraft attitude, heading, and target geometry and cannot guarantee illumination of a specific rescuee.
- Smoke markers depend strongly on wind and visibility and introduce fire, air-quality, and deployment-safety concerns.

### 10.8 Endurance Limitation

The current MANAR v1.0 design target is:

> **At least 1 hour of continuous active search under the defined representative mission profile.**

Actual endurance will vary with:

- payload mass;
- battery capacity;
- aircraft size;
- wind;
- altitude;
- temperature;
- flight speed;
- hover time;
- sensor duty cycles;
- compute load;
- guidance-system use.

The 1-hour value remains a **design target** until validated on the final aircraft configuration.
