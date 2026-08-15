# MANAR Engineering Decisions

## Mamba for temporal sensor fusion
**Decision:** Use Mamba for time-based multisensor fusion.

**Why:** Linear sequence complexity and good fit for continuous sensor streams.

**Alternatives considered:** LSTM, GRU, Transformer.

**Status:** Planned

---

## YOLO11n for RGB detection
**Decision:** Use YOLO11n as the V1 RGB human-detection baseline.

**Why:** Best balance from current testing between latency, detection quality, and model size.

**Alternatives considered:** D-FINE-N.

**Status:** Accepted

---

## Greedy route optimization
**Decision:** Use greedy nearest-neighbor ordering for multiple search locations.

**Why:** Simple, deterministic, fast, and sufficient for the small number of operator-defined locations.

**Alternatives considered:** Exact shortest-route search.

**Status:** Accepted
