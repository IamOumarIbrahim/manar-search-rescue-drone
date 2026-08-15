# MANAR Computer Vision · Visual Target Detection & Benchmark

Supervised-autonomy computer vision pipeline for aerial search-and-rescue target detection. This directory houses the selected **MANAR V1 YOLO11n detector module**, the **D-FINE-N reference baseline**, and the empirical multisensor evaluation records running on **ONNX Runtime C++** and **OpenCV**.

```
┌─────────────────┐     ┌──────────────────────┐     ┌───────────────────┐     ┌──────────────────┐
│  Camera Frame   │ ──> │ Yolo11nDetector      │ ──> │ Raw Detections    │ ──> │ MANAR Perception │
│ (RGB/IR/Thermal)│     │ (Preprocessing + ORT)│     │ (BBox, Conf, Cls) │     │ / System Fusion  │
└─────────────────┘     └──────────────────────┘     └───────────────────┘     └──────────────────┘
```

---

## Model Status Matrix

| Model | Architecture | Status | Purpose in MANAR |
| :--- | :--- | :--- | :--- |
| **YOLO11n** | Ultralytics YOLO11 Nano (~2.6M params) | **Selected MANAR V1 Detector** | Primary raw visual human candidate detector |
| **D-FINE-N** | Fine-grained Distribution Refinement DETR Nano (~4.0M params) | **Reference Baseline** | Evaluated reference benchmark detector |

---

## Directory Structure

```
perception/vision/
├── apps/
│   ├── dfine_video.cpp        # D-FINE-N reference video test application
│   ├── dfine_live.cpp         # D-FINE-N reference live stream test application
│   ├── yolo11n_video.cpp      # YOLO11n video test harness application
│   └── yolo11n_live.cpp       # YOLO11n live camera/stream test application
│
├── include/
│   └── yolo11n_detector.hpp   # Reusable YOLO11n detector interface header
│
├── src/
│   └── yolo11n_detector.cpp   # Reusable YOLO11n detector implementation
│
├── models/
│   ├── dfine_n.onnx           # Frozen D-FINE-N reference baseline model
│   └── yolo11n.onnx           # Frozen YOLO11n MANAR V1 detector model
│
├── benchmarks/
│   ├── batch_benchmark.py     # Batch runner executing benchmark across training sets
│   └── results.md             # Complete empirical benchmark records
│
├── tests/
│   └── test_pipeline.py       # Quick Python verification harness
│
├── output/
│   └── .gitkeep               # Generated test run directory
├── temp/
│   └── .gitkeep               # Generated evaluation artifacts directory
│
├── CMakeLists.txt             # CMake build configuration for library & test targets
└── README.md                  # Subsystem guide and model specifications
```

---

## Integration Module: `Yolo11nDetector`

The detector is encapsulated in [`include/yolo11n_detector.hpp`](include/yolo11n_detector.hpp) and [`src/yolo11n_detector.cpp`](src/yolo11n_detector.cpp). It has a narrow responsibility: receiving an external `cv::Mat` frame and returning a list of `PersonDetection` structs.

### Data Structure
```cpp
struct PersonDetection {
    cv::Rect2f box;      // Bounding box in original frame pixel coordinates [x, y, width, height]
    float confidence;    // Detection confidence score [0.0, 1.0]
    int classId{0};      // Target class index (0 = COCO person)
};
```

### Integration Example
```cpp
#include "yolo11n_detector.hpp"

// 1. Initialize detector (loads ONNX Runtime session once)
Yolo11nDetector detector("models/yolo11n.onnx", 0.50f, 0.45f);

if (!detector.isLoaded()) {
    // Handle error
}

// 2. Pass any incoming frame from camera or sensor pipeline
cv::Mat frame = ...;
double inferMs = 0.0;
std::vector<PersonDetection> candidates = detector.detect(frame, &inferMs);

// 3. Process raw detections in downstream perception layers
for (const auto& candidate : candidates) {
    // candidate.box, candidate.confidence, candidate.classId
}
```

---

## Model Checksums & Lockdown

To guarantee reproducibility and prevent regression from re-exports, model artifacts are locked:

| Property | YOLO11n (Selected Detector) | D-FINE-N (Reference Baseline) |
| :--- | :--- | :--- |
| **Model Filename** | `models/yolo11n.onnx` | `models/dfine_n.onnx` |
| **Architecture** | YOLO11 Nano (Ultralytics) | D-FINE-N (Distribution Refinement DETR) |
| **Parameters** | ~2.6M | ~4.0M |
| **Training Base** | COCO 2017 Pretrained | COCO 2017 Pretrained |
| **Input Shape** | `[1, 3, 640, 640]` float32 | `[1, 3, 640, 640]` float32 |
| **File Size** | `10,720,228 bytes` (10.22 MB) | `15,258,358 bytes` (14.55 MB) |
| **SHA-256 Hash** | `7d8fd1717d9d5bbab6986cd134afb620649c7a394303d55b1e09fc00804cc5c1` | `0f684f409618ee8a822410e754a29caa817d1aa16283ce89cad936d0a48e2f35` |
| **MD5 Hash** | `8dc8b2174d5e75eadd793d8070c64fe0` | `0b6911911a60839600fd10be563cd016` |

---

## Empirical Benchmark Results (26 Video Evaluation)

All 26 training and evaluation videos in `assets/Videos/Training videos/` were processed under identical benchmark controls (`INPUT_SIZE = 640`, `CONFIDENCE = 0.50`, `PERSON_CLASS = 0`, CPU execution provider).

### Benchmark Comparison Matrix

| # | Video Dataset & Scenario | Modality / Condition | D-FINE-N Latency | YOLO11n Latency | D-FINE Hits | YOLO11 Hits |
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
| **AVG** | **Full 26 Video Benchmark Average** | — | **50.5 ms** | **29.2 ms** | — | — |

### Empirical Findings
* **Inference Speed**: YOLO11n averaged **29.2 ms** per frame (~34.2 FPS on CPU), running ~1.73× faster than D-FINE-N (**50.5 ms** per frame, ~19.8 FPS).
* **Negative Rejection**: Both detectors exhibited **0 false detections** on all clean-negative and environmental clutter videos across RGB, Infrared, and Thermal streams.
* **Sensitivity**: YOLO11n yielded higher candidate detection totals in stationary, concealed, thermal, and multi-target scenarios.
* **IR Dynamic Motion**: D-FINE-N produced higher detection volume on the moving infrared target scenario (`273` vs `182` detections).

---

## Building & Running

### Requirements
- **C++17** compiler
- **OpenCV 4.x**
- **ONNX Runtime 1.14+**

### CMake Build
```bash
cd perception/vision
mkdir build && cd build

cmake .. -DOpenCV_DIR="C:/path/to/opencv/build" -DONNXRUNTIME_ROOT_DIR="C:/path/to/onnxruntime"
cmake --build . --config Release
```

Build targets generated:
- `manar_yolo11n_detector` (Static integration library)
- `yolo11n_video` (YOLO11n video test harness)
- `yolo11n_live` (YOLO11n live camera/RTSP harness)
- `dfine_video` (D-FINE-N reference video harness)
- `dfine_live` (D-FINE-N reference live harness)
