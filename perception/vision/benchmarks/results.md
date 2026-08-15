# MANAR Computer Vision — Comparative Benchmark

## D-FINE-N vs YOLO11n Empirical Evaluation

This document records the empirical results of evaluating the **D-FINE-N reference baseline** against the **YOLO11n candidate detector** across 26 search-and-rescue test video scenarios.

### Full 26-Video Comparative Benchmark Matrix

| # | Test Video Scenario | Modality / Condition | D-FINE-N Latency | YOLO11n Latency | D-FINE Hits | YOLO11 Hits |
| :-: | :--- | :--- | :---: | :---: | :---: | :---: |
| 1 | `First round/Infra False Positive.mp4` | IR Clutter / Negative | 51.3 ms | 29.7 ms | 0 | 0 |
| 2 | `First round/Infra No Target.mp4` | IR Clean Rejection | 50.6 ms | 29.3 ms | 0 | 0 |
| 3 | `First round/Infra Partially Concealed Rescuee.mp4` | IR Concealed Target | 53.6 ms | 30.6 ms | 0 | 16 |
| 4 | `First round/RGB False Positive.mp4` | RGB Clutter / Negative | 50.6 ms | 29.7 ms | 0 | 0 |
| 5 | `First round/RGB No Target.mp4` | RGB Clean Rejection | 49.7 ms | 29.5 ms | 0 | 0 |
| 6 | `First round/RGB Partially Concealed Rescuee.mp4` | RGB Concealed Target | 50.5 ms | 31.8 ms | 0 | 24 |
| 7 | `First round/Thermal False Positive.mp4` | Thermal Clutter / Negative | 64.5 ms | 28.2 ms | 0 | 0 |
| 8 | `First round/Thermal No Target.mp4` | Thermal Clean Rejection | 47.9 ms | 28.7 ms | 0 | 0 |
| 9 | `First round/Thermal Partially Concealed Rescuee.mp4` | Thermal Concealed Target | 49.5 ms | 28.9 ms | 0 | 4 |
| 10 | `Low-Light IR False Positive.mp4` | Low-Light IR Clutter | 50.9 ms | 29.7 ms | 0 | 0 |
| 11 | `Low-Light IR No Target.mp4` | Low-Light IR Rejection | 51.6 ms | 28.3 ms | 0 | 0 |
| 12 | `Low-Light IR Partially Concealed Rescuee.mp4` | Low-Light IR Concealed | 50.7 ms | 28.5 ms | 0 | 0 |
| 13 | `RGB False Positive.mp4` | RGB Clutter / Negative | 48.9 ms | 28.1 ms | 0 | 0 |
| 14 | `RGB No Target.mp4` | RGB Clean Rejection | 48.3 ms | 29.0 ms | 0 | 0 |
| 15 | `RGB Partially Concealed Rescuee.mp4` | RGB Concealed Target | 48.8 ms | 29.7 ms | 0 | 24 |
| 16 | `Second round/Clear/Infra Clear Rescuee.mp4` | IR Still Target | 49.1 ms | 28.6 ms | 0 | 0 |
| 17 | `Second round/Clear/RGB Clear Rescuee.mp4` | RGB Still Target | 49.5 ms | 28.7 ms | 27 | 118 |
| 18 | `Second round/Clear/Thermal Clear Rescuee.mp4` | Thermal Still Target | 48.8 ms | 28.4 ms | 0 | 17 |
| 19 | `Second round/Moving/Infra Moving Rescuee.mp4` | IR Moving Target | 50.1 ms | 28.3 ms | **273** | 182 |
| 20 | `Second round/Moving/RGB Moving Rescuee.mp4` | RGB Moving Target | 49.6 ms | 29.1 ms | 51 | **220** |
| 21 | `Second round/Moving/Thermal Moving Rescuee.mp4` | Thermal Moving Target | 49.3 ms | 28.9 ms | 10 | **34** |
| 22 | `Second round/Multiple/Infra RGB Multiple Rescuees.mp4` | IR Multiple Targets | 53.1 ms | 29.5 ms | 39 | **97** |
| 23 | `Second round/Multiple/RGB Multiple Rescuees.mp4` | RGB Multiple Targets | 49.0 ms | 29.8 ms | 492 | **691** |
| 24 | `Second round/Multiple/Thermal RGB Multiple Rescuees.mp4` | Thermal Multiple Targets | 49.5 ms | 29.4 ms | 0 | **70** |
| 25 | `Thermal No Target.mp4` | Thermal Clean Rejection | 48.8 ms | 30.3 ms | 0 | 0 |
| 26 | `Thermal Partially Concealed Rescuee.mp4` | Thermal Concealed Target | 49.5 ms | 29.8 ms | 0 | 0 |
| **AVG** | **Overall 26-Video Dataset Average** | — | **50.5 ms** | **29.2 ms** | — | — |

---

## Key Empirical Takeaways

1. **Inference Latency**
   - **YOLO11n**: **29.2 ms average** (~34.2 FPS on CPU).
   - **D-FINE-N**: **50.5 ms average** (~19.8 FPS on CPU).
   - YOLO11n demonstrates **~42.2% lower inference latency** and is approximately **1.73× faster**.

2. **Clean Rejection / False Positives**
   - Both models produced **0 false detections** across all `No Target` and `False Positive` environmental clutter videos.

3. **Target Sensitivity**
   - YOLO11n produced higher raw detection counts in stationary, moving, concealed, thermal, and multiple-target scenarios.
   - D-FINE-N produced higher detection volume in the **IR moving-target** test (**273 vs 182**).
   - Both models failed the low-contrast IR still-target and thermal concealed-target clips.

4. **Model Selection**
   - **YOLO11n is selected as the MANAR V1 raw visual human detector.**
   - **D-FINE-N is retained as the reference benchmark implementation.**
   - Raw detection counts represent direct frame-level outputs; accuracy/recall normalization will be addressed with ground-truth target bounding box annotations in downstream perception layers.

---

## Scope

This benchmark covers raw frame-level human detection only.

Not included in this layer:
- Candidate persistence
- Tracking / Kalman filtering
- Missed-frame handling
- Temporal confidence smoothing
- Multisensor fusion
- Rescuee confirmation logic
- Mission control logic
