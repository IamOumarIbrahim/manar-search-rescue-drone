# MANAR Perception · Mamba SSM Multi-Sensor Temporal Fusion

Research module investigating **Mamba State Space Models (SSM)** for real-time temporal sensor fusion across multi-modal aerial search-and-rescue telemetry.

---

## Concept & Purpose

During search-and-rescue sweeps, isolated single-frame detections (from RGB, Infrared, or Thermal cameras) can suffer from brief occlusions, thermal shadows, or environmental clutter. MANAR investigates temporal sequence modeling to filter alerts before escalating them to human operators.

```text
┌────────────────────────────────────────────────────────┐
│ Synchronized 10 Hz Multisensor Feature Window (2–5 s)   │
│  - Visual Bounding Box & 1D Embedding (YOLO11n)        │
│  - 24 GHz FMCW Micro-Doppler & Range Returns           │
│  - Passive RF CFAR Anomaly Derivative (ΔSk)             │
│  - Acoustic Peak Spectral Energy                       │
└──────────────────────────┬─────────────────────────────┘
                           │
                           ▼
┌────────────────────────────────────────────────────────┐
│ Mamba SSM Sequential Evaluator (Linear O(L) Complexity)│
│  - Linear-time temporal attention over feature stream  │
│  - Minimal compute/memory footprint on edge companion   │
└──────────────────────────┬─────────────────────────────┘
                           │
                           ▼
┌────────────────────────────────────────────────────────┐
│ Candidate Escalation State                             │
│  [ REJECT ]  ──>  [ CONTINUE HOVER ]  ──>  [ ALERT ]   │
└────────────────────────────────────────────────────────┘
```

---

## Why Mamba SSM (Planned Design)?
- **Linear $O(L)$ Sequence Complexity**: Unlike Transformers with quadratic $O(L^2)$ attention, Mamba scales linearly with sequence length, making 10 Hz continuous sliding-window evaluations feasible on low-SWaP edge companion computers.
- **Selective State Update**: Selectively forgets high-frequency terrain clutter noise while maintaining long-range temporal state for subtle biometric/radar/thermal cues.
- **Alert Gate Role**: Mamba acts strictly as an automated confidence filter to escalate potential target candidates; final confirmation and rescue dispatch remain reserved for the human operator.

---

## Status
- **Current Phase**: Architectural specification, feature vector definition, and model prototyping (`model.py`).
- **Upcoming Work**: Synthetic sensor dataset generation, fusion bus alignment, and integration with the C++ perception pipeline.
