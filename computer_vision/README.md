# MANAR Computer Vision · D-FINE-N Vision Pipeline

Supervised-autonomy computer vision pipeline for aerial search-and-rescue target detection using **D-FINE-N** (Distribution-refined DETR) and **ONNX Runtime C++**.

```
┌─────────────────┐     ┌──────────────┐     ┌────────────────┐     ┌───────────────┐     ┌──────────────────┐
│ MP4 / Live Feed │ ──> │ OpenCV Decode│ ──> │ D-FINE-N ONNX  │ ──> │ Person Boxes  │ ──> │ Live Window      │
│  Camera Stream  │     │ & Letterbox  │     │ (ONNX Runtime) │     │ (Class 0 / S&R│     │ & output.mp4     │
└─────────────────┘     └──────────────┘     └────────────────┘     └───────────────┘     └──────────────────┘
```

---

## Directory Structure

```
computer_vision/
├── CMakeLists.txt         # CMake build configuration (dfine_video & dfine_live)
├── README.md              # Documentation, model lockdown & test logs
├── dfine_video.cpp        # C++ video file inference pipeline
├── dfine_live.cpp         # C++ live camera & RTSP stream pipeline
├── models/
│   └── dfine_n.onnx       # Locked baseline ONNX model graph
└── output/
    └── output.mp4         # Rendered annotated output
```

---

## Evaluation & Test Record

```yaml
Model: D-FINE-N
Dataset training: COCO
Purpose in MANAR: Human candidate detection
```

### Modality Test Observations

#### RGB:
- Strong moving-target detection
- Moderate stationary detection
- Weak partially concealed detection

#### Infrared:
- Strong moving-target detection
- Variable stationary performance
- Some useful concealed-target detection

#### Thermal:
- White-hot / black-cold grayscale
- Intermittent detection
- Not relied upon as sole detector

### Decision:
**Accepted as MANAR V1 RGB detector baseline.**

---

## Baseline Model Lockdown (Frozen Weights)

To guarantee deterministic reproducibility across future benchmark evaluations and prevent silent regression from re-exports, the exact evaluated `.onnx` model artifact is locked as follows:

| Property | Value |
| :--- | :--- |
| **Model Filename** | `models/dfine_n.onnx` |
| **Architecture** | D-FINE-N (Fine-grained Distribution Refinement DETR Nano) |
| **Parameters** | ~4.0M |
| **Source Config** | `configs/dfine/dfine_hgnetv2_n_coco.yml` |
| **Training Base** | COCO 2017 Dataset Pretrained |
| **File Size** | `15,258,358 bytes` (14.55 MB) |
| **SHA-256 Hash** | `0f684f409618ee8a822410e754a29caa817d1aa16283ce89cad936d0a48e2f35` |
| **MD5 Hash** | `0b6911911a60839600fd10be563cd016` |
| **Input Shape** | `[1, 3, 640, 640]` float32 (RGB normalized $[0.0, 1.0]$) |
| **Output Shapes** | `logits: [1, 300, 80]`, `pred_boxes: [1, 300, 4]` |

> [!IMPORTANT]
> Do not replace or overwrite `models/dfine_n.onnx` without updating the validation hash and logging a formal baseline transition.

---

## Building & Running

### Requirements
- **C++17** compiler (MSVC 2019+, GCC 9+, or Clang 11+)
- **OpenCV 4.x** (C++ development headers and binaries)
- **ONNX Runtime 1.14+** (C++ API headers and library)

### Build with CMake
```bash
cd computer_vision
mkdir build && cd build

# Configure build targets
cmake .. -DOpenCV_DIR="C:/path/to/opencv/build" -DONNXRUNTIME_ROOT_DIR="C:/path/to/onnxruntime"

# Compile both video and live executables
cmake --build . --config Release
```

### Running the Programs

#### 1. Recorded Video File Pipeline (`dfine_video`)
```bash
./dfine_video
```
Prompts for an MP4 file path, renders candidate bounding boxes in a real-time playback window, and writes the annotated output stream to `output/output.mp4`.

#### 2. Live Camera / RTSP Stream Feed (`dfine_live`)
```bash
./dfine_live
```
Connects to an onboard camera (Device index `0`, `1`) or an RTSP sensor stream, performing live frame-by-frame inference with latency HUD and real-time human candidate count diagnostics.
