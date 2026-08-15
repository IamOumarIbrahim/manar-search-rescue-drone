# MANAR V1 Passive RF Decisions

## 1. Purpose

The MANAR V1 passive RF subsystem is a low-SWaP supporting search sensor.

Its primary purpose is to detect unusual RF activity that may indicate an
electronic device in the surrounding search area.

Examples include emissions associated with:

- cellular devices
- Wi-Fi devices
- other relevant RF-emitting personal electronics within monitored bands

The RF subsystem is not intended to independently identify or confirm a
rescuee.

It acts as an ATTENTION TRIGGER for the rest of the multisensor system.

---

## 2. Selected Hardware Capability

OWNER DECISION:

    Level 1 Passive RF Presence Detection

MANAR V1 does NOT require dedicated RF:

- bearing estimation
- direction finding
- Angle of Arrival
- coherent beamforming
- transmitter triangulation
- transmitter localization hardware

A KrakenSDR / coherent five-antenna AoA architecture is therefore NOT part
of the MANAR V1 baseline.

The hardware goal is:

    one low-SWaP passive RF receiver
    +
    one suitable omnidirectional / multiband antenna

Exact receiver and antenna hardware remain TBD.

---

## 3. Level 1 Detection Role

During search, the RF subsystem monitors selected RF frequency regions.

Its job is conceptually:

    RF environment
        ↓
    received RF power measurements
        ↓
    adaptive background estimation
        ↓
    anomaly detection
        ↓
    RF activity event

An RF activity event means:

    "RF activity significantly different from the currently estimated
    local RF background has been detected."

It does NOT mean:

    "A rescuee has been detected."

RF remains supporting evidence only.

---

## 4. Adaptive Detection

OWNER DECISION:

The RF detector will use an ADAPTIVE threshold rather than a single fixed
RSSI threshold.

Different operating environments may have substantially different RF
background levels.

Therefore MANAR should estimate the current local RF background and detect
signals relative to that background.

The detector should not rely on a universal rule such as:

    RSSI > -70 dBm

for every environment.

---

## 5. CFAR-Based Anomaly Detection

OWNER DECISION:

MANAR V1 will use a deterministic CFAR-style adaptive RF anomaly detector.

CFAR = Constant False Alarm Rate.

The general processing concept is:

    measure RF power
        ↓
    estimate background using surrounding / historical reference samples
        ↓
    calculate adaptive detection threshold
        ↓
    compare current measurement with threshold
        ↓
    generate RF anomaly event if exceeded

This detection system is DETERMINISTIC.

No machine-learning model is required for passive RF anomaly detection.

The RF subsystem should not use an ML model merely to determine whether
received RF energy is anomalous.

Exact CFAR implementation details remain TBD, including:

- CFAR variant
- number of reference cells / samples
- number of guard cells if applicable
- threshold scaling
- target false-alarm probability
- temporal persistence requirements
- frequency-bin resolution
- background update speed

These parameters must be determined through implementation and testing
rather than invented at the architecture stage.

---

## 6. Background Adaptation

The detector must adapt to changing RF environments.

Conceptually:

    Current RF measurement
        ↓
    Compare with estimated local background
        ↓
    If sufficiently abnormal:
        RF anomaly = TRUE
    Otherwise:
        RF anomaly = FALSE

Detected anomalous samples should not immediately cause the adaptive
background estimate to rise until the anomaly disappears.

Otherwise the detector could rapidly absorb a real signal into its own
baseline.

The exact baseline-update algorithm remains TBD.

---

## 7. RF Data Output

The passive RF subsystem should make the following conceptual information
available to MANAR:

- timestamp
- monitored frequency / band
- received RF power / RSSI
- estimated local background
- adaptive detection threshold
- difference relative to background
- RF anomaly / activity flag
- aircraft latitude
- aircraft longitude

Exact message structure and data types remain an implementation decision.

---

## 8. Level 2-Style Software Capability

OWNER DECISION:

Although MANAR V1 uses Level 1 RF HARDWARE, its RF measurements should be
stored with aircraft position.

This enables optional Level 2-style spatial RSSI analysis using the same
hardware.

Example:

    Position A → weak
    Position B → stronger
    Position C → strongest
    Position D → weaker

Software can therefore infer:

    "RF activity was strongest around Position C."

This is equivalent to a simple GPS-tagged "hot/cold" RF map.

It does NOT require:

- directional antennas
- a second receiver
- coherent receivers
- phase measurements
- AoA hardware

This software capability should remain OPTIONAL.

The primary V1 RF requirement remains Level 1 presence detection.

Do not redesign the aircraft flight path solely for RF localization unless
future testing demonstrates that this is necessary.

---

## 9. Interaction With MANAR Search Behavior

Conceptually, an RF anomaly may cause MANAR to increase attention in the
area.

Example high-level behavior:

    Normal Search
        ↓
    RF anomaly detected
        ↓
    Area marked as interesting
        ↓
    MANAR may slow / enter INSPECT / hover
        ↓
    Thermal / RGB / FMCW radar / audio investigate
        ↓
    Multisensor evidence determines whether a rescuee candidate exists

This document does NOT freeze the exact control-state transition.

Exact rules for:

- slowing down
- entering INSPECT
- hover duration
- search radius
- sensor escalation
- number of RF detections required

remain implementation / behavior decisions.

---

## 10. RF Attention Escalation Decision

OWNER DECISION:

MANAR will use a deterministic two-stage RF attention system after CFAR threshold detection.

*(Note: Do not confuse **RF Capability Level 1 / Level 2** hardware/software capability levels with **Attention Level 1 / Attention Level 2** behavioral escalation states. They are completely different concepts.)*

### Attention Level 1 — Initial RF Attention

When one or a small number of RF samples exceed the current adaptive CFAR threshold:

- trigger `RF Attention Level 1`
- begin/count consecutive or qualifying above-threshold samples
- retain the RF measurement history
- do NOT yet assume that a meaningful target exists
- do NOT immediately perform full RF spatial tracking

The exact number/persistence rules for initially entering Attention Level 1 remain TBD unless already defined elsewhere.

### Attention Level 2 — Sustained RF Attention

While Attention Level 1 is active, MANAR counts qualifying samples that remain above the adaptive threshold.

If the count reaches a specific hard-coded persistence requirement:

- escalate to `RF Attention Level 2`
- trigger the appropriate flight-mode / inspection behavior
- begin using stored position-tagged RSSI history to determine whether RF strength is generally increasing or decreasing as the aircraft moves

Conceptually:

    increasing RF strength over movement
        → aircraft is likely moving toward the RF hotspot

    decreasing RF strength over movement
        → aircraft is likely moving away from the RF hotspot

This is the optional Level 2-style SOFTWARE capability previously approved.

It still uses the same Level 1 passive RF HARDWARE.

Do NOT describe this as:
- AoA
- bearing estimation
- triangulation
- true RF localization
- Level 2 RF hardware

It is only movement-correlated RSSI history / "hot and cold" spatial reasoning.

### Important Constraints

- The system remains deterministic.
- No machine learning is required.
- CFAR remains responsible for adaptive anomaly thresholding.
- Attention Level 1/2 is responsible for persistence and behavioral escalation AFTER CFAR detection.
- Do not invent the hard-coded sample-count value yet.
- Do not invent exact timing windows yet.
- Do not invent exact flight-mode transitions if they are not already frozen.
- Preserve all previously settled RF decisions.

---

## 11. Relationship With FMCW Radar

Passive RF and FMCW radar have different roles.

Passive RF:

    Detect abnormal RF emissions.

FMCW radar:

    Provide radar-derived spatial / range / motion evidence.

Therefore MANAR does NOT require the passive RF hardware itself to provide
direction or localization.

RF may simply draw attention to an area, after which other sensors can
provide stronger localization and confirmation evidence.

---

## 12. Software and Compute Impact

CFAR processing and optional GPS-tagged RSSI mapping are expected to be
computationally lightweight compared with MANAR's main perception workload.

No additional dedicated compute module is currently required.

For engineering budgeting:

    Additional hardware mass:
        0 g beyond the Level 1 RF receiver/antenna

    Incremental compute power:
        TBD through measurement

Do NOT treat previous rough estimates of approximately 0.5–1.0 W as
verified hardware requirements.

They are only rough engineering expectations until measured on the actual
compute platform.

---

## 13. Decisions Closed

The following decisions are CLOSED for MANAR V1:

- Passive RF is a supporting / attention-trigger sensor.
- Level 1 passive RF hardware is selected.
- RF direction finding is not required.
- KrakenSDR Level 3 AoA is not part of the V1 baseline.
- Position-tagged RSSI measurements will be retained.
- Optional Level 2-style software spatial mapping is permitted.
- RF anomaly detection will use an adaptive deterministic method.
- CFAR-style detection is selected as the detection approach.
- Machine learning is not required for RF anomaly detection.
- Deterministic two-stage RF attention escalation (Attention Level 1 & Attention Level 2) is selected for post-CFAR behavioral escalation.
- Movement-correlated RSSI spatial reasoning ("hot/cold") is used during Attention Level 2 using Level 1 hardware.

---

## 14. Decisions Still Open

The following remain TBD:

- exact SDR / RF receiver
- exact antenna
- exact monitored frequency ranges
- exact scan strategy
- exact receiver bandwidth requirements
- RF front-end filtering requirements
- CFAR variant
- CFAR parameters
- adaptive baseline update algorithm
- calibration procedure
- anomaly persistence requirements / hard-coded sample count to trigger Attention Level 2
- exact timing windows for Attention Level 1/2 sample decay
- acceptable false-alarm rate
- real-world RF detection range
- measured RF subsystem power
- environmental test methodology

Do not resolve these items without explicit engineering analysis or owner
decision.
