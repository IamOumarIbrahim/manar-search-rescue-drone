# MANAR Operator Guide & Mission Simulation Walkthrough

This document provides complete instructions for configuring, launching, and executing simulated search-and-rescue missions using the MANAR C++ Core and Interactive Operator Terminal.

---

## 1. System Requirements & Build

### Prerequisites
- **Operating System**: Windows 10/11, Linux, or macOS
- **Compiler**: C++17 compliant compiler (`g++` / MinGW-w64, Clang, or MSVC)

### Build Instructions

From the repository root, compile the three core applications:

```powershell
cd core
mkdir -p build

# 1. Compile Control Core
g++ -std=c++17 apps/control.cpp system/shared.cpp system/flight.cpp system/components.cpp system/drone.cpp system/mission.cpp system/route_optimizer.cpp -I. -Ithird_party -o build/control.exe

# 2. Compile Operator Terminal
g++ -std=c++17 apps/terminal.cpp -I. -Ithird_party -o build/terminal.exe

# 3. Compile Setup Utility
g++ -std=c++17 apps/setup.cpp -I. -Ithird_party -o build/setup.exe
```

---

## 2. Pre-Flight Configuration (Optional)

Run the interactive setup utility to configure flight profiles, homebase coordinates, or battery thresholds:

```powershell
.\build\setup.exe
```

Available configurations:
- **Slot Selection**: Choose active configuration profile (Slots 1–3).
- **Homebase Coordinates**: Set origin GPS latitude and longitude.
- **Flight Limits**: Configure maximum operational altitude and search speed.
- **Failsafe Thresholds**: Configure battery-save trigger (%) and critical RTL reserve (%).

---

## 3. Launching the System

MANAR runs as a decoupled client-server architecture via structured JSON IPC (`core/runtime/`):

1. **Terminal 1 (Flight Core Engine)**:
   ```powershell
   cd core
   .\build\control.exe
   ```
   *Runs the deterministic 10 Hz state machine, telemetry broadcaster, and flight dynamics simulation.*

2. **Terminal 2 (Operator Command Station)**:
   ```powershell
   cd core
   .\build\terminal.exe
   ```
   *Provides the interactive menu-driven interface for mission control.*

---

## 4. End-to-End Mission Walkthrough

Follow this step-by-step sequence to simulate a complete aerial search-and-rescue operation:

### Step 1: Set Search Locations (`Option 1`)
- Input multiple GPS target waypoints:
  ```text
  25.336421, 55.344471
  25.338500, 55.346200
  25.341200, 55.348100
  F
  ```
- Prompt: **Enable Greedy Route Optimization? (Y/N)**
  - Press `Y` to automatically sequence waypoints using the $O(n^2)$ nearest-neighbor solver to minimize transit distance.

### Step 2: Start Mission & Launch Drone (`Option 2` then `Option 4`)
- `Option 2 (Start Mission)`: Validates and locks the active search plan.
- `Option 4 (Launch Drone)`: Triggers automated pre-flight checks, takeoff sequence, and transition to search cruise.

### Step 3: Monitor Live Telemetry (`Option 3`)
- Displays real-time state:
  - Flight Mode (`Quick`, `Active`, `Inspect`, `Hover`)
  - Current GPS Coordinates & Altitude
  - Battery SOC (%) & Estimated Endurance
  - Active Target Waypoint & Progress

### Step 4: Sensor & Payload Management (`Option 6`)
- Dynamically toggle individual or batch payload systems:
  - **Sensors**: Long-Wave Thermal IR, Daytime RGB, 24 GHz FMCW Radar, Passive RF Receiver.
  - **Actuators & Signaling**: 360° Amber Beacon, Directional White Strobe, High-Intensity Spotlight, Smoke Marker.

### Step 5: Adjust Flight Profile on the Fly (`Option 5`)
- Switch operational modes depending on terrain and visibility:
  - `Quick`: High-speed transit between search sectors.
  - `Active`: Standard lawnmower sweep with multisensor scanning.
  - `Inspect`: Low-altitude close-range verification.
  - `Hover`: Stationary hovering for multi-angle visual/acoustic target acquisition.

### Step 6: Target Confirmation & Rescue Dispatch (`Option 7` / `Option 8`)
- Upon receiving a target candidate detection from the perception pipeline:
  - Confirm target visually / through sensor fusion.
  - Transmit exact geolocated GPS coordinates to ground rescue teams.

### Step 7: Return-to-Launch (`Option 0`)
- Command a manual RTL or allow automatic battery-reserve RTL protection to return the drone safely to homebase coordinates and execute an automated landing.
