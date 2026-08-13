# MANAR System Decisions

> LAST UPDATED: 13-08-2026 9:38PM (UTC+4)  
> This version supersedes earlier decisions where they conflict with `NEXT_STEPS_HUMAN.md`.

# CLOSED

## 1. Project Definition

* MANAR is a supervised-autonomy UAV system for search and rescue.
* MANAR V1 operates one active mission at a time.
* The deterministic C++ control system remains the authority over mission, flight, component, navigation, rescuee, configuration-selection, and runtime state.
* Mission-critical control remains under supervised operator control.

---

## 2. Software Stack

* Operator GUI:

  > TypeScript + React

* Communication:

  > WebSocket + JSON

* Deterministic control:

  > C++

* On-demand detailed state snapshot:

  > `runtime.json`

* Persistent configuration:

  > `config.json`

* Machine learning:

  > Python + PyTorch later in development

> TypeScript/React handles presentation and operator interaction, WebSocket handles communication, and C++ owns authoritative control logic and state.

---

## 3. Operator → Control Commands

The operator interface may send commands to configure or request:

### Mission

> Start mission

`ABORT_MISSION` is not part of MANAR V1.

### Flight

> Mode / Altitude / Launch / Land

Manual speed alteration is not exposed as an operator command.

### Components

> ON / OFF each controllable component, subject to any later-defined authorization or state restrictions

### Navigation

> Set/change destination coordinates  
> Set/search a list of mission locations  
> Trigger RTL

### Rescuees

> Create/update multiple rescuee records  
> Assign independent status/state information to each rescuee

### Configuration

> Select a saved configuration profile for the mission

Every command must receive an explicit success or rejection result.

Commands must never be silently consumed.

---

## 4. Control → Operator Information

The control system provides:

### Mission

> Mission state / search progress / selected configuration

### Flight

> Speed / Altitude / Mode / Status

### Components

> ON / OFF / health status

### Battery

> Current battery / current power usage / estimated time until CRITICAL / power-saving status

### Navigation

> Current GPS position / current destination / search-location list / waypoints / ETA / estimated battery at arrival

### Detection

> Sensor detections / confidence / rescuee records / important alerts

---

## 5. Runtime State Ownership

* `control.cpp` owns canonical runtime state.
* Subsystems may update their own internal variables or return results to control.
* Subsystems do not independently persist or overwrite canonical runtime state.
* Operator interfaces do not directly modify control-owned state.
* External interfaces must not observe partially applied state transitions.
* Future GUI and ML layers must follow the same behavioral contract rather than defining their own mission semantics.

### `runtime.json`

`runtime.json` is no longer defined as the continuously updated authoritative runtime-state transport.

Instead, it is an **on-demand detailed snapshot/export** generated from control-owned state.

The snapshot may include:

* Full component status
* Full mission state
* Diagnostics
* Active configuration
* Detailed battery information
* RF history
* Logs

> The authority is the C++ runtime state itself. `runtime.json` is a representation of that state when a detailed snapshot is requested.

High-rate telemetry and high-bandwidth streams are transported separately.

---

## 6. Operator Information Model

Operator information is divided into:

1. continuously displayed telemetry,
2. continuously maintained but optionally displayed state,
3. on-demand detailed state.

> Sensors and control may operate continuously without continuously rewriting every piece of state into `runtime.json`.

High-bandwidth data such as camera feeds is transported separately from `runtime.json`.

---

## 7. GUI Information Division

### Always on display

**MISSION**

* Mission state
* Flight mode
* Altitude
* Speed
* Battery
* Location
* Destination
* ETA
* Link status

**DETECTION**

* Passive RF indicator
* FMCW detection indicator
* Detection state / confidence
* Rescuee summary/status
* Important alerts

**IMAGING**

* RGB feed
* Thermal feed
* Low-light / IR feed

### Live but not necessarily on main display

* FMCW detailed values
* RF history graph
* Component health
* GNSS accuracy
* Radar altitude
* Microphone activity
* Current waypoint details
* Power consumption
* Sensor temperatures
* Mission search-location progress

### On-demand snapshot

* Full component status
* Full mission state
* Diagnostics
* Configuration
* Detailed battery information
* RF history
* Logs

---

## 8. Mission Lifecycle

`START_MISSION` is the first mission-lifecycle action.

Mission start does **not** require the aircraft to already be launched.

```text
START_MISSION
    ↓
Mission active
    ↓
Configure / inspect / set destination if required
    ↓
LAUNCH_DRONE
    ↓
Flight begins
```

* A mission may remain active while the aircraft is on the ground.
* Components, destination, search locations, and other mission configuration may be changed without the aircraft being launched.
* Starting a new mission clears transient state from the previous mission.
* A separate `RESET_MISSION` command is not required for V1.
* Old destination state, rescuee state/status, search progress, and other transient mission information must not carry into a new mission.
* Mission start restores/applies the configuration profile selected for that mission.
* Protection must prevent stale transient state from contaminating a newly started mission.

`ABORT_MISSION` is removed from MANAR V1.

---

## 9. Flight Behavior

### STOP_FLIGHT / LAND

Landing the aircraft does not automatically terminate the active mission.

After landing:

```text
flight.launched = false
flight.speed = 0
flight.altitude = 0
flight.mode = "Stall"
mission.enroute = false
```

### Relaunch

* A landed aircraft may relaunch during an existing mission.
* Relaunch does not reset mission state, rescuee records, search locations, or destination.

### Flight mode and speed

* Flight modes define speed presets.
* Mode speeds are configurable.
* The operator selects a flight mode rather than directly altering flight speed.
* Manual speed override is removed from MANAR V1.
* Any final safety limits remain enforced by control.

### Destination changes

* Changing destination does not implicitly launch the aircraft.
* A destination may be configured while the aircraft is landed.
* Destination may be changed during an active mission where allowed by control.

---

## 10. RTL Behavior

The former `RTH` terminology is replaced by `RTL` — **Return to Launch**.

`RTL` is an **on-demand navigation command**.

When accepted:

```text
destination = configured_launch_location
```

RTL:

* defines/changes the destination to the configured launch location,
* does not automatically launch the aircraft,
* is not a persistent runtime operation,
* does not create a permanent `returning` mission mode,
* does not lock the destination,
* may be followed by another accepted destination command.

> RTL describes the requested destination action, not a long-lived mission state.

---

## 11. Rescuee Model

MANAR V1 is not limited to one rescuee.

* A mission may contain multiple rescuee records.
* Each rescuee may have its own independent state/status.
* Updating one rescuee must not overwrite another rescuee.
* Rescuee information is mission-scoped transient state and is cleared when a new mission begins.
* The operator may update rescuee-specific status information.

The exact rescuee record schema and final allowed status values remain open.

---

## 12. Search Location Model

The operator may configure a **list of locations to be searched**.

* Search locations are stored as a collection rather than a single mission destination.
* Search-location state belongs to the current mission.
* Starting a new mission clears previous transient search-location progress.
* The current flight destination and the overall list of locations to search are separate concepts.

The exact location schema, ordering rules, completion tracking, and route-generation behavior remain open.

---

## 13. Configuration Profiles

MANAR supports multiple saved configuration states/profiles for different operating environments.

Profiles may represent combinations such as:

* Day / night
* Dry / rainy / snowy / hail
* Sandstorm / thunderstorm / snowstorm / fog

A mission selects a configuration profile.

When a mission starts, the selected configuration is applied/reset as the mission's active configuration.

The exact profile schema, inheritance behavior, validation rules, and allowed operator overrides remain open.

---

## 14. Battery Model

MANAR includes a configurable battery-saving mode.

The control system should expose:

* Current battery state
* Current power usage
* Estimated time until `CRITICAL`
* Whether battery-saving mode is active

The time-until-`CRITICAL` estimate is based on current power-usage statistics rather than battery percentage alone.

The exact prediction algorithm, thresholds, and power-saving actions remain open.

---

## 15. Passive RF Role

* Passive RF is a supporting search sensor and attention trigger.
* Passive RF does not independently identify or confirm a rescuee.
* Other MANAR sensors provide stronger localization and confirmation evidence after RF draws attention to an area.

---

## 16. Passive RF Hardware

MANAR V1 uses Level 1 passive RF hardware:

```text
One low-SWaP passive RF receiver
+
One suitable omnidirectional / multiband antenna
```

V1 does not require:

* RF direction finding
* Bearing estimation
* Angle of Arrival
* Coherent beamforming
* Transmitter triangulation
* Dedicated RF localization hardware
* KrakenSDR / coherent five-antenna AoA architecture

---

## 17. Passive RF Detection

* RF anomaly detection is deterministic.
* MANAR uses adaptive detection rather than one universal RSSI threshold.
* CFAR-style detection is the selected adaptive anomaly-detection approach.
* Machine learning is not required for passive RF anomaly detection.

Conceptually:

```text
RF power measurement
    ↓
Estimate local background
    ↓
Calculate adaptive threshold
    ↓
Compare current measurement
    ↓
Generate RF anomaly if exceeded
```

An RF anomaly means:

> RF activity significantly different from the estimated local RF background has been detected.

It does not mean:

> A rescuee has been detected.

---

## 18. RF Attention Escalation

MANAR uses a deterministic two-stage attention system after CFAR detection.

### Attention Level 1

Initial or limited above-threshold RF activity:

* Trigger RF Attention Level 1.
* Begin tracking qualifying above-threshold samples.
* Retain RF measurement history.
* Do not assume a meaningful target exists yet.
* Do not immediately perform full spatial RSSI reasoning.

### Attention Level 2

After sustained qualifying RF activity:

* Trigger RF Attention Level 2.
* Trigger appropriate inspection behavior.
* Use position-tagged RSSI history.
* Determine whether signal strength is generally increasing or decreasing as MANAR moves.

Conceptually:

```text
Increasing RSSI
    → moving toward RF hotspot

Decreasing RSSI
    → moving away from RF hotspot
```

This is movement-correlated RSSI reasoning, not:

* AoA
* Bearing estimation
* Triangulation
* True RF localization

---

## 19. RF Position History

RF measurements may retain:

* Timestamp
* Frequency / band
* Received RF power / RSSI
* Estimated local background
* Adaptive threshold
* Difference from background
* RF anomaly flag
* Aircraft latitude
* Aircraft longitude

This allows optional GPS-tagged "hot/cold" spatial RF analysis using the same Level 1 hardware.

The aircraft flight path will not be redesigned solely for RF localization unless future testing justifies it.

---

## 20. Passive RF and FMCW Relationship

Passive RF and FMCW have separate roles.

**Passive RF**

> Detect abnormal RF emissions and attract system attention.

**FMCW**

> Provide radar-derived range, spatial, motion and micro-motion evidence.

Passive RF therefore does not need dedicated direction or localization capability.

---

# OPEN

## 1. Communication and GUI

* Live telemetry update rate.
* Camera streaming mechanism.
* Exact continuous telemetry/state transport structure.
* RF/FMCW visualization design.
* Amount of sensor history exposed to the GUI.
* Exact request/response mechanism for on-demand `runtime.json` snapshots.

---

## 2. Command Architecture

* Final WebSocket message schemas.
* Command ID / acknowledgement structure.
* Duplicate-command protection.
* Command timeout behavior.
* Connection-loss behavior.
* Reconnection and state resynchronization behavior.
* Exact command used to request/export detailed runtime snapshots.

---

## 3. Mission Lifecycle

* Exact complete mission-state machine.
* Exact definition of mission completion/termination now that `ABORT_MISSION` is removed.
* Exact mission-start stale-state protection.
* Exact list of transient fields cleared by `START_MISSION`.
* Exact behavior if mission initialization/configuration application fails.

---

## 4. Flight and Navigation

* Exact altitude semantics where still required.
* Exact safety bounds for operator-controlled flight values.
* Exact configured speed for each flight mode.
* Exact behavior when destinations are unreachable.
* Exact behavior when navigation data becomes unavailable.
* Exact RTL behavior if no valid launch location exists.
* Exact route behavior after the RTL destination is set.

---

## 5. Rescuee Model

* Rescuee identifier format.
* Exact rescuee states/status values.
* Rescuee location representation.
* Whether rescuee history is retained within a mission.
* Rules for merging repeated detections into an existing rescuee versus creating a new rescuee.
* Exact operator controls for rescuee-specific state changes.

---

## 6. Search Locations

* Search-location data structure.
* Maximum number of configured locations.
* Ordering/prioritization.
* Visited / unvisited / skipped state model.
* How search locations become navigation waypoints or search regions.
* Exact behavior when the operator edits the list during a mission.

---

## 7. Configuration Profiles

* Exact `config.json` profile schema.
* Profile naming and identifiers.
* Required/default profiles.
* Exact environmental parameters controlled by each profile.
* Validation and fallback behavior.
* Whether profile settings may be changed during an active mission.
* How selected-profile reset/protection works at mission start.

---

## 8. Battery and Power-Saving Mode

* Definition of `CRITICAL`.
* Battery-saving entry/exit conditions.
* Which operations/components are reduced or disabled in battery-saving mode.
* Which battery-saving behaviors are configurable.
* Exact time-until-`CRITICAL` estimation algorithm.
* Smoothing/windowing of power-usage statistics.
* Behavior when estimated remaining time becomes unreliable.

---

## 9. Component Restrictions

Determine whether components may be enabled or disabled in every system state.

Examples:

* Smoke marker while grounded.
* Cameras during active search.
* Spotlight while waiting for help.
* Sensors during RTL-directed navigation.

No restriction should be invented until required by MANAR's intended behavior.

---

## 10. Component Authorization

Determine which component actions require explicit operator authorization rather than a normal toggle.

Potential examples:

* Smoke marker
* High-intensity spotlight
* Amber beacon
* White strobe

Authorization and state restriction remain separate concepts.

---

## 11. Passive RF Hardware

* Exact SDR / RF receiver.
* Exact antenna.
* Monitored frequency ranges.
* Scan strategy.
* Receiver bandwidth.
* RF front-end filtering.

---

## 12. Passive RF Detection

* CFAR variant.
* Reference sample/cell count.
* Guard samples/cells.
* Threshold scaling.
* Target false-alarm probability.
* Frequency resolution.
* Background-update algorithm.
* Calibration procedure.
* Environmental adaptation behavior.

---

## 13. RF Attention Behavior

* Sample persistence required to enter Attention Level 1.
* Hard-coded persistence requirement for Attention Level 2.
* Attention Level 1/2 timing and decay windows.
* Exact flight behavior caused by Attention Level 2.
* Inspection duration.
* Search radius.
* Sensor escalation behavior.
* Acceptable RF false-alarm rate.

---

## 14. RF Validation

* Real-world detection range.
* Measured RF subsystem power.
* Environmental test methodology.
* Actual compute cost of CFAR and spatial RSSI processing.
