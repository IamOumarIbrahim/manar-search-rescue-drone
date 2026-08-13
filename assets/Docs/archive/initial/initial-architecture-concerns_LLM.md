Listed directory manar-search-rescue-drone
Listed directory core
Listed directory webdashboardv1
Listed directory manar-landing-page
Viewed README.md:1-188
Viewed MANAR.md:1-150
Viewed control.cpp:1-250
Viewed control.cpp:251-600
Viewed control.cpp:601-1159
Viewed terminal.cpp:1-250
Viewed setup.cpp:1-91
Viewed script.js:1-121

### What I Understand

Based on my analysis of the repository ([`README.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/README.md), [`MANAR.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/MANAR.md), [`payload.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/payload.md), and the source in [`core/`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/core)):

1. **System & Objective**: MANAR is a supervised-autonomy UAV system for Search-and-Rescue (SAR). It uses multisensor payloads (thermal, RGB, IR, FMCW radar, passive RF, directional visual/acoustic guidance) to locate victims and coordinate rescue efforts.
2. **Architecture**: 
   - **State Authority**: The C++ control core ([`control.cpp`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/core/control.cpp)) is designed as the deterministic single source of truth for runtime state.
   - **Operator GUI**: Planned as a React/TypeScript interface connected via WebSocket. Currently prototyped as a CLI terminal client ([`terminal.cpp`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/core/terminal.cpp)) and a standalone video feed UI ([`webdashboardv1/`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/webdashboardv1/script.js)).
   - **Inter-Process Communication (IPC)**: Currently operating via disk file polling (`commands.json` for operator intent and `runtime.json` for canonical state snapshots).
3. **Current Phase**: You are transitioning from architectural definition to building out the deterministic C++ simulation and core control engine.

---

### What Looks Good

- **Clear Responsibility Allocation & State Authority**: Defining the C++ core as the absolute owner of state (with GUI as a view and ML as a supervised advisory system) is a solid system engineering choice for safety-critical drone operations.
- **Thorough Specifications**: [`MANAR.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/MANAR.md) and [`payload.md`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/payload.md) establish strict behavioral constraints, command contracts, and sensor roles before prematurely over-engineering code.
- **Decoupled Terminal Client**: Moving operator interaction out of [`control.cpp`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/core/control.cpp) into [`terminal.cpp`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/core/terminal.cpp) was a clean architectural step. Sending JSON command payloads mirrors what the future React/WebSocket GUI will do.
- **Geographic & Grid Utilities**: Functions like DMS/Decimal coordinate parsing ([`control.cpp:L53-L124`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/core/control.cpp#L53-L124)), Haversine distance routines ([`control.cpp:L511-L532`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/core/control.cpp#L511-L532)), and lawnmower waypoint generation ([`control.cpp:L828-L918`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/core/control.cpp#L828-L918)) are straightforward and readable.

---

### Concerns

#### 1. Drone Simulated Position is Static (Logic Bug)
* **What**: In [`control.cpp`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/core/control.cpp), the `drone` class initializes its `latitude` and `longitude` to `0.0, 0.0` ([`control.cpp:L485-L486`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/core/control.cpp#L485-L486)), but there is no update loop advancing these coordinates over time towards `destlat` / `destlon`.
* **Why**: When `lawnmower()` or `reachedDestination()` executes, `currentlat` and `currentlon` read the un-moving drone position (`0.0, 0.0`). The waypoint calculation in [`control.cpp:L828`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/core/control.cpp#L828) computes off fixed coordinates, meaning the drone never actually "travels" or advances through the search grid.
* **Conditions**: Manifests as soon as you run a mission and expect state updates over time.
* **Urgency**: **High** — Must be resolved for the control loop to simulate actual flight progress.

---

#### 2. IPC Command Loss and File Race Conditions (Fragility Issue)
* **What**: Communication currently relies on non-atomic disk file polling. [`terminal.cpp`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/core/terminal.cpp#L122-L176) overwrites `commands.json` with a single command object, and [`control.cpp`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/core/control.cpp#L1147-L1155) reads `commands.json` inside a 1-second loop (`sleep_for(chrono::seconds(1))`).
* **Why**:
  1. *Command Loss*: If an operator sends two commands within 1 second (e.g. `LAUNCH_DRONE` followed by `CHANGE_MODE`), the second command overwrites `commands.json` before `control.cpp` polls it. The first command is lost permanently.
  2. *Read/Write Collisions*: If `control.cpp` opens `commands.json` for reading at the exact millisecond `terminal.cpp` is writing to it, JSON parsing fails. The exception is swallowed in `readcommands()` ([`control.cpp:L145`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/core/control.cpp#L145)), causing the command to be dropped silently.
* **Conditions**: Occurs under rapid operator input or simultaneous disk access.
* **Urgency**: **Medium/High** — Sufficient for simple single-command manual testing, but will break when building a responsive operator interface.

---

#### 3. High-Frequency Synchronous Disk I/O (State Bottleneck)
* **What**: `saveRuntime()` ([`control.cpp:L125-L130`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/core/control.cpp#L125-L130)) opens, writes, formats, and closes `runtime.json` synchronously on disk on **every single state change** (e.g., inside every individual component switch like `thermal()`, `rgb()`, `setspeed()`, `setaltitude()`).
* **Why**: Performing synchronous file creation on every property mutation introduces disk I/O latency into the main thread. Additionally, reading `runtime.json` from another process while it is being rewritten can lead to reading incomplete JSON structures.
* **Conditions**: Becomes problematic as telemetry update frequency increases (e.g., updating position/battery 10 times a second).
* **Urgency**: **Medium** — Can wait until after basic simulation movement is working, but worth planning around early.

---

#### 4. Shared Log File Handle Contention (Runtime Safety Issue)
* **What**: Both [`control.cpp:L26`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/core/control.cpp#L26) and [`terminal.cpp:L18`](file:///c:/Dev/repos/Public%20repos/manar-search-rescue-drone/core/terminal.cpp#L18) open global file streams to the same file name: `ofstream fout("logs.txt")`.
* **Why**: Operating systems (especially Windows) do not permit uncoordinated concurrent writes to the same file path without explicit OS locks. Logs can become scrambled, truncated, or throw file access permission errors on launch.
* **Conditions**: Happens whenever `control.exe` and `terminal.exe` run simultaneously from the same directory.
* **Urgency**: **Low/Medium** — Simple fix (e.g., separate log filenames or let `control.cpp` exclusively handle runtime logs).

---

### Questions I Should Decide

1. **How detailed should the V1 flight kinematic simulation be?**
   - *Simple Kinematics (Recommended for V1)*: Calculate new `latitude` and `longitude` each second based on speed ($v$), heading angle to destination ($\theta$), and delta time ($dt$).
   - *Advanced Physics*: Include acceleration curves, wind drift, turning radiuses, or external simulator integration (e.g., PX4 SITL / Gazebo via MAVLink).
   
2. **How and when do you want to transition IPC from files to networking?**
   - *Option A (C++ Native Socket)*: Add a lightweight C++ WebSocket library (e.g. `uWebSockets`, `websocketpp`, or `cpp-httplib`) directly into `control.cpp`.
   - *Option B (Bridge Process)*: Keep C++ standard I/O (stdin/stdout) clean and use a lightweight bridge script (Node.js or Python) that hosts WebSockets for the React GUI and relays JSON messages to C++.

3. **Should commands be queued or single-instance?**
   - Should `commands.json` / IPC transport a list/queue of pending commands (so multiple rapid commands are all processed sequentially), or remain single-command with explicit ACK/NACK responses?

---

### Suggested Next Step

**Implement a basic position step generator in `control.cpp`.**

Before tackling networking or GUI integration, give your drone movement capability in the main control loop. 
In `main()` or `missionstatusupdater()`, when `flightlaunched` is `true` and the drone has a destination:
1. Compute the bearing/vector from the current position `(latitude, longitude)` toward `(destlat, destlon)`.
2. Move the drone's position forward by `distance = speed * dt` meters.
3. Check if the distance remaining is within `config["reached_radius"]`.

Once position updates work per iteration, you can run `control.exe` and `terminal.exe`, trigger a mission launch or lawnmower search, and watch `runtime.json` live-update realistic drone coordinates advancing across the search area!