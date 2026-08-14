# MANAR · Developer Log & Quick Reference

> Practical dev log and locked decision reference. Check this before writing code to avoid building deferred features or breaking locked architectures.

---

## ⚡ Quick Log Template (Copy & Paste for New Entries)

```markdown
### [YYYY-MM-DD] <Short Title>
- **Subsystem:** [Core | CV | GUI | Hardware | RF | Mission]
- **Change / Decision:** <What was added, changed, or locked>
- **Why:** <Key benchmark, constraint, or rationale>
- **Boundary / Do Not:** <Any constraints or forbidden scope>
```

---

## 🔒 Locked Architecture & Guardrails (Do Not Reinvent)

| Subsystem | Locked Decision | Authority / Rule | 🚫 What NOT to Build Yet |
| :--- | :--- | :--- | :--- |
| **Control Core** | C++17 deterministic engine | `control.cpp` owns canonical runtime state. No subsystem overwrites state directly. | No multi-master state; UI never mutates state directly. |
| **State Snapshot** | `runtime.json` | **On-demand snapshot/export only**, NOT live telemetry bus. | Do not stream continuous high-rate data through `runtime.json`. |
| **Telemetry** | WebSocket + JSON | 1–2 Hz normal (heartbeat/pos), 5 Hz in proximity, events immediate. | Do not blast raw sensor feeds over control socket. |
| **Path Planning** | Greedy O(n²) lawnmower | Practical deterministic route planning. | Do not write genetic/TSP/complex global optimizers. |
| **Visual CV** | **YOLO11n** (~29 ms ONNX) | Frame-level raw human detection (`PersonDetection` struct). | Do NOT build Kalman tracking, MOT, or fusion inside detector. |
| **Passive RF** | Level 1 CFAR anomaly trigger | Single low-SWaP receiver + adaptive CFAR threshold (3+ hits = INSPECT). | No AoA, KrakenSDR, direction finding, or beamforming. |
| **GUI & ML** | TypeScript + React (GUI) / PyTorch (ML) | GUI presents; PyTorch handles downstream fusion in later phase. | No hardcoded ML dependencies inside core control logic. |

---

## 📋 Active Engineering Log

### [2026-08-15] Visual Detector Baseline Locked (YOLO11n)
- **Subsystem:** Computer Vision (`computer_vision/`)
- **Decision:** **YOLO11n** (`models/yolo11n.onnx`) is locked as the MANAR V1 human detector. **D-FINE-N** (`models/dfine_n.onnx`) retained as reference baseline.
- **Why:** Benchmarked across 26 SAR videos. YOLO11n achieves **29.2 ms** latency (~34 FPS CPU) vs **50.5 ms** for D-FINE-N (~1.73× faster), 0 false positives on clutter, and better concealment detection.
- **Boundary:** YOLO11n outputs raw frame-level detections only. Candidate tracking, temporal confidence smoothing, and multi-sensor fusion are deferred to the fusion pipeline.

### [2026-08-14] Core Mission & Control Prototype
- **Subsystem:** Core Engine (`core/`)
- **Decision:** Modularized C++ control architecture (`control`, `flight`, `mission`, `drone`, `components`, `route_optimizer`).
- **Features:** Greedy O(n²) multi-location search sequencing, battery-save RTL, configurable slots, batch payload control.
- **Boundary:** `control.cpp` is single source of truth. `runtime.json` is exported on-demand for operator visibility.

---

## 🎯 Next Priorities & Deferred Roadmap

### Immediate Next Priorities
- [ ] Ground hardware specs (payload mass, power draw, dimensions) in real components.
- [ ] Calculate propulsion thrust-to-weight and battery capacity for 1-hour active search budget.
- [ ] Build TypeScript / React Operator GUI and connect via WebSocket.
- [ ] Design Python / PyTorch multisensor fusion module interfacing with C++ core.

### Deferred Scope (Do Not Touch Yet)
- ⏳ **Perception & Tracking:** Kalman filtering, ByteTrack/MOT candidate IDs, spatial GPS projection, thermal/RGB registration.
- ⏳ **Sensor Fusion:** Combining YOLO11n + 24 GHz FMCW radar + RF anomalies + audio direction finding.
- ⏳ **Multi-Rescuee Management:** Tracking and managing >1 simultaneous rescuee states.
- ⏳ **Battery Prediction:** Time-to-critical battery dynamic forecasting.
- ⏳ **Advanced RF:** Direction-finding, Angle of Arrival (AoA), coherent RF imaging.
- ⏳ **FMCW Radar Band Selection:** Selecting and testing optimal frequency bands (e.g. 24 GHz vs 60/77 GHz).
