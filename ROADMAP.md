# MANAR Development Roadmap

This roadmap tracks the major development stages for MANAR in dependency order.

The priority is to stabilize the deterministic C++ control core first, then make the simulation meaningful, define the external interface, restore the operator GUI, and finally integrate machine learning and supervised autonomy.

This file is for **major milestones**, not day-to-day cleanup. Smaller refactors, bugs, and implementation details should be tracked separately through GitHub Issues or a `TODO.md`.

---

## 1. Runtime & State Ownership

**Goal:** Establish one authoritative owner for MANAR runtime state.

### Tasks
- [ ] Make the control process the sole authoritative owner of runtime state.
- [ ] Keep terminal and future GUI code responsible only for sending requests and displaying state.
- [ ] Remove direct `runtime` mutation and `saveRuntime()` calls from mission, flight, drone, and component methods.
- [ ] Let subsystem methods update their own internal state and report success, failure, or resulting values back to control.
- [ ] Let control apply the final state transition.
- [ ] Build one complete runtime snapshot from the authoritative in-memory state.
- [ ] Initialize runtime using a complete known schema instead of starting from `{}`.
- [ ] Ensure only control writes `runtime.json`.

**Done when:** Every runtime state change follows a clear request → control → state path, subsystem methods no longer independently persist runtime state, and `runtime.json` always represents a complete authoritative snapshot.

---

## 2. Reliable Command Lifecycle

**Goal:** Make every operator command traceable from request to final result.

### Tasks
- [ ] Give every command a unique command ID.
- [ ] Validate a command completely before executing it.
- [ ] Reject unknown commands, invalid values, and unsupported component requests explicitly.
- [ ] Distinguish command states such as `SUCCESS` and `REJECTED`.
- [ ] Never mark a rejected or ignored command as successfully executed.
- [ ] Allow only one outstanding request while file-based IPC remains in use.
- [ ] Do not allow the next request to replace the current one until control acknowledges it.
- [ ] Persist or safely restore the authoritative last-processed command state across control restarts.
- [ ] Expose the most recent command ID and result in runtime state.
- [ ] Prevent the same command from being executed twice after restart or partial failure.

**Done when:** Every submitted command either executes once or is explicitly rejected, its result can be observed, and no command silently disappears or falsely reports success.

---

## 3. JSON & File Reliability

**Goal:** Make MANAR's file-based state and command transport safe enough for continued prototype development.

### Tasks
- [ ] Load a complete runtime schema at startup.
- [ ] Handle missing JSON files without uncontrolled crashes.
- [ ] Handle malformed or partially written JSON safely.
- [ ] Validate expected fields and field types before use.
- [ ] Make runtime reads resilient to write collisions.
- [ ] Write JSON through a temporary file and replace the destination atomically.
- [ ] Prevent terminal and control from truncating or corrupting the same log file.
- [ ] Establish stable file paths instead of relying blindly on the current working directory.
- [ ] Add a control heartbeat timestamp to runtime state.
- [ ] Log clear errors when file or JSON operations fail.

**Done when:** Invalid or partial files are handled safely, runtime snapshots cannot normally be observed halfway through a write, logs are not corrupted by competing processes, and consumers can distinguish a live control process from a stale runtime file.

---

## 4. Deterministic Drone & Mission State

**Goal:** Make flight, mission, navigation, and component state internally consistent.

### Tasks
- [ ] Define the authoritative mission state and transition rules.
- [ ] Define authoritative flight mode, speed, altitude, position, heading, and destination state.
- [ ] Define launch, stop, abort, RTH, rescuee-found, mission-complete, and new-mission behavior.
- [ ] Ensure `STOP_FLIGHT` cannot leave contradictory mission state such as active enroute flight.
- [ ] Ensure reaching home clears or updates returning state correctly.
- [ ] Reset required mission flags when a new mission starts.
- [ ] Remove duplicated or conflicting RTH transition paths.
- [ ] Keep rescuee-found and waiting-for-help transitions consistent.
- [ ] Keep component status consistent with the canonical runtime state.
- [ ] Preserve the current simple priority-based mission updater unless a real limitation requires changing it.

**Done when:** Given the same starting state and command sequence, MANAR reaches the same valid resulting state every time without contradictory flight or mission flags.

---

## 5. Movement Simulation

**Goal:** Make the simulated drone move through space over time instead of only changing destinations.

### Tasks
- [ ] Progress latitude and longitude toward the active destination over time.
- [ ] Use current flight speed when calculating movement.
- [ ] Progress altitude toward commanded altitude instead of changing only as static state.
- [ ] Update distance-to-destination while moving.
- [ ] Update distance-to-home while moving.
- [ ] Detect when the active destination has actually been reached.
- [ ] Make RTH use the same movement system.
- [ ] Keep the first movement model simple and deterministic rather than building a high-fidelity physics engine.

**Done when:** A launched simulated drone moves continuously from its current position to a destination according to its current speed and reaches that destination without manual state manipulation.

---

## 6. Dynamic Battery & Safety Behavior

**Goal:** Make battery state evolve during missions and connect existing safety thresholds to real behavior.

### Tasks
- [ ] Decrease battery during active operation.
- [ ] Start with a simple predictable drain model.
- [ ] Call the battery safety logic during normal control execution.
- [ ] Trigger battery warnings at configured thresholds.
- [ ] Trigger RTH at the configured RTH threshold.
- [ ] Ensure emergency battery conditions produce a real state transition rather than only setting an unused flag.
- [ ] Verify emergency behavior remains consistent with mission and flight state.
- [ ] Add flight-mode or payload-dependent power consumption later only if useful.

**Done when:** Battery percentage changes during a simulated mission and naturally triggers the expected warning, RTH, and emergency behaviors.

---

## 7. Search & Mission Execution

**Goal:** Turn the existing deterministic navigation logic into a complete executable simulated mission.

### Tasks
- [ ] Execute lawnmower search waypoints sequentially.
- [ ] Advance to the next waypoint only after the current waypoint is reached.
- [ ] Connect mission-state changes to actual movement behavior.
- [ ] Support interruption by abort and RTH commands.
- [ ] Preserve required human authorization gates.
- [ ] Handle rescuee-found behavior consistently during an active search.
- [ ] Handle mission completion and return-to-home cleanly.
- [ ] Test missions with different search boundaries, flight modes, and interruption points.

**Done when:** MANAR can launch, fly a generated search pattern, respond to operator commands during the mission, complete or abort the mission, and return home using the same deterministic state system.

---

## 8. External Interface / Transport

**Goal:** Define a reliable boundary between the C++ control core and future external clients.

### Tasks
- [ ] Evaluate the remaining limitations of file-based JSON IPC after the core is stable.
- [ ] Define the command contract external clients must send.
- [ ] Define the telemetry/runtime contract external clients receive.
- [ ] Decide whether MANAR continues with file polling or moves to HTTP, WebSocket, sockets, or another local transport.
- [ ] Separate transport behavior from MANAR control decisions.
- [ ] Ensure the chosen interface supports command acknowledgment and control liveness.
- [ ] Keep the interface usable by both the future operator GUI and Python perception process.
- [ ] Avoid introducing transport frameworks before the interface requirements are clear.

**Done when:** An external program can reliably send MANAR commands and receive current state through a documented interface without directly modifying control internals.

---

## 9. Operator GUI

**Goal:** Restore a functional operator interface after the deterministic core and transport boundary are stable.

> The previous GUI scaffold was intentionally reverted. GUI development resumes here rather than being treated as currently implemented functionality.

### Tasks
- [ ] Create the operator GUI structure.
- [ ] Keep the GUI's knowledge of C++ behind one interface/API layer.
- [ ] Display mission state, flight state, battery, location, destination, and payload status.
- [ ] Display control connection/liveness state.
- [ ] Add operator controls for supported deterministic commands.
- [ ] Display command acceptance, success, and rejection feedback.
- [ ] Add map and route visualization when live movement telemetry is ready.
- [ ] Keep mission and flight decisions inside the C++ control core rather than duplicating them in TypeScript.

**Done when:** An operator can observe MANAR state, determine whether control is online, and issue all supported core commands through the GUI without relying on the terminal interface.

---

## 10. ML & Supervised Autonomy

**Goal:** Integrate machine-learning perception only after the deterministic system has a stable place for detection events to enter.

> The previous ML scaffold was intentionally reverted. ML work resumes after deterministic control, simulation, and external interface contracts are established.

### Tasks
- [ ] Define the structured perception event format expected by the C++ core.
- [ ] Define how RGB, low-light/IR, thermal, radar, audio, and RF evidence will eventually enter the perception pipeline.
- [ ] Build the Python/PyTorch perception layer incrementally.
- [ ] Send structured candidate detections to the deterministic core.
- [ ] Connect perception events to deterministic mission-state transitions.
- [ ] Preserve required human authorization before high-impact actions.
- [ ] Integrate candidate verification and rescuee confirmation logic.
- [ ] Keep ML responsible for perception rather than direct flight control.
- [ ] Test the complete supervised-autonomy mission loop.

**Done when:** A structured perception event can enter the C++ core, influence mission behavior through deterministic rules, request operator authorization where required, and complete an end-to-end supervised search-and-rescue simulation.

---

# Completion Overview

- [ ] **Milestone 1:** Runtime & State Ownership
- [ ] **Milestone 2:** Reliable Command Lifecycle
- [ ] **Milestone 3:** JSON & File Reliability
- [ ] **Milestone 4:** Deterministic Drone & Mission State
- [ ] **Milestone 5:** Movement Simulation
- [ ] **Milestone 6:** Dynamic Battery & Safety Behavior
- [ ] **Milestone 7:** Search & Mission Execution
- [ ] **Milestone 8:** External Interface / Transport
- [ ] **Milestone 9:** Operator GUI
- [ ] **Milestone 10:** ML & Supervised Autonomy

---

## Not Roadmap Milestones

The following are useful cleanup or refactoring tasks, but should not become major project stages on their own:

- Splitting `control.cpp` into headers and source files.
- Removing duplicated `parseCoordinates()` code.
- Removing duplicated timestamp helpers.
- Simplifying the component dispatch chain.
- Replacing component `int` ON/OFF values with `bool`.
- Cleaning up duplicated Haversine logic.
- Moving `activateRTH()` into a more natural class location.
- Introducing CMake or another build system.
- Structured logging, log levels, or log rotation.
- General code organization cleanup.

These should be handled when the surrounding code is already being changed or when they become necessary for testing, GUI integration, or maintainability.
