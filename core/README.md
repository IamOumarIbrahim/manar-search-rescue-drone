# MANAR C++ Deterministic Flight Control Core

Deterministic C++17 control system engine, mission state machine, and IPC communication bridge for the MANAR Search-and-Rescue aerial platform.

---

## Subsystem Architecture

```text
core/
├── apps/
│   ├── control.cpp            # Main control engine daemon & flight loop
│   ├── terminal.cpp           # Interactive CLI operator command station
│   └── setup.cpp              # Pre-flight parameter & slot configuration utility
│
├── system/
│   ├── flight.hpp / .cpp      # Flight dynamics, speed limits, kinematics, altitude
│   ├── drone.hpp / .cpp       # Drone state representation & battery monitoring
│   ├── mission.hpp / .cpp     # Mission state machine, RTL preemption, lawnmower sweeps
│   ├── components.hpp / .cpp  # Sensor & actuator payload toggles (Thermal, FMCW, Lights)
│   ├── route_optimizer.hpp / .cpp # O(n²) Greedy nearest-neighbor route solver
│   └── shared.hpp / .cpp      # Timestamping, common utilities, and string parsing
│
├── configs/                   # Named persistent configuration profiles (Slot 1–3)
├── runtime/                   # Active JSON IPC exchange files (commands.json, runtime.json)
└── third_party/               # Header-only dependencies (nlohmann/json)
```

---

## Applications

| Application | Role |
| :--- | :--- |
| **`control.exe`** | Primary flight engine running a 10 Hz deterministic loop. Manages state machine transitions (`STANDBY`, `TAKEOFF`, `SEARCH`, `INSPECT`, `HOVER`, `RTL`, `LANDED`), processes operator commands via `runtime/commands.json`, and streams telemetry into `runtime/runtime.json`. |
| **`terminal.exe`** | Interactive menu-driven operator command console. Allows real-time mission dispatch, waypoint input, sensor toggling, telemetry inspection, and manual override. |
| **`setup.exe`** | Interactive configuration utility to adjust homebase GPS coordinates, speed/altitude limits, and battery failsafe triggers across named slots. |

---

## Compilation

```powershell
mkdir -p build

# Compile all applications
g++ -std=c++17 apps/control.cpp system/shared.cpp system/flight.cpp system/components.cpp system/drone.cpp system/mission.cpp system/route_optimizer.cpp -I. -Ithird_party -o build/control.exe
g++ -std=c++17 apps/terminal.cpp -I. -Ithird_party -o build/terminal.exe
g++ -std=c++17 apps/setup.cpp -I. -Ithird_party -o build/setup.exe
```

For complete mission execution steps and operator workflows, see the **[Operator Guide](../docs/OPERATOR_GUIDE.md)**.
