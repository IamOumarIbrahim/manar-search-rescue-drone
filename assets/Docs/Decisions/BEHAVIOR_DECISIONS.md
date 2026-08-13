> LAST UPDATED: 13-08-2026 8:18PM (UTC+4)

# CLOSED

## 1. Project Definition

* MANAR is a supervised-autonomy UAV system for search and rescue.
* MANAR V1 operates one mission at a time.
* The deterministic C++ control system remains the authority over mission, flight, component, navigation, and runtime state.

---

## 2. Software Stack

* Operator GUI:

  > TypeScript + React
* Communication:

  > WebSocket + JSON
* Deterministic control:

  > C++
* Runtime state:

  > `runtime.json`
* Persistent configuration:

  > `config.json`
* Machine learning:

  > Python + PyTorch later in development

> TypeScript/React handles presentation and operator interaction, WebSocket handles communication, and C++ owns control logic and authoritative runtime state.

---

## 3. Operator → Control Commands

The operator interface may send commands to configure:

### Mission

> Start / Abort

### Flight

> Speed / Altitude / Mode / Launch / Land

### Components

> ON / OFF each controllable component

### Navigation

> Set/change destination coordinates

### Rescuee

> Found / Not Found

### RTH

> Trigger Return-To-Home

Every command must receive an explicit success or rejection result.

Commands must never be silently consumed.

---

## 4. Control → Operator Information

The control system provides:

### Mission

> Started / Ongoing / Aborted / relevant mission state

### Flight

> Speed / Altitude / Mode / Status

### Components

> ON / OFF / health status

### Battery

> Current battery / estimated time until CRITICAL / power-saving status

### Navigation

> Current GPS position / destination / waypoints / ETA / estimated battery at arrival

### Detection

> Sensor detections / confidence / rescuee state / important alerts

---

## 5. Runtime State Ownership

* `control.cpp` owns the canonical runtime state.
* `runtime.json` represents a complete authoritative snapshot of control-owned state.
* External interfaces must not observe partially applied state transitions.
* Subsystems may update internal variables or return results to control.
* Subsystems do not independently persist canonical runtime state.
* Operator interfaces read state but do not directly modify runtime state.
* Future GUI and ML layers must follow the same behavioral contract rather than defining their own mission semantics.

---

## 6. Operator Information Model

Operator information is divided into:

1. continuously displayed telemetry,
2. continuously maintained but optionally displayed state,
3. on-demand detailed state.

> Sensors operate continuously, control maintains state continuously, and the operator interface decides what information is displayed continuously.

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
* Rescuee status
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

### On-demand

* Full component status
* Full mission state
* Diagnostics
* Configuration
* Detailed battery information
* RF history
* Logs

---

## 8. Mission Lifecycle

* `START_MISSION` is the first mission-lifecycle action.
* Mission start does not require the aircraft to already be launched.

```text
START_MISSION
    ↓
Mission active
    ↓
LAUNCH_DRONE
    ↓
Flight begins
```

* A mission may remain active while the aircraft is on the ground.
* Starting a new mission clears transient state from the previous mission.
* A separate `RESET_MISSION` command is not required for V1.
* Previous aborted, finished, returning, rescuee-found, waiting-for-help and search-progress state must not carry into a new mission.

---

## 9. Flight Behavior

### STOP_FLIGHT

`STOP_FLIGHT` means safely decelerate and land.

It does not automatically abort the mission.

After completion:

```text
flight.launched = false
flight.speed = 0
flight.altitude = 0
flight.mode = "Stall"
mission.enroute = false
```

### Relaunch

* A landed aircraft may relaunch during an existing mission.
* Relaunch does not reset mission state or destination.

### Flight mode and speed

* Flight modes provide speed presets.
* Manual speed may override the current mode's preset.
* Manual speed does not change the selected flight mode.
* Manual speed must remain within configured safety limits.

```text
0 <= flight.speed <= config.maximum_speed
```

### Destination changes

* Changing destination does not implicitly launch the aircraft.
* Destination may be changed while waiting for help.

---

## 10. RTH Behavior

* `RTH` sets the current destination to configured home coordinates.
* RTH is a navigation state, not an irreversible destination lock.

While returning:

```text
mission.returning = true
destination = home
```

If an accepted `CHANGE_DEST` changes the destination away from home:

```text
mission.returning = false
destination = new destination
```

Therefore:

> `returning == true` always means the current destination is home.

---

## 11. Mission Abort

`ABORT_MISSION` terminates the current mission objective and initiates RTH.

```text
mission.aborted = true
mission.enroute = false
destination = home
mission.returning = true
```

If airborne, the aircraft begins or continues return flight.

---

## 12. Waiting for Help

* `mission.waitingforhelp == true` represents a safely landed state unless explicitly changed later.
* Destination may be changed while waiting for help.
* Changing destination does not itself launch the aircraft or begin movement.

---

## 13. Passive RF Role

* Passive RF is a supporting search sensor and attention trigger.
* Passive RF does not independently identify or confirm a rescuee.
* Other MANAR sensors provide stronger localization and confirmation evidence after RF draws attention to an area.

---

## 14. Passive RF Hardware

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

## 15. Passive RF Detection

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

## 16. RF Attention Escalation

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

## 17. RF Position History

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

## 18. Passive RF and FMCW Relationship

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
* Exact runtime fields displayed continuously.
* RF/FMCW visualization design.
* Amount of sensor history exposed to the GUI.

---

## 2. Command Architecture

* Final WebSocket message schemas.
* Command ID / acknowledgement structure.
* Duplicate-command protection.
* Command timeout behavior.
* Connection-loss behavior.
* Reconnection and state resynchronization behavior.

---

## 3. Mission and Flight

* Exact complete mission-state machine.
* Exact altitude semantics where still required.
* Exact safety bounds for operator-controlled flight values.
* Exact behavior when destinations are unreachable.
* Exact behavior when navigation data becomes unavailable.

---

## 4. Abort While Landed

If `ABORT_MISSION` occurs while the aircraft is already landed:

1. automatically relaunch and execute RTH, or
2. enter RTH state and wait for explicit `LAUNCH_DRONE`.

---

## 5. Component Restrictions

Determine whether components may be enabled or disabled in every system state.

Examples:

* Smoke marker while grounded.
* Cameras during active search.
* Spotlight while waiting for help.
* Sensors during RTH.

No restriction should be invented until required by MANAR's intended behavior.

---

## 6. Component Authorization

Determine which component actions require explicit operator authorization rather than a normal toggle.

Potential examples:

* Smoke marker
* High-intensity spotlight
* Amber beacon
* White strobe

Authorization and state restriction remain separate concepts.

---

## 7. Passive RF Hardware

* Exact SDR / RF receiver.
* Exact antenna.
* Monitored frequency ranges.
* Scan strategy.
* Receiver bandwidth.
* RF front-end filtering.

---

## 8. Passive RF Detection

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

## 9. RF Attention Behavior

* Sample persistence required to enter Attention Level 1.
* Hard-coded persistence requirement for Attention Level 2.
* Attention Level 1/2 timing and decay windows.
* Exact flight behavior caused by Attention Level 2.
* Inspection duration.
* Search radius.
* Sensor escalation behavior.
* Acceptable RF false-alarm rate.

---

## 10. RF Validation

* Real-world detection range.
* Measured RF subsystem power.
* Environmental test methodology.
* Actual compute cost of CFAR and spatial RSSI processing.
