"""
MANAR Computer Vision - Benchmark Test Harness & Pipeline Verifier
Supports D-FINE-N (Baseline) and YOLO11n (Candidate) models in ONNX format.
"""

import os
import sys
import time
import cv2
import numpy as np
import onnxruntime as ort

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
CV_ROOT = os.path.dirname(SCRIPT_DIR)

DFINE_MODEL_PATH = os.path.join(CV_ROOT, "models", "dfine_n.onnx")
YOLO_MODEL_PATH = os.path.join(CV_ROOT, "models", "yolo11n.onnx")
OUTPUT_DIR = os.path.join(CV_ROOT, "output")

INPUT_SIZE = 640
CONFIDENCE_THRESHOLD = 0.50
NMS_THRESHOLD = 0.45
PERSON_CLASS = 0


def clean_path(path: str) -> str:
    path = path.strip()
    if path.startswith('"') and path.endswith('"'):
        path = path[1:-1]
    return path


def letterbox_image(image: np.ndarray, target_size: int = 640):
    h, w = image.shape[:2]
    ratio = min(target_size / w, target_size / h)
    new_w = int(w * ratio)
    new_h = int(h * ratio)

    resized = cv2.resize(image, (new_w, new_h), interpolation=cv2.INTER_LINEAR)

    pad_x = (target_size - new_w) // 2
    pad_y = (target_size - new_h) // 2

    canvas = np.zeros((target_size, target_size, 3), dtype=np.uint8)
    canvas[pad_y : pad_y + new_h, pad_x : pad_x + new_w] = resized

    return canvas, ratio, pad_x, pad_y, new_w, new_h


def sigmoid(x):
    return 1.0 / (1.0 + np.exp(-np.clip(x, -20.0, 20.0)))


def run_pipeline(video_path: str, model_path: str = DFINE_MODEL_PATH):
    model_name = os.path.basename(model_path)
    is_yolo = "yolo" in model_name.lower()
    tag = "YOLO11N" if is_yolo else "D-FINE-N"

    print("=" * 55)
    print(f"   MANAR {tag} RESCUE VISION BENCHMARK TEST   ")
    print("=" * 55)

    if not os.path.exists(model_path):
        print(f"\n[!] Model file not found at: {model_path}")
        if is_yolo:
            print("-----------------------------------------------------------")
            print("To export the official YOLO11n ONNX model:")
            print("  1. pip install ultralytics")
            print("  2. yolo export model=yolo11n.pt format=onnx imgsz=640 dynamic=False opset=17")
            print("  3. copy yolo11n.onnx models\\yolo11n.onnx")
            print("-----------------------------------------------------------")
        return

    # Load ONNX Runtime session
    opts = ort.SessionOptions()
    opts.graph_optimization_level = ort.GraphOptimizationLevel.ORT_ENABLE_ALL
    session = ort.InferenceSession(model_path, sess_options=opts, providers=["CPUExecutionProvider"])

    input_names = [i.name for i in session.get_inputs()]
    output_names = [o.name for o in session.get_outputs()]
    print(f"[+] Model Loaded:  {model_name}")
    print(f"    Input Tensors:  {input_names}")
    print(f"    Output Tensors: {output_names}")

    cap = cv2.VideoCapture(video_path)
    if not cap.isOpened():
        print(f"[!] Error: Could not open video '{video_path}'")
        return

    os.makedirs(OUTPUT_DIR, exist_ok=True)
    out_filename = "yolo11n_output.mp4" if is_yolo else "output.mp4"
    output_path = os.path.join(OUTPUT_DIR, out_filename)

    width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
    height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
    fps = cap.get(cv2.CAP_PROP_FPS) or 30.0
    total_frames = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))

    fourcc = cv2.VideoWriter_fourcc(*"mp4v")
    writer = cv2.VideoWriter(output_path, fourcc, fps, (width, height))

    print(f"\n[+] Processing '{video_path}' ({width}x{height} @ {fps:.1f} FPS)")
    print(f"[+] Output will be saved to: {output_path}")
    print("[+] Live view window active. Press [ESC] or 'q' to stop.")

    frame_idx = 0
    total_infer_ms = 0.0

    while cap.isOpened():
        ret, frame = cap.read()
        if not ret:
            break

        frame_idx += 1

        # 1. Letterbox preprocessing
        input_img, ratio, pad_x, pad_y, new_w, new_h = letterbox_image(frame, INPUT_SIZE)

        # 2. RGB + Normalize + NCHW float32
        rgb = cv2.cvtColor(input_img, cv2.COLOR_BGR2RGB)
        tensor = (rgb.astype(np.float32) / 255.0).transpose(2, 0, 1)
        tensor = np.expand_dims(tensor, axis=0)

        # 3. Model Inference (measure raw inference time only)
        if is_yolo:
            t0 = time.perf_counter()
            out = session.run(output_names, {input_names[0]: tensor})
            t1 = time.perf_counter()
            infer_ms = (t1 - t0) * 1000.0

            # Parse YOLO output [1, 84, 8400] or [1, 8400, 84]
            raw = out[0][0]
            if raw.shape[0] < raw.shape[1]:  # [84, 8400]
                dimensions, anchors = raw.shape
                is_transposed = False
            else:  # [8400, 84]
                anchors, dimensions = raw.shape
                is_transposed = True

            cand_boxes = []
            cand_scores = []

            for i in range(anchors):
                if not is_transposed:
                    cx, cy, w, h = raw[0, i], raw[1, i], raw[2, i], raw[3, i]
                    score = raw[4 + PERSON_CLASS, i]
                else:
                    cx, cy, w, h = raw[i, 0], raw[i, 1], raw[i, 2], raw[i, 3]
                    score = raw[i, 4 + PERSON_CLASS]

                if score >= CONFIDENCE_THRESHOLD:
                    bx = cx - (w / 2.0)
                    by = cy - (h / 2.0)
                    cand_boxes.append([int(bx), int(by), int(w), int(h)])
                    cand_scores.append(float(score))

            indices = cv2.dnn.NMSBoxes(cand_boxes, cand_scores, CONFIDENCE_THRESHOLD, NMS_THRESHOLD)
            detections = []
            if len(indices) > 0:
                for idx in indices.flatten():
                    bx, by, bw, bh = cand_boxes[idx]
                    score = cand_scores[idx]
                    x1 = max(0, min(width - 1, (bx - pad_x) / ratio))
                    y1 = max(0, min(height - 1, (by - pad_y) / ratio))
                    x2 = max(0, min(width - 1, (bx + bw - pad_x) / ratio))
                    y2 = max(0, min(height - 1, (by + bh - pad_y) / ratio))
                    detections.append((x1, y1, x2, y2, score))

        elif "orig_target_sizes" in input_names:
            # Official D-FINE exported graph format
            orig_sizes = np.array([[INPUT_SIZE, INPUT_SIZE]], dtype=np.int64)
            feed = {input_names[0]: tensor, "orig_target_sizes": orig_sizes}

            t0 = time.perf_counter()
            out = session.run(None, feed)
            t1 = time.perf_counter()
            infer_ms = (t1 - t0) * 1000.0

            labels, raw_boxes, raw_scores = out[0][0], out[1][0], out[2][0]
            detections = []
            for label, box, score in zip(labels, raw_boxes, raw_scores):
                if score >= CONFIDENCE_THRESHOLD and int(label) == PERSON_CLASS:
                    x1 = max(0, min(width - 1, (box[0] - pad_x) / ratio))
                    y1 = max(0, min(height - 1, (box[1] - pad_y) / ratio))
                    x2 = max(0, min(width - 1, (box[2] - pad_x) / ratio))
                    y2 = max(0, min(height - 1, (box[3] - pad_y) / ratio))
                    detections.append((x1, y1, x2, y2, float(score)))

        else:
            # DETR / HuggingFace format
            feed = {input_names[0]: tensor}

            t0 = time.perf_counter()
            out = session.run(None, feed)
            t1 = time.perf_counter()
            infer_ms = (t1 - t0) * 1000.0

            logits = out[0][0] if len(out[0].shape) == 3 else out[1][0]
            boxes = out[1][0] if len(out[1].shape) == 3 else out[0][0]

            probs = sigmoid(logits)
            pred_classes = np.argmax(probs, axis=-1)
            pred_scores = np.max(probs, axis=-1)

            detections = []
            for cls_idx, score, box in zip(pred_classes, pred_scores, boxes):
                if score >= CONFIDENCE_THRESHOLD and int(cls_idx) == PERSON_CLASS:
                    cx, cy, bw, bh = box
                    bx1 = (cx - bw / 2.0) * INPUT_SIZE
                    by1 = (cy - bh / 2.0) * INPUT_SIZE
                    bx2 = (cx + bw / 2.0) * INPUT_SIZE
                    by2 = (cy + bh / 2.0) * INPUT_SIZE

                    x1 = max(0, min(width - 1, (bx1 - pad_x) / ratio))
                    y1 = max(0, min(height - 1, (by1 - pad_y) / ratio))
                    x2 = max(0, min(width - 1, (bx2 - pad_x) / ratio))
                    y2 = max(0, min(height - 1, (by2 - pad_y) / ratio))
                    detections.append((x1, y1, x2, y2, float(score)))

        total_infer_ms += infer_ms

        # 4. Render Annotations
        result = frame.copy()
        for x1, y1, x2, y2, score in detections:
            cv2.rectangle(result, (int(x1), int(y1)), (int(x2), int(y2)), (0, 0, 255), 2)
            label_text = f"PERSON {int(score * 100)}%"
            (tw, th), bl = cv2.getTextSize(label_text, cv2.FONT_HERSHEY_SIMPLEX, 0.55, 2)
            badge_y = max(int(y1) - 6, th + 4)
            cv2.rectangle(
                result,
                (int(x1), badge_y - th - 4),
                (int(x1) + tw + 6, badge_y + bl),
                (0, 0, 255),
                cv2.FILLED,
            )
            cv2.putText(
                result,
                label_text,
                (int(x1) + 3, badge_y - 2),
                cv2.FONT_HERSHEY_SIMPLEX,
                0.55,
                (255, 255, 255),
                2,
            )

        # Performance Overlay
        cv2.putText(
            result,
            f"Latency: {int(infer_ms)} ms | Persons: {len(detections)}",
            (16, 32),
            cv2.FONT_HERSHEY_SIMPLEX,
            0.7,
            (0, 255, 0),
            2,
        )

        writer.write(result)
        cv2.imshow(f"MANAR - {tag} Rescue Vision", result)

        if total_frames > 0:
            print(f"\rProcessing frame: {frame_idx}/{total_frames} ({int(infer_ms)} ms)", end="", flush=True)
        else:
            print(f"\rProcessing frame: {frame_idx} ({int(infer_ms)} ms)", end="", flush=True)

        key = cv2.waitKey(1) & 0xFF
        if key in (27, ord("q")):
            print("\n[User Interrupted]")
            break

    cap.release()
    writer.release()
    cv2.destroyAllWindows()

    avg_ms = total_infer_ms / max(1, frame_idx)
    print("\n\n" + "=" * 55)
    print(f"FINISHED BENCHMARK RUN: {tag}")
    print(f"Total Frames:  {frame_idx}")
    print(f"Avg Latency:   {avg_ms:.1f} ms/frame ({1000.0 / max(1e-5, avg_ms):.1f} FPS)")
    print(f"Saved Output:  {output_path}")
    print("=" * 55)


def make_sample_video(path: str = "sample_test.mp4", num_frames: int = 120):
    w, h = 1280, 720
    writer = cv2.VideoWriter(path, cv2.VideoWriter_fourcc(*"mp4v"), 30.0, (w, h))

    for i in range(num_frames):
        frame = np.full((h, w, 3), (34, 100, 34), dtype=np.uint8)

        px = int(200 + (i / num_frames) * 800)
        py = int(300 + np.sin(i / 10.0) * 50)

        cv2.circle(frame, (px, py - 30), 15, (180, 180, 180), -1)
        cv2.rectangle(frame, (px - 15, py - 15), (px + 15, py + 40), (220, 50, 50), -1)
        cv2.line(frame, (px - 10, py + 40), (px - 10, py + 80), (50, 50, 50), 6)
        cv2.line(frame, (px + 10, py + 40), (px + 10, py + 80), (50, 50, 50), 6)

        cv2.putText(frame, "MANAR EO SENSOR SIM", (20, 40), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (255, 255, 255), 2)
        cv2.putText(frame, f"REC [O] Frame {i+1:04d}", (w - 240, 40), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2)

        writer.write(frame)

    writer.release()
    print(f"[OK] Generated synthetic sample video: {path}")


if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser(description="MANAR Vision Benchmark Runner")
    default_sample = os.path.join(SCRIPT_DIR, "sample_test.mp4")
    parser.add_argument("video", nargs="?", default=default_sample, help="Path to video file")
    parser.add_argument("--model", choices=["dfine", "yolo11n"], default="dfine", help="Model to evaluate")
    parser.add_argument("--model-path", default=None, help="Explicit path to ONNX model")
    parser.add_argument("--make-sample", action="store_true", help="Generate sample synthetic video")

    args = parser.parse_args()

    if args.make_sample:
        make_sample_video(default_sample)
        sys.exit(0)

    chosen_model = args.model_path
    if chosen_model is None:
        chosen_model = YOLO_MODEL_PATH if args.model == "yolo11n" else DFINE_MODEL_PATH

    vpath = clean_path(args.video)
    if not os.path.exists(vpath):
        if vpath == default_sample or vpath == "sample_test.mp4":
            make_sample_video(default_sample)
            vpath = default_sample
        else:
            print(f"[!] Error: Video file not found at '{vpath}'")
            sys.exit(1)

    run_pipeline(vpath, chosen_model)
