# MANAR Milestones 1–4 Implementation Map

**Version:** 2  
**Repository commit inspected:** `f21eb16`  
**Updated:** 2026-08-13  
**Authoritative behavior source:** [`BEHAVIOR_DECISIONS.md`](./BEHAVIOR_DECISIONS.md)  
**Roadmap source:** [`ROADMAP.md`](../../../ROADMAP.md)

This document is a read-only implementation plan for Roadmap Milestones 1–4. It supersedes Version 1 as the current planning guide but does not replace or modify the original file.

Version 2 incorporates locked decisions BD-01 through BD-08 and invariants INV-01 through INV-20. Items UD-01 through UD-03 remain unresolved and are explicitly preserved as design gates.

No implementation code is provided here. Each step is intended to be small enough for one focused Git commit.

---

# 1. Repository baseline

The current remote and local `main` branch and remote `HEAD` all identify commit `f21eb16`.

The Milestone 1–4 implementation is concentrated in:

- [`core/control.cpp`](../../../core/control.cpp)
- [`core/terminal.cpp`](../../../core/terminal.cpp)
- [`core/setup.cpp`](../../../core/setup.cpp)
- [`core/runtime_template.json`](../../../core/runtime_template.json)
- [`ROADMAP.md`](../../../ROADMAP.md)
- [`BEHAVIOR_DECISIONS.md`](./BEHAVIOR_DECISIONS.md)

Until stable paths are implemented in Milestone 3, control, terminal, and setup must be run with `core/` as their working directory.

## 1.1 Current control globals

At `control.cpp` lines 21–26:

- `json config`
- `json runtime`
- `json commands`
- `int lastProcessedCommandID = 0`
- `ofstream fout("logs.txt")`

The actual operational state is separately stored in object members:

- `flight`: `speed`, `altitude`, `destlat`, `destlon`, `mode`, `flightlaunched`
- `components`: eleven mutable component-status booleans
- `drone`: `latitude`, `longitude`, `battery`, `distfromhome`, `closefromhome`, `savebattery`
- `mission`: lifecycle, navigation, rescue, return, and lawnmower flags

Subsystem methods change their member variables and then patch selected fields into global `runtime`. The class members drive control behavior; the JSON object is a second, incomplete copy.

Terminal has separate process-local `commands`, `runtime`, `config`, and `commandID` globals at `terminal.cpp` lines 18–23. Terminal’s `runtime` is a display copy, not control state.

## 1.2 Complete `saveRuntime()` inventory

There are exactly 32 current call sites.

| Owner | Functions and save lines |
|---|---|
| `flight` | `setspeed` 169; `setaltitude` 179; `stopflight` 206; `launch` 221; `setmodename` 227; `setdestination` 283 |
| `components` | `thermal` 337 and 346; `rgb` 359; `infrared` 371; `fmcw` 383; `speaker` 394; `microphone` 405; `rf` 416; `beacon` 427; `strobe` 438; `spotlight` 449; `smoke` 460 |
| `drone` | `transmitinfo` 504 |
| `missionstatusupdater` | returning 655; finished-at-home 664; aborted 679; rescuee-found 703; normal mission 716 |
| Other `mission` methods | `configurerescueestate` 727; `startmission` 742; `abortmission` 752; `setreturningON` 808; lawnmower completion 892 |
| Control/free functions | `activateRTH` 935; `checkcommands` acknowledgement 1129; startup 1138 |

Those functions contain 50 individual `runtime["..."]` assignments. In addition, `main()` assigns `runtime = json::object()` at line 1137.

The `thermal()` ON path saves twice: at lines 337 and 346.

## 1.3 Current startup sequence

At `control.cpp` lines 1132–1152:

1. `config.json` is read without checking open, parse, required fields, or types.
2. `runtime` becomes an empty object.
3. The empty object is written to `runtime.json`.
4. `mission mymission` is constructed with default in-memory state.
5. `lastProcessedCommandID` remains zero.
6. The request still present in `commands.json` can execute again.
7. Each loop calls `checkcommands()`, then `missionstatusupdater()`, then sleeps one second.

`runtime_template.json` is never loaded or validated by any executable.

## 1.4 Current command producer call sites

The four `sendcommand()` overloads begin at terminal lines 129, 142, 153, and 166.

Menu submissions are:

- `RTH`: 232
- `START_MISSION`: 252
- `CHANGE_DEST`: 272
- `LAUNCH_DRONE`: 285
- `CHANGE_MODE`: 307, 312, 317, 322
- `CHANGE_SPEED`: 335
- `CHANGE_ALTITUDE`: 343
- `STOP_FLIGHT`: 349
- Eleven `CHANGE_COMPONENT` ON/OFF pairs: 394–559
- `RESCUEE_FOUND`: 577
- `TRANSMIT_INFO`: 583
- `ABORT_MISSION`: 589

The consumer is `checkcommands()` at control line 945, called at line 1149.

## 1.5 Locked behavior now applied to this map

| Decision | Implementation consequence |
|---|---|
| BD-01 / INV-02 | `START_MISSION` must succeed without prior launch. A started mission may be grounded. |
| BD-02 / INV-01 / INV-03 | Starting a new mission clears previous mission outcome, return, rescue, waiting, and search-progress state. |
| BD-03 / INV-04 / INV-07 | `STOP_FLIGHT` lands and clears `enroute` but does not automatically abort the mission. |
| BD-04 / INV-08 | `LAUNCH_DRONE` may relaunch during an existing mission without resetting mission state or destination. |
| BD-05 / INV-11–13 | RTH sets home plus `returning=true`; a later accepted non-home `CHANGE_DEST` clears `returning`. |
| BD-06 / INV-09–10 | Manual speed may differ from the selected mode preset but remains bounded. |
| BD-07 / INV-14–15 | `CHANGE_DEST` is allowed while waiting; it does not launch the landed aircraft. |
| BD-08 / INV-16 | Accepted abort marks aborted, clears enroute, sets home destination, and sets returning. Landed-abort flight behavior remains unresolved. |
| INV-05 | `enroute=true` is only valid while the drone is launched and actually traveling. |
| INV-06 | A grounded drone must have speed 0, altitude 0, and mode `"Stall"`. |
| INV-17–20 | Every command has a final result; runtime is complete, externally consistent, and persisted only by control. |

---

# 2. Remaining design decisions

Only unresolved choices are marked `DESIGN DECISION REQUIRED` below. Locked behavior from `BEHAVIOR_DECISIONS.md` is not presented as optional.

## DESIGN DECISION REQUIRED — D-01 Runtime schema source

Choose one:

1. Construct the complete snapshot in C++.
2. Load `runtime_template.json` and replace every field from authoritative state.
3. Construct in C++ and validate the checked-in template against the constructed schema.

Every option must create a fresh complete object. Patching a previous runtime snapshot is not acceptable under INV-18 and INV-19.

## DESIGN DECISION REQUIRED — D-02 Restart state

Choose what control restores:

1. Command acknowledgement only; reset mission/drone state.
2. Entire validated authoritative state.
3. A documented hybrid subset.

This choice affects whether a successful component, destination, stopped mission, or active return survives a control restart.

## DESIGN DECISION REQUIRED — D-03 Command ID strategy

Choose:

1. Monotonically increasing integer with exactly one permitted producer.
2. Globally unique string IDs.
3. Integer IDs combined with a persistent session/epoch.

Also decide whether IDs must be contiguous or merely greater than the last processed ID.

## DESIGN DECISION REQUIRED — D-04 Command acknowledgement schema

Define the exact runtime fields for:

- command ID
- command name
- final status
- rejection/error reason
- optional intermediate `PROCESSING` state

At minimum, final statuses must distinguish `SUCCESS` and `REJECTED`.

## DESIGN DECISION REQUIRED — D-05 Crash guarantee

Choose:

1. Persist `PROCESSING` before execution and never automatically retry after restart.
2. Persist resulting state and final acknowledgement atomically and restore complete state.
3. Re-execute unacknowledged commands, accepting at-least-once behavior.

Option 3 does not satisfy the roadmap’s duplicate-prevention goal.

## DESIGN DECISION REQUIRED — D-06 Stable data directory

Choose one common base for control, terminal, and setup:

- executable directory
- explicit command-line data directory
- documented repository `core/` data directory

## DESIGN DECISION REQUIRED — D-07 Missing/corrupt files

Define behavior separately for:

- missing/malformed config
- missing/malformed runtime
- missing/malformed commands
- missing/malformed runtime template, if used

Resetting corrupt runtime without recovering command acknowledgement can replay a prior command.

## DESIGN DECISION REQUIRED — D-08 Atomic replacement portability

Choose Windows/POSIX replacement support or document one platform target. Delete-then-rename is not atomic.

## DESIGN DECISION REQUIRED — D-09 Heartbeat representation

Choose UTC ISO-8601, epoch milliseconds, or both, and define the stale threshold.

## DESIGN DECISION REQUIRED — D-10 Mission representation and terminal outcomes

The behavioral invariants are locked, but representation remains open:

1. Keep booleans and enforce every invariant.
2. Add one authoritative phase and derive compatibility booleans.
3. Replace runtime booleans with a phase, intentionally changing the contract.

Also decide the final relationships among `started`, `aborted`, `finished`, and `rescueefound` after home arrival.

## DESIGN DECISION REQUIRED — D-11 Initial position and heading

Choose:

- initial position at home
- position unknown until supplied
- preserve `0,0` as the simulation origin

Also choose heading schema and its stationary/unknown value.

## DESIGN DECISION REQUIRED — D-12 Repeated START and destination prerequisite

BD-01 says launch is not a prerequisite. BD-02 says a new mission resets old mission state. Still decide:

- whether START requires a non-home destination
- whether START during an already active mission is rejected, idempotent, or explicitly begins a new mission

Do not infer that every repeated button press means “new mission.”

## DESIGN DECISION REQUIRED — D-13 Preflight speed, altitude, and mode commands

INV-06 requires grounded speed 0, altitude 0, and mode Stall. Current code permits `CHANGE_SPEED`, `CHANGE_ALTITUDE`, and `CHANGE_MODE` while grounded.

Choose:

1. Reject non-ground speed, altitude, and mode changes while grounded.
2. Add separate target/commanded speed, altitude, and mode values while preserving actual grounded state.

Do not let the existing actual-state fields violate INV-06.

## DESIGN DECISION REQUIRED — D-14 RTH acceptance conditions

BD-05 defines successful RTH semantics but does not explicitly say whether RTH is accepted:

- before mission start
- while grounded
- while waiting for help

Whichever requests are accepted must set home destination and `returning=true` without implicitly launching unless that is separately decided.

## DESIGN DECISION REQUIRED — D-15 Rescue repeat and post-wait launch

INV-14 resolves rescue waiting as landed, not hovering. Still decide:

- repeated `RESCUEE_FOUND`: reject or idempotent success
- whether `LAUNCH_DRONE` after rescue clears `waitingforhelp` and resumes the mission

## DESIGN DECISION REQUIRED — UD-01 Component state restrictions

Do not invent state restrictions for component toggles. Until this is decided, Milestones 1–4 should preserve ordinary supported toggles and avoid claiming that the policy is final.

## DESIGN DECISION REQUIRED — UD-02 Explicit component authorization

Do not add or remove safety confirmations for smoke, spotlight, beacon, or strobe without an owner decision.

## DESIGN DECISION REQUIRED — UD-03 Abort while already landed

After setting `aborted=true`, `enroute=false`, home destination, and `returning=true`, choose:

1. Automatically relaunch.
2. Remain landed until explicit `LAUNCH_DRONE`.

## DESIGN DECISION REQUIRED — D-16 Home-arrival completion

Decide whether home arrival:

- stops flight automatically
- clears `returning`
- clears or preserves `started`
- sets `finished` after an aborted mission
- preserves `aborted` and `rescueefound` as history

## DESIGN DECISION REQUIRED — D-17 Public heliograph state

`heliograph_mirror_status` is always true internally but absent from runtime and command dispatch. Decide whether it remains internal or becomes a public runtime component.

## DESIGN DECISION REQUIRED — D-18 Idempotent/repeated command results

Define whether an already-satisfied request is `SUCCESS` or `REJECTED`, including:

- LAUNCH while already launched
- STOP while already grounded
- RTH while already returning home
- CHANGE_DEST to the current destination
- CHANGE_SPEED, CHANGE_ALTITUDE, or CHANGE_MODE to the current value
- CHANGE_COMPONENT to its current value

Repeated START and repeated rescue remain separately governed by D-12 and D-15.

## DESIGN DECISION REQUIRED — D-19 Flight commands before mission start

BD-01 locks START as the first mission-lifecycle action and establishes START → LAUNCH as valid. It does not explicitly state whether standalone flight preparation is permitted before START.

Decide whether `LAUNCH_DRONE` is rejected before a mission starts. Destination and grounded speed/altitude/mode preparation are already affected by D-12 and D-13. RTH is governed by D-14.

`ABORT_MISSION` and `RESCUEE_FOUND` should not succeed before START because both are mission-lifecycle transitions and START comes first.

## DESIGN DECISION REQUIRED — D-20 Meaning of enroute before movement exists

INV-05 says enroute means actively airborne and traveling, but Milestone 5 movement does not yet exist.

Choose the Milestone 4 interpretation:

1. Treat launched + positive speed + active outbound navigation as “traveling” for the current static prototype.
2. Keep enroute false until position actually progresses in Milestone 5.

Do not set enroute merely because `mission.started` or `flight.launched` is true.

---

# 3. Milestone 1 — Runtime & State Ownership

## Step 1.1 — Expose authoritative state and operation outcomes

**Files/functions involved:**

- `flight` at control line 153
- `components` at line 308
- `drone` at line 482
- `mission` at line 621
- Calls from `checkcommands()`, `missionstatusupdater()`, `landwhereyouare()`, `lawnmower()`, and `activateRTH()`

**Current behavior:**

Flight has several getters, but mission flags and component statuses do not. Most mutators return `void`, so control cannot tell whether a request changed state.

**Required behavior:**

Expose read-only access to:

- flight launched, speed, altitude, mode, and destination
- all eleven component statuses
- drone position and last-transmitted position
- mission started, aborted, finished, rescue found, enroute, returning, and waiting

Mutators used by commands must report success/failure or an equivalent outcome.

**Dependencies:**

Required before snapshot generation and command acknowledgement.

**Test:**

Issue one valid speed change and component change. Confirm behavior remains the same while the caller can distinguish success from failure.

**Do not change:**

- Mode mapping: 1 Quick, 2 Active, 3 Inspect, 4 Hover.
- Mode preset speeds.
- Manual-speed independence from mode under BD-06.
- Inclusive configured speed and altitude limits.
- Component names.

## Step 1.2 — Internalize last-transmitted coordinates

**Files/functions involved:**

- `drone::transmitinfo()` at line 500
- `getdronelat()` and `getdronelong()`
- `mission.last_saved_latitude` and `last_saved_longitude`

**Current behavior:**

Last-saved coordinates exist only in global JSON.

**Required behavior:**

Store them as authoritative in-memory values updated only by `transmitinfo()`. Snapshot generation reads those stored values.

**Test:**

Send `TRANSMIT_INFO`, then an unrelated component command. The last-saved values must not change during the unrelated command.

**Do not change:**

Do not turn transmission into networking. It remains a local record/log operation.

## Step 1.3 — Build one complete runtime snapshot

**Files/functions involved:**

- Global `runtime`
- `runtime_template.json`
- `mission`, `drone`, `flight`, `components`
- command metadata

**Current behavior:**

Runtime completeness depends on which methods have run.

**Required behavior:**

Create a new complete object on every publication. Initially include every existing template field:

- control last-processed metadata
- flight launched, speed, altitude, mode
- destination latitude/longitude
- all eleven component keys
- all existing mission keys

Later steps add result, heartbeat, position, and heading.

**Dependencies:**

Requires Steps 1.1–1.2 and D-01.

**Test:**

Start control with no new command. Every template field must immediately exist with the expected type.

**Do not change:**

Do not publish internal lawnmower flags merely because they are available.

## Step 1.4 — Remove persistence from `flight`

**Files/functions involved:**

- `setspeed()`
- `setaltitude()`
- `stopflight()`
- `launch()`
- `setmodename()`
- `setmode()`
- `setdestination()`

**Current behavior:**

Each method patches runtime and saves. `setmode()` can expose new speed with the old mode.

**Required behavior:**

These functions update only flight members and return an outcome. Remove their runtime assignments and saves.

**Test:**

Issue mode, altitude, launch, and stop commands. Runtime must show only complete final combinations.

**Do not change:**

- Launch still sets launch altitude and Hover mode in the current instantaneous simulation.
- Stop still produces the grounded values required by INV-06.
- Manual `CHANGE_SPEED` does not rename the mode.

## Step 1.5 — Remove persistence from `components`

**Files/functions involved:**

All eleven component methods at lines 332–460 and their dispatch calls at lines 1015–1104.

**Current behavior:**

Each method writes one runtime fragment. Thermal ON writes and saves twice.

**Required behavior:**

Component methods update only their owned status and return success/failure.

**Test:**

Toggle one camera and one signaling component. All component keys must remain present, and only the selected status may change.

**Do not change:**

Do not invent UD-01 state restrictions or UD-02 authorization rules.

## Step 1.6 — Remove persistence from `drone` and `mission`

**Files/functions involved:**

- `drone::transmitinfo()`
- `missionstatusupdater()`
- `configurerescueestate()`
- `startmission()`
- `abortmission()`
- `setreturningON()`
- `lawnmower()`

**Current behavior:**

Every branch patches and saves fragments.

**Required behavior:**

Remove all direct global-runtime writes and saves. These methods modify authoritative members only.

**Test:**

Start a mission and invoke rescue/abort paths. After central publication, runtime must remain complete.

**Do not change:**

This ownership commit must not yet add movement, battery drain, or a new mission framework.

## Step 1.7 — Remove persistence from `activateRTH()`

**Files/functions involved:**

- `activateRTH()` at line 922
- RTH call at line 976
- abort call at line 1118

**Current behavior:**

RTH saves returning state, destination state, and direct destination fields separately.

**Required behavior:**

Update internal return/destination state only. Control publishes the complete result.

**Test:**

Issue RTH and repeatedly read runtime. No snapshot may show `returning=true` with a non-home destination.

**Do not change:**

Successful RTH must follow BD-05 and INV-11–12.

## Step 1.8 — Make control the sole publisher

**Files/functions involved:**

- `checkcommands()` lines 1126–1129
- `main()` lines 1132–1152
- `saveRuntime()`

**Current behavior:**

Startup publishes `{}` and subsystems publish independently.

**Required behavior:**

1. Load validated configuration.
2. Restore the state selected by D-02.
3. Construct authoritative objects.
4. Publish a complete initial snapshot.
5. For each loop: process command, progress mission once, build snapshot, save once, sleep.

Static verification:

- `runtime["..."]` appears only in snapshot construction.
- `saveRuntime()` appears only in control publication.
- Terminal never writes `runtime.json`.

**Test:**

Start control, issue several commands, and repeatedly parse runtime. It must always be complete.

**Do not change:**

Keep one-second polling and command-before-mission-update ordering.

## Milestone 1 bugs naturally removed

- Startup `{}`.
- Missing untouched keys.
- Half-published compound transitions.
- Thermal duplicate save.
- Speed/mode intermediate snapshots.
- RTH returning/destination intermediate snapshots.
- JSON-only last-transmitted state.

## Milestone 1 bugs deferred

- False or duplicate command handling: Milestone 2.
- File truncation/collision/path failures: Milestone 3.
- Mission contradictions: Milestone 4.
- No movement or battery drain: later milestones.

---

# 4. Milestone 2 — Reliable Command Lifecycle

## Step 2.1 — Parse requests into a validated local value

**Files/functions involved:**

- `readcommands()`
- `checkcommands()`
- terminal `sendcommand()` overloads

**Current behavior:**

Only `id` and `command` presence are checked. Wrong types or missing arguments may terminate control.

**Required behavior:**

Validate before executing:

| Command | Required structure and locked semantic notes |
|---|---|
| `START_MISSION` | No arguments. Does not require launch. Applies BD-02 reset when it begins a new mission. D-12 remains for repeated START and destination prerequisites. |
| `RTH` | No arguments. On success sets home and returning. Acceptance states remain D-14. |
| `LAUNCH_DRONE` | No arguments. Relaunch is allowed and preserves mission/destination under BD-04. Pre-mission acceptance remains D-19. |
| `STOP_FLIGHT` | No arguments. Safe landing, no automatic abort, enroute false. |
| `RESCUEE_FOUND` | No arguments. Reject before mission start rather than falsely succeeding. Repeated behavior remains D-15. |
| `ABORT_MISSION` | No arguments. Accepted active abort sets BD-08 state. Landed behavior remains UD-03. |
| `TRANSMIT_INFO` | No arguments. |
| `CHANGE_DEST` | Finite latitude/longitude in geographic bounds. Allowed while waiting. Non-home override clears returning. |
| `CHANGE_MODE` | Integer-valued option 1–4. Grounded handling remains D-13. |
| `CHANGE_SPEED` | Finite value from 0 through maximum speed. Manual override does not change mode. Grounded handling remains D-13. |
| `CHANGE_ALTITUDE` | Finite value from 0 through maximum altitude. Grounded handling remains D-13. |
| `CHANGE_COMPONENT` | Supported component string plus actual JSON boolean. No invented UD-01/UD-02 policy. |

Supported component strings remain:

- `thermal_camera`
- `rgb_camera`
- `infrared_camera`
- `fmcw_radar`
- `speaker`
- `microphone`
- `passive_rf`
- `amber_beacon`
- `white_strobe`
- `downward_spotlight`
- `smoke_marker`

A valid ID with an invalid body ends `REJECTED`. Malformed JSON or an unusable ID is a file/protocol failure because it cannot be safely acknowledged by identity.

**DESIGN DECISION REQUIRED:**

Decide whether unexpected extra arguments are rejected or ignored.

**Test:**

Submit `CHANGE_SPEED` with no value under a valid new ID. Control must stay alive, leave speed unchanged, and reject it.

**Do not change:**

Do not add a new transport or queue.

## Step 2.2 — Return truthful subsystem outcomes

**Files/functions involved:**

Every branch in `checkcommands()` and mutators from Milestone 1.

**Current behavior:**

Unknown, invalid, unsupported, and state-inapplicable requests can be logged as executed.

**Required behavior:**

Every identified request reaches one final result:

- `SUCCESS`: requested valid transition occurred.
- `REJECTED`: validation or state precondition failed.

Only successful commands receive “executed” logs. Rejections include a reason.

Rejected commands with valid IDs count as processed so the single slot can be released.

D-18 determines whether an already-satisfied valid request is successful or rejected.

**Test:**

Submit an unknown command, mode 9, unsupported component, and rescue-found before START. Each must be rejected with no state change.

**Do not change:**

Do not silently convert malformed values into defaults.

## Step 2.3 — Apply locked state-dependent command results

**Files/functions involved:**

- START, LAUNCH, STOP, RTH, CHANGE_DEST, CHANGE_MODE, CHANGE_SPEED, RESCUE, and ABORT branches
- Milestone 4 transition operations

**Current behavior conflicts:**

- START can produce enroute while grounded.
- STOP can be followed by updater reasserting enroute.
- CHANGE_DEST does not cancel returning.
- Aborted state reasserts RTH on every updater tick, defeating later destination override.
- Speed/mode can violate grounded-state values.

**Required behavior:**

- START without launch is not rejected solely for being grounded.
- A grounded started mission has `enroute=false`.
- Relaunch preserves mission and destination.
- STOP succeeds as safe landing, clears enroute, and does not set aborted.
- Non-home CHANGE_DEST clears returning and does not launch.
- CHANGE_DEST while waiting remains landed and waiting.
- Manual speed stays bounded and does not change mode.
- Accepted abort uses BD-08.
- ABORT and RESCUEE_FOUND before START are rejected because START is the first mission-lifecycle action.

Unresolved D-12 through D-20 and UD-03 must be decided before their affected cases can have final result rules.

**Test:**

Run:

1. START while grounded.
2. LAUNCH.
3. STOP.
4. LAUNCH again.

Confirm each successful acknowledgement corresponds to the locked state transition.

**Do not change:**

Do not equate START with launch or STOP with abort.

## Step 2.4 — Publish a final acknowledgement with resulting state

**Files/functions involved:**

- `checkcommands()`
- snapshot builder
- runtime schema/template

**Current behavior:**

Only `last_processed_command` is exposed.

**Required behavior:**

Publish the D-04 metadata in the same complete snapshot as the command’s resulting state.

For example, a successful component command’s snapshot must contain both final command success and the new component value.

**Test:**

Toggle one component. Confirm command ID/status and component state become observable together.

**Do not change:**

The roadmap requires only the most recent result, not a command history.

## Step 2.5 — Restore last-processed state

**Files/functions involved:**

- `lastProcessedCommandID`
- control startup
- runtime loader
- initial snapshot

**Current behavior:**

Restart resets the ID to zero and replays the request file.

**Required behavior:**

Restore validated durable command metadata before accepting requests or publishing startup runtime. Never execute an already-final ID again.

If an old ID appears with different contents, report a protocol violation.

**Dependencies:**

D-02, D-03, D-05, and Milestone 3 atomic persistence.

**Test:**

Execute a component command, restart control without changing `commands.json`, and confirm there is no second execution log.

**Do not change:**

Do not infer completion solely because an ID exists in `commands.json`.

## Step 2.6 — Implement the selected crash boundary

**Files/functions involved:**

- command execution
- runtime publication
- startup recovery
- terminal outstanding-state check

**Required behavior:**

Implement D-05. If using `PROCESSING`, the terminal must not treat it as a final acknowledgement or overwrite the request.

**Test:**

Terminate control at the chosen execution boundary, restart, and verify the command is completed at most once or explicitly interrupted according to policy.

**Do not change:**

Do not claim exactly-once behavior beyond what the selected policy actually guarantees.

## Step 2.7 — Block overwriting an outstanding request

**Files/functions involved:**

- `loadlastcommandid()`
- `readruntime()`
- `savecommands()`
- four `sendcommand()` overloads
- every terminal “sent” message

**Current behavior:**

Terminal overwrites `commands.json` without waiting for control.

**Required behavior:**

Before writing:

1. Read the current request identity.
2. Read the most recent final acknowledgement.
3. If they do not match, refuse the new request.
4. Do not increment the ID or change the file.
5. Report the outstanding ID.

`sendcommand()` must return submission success so the menu only prints “sent” when the file was actually replaced.

**Test:**

With control stopped, submit one command and attempt a second. The second must be refused and the command file remain unchanged.

**Do not change:**

Terminal remains a producer/viewer only.

## Step 2.8 — Lifecycle regression

**Test sequence:**

1. Valid command → SUCCESS.
2. Invalid value → REJECTED, no state change.
3. Unknown command → REJECTED.
4. Unsupported component → REJECTED.
5. Second request while outstanding → producer refusal.
6. Restart after final result → no replay.
7. Old ID with changed body → no execution.
8. Malformed command file → control survives.
9. START grounded → success under BD-01.
10. STOP active mission → safe landing without abort.
11. CHANGE_DEST during waiting → success without launch.
12. CHANGE_DEST after RTH → returning becomes false.

## Milestone 2 bugs naturally removed

- Silent unknown-command consumption.
- Invalid commands falsely logged as executed.
- Unsupported components falsely succeeding.
- Rescue-found before START falsely succeeding.
- Outstanding requests overwritten.
- Ordinary restart replay.
- Destination override failing to cancel RTH.

## Milestone 2 bugs deferred

- Torn files: Milestone 3.
- Concurrent producer races unless D-03 handles them.
- UD-03 landed-abort result.
- D-13 grounded speed/altitude/mode policy.
- D-18 repeated/idempotent result policy.
- D-19 pre-mission launch policy.

---

# 5. Milestone 3 — JSON & File Reliability

`config.json`, `runtime.json`, `commands.json`, and `logs.txt` are ignored. A fresh clone normally lacks them.

## Step 3.1 — Establish stable paths

**Files/functions involved:**

- Control log/runtime/commands/config paths
- Terminal commands/runtime/config paths
- Setup config path
- Runtime template path if D-01 uses it

**Current behavior:**

Every path depends on current working directory.

**Required behavior:**

Resolve every file from D-06 before opening streams, including temporary files.

**Test:**

Launch control and terminal from different working directories. Both must use the same files.

**Do not change:**

Do not introduce CMake, services, or a path framework.

## Step 3.2 — Validate configuration before object construction

**Files/functions involved:**

- all three `main()` functions
- config-dependent member initializers

**Current behavior:**

Missing/malformed config or wrong types may throw during startup or later object construction.

**Required behavior:**

Parse into a local value and validate before assigning global `config` or constructing `mission`.

Required string:

- `home_base_name`

Required numerics:

- home latitude/longitude
- maximum altitude/speed
- launch altitude
- Quick/Active/Inspect/Hover speeds
- home-arrival radius
- battery thresholds and starting battery
- reached radius
- search width/height
- row spacing

Validate geographic bounds, nonnegative limits, launch/mode values within maximums, battery ranges/order, and positive search dimensions.

**Test:**

Temporarily remove config. Each executable must follow D-07 with a clear controlled result.

**Do not change:**

Do not expand setup into a configuration framework.

## Step 3.3 — Parse files locally and preserve last good state

**Files/functions involved:**

- `readcommands()`
- terminal `readruntime()`
- `loadlastcommandid()`
- startup restoration

**Current behavior:**

Parsers write directly into global JSON. Terminal runtime parsing can throw.

**Required behavior:**

For each read:

1. Open/check.
2. Parse a local JSON value.
3. Validate structure/types.
4. Only then replace the local cache.
5. Return a meaningful status.
6. Log the specific failure.

Do not display a stale cached runtime as current without an explicit stale warning.

**Test:**

Provide malformed runtime while terminal is active. Terminal must report failure and remain alive.

**Do not change:**

Invalid transport data never mutates authoritative state.

## Step 3.4 — Write JSON atomically

**Files/functions involved:**

- control `saveRuntime()`
- terminal `savecommands()`
- setup config writer

**Current behavior:**

Direct opening truncates the destination before the document is complete.

**Required behavior:**

1. Serialize the complete document.
2. Write a temporary file in the same directory.
3. Check write, flush, and close.
4. Atomically replace the destination using D-08.
5. Preserve the old destination on failure.
6. Report errors.

**Test:**

Continuously parse runtime during publication. Every successful read must contain the complete old or complete new snapshot.

**Do not change:**

Do not add a database or queue.

## Step 3.5 — Add bounded read retry

**Files/functions involved:**

- terminal runtime reads
- control command reads
- startup loaders

**Current behavior:**

Transient sharing/open failure loses the read or may terminate terminal.

**Required behavior:**

Retry a small bounded number of times for transient open/replacement collisions, then report failure safely.

**Test:**

Rapidly submit commands while displaying runtime. Neither process may crash or execute partial JSON.

**Do not change:**

Do not create worker threads.

## Step 3.6 — Give logs one writer

**Files/functions involved:**

- control `fout` line 26
- unused terminal `fout` line 18

**Current behavior:**

Both processes open and truncate the same log during global initialization.

**Required behavior:**

- Remove terminal’s unused stream.
- Resolve the control log path before opening.
- Open control log in append mode.
- Check success.
- Use `stderr` if the log cannot be opened.

**Test:**

Write a control log entry, then start terminal. The entry must remain.

**Do not change:**

No structured logging or rotation.

## Step 3.7 — Add heartbeat

**Files/functions involved:**

- control loop
- snapshot builder
- runtime schema

**Current behavior:**

A valid runtime file may be stale.

**Required behavior:**

Update the D-09 heartbeat once per loop and publish it in every complete snapshot.

**Test:**

Observe heartbeat changes, stop control, and confirm the heartbeat becomes stale.

**Do not change:**

No watchdog thread or service.

## Step 3.8 — File failure regression

**Test sequence:**

1. Missing config.
2. Malformed config.
3. Missing commands.
4. Malformed commands.
5. Missing runtime when terminal reads.
6. Malformed runtime.
7. Stale temporary file.
8. Runtime replacement while terminal reads.
9. Command replacement while control reads.
10. Log path unwritable.
11. Launch from different working directories.

## Milestone 3 bugs naturally removed

- Half-written runtime/commands/config.
- Terminal runtime parse crashes.
- Log truncation by terminal.
- Accidental files in arbitrary working directories.
- Stale runtime mistaken for live control.

## Milestone 3 bugs deferred

- D-05 command crash semantics.
- Concurrent command producers unless D-03 supports them.
- Mission state policy.

---

# 6. Milestone 4 — Deterministic Drone & Mission State

## 6.1 Current behavior conflicts with locked decisions

### `missionstatusupdater()`

At control lines 640–719:

- It returns if mission is not started.
- Returning has priority.
- It sets finished at home but leaves returning true.
- Aborted state sets returning every tick.
- Waiting returns immediately.
- Rescue-found lands, sets waiting, and transmits twice.
- The normal branch always sets `enroute=true`, even while grounded.

It is invoked by:

- `configurerescueestate()` line 728
- `startmission()` line 736
- `abortmission()` line 747
- main line 1151

### Command conflicts

- START immediately reaches the updater’s normal branch and sets enroute while grounded, violating INV-05.
- START does not reset prior mission state, violating BD-02/INV-03.
- STOP grounds flight, then updater can restore enroute, violating BD-03/INV-07.
- CHANGE_DEST does not clear returning, violating BD-05/INV-13.
- The aborted branch reactivates RTH after a later destination override, violating BD-05 and INV-16’s explicit override allowance.
- CHANGE_SPEED, CHANGE_ALTITUDE, and CHANGE_MODE can violate grounded INV-06.
- ABORT reaches return state twice.
- Rescue-found transmits twice.
- Arrival leaves returning true.

## Step 4.1 — Implement the locked invariants in authoritative state

**Files/functions involved:**

- mission flags at lines 624–636
- flight state
- destination
- snapshot builder

**Current behavior:**

Independent flags permit contradictory combinations.

**Required behavior:**

Regardless of D-10 representation, enforce:

- START may be active while grounded.
- `enroute=true` requires launched and traveling.
- Grounded requires speed 0, altitude 0, Stall.
- STOP clears enroute without automatically aborting.
- Returning implies home destination.
- Waiting is landed unless a later explicit command transitions it.
- Manual speed is bounded but need not match mode preset.

Treat `missionabort` as outcome/history after the abort transition, not as a periodic instruction that automatically reasserts RTH forever. This is necessary so BD-05 destination override can work after abort.

Apply D-20 until Milestone 5 supplies actual movement; never equate “started” or “launched” alone with enroute.

**Test:**

After every current command, inspect the complete snapshot and verify INV-01 through INV-16.

**Do not change:**

Do not introduce a state-machine framework. A small explicit transition representation is enough if D-10 selects it.

## Step 4.2 — Define canonical position, heading, and destination

**Files/functions involved:**

- `drone::latitude` and `longitude`
- `flight::destlat` and `destlon`
- distance/arrival functions
- `transmitinfo()` and `landwhereyouare()`
- runtime schema

**Current behavior:**

Position begins at `0,0`, destination begins at home, and heading is absent.

**Required behavior:**

Apply D-11. Publish current position and heading separately from destination and last-transmitted position. Every distance calculation must use the same canonical position.

**Test:**

At startup, compare runtime position, home distance, destination, and last-transmitted position. Their meanings must be consistent.

**Do not change:**

No movement or heading progression yet.

## Step 4.3 — Make START create a clean grounded-capable mission

**Files/functions involved:**

- `mission::startmission()`
- START branch in `checkcommands()`
- updater
- search bookkeeping

**Current behavior:**

Only `missionstarted` becomes true; updater immediately sets enroute.

**Required behavior under BD-01 and BD-02:**

For an accepted new mission:

- `missionstarted = true`
- `missionabort = false`
- `missionfinished = false`
- `rescueefound = false`
- `returning = false`
- `waitingforhelp = false`
- `emergencyrth = false`
- `lawnmowerstarted = false`
- `horizontalmove = true`
- `moveeast = true`
- `searchrow = 0`
- `enroute = false` while grounded

Do not alter flight merely to make the mission active.

D-12 still governs repeated START, destination prerequisite, and which non-listed values reset.

**Test:**

Start after an aborted or waiting mission. All listed mission/search values must be clean while flight remains unchanged.

**Do not change:**

START does not launch.

## Step 4.4 — Make LAUNCH preserve and resume mission state

**Files/functions involved:**

- `flight::launch()`
- LAUNCH branch
- updater

**Current behavior:**

Launch sets launched, altitude, and Hover mode and does not directly reset mission/destination. The following updater may set enroute without actual travel.

**Required behavior under BD-04 and INV-08:**

- Relaunch is allowed during an existing mission.
- Mission identity, outcome flags, search progress, and destination are preserved.
- Launch establishes airborne Hover state.
- `enroute` becomes true only when the aircraft is actually traveling, not merely because launch occurred.

D-15 governs launch after waiting-for-help. D-19 governs launch before START. D-18 governs launch while already launched.

**Test:**

START → LAUNCH → STOP → LAUNCH. Mission identity and destination must remain unchanged through both launches.

**Do not change:**

Launch does not create/reset a mission.

## Step 4.5 — Make STOP safe landing without abort

**Files/functions involved:**

- `flight::stopflight()`
- STOP branch
- mission transition
- updater

**Current behavior:**

Flight becomes grounded, but mission can remain enroute.

**Required behavior under BD-03 and INV-04/06/07:**

- launched false
- speed 0
- altitude 0
- mode Stall
- enroute false
- mission is not automatically aborted
- active mission may remain started

Preserve destination. Preserve other mission state unless a locked rule requires a change.

**Test:**

START → LAUNCH → travel state → STOP. Confirm grounded invariant, enroute false, started still true, aborted unchanged, and destination preserved.

**Do not change:**

Do not turn STOP into ABORT or create a reset command.

## Step 4.6 — Keep manual speed independent from mode and enforce grounded command state

**Files/functions involved:**

- `flight::setmode()`
- `setspeed()`
- `setaltitude()`
- `setmodename()`
- CHANGE_MODE, CHANGE_SPEED, and CHANGE_ALTITUDE validation

**Current behavior:**

Mode applies its preset speed. A later manual speed leaves the mode name unchanged, which already agrees with BD-06. Speed, altitude, and mode commands can all violate INV-06 while grounded.

**Required behavior:**

- Mode changes apply configured presets when accepted.
- Later manual speed changes speed only.
- Manual speed remains within bounds.
- Altitude remains within configured bounds.
- Apply D-13 to all three grounded command types.

**Test:**

While airborne, set Quick, manually set 2 m/s, and set a valid altitude. Runtime must show Quick plus 2 m/s and the requested altitude. Repeat the three requests while grounded and confirm the selected D-13 behavior preserves INV-06.

**Do not change:**

Do not automatically rename mode after manual speed.

## Step 4.7 — Make CHANGE_DEST and RTH exact inverse navigation transitions

**Files/functions involved:**

- `flight::setdestination()`
- `activateRTH()`
- `setreturningON()`
- CHANGE_DEST and RTH branches
- updater

**Current behavior:**

RTH sets home/returning, but CHANGE_DEST changes only destination. Aborted updater may reassert returning.

**Required behavior under BD-05, BD-07, and INV-11–15:**

Successful RTH:

- destination home
- returning true

Successful non-home CHANGE_DEST while returning:

- destination new coordinates
- returning false

CHANGE_DEST while waiting:

- is accepted
- changes destination
- does not launch
- preserves landed flight
- preserves waiting unless another explicit transition changes it

An old `missionabort=true` must not re-enable RTH after a later explicit override.

**DESIGN DECISION REQUIRED:**

Decide whether `CHANGE_DEST` to the exact home coordinates sets `returning=true` or whether only `RTH` establishes returning.

**Test:**

RTH → verify home/returning → CHANGE_DEST away from home → verify new destination/returning false. Repeat while waiting and confirm no launch.

**Do not change:**

RTH is not a permanent destination lock.

## Step 4.8 — Consolidate return-home transition paths

**Files/functions involved:**

- `activateRTH()`
- `setreturningON()`
- `abortmission()`
- updater abort branch
- lawnmower completion
- RTH/ABORT branches

**Current behavior:**

Four paths establish returning differently.

**Required behavior:**

Use one internal return-navigation operation for:

- operator RTH
- abort
- lawnmower completion
- later battery RTH

It sets home and returning once, clears enroute, and optionally sets aborted based on caller. It does not persist runtime.

**Test:**

Compare RTH, airborne abort, and lawnmower completion. All must satisfy home/return invariants with only intended outcome differences.

**Do not change:**

Do not move the function solely for organizational cleanup.

## Step 4.9 — Make ABORT apply BD-08 once

**Files/functions involved:**

- `mission::abortmission()`
- ABORT branch
- consolidated return transition
- updater

**Current behavior:**

`abortmission()` sets abort and invokes updater; `checkcommands()` then calls `activateRTH()` again.

**Required behavior for airborne abort:**

- aborted true
- enroute false
- destination home
- returning true
- transition occurs once

Afterward, aborted remains historical state. A later accepted CHANGE_DEST may clear returning without the updater forcing it back.

UD-03 must be decided before implementing automatic flight behavior for landed abort.

**Test:**

Abort while airborne. Confirm one return transition/log and exact BD-08 state. Then CHANGE_DEST away and confirm returning stays false on later updater ticks.

**Do not change:**

Do not guess landed automatic relaunch.

## Step 4.10 — Make rescue-found enter landed waiting exactly once

**Files/functions involved:**

- `configurerescueestate()`
- rescue branch in updater
- `landwhereyouare()`
- `transmitinfo()`
- `stopflight()`

**Current behavior:**

Active rescue-found lands and transmits twice. Before START it does nothing but is logged as executed.

**Required behavior under INV-14:**

For a valid active mission:

- rescue found true
- enroute false
- returning false
- waiting true
- landed grounded state
- destination/current-location behavior applied consistently
- location recorded/transmitted once

Before mission start, reject explicitly.

D-15 governs repeated rescue and later launch.

**Test:**

During active flight, issue RESCUEE_FOUND. Confirm one transmission, waiting true, grounded invariant, and no enroute/returning.

**Do not change:**

Do not add perception or automatic authorization logic.

## Step 4.11 — Complete home arrival consistently

**Files/functions involved:**

- returning branch of updater
- `determineifclose()`
- `getclose()`
- flight stop
- completion flags

**Current behavior:**

Arrival sets finished but leaves returning true and flight unresolved.

**Required behavior:**

Apply D-16 once when the home-arrival condition becomes true. Subsequent updater ticks must not repeat completion side effects.

**Test:**

Place the canonical position within home-arrival radius, trigger RTH, and run multiple updater cycles. Final state must stabilize after the first completion transition.

**Do not change:**

Keep `home_arrival_radius` distinct from `reached_radius`.

## Step 4.12 — Make updater periodic, not command-recursive

**Files/functions involved:**

- updater calls at lines 728, 736, 747, 1151
- command transition methods

**Current behavior:**

START, ABORT, and RESCUE invoke updater internally; main invokes it again.

**Required behavior:**

Command methods perform one explicit transition and return. Main invokes the updater once per loop for periodic conditions:

- arrival
- search-location checks
- future movement/battery progression

The updater must not:

- set enroute merely because mission started
- reassert RTH merely because aborted is historically true
- repeat rescue landing/transmission

**Test:**

START, ABORT, and RESCUE must each produce one transition/log sequence.

**Do not change:**

Keep main ordering: command, periodic update, snapshot, sleep.

## Step 4.13 — Keep component state canonical without inventing policy

**Files/functions involved:**

- `components`
- CHANGE_COMPONENT validator/dispatch
- snapshot builder

**Current behavior:**

Runtime may contain only touched components; unsupported names falsely succeed.

**Required behavior:**

- all eleven existing keys always present
- internal statuses are canonical
- valid toggle changes exactly one state
- unsupported name rejected
- no state restriction or extra authorization added until UD-01/UD-02
- D-17 governs heliograph publication

**Test:**

Toggle every supported component in grounded, active, waiting, and returning states. Until UD-01/UD-02 are resolved, the implementation must not introduce inconsistent ad hoc restrictions.

**Do not change:**

Do not simplify dispatch or convert its integer ON/OFF API as unrelated cleanup.

## Step 4.14 — Deterministic scenario regression

Run each sequence twice from the same starting state and compare runtime excluding heartbeat:

1. START while grounded.
2. START → LAUNCH.
3. START → LAUNCH → STOP.
4. START → LAUNCH → STOP → LAUNCH.
5. START → LAUNCH → CHANGE_MODE Quick → CHANGE_SPEED 2.
6. RTH → CHANGE_DEST away from home.
7. START → LAUNCH → ABORT → CHANGE_DEST away.
8. START → LAUNCH → RESCUEE_FOUND.
9. Waiting → CHANGE_DEST.
10. RESCUEE_FOUND before START.
11. Repeated START under D-12.
12. Repeated rescue under D-15.
13. Repeated/idempotent commands under D-18.
14. LAUNCH before START under D-19.
15. Enroute behavior under D-20.
16. RTH in each state under D-14.
17. Landed abort under UD-03.
18. Return-home arrival under D-16.
19. Component toggles in each state.
20. Restart with an acknowledged command.

Each run must produce the same final authoritative state and command result.

## Milestone 4 bugs naturally removed

- START setting enroute while grounded.
- START retaining previous mission/search state.
- STOP restoring enroute or implicitly aborting.
- Relaunch accidentally resetting mission/destination.
- CHANGE_DEST failing to cancel returning.
- Aborted updater defeating post-abort destination override.
- Duplicate abort/RTH transition.
- Separate lawnmower return semantics.
- Rescue transmission happening twice.
- Rescue before START falsely succeeding.
- Arrival remaining in an active return loop.
- Component runtime fragments.

## Milestone 4 bugs intentionally deferred

- Drone movement: Milestone 5.
- Altitude progression: Milestone 5.
- Battery drain and emergency transitions: Milestone 6.
- Lawnmower execution through waypoints: Milestones 5 and 7.
- Coordinate parser and Haversine duplication: non-milestone cleanup.
- UD-01/UD-02/UD-03 until owner decisions are locked.

---

# 7. Updated commit order

1. Add authoritative state getters and operation outcomes.
2. Internalize last-transmitted coordinates.
3. Add complete snapshot construction.
4. Remove flight runtime writes/saves.
5. Remove component runtime writes/saves.
6. Remove drone and mission runtime writes/saves.
7. Remove RTH runtime writes/saves.
8. Make main/control the sole publisher.
9. Add local structural command validation.
10. Add truthful SUCCESS/REJECTED execution outcomes.
11. Apply locked state-dependent results that do not depend on unresolved choices.
12. Add final acknowledgement fields.
13. Restore last-processed command state.
14. Implement the selected command crash boundary.
15. Block overwriting outstanding requests.
16. Establish stable shared paths.
17. Validate config before object construction.
18. Harden all JSON readers.
19. Add atomic runtime/command/config writers.
20. Add bounded read retry.
21. Give logs one owner.
22. Add heartbeat.
23. Implement the selected authoritative mission representation.
24. Add canonical position and heading.
25. Make START reset mission/search state without launching.
26. Make LAUNCH preserve/resume an existing mission.
27. Make STOP land without aborting.
28. Enforce manual-speed/mode behavior plus grounded speed/altitude/mode invariants.
29. Implement RTH/CHANGE_DEST override behavior.
30. Consolidate all return-home entry paths.
31. Make airborne abort apply BD-08 once.
32. Resolve and implement UD-03 landed abort.
33. Make rescue enter landed waiting once.
34. Implement home-arrival completion.
35. Remove command-internal updater calls.
36. Verify canonical component behavior without inventing UD-01/UD-02.
37. Run full command, file-failure, restart, and deterministic sequence regression.

Milestone 2’s restart guarantee depends on Milestone 3 atomic persistence. State-dependent command outcomes depend on the relevant Milestone 4 transition rules. Steps blocked by an unresolved design decision should not be guessed or marked complete.

---

# 8. Completion checklist

Milestone 1 is complete only when:

- control-owned objects are authoritative
- every runtime snapshot is complete
- subsystem methods contain no runtime writes or saves
- control is the only `runtime.json` writer

Milestone 2 is complete only when:

- every identified request has a unique identity
- every valid-ID request ends SUCCESS or REJECTED
- only one request is outstanding
- restart does not replay a final command
- crash behavior matches the documented policy

Milestone 3 is complete only when:

- paths are stable
- config/runtime/commands are safely parsed and validated
- JSON writes are atomic
- terminal cannot truncate control logs
- heartbeat distinguishes live and stale control

Milestone 4 is complete only when:

- BD-01 through BD-08 are implemented
- INV-01 through INV-16 hold after every transition
- START/LAUNCH/STOP remain distinct operations
- RTH is overridable through CHANGE_DEST
- airborne abort initiates RTH exactly once
- waiting-for-help is landed
- identical sequences produce identical results
- unresolved decisions are either locked and implemented or clearly remain blocking
