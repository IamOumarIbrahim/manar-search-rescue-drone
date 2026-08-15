"""
MANAR Computer Vision - Batch Benchmark Runner
Processes all videos in Training videos/ through both D-FINE-N and YOLO11n,
writing annotated results into temp/dfine/ and temp/yolo11n/.
"""

import os
import sys
import time
import cv2
import numpy as np
import onnxruntime as ort

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
CV_ROOT = os.path.dirname(SCRIPT_DIR)
TRAINING_VIDEOS_DIR = os.path.normpath(
    os.path.join(CV_ROOT, "..", "..", "assets", "Videos", "Training videos")
)

DFINE_MODEL_PATH = os.path.join(CV_ROOT, "models", "dfine_n.onnx")
YOLO_MODEL_PATH = os.path.join(CV_ROOT, "models", "yolo11n.onnx")

TEMP_DIR = os.path.join(CV_ROOT, "temp")
TEMP_DFINE_DIR = os.path.join(TEMP_DIR, "dfine")
TEMP_YOLO_DIR = os.path.join(TEMP_DIR, "yolo11n")

INPUT_SIZE = 640
CONFIDENCE_THRESHOLD = 0.50
NMS_THRESHOLD = 0.45
PERSON_CLASS = 0


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

    return canvas, ratio, pad_x, pad_y


def sigmoid(x):
    return 1.0 / (1.0 + np.exp(-np.clip(x, -20.0, 20.0)))


def find_all_videos(root_dir: str):
    video_files = []
    for root, _, files in os.walk(root_dir):
        for f in files:
            if f.lower().endswith(".mp4"):
                full_path = os.path.join(root, f)
                rel_path = os.path.relpath(full_path, root_dir)
                video_files.append((full_path, rel_path))
    return sorted(video_files)


def process_video_dfine(session, video_path: str, output_path: str):
    input_names = [i.name for i in session.get_inputs()]
    output_names = [o.name for o in session.get_outputs()]

    cap = cv2.VideoCapture(video_path)
    if not cap.isOpened():
        return False, 0, 0.0, 0

    os.makedirs(os.path.dirname(output_path), exist_ok=True)
    width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
    height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
    fps = cap.get(cv2.CAP_PROP_FPS) or 30.0
    total_frames = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))

    fourcc = cv2.VideoWriter_fourcc(*"mp4v")
    writer = cv2.VideoWriter(output_path, fourcc, fps, (width, height))

    frame_count = 0
    total_infer_ms = 0.0
    total_person_detections = 0

    while cap.isOpened():
        ret, frame = cap.read()
        if not ret:
            break

        frame_count += 1

        # Letterbox preprocessing
        input_img, ratio, pad_x, pad_y = letterbox_image(frame, INPUT_SIZE)
        rgb = cv2.cvtColor(input_img, cv2.COLOR_BGR2RGB)
        tensor = (rgb.astype(np.float32) / 255.0).transpose(2, 0, 1)
        tensor = np.expand_dims(tensor, axis=0)

        # Inference
        if "orig_target_sizes" in input_names:
            orig_sizes = np.array([[INPUT_SIZE, INPUT_SIZE]], dtype=np.int64)
            t0 = time.perf_counter()
            out = session.run(None, {input_names[0]: tensor, "orig_target_sizes": orig_sizes})
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
            t0 = time.perf_counter()
            out = session.run(None, {input_names[0]: tensor})
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
        total_person_detections += len(detections)

        # Draw annotations
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

        # Status text
        cv2.putText(
            result,
            f"D-FINE-N | Latency: {int(infer_ms)} ms | Targets: {len(detections)}",
            (16, 32),
            cv2.FONT_HERSHEY_SIMPLEX,
            0.65,
            (0, 255, 0),
            2,
        )
        writer.write(result)

    cap.release()
    writer.release()

    avg_ms = total_infer_ms / max(1, frame_count)
    return True, frame_count, avg_ms, total_person_detections


def process_video_yolo(session, video_path: str, output_path: str):
    input_names = [i.name for i in session.get_inputs()]
    output_names = [o.name for o in session.get_outputs()]

    cap = cv2.VideoCapture(video_path)
    if not cap.isOpened():
        return False, 0, 0.0, 0

    os.makedirs(os.path.dirname(output_path), exist_ok=True)
    width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
    height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
    fps = cap.get(cv2.CAP_PROP_FPS) or 30.0
    total_frames = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))

    fourcc = cv2.VideoWriter_fourcc(*"mp4v")
    writer = cv2.VideoWriter(output_path, fourcc, fps, (width, height))

    frame_count = 0
    total_infer_ms = 0.0
    total_person_detections = 0

    while cap.isOpened():
        ret, frame = cap.read()
        if not ret:
            break

        frame_count += 1

        # Letterbox preprocessing
        input_img, ratio, pad_x, pad_y = letterbox_image(frame, INPUT_SIZE)
        rgb = cv2.cvtColor(input_img, cv2.COLOR_BGR2RGB)
        tensor = (rgb.astype(np.float32) / 255.0).transpose(2, 0, 1)
        tensor = np.expand_dims(tensor, axis=0)

        # Inference
        t0 = time.perf_counter()
        out = session.run(output_names, {input_names[0]: tensor})
        t1 = time.perf_counter()
        infer_ms = (t1 - t0) * 1000.0

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

        total_infer_ms += infer_ms
        total_person_detections += len(detections)

        # Draw annotations
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

        # Status text
        cv2.putText(
            result,
            f"YOLO11N | Latency: {int(infer_ms)} ms | Targets: {len(detections)}",
            (16, 32),
            cv2.FONT_HERSHEY_SIMPLEX,
            0.65,
            (0, 255, 0),
            2,
        )
        writer.write(result)

    cap.release()
    writer.release()

    avg_ms = total_infer_ms / max(1, frame_count)
    return True, frame_count, avg_ms, total_person_detections


def main():
    print("=" * 65)
    print("      MANAR COMPUTER VISION - BATCH BENCHMARK SUITE          ")
    print("=" * 65)

    os.makedirs(TEMP_DFINE_DIR, exist_ok=True)
    os.makedirs(TEMP_YOLO_DIR, exist_ok=True)

    videos = find_all_videos(TRAINING_VIDEOS_DIR)
    print(f"[+] Found {len(videos)} training videos in:")
    print(f"    {TRAINING_VIDEOS_DIR}\n")

    if not videos:
        print("[!] No MP4 videos found.")
        return

    # Initialize ONNX Runtime Sessions
    opts = ort.SessionOptions()
    opts.graph_optimization_level = ort.GraphOptimizationLevel.ORT_ENABLE_ALL

    print("[+] Loading D-FINE-N Baseline ONNX...")
    dfine_sess = ort.InferenceSession(DFINE_MODEL_PATH, sess_options=opts, providers=["CPUExecutionProvider"])

    print("[+] Loading YOLO11n Candidate ONNX...")
    yolo_sess = ort.InferenceSession(YOLO_MODEL_PATH, sess_options=opts, providers=["CPUExecutionProvider"])
    print("[+] Both models loaded successfully.\n")

    results = []

    for idx, (full_path, rel_path) in enumerate(videos, 1):
        print(f"[{idx}/{len(videos)}] Processing: {rel_path}")

        flat_name = rel_path.replace("\\", "_").replace("/", "_")
        dfine_out = os.path.join(TEMP_DFINE_DIR, flat_name)
        yolo_out = os.path.join(TEMP_YOLO_DIR, flat_name)

        # 1. Run D-FINE-N
        t0 = time.perf_counter()
        ok_d, frames_d, lat_d, det_d = process_video_dfine(dfine_sess, full_path, dfine_out)
        time_d = time.perf_counter() - t0

        # 2. Run YOLO11n
        t0 = time.perf_counter()
        ok_y, frames_y, lat_y, det_y = process_video_yolo(yolo_sess, full_path, yolo_out)
        time_y = time.perf_counter() - t0

        results.append({
            "rel_path": rel_path,
            "frames": frames_d,
            "dfine_lat": lat_d,
            "dfine_det": det_d,
            "dfine_fps": frames_d / max(1e-5, time_d),
            "yolo_lat": lat_y,
            "yolo_det": det_y,
            "yolo_fps": frames_y / max(1e-5, time_y),
        })

        print(f"    -> D-FINE-N: {frames_d} frames | Latency: {lat_d:.1f} ms | Detections: {det_d} | Output: temp/dfine/{flat_name}")
        print(f"    -> YOLO11N : {frames_y} frames | Latency: {lat_y:.1f} ms | Detections: {det_y} | Output: temp/yolo11n/{flat_name}")

    print("\n" + "=" * 80)
    print("                    BATCH BENCHMARK SUMMARY REPORT                     ")
    print("=" * 80)
    print(f"{'Video Name':<45} | {'D-FINE (ms)':<12} | {'YOLO11 (ms)':<12} | {'D-FINE Hits':<11} | {'YOLO Hits'}")
    print("-" * 80)

    total_dfine_lat = 0.0
    total_yolo_lat = 0.0

    for r in results:
        vname = os.path.basename(r["rel_path"])
        if len(vname) > 42:
            vname = vname[:39] + "..."
        print(f"{vname:<45} | {r['dfine_lat']:<12.1f} | {r['yolo_lat']:<12.1f} | {r['dfine_det']:<11} | {r['yolo_det']}")
        total_dfine_lat += r["dfine_lat"]
        total_yolo_lat += r["yolo_lat"]

    n = max(1, len(results))
    print("-" * 80)
    print(f"{'AVERAGE':<45} | {total_dfine_lat/n:<12.1f} | {total_yolo_lat/n:<12.1f} |")
    print("=" * 80)
    print(f"\n[OK] All {len(videos)} videos processed for both models.")
    print(f"[OK] D-FINE-N outputs: {TEMP_DFINE_DIR}")
    print(f"[OK] YOLO11N  outputs: {TEMP_YOLO_DIR}")


if __name__ == "__main__":
    main()
