"""
MANAR Computer Vision - Custom Video Benchmark Runner
Evaluates D-FINE-N and YOLO11n on a specified video and saves annotated outputs.
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


def run_dfine(video_path: str, output_path: str):
    print("=" * 65)
    print("   RUNNING BENCHMARK 1/2: D-FINE-N (Reference Baseline)   ")
    print("=" * 65)

    opts = ort.SessionOptions()
    opts.graph_optimization_level = ort.GraphOptimizationLevel.ORT_ENABLE_ALL
    session = ort.InferenceSession(DFINE_MODEL_PATH, sess_options=opts, providers=["CPUExecutionProvider"])

    input_names = [i.name for i in session.get_inputs()]
    output_names = [o.name for o in session.get_outputs()]

    cap = cv2.VideoCapture(video_path)
    if not cap.isOpened():
        print(f"[!] Error: Could not open {video_path}")
        return None

    width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
    height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
    fps = cap.get(cv2.CAP_PROP_FPS) or 30.0
    total_frames = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))

    os.makedirs(os.path.dirname(output_path), exist_ok=True)
    fourcc = cv2.VideoWriter_fourcc(*"mp4v")
    writer = cv2.VideoWriter(output_path, fourcc, fps, (width, height))

    frame_count = 0
    total_infer_ms = 0.0
    total_detections = 0
    t_start_all = time.time()

    print(f"[+] Input:  {video_path} ({width}x{height} @ {fps:.1f} FPS, {total_frames} frames)")
    print(f"[+] Output: {output_path}")

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
        total_detections += len(detections)

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

        # HUD Diagnostics Overlay
        stat_text = f"D-FINE-N | Latency: {int(infer_ms)} ms | Humans: {len(detections)}"
        cv2.rectangle(result, (10, 10), (360, 48), (0, 0, 0), cv2.FILLED)
        cv2.putText(result, stat_text, (18, 36), cv2.FONT_HERSHEY_SIMPLEX, 0.65, (0, 255, 0), 2)

        writer.write(result)

        if frame_count % 200 == 0 or frame_count == total_frames:
            elapsed = time.time() - t_start_all
            fps_proc = frame_count / max(1e-5, elapsed)
            print(f"  [D-FINE-N] Frame {frame_count}/{total_frames} ({frame_count*100//total_frames}%) | Speed: {fps_proc:.1f} FPS | Hits: {total_detections}")

    cap.release()
    writer.release()

    avg_latency = total_infer_ms / max(1, frame_count)
    wall_time = time.time() - t_start_all
    print(f"[+] D-FINE-N Finished: {frame_count} frames in {wall_time:.1f}s | Avg Latency: {avg_latency:.1f} ms | Total Detections: {total_detections}\n")

    return {
        "model": "D-FINE-N (Baseline)",
        "frames": frame_count,
        "avg_latency_ms": avg_latency,
        "total_detections": total_detections,
        "wall_time_s": wall_time,
        "output_path": output_path,
    }


def run_yolo(video_path: str, output_path: str):
    print("=" * 65)
    print("   RUNNING BENCHMARK 2/2: YOLO11n (Selected Detector)   ")
    print("=" * 65)

    opts = ort.SessionOptions()
    opts.graph_optimization_level = ort.GraphOptimizationLevel.ORT_ENABLE_ALL
    session = ort.InferenceSession(YOLO_MODEL_PATH, sess_options=opts, providers=["CPUExecutionProvider"])

    input_names = [i.name for i in session.get_inputs()]
    output_names = [o.name for o in session.get_outputs()]

    cap = cv2.VideoCapture(video_path)
    if not cap.isOpened():
        print(f"[!] Error: Could not open {video_path}")
        return None

    width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
    height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
    fps = cap.get(cv2.CAP_PROP_FPS) or 30.0
    total_frames = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))

    os.makedirs(os.path.dirname(output_path), exist_ok=True)
    fourcc = cv2.VideoWriter_fourcc(*"mp4v")
    writer = cv2.VideoWriter(output_path, fourcc, fps, (width, height))

    frame_count = 0
    total_infer_ms = 0.0
    total_detections = 0
    t_start_all = time.time()

    print(f"[+] Input:  {video_path} ({width}x{height} @ {fps:.1f} FPS, {total_frames} frames)")
    print(f"[+] Output: {output_path}")

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
        total_detections += len(detections)

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

        # HUD Diagnostics Overlay
        stat_text = f"YOLO11N | Latency: {int(infer_ms)} ms | Humans: {len(detections)}"
        cv2.rectangle(result, (10, 10), (360, 48), (0, 0, 0), cv2.FILLED)
        cv2.putText(result, stat_text, (18, 36), cv2.FONT_HERSHEY_SIMPLEX, 0.65, (0, 255, 0), 2)

        writer.write(result)

        if frame_count % 200 == 0 or frame_count == total_frames:
            elapsed = time.time() - t_start_all
            fps_proc = frame_count / max(1e-5, elapsed)
            print(f"  [YOLO11N] Frame {frame_count}/{total_frames} ({frame_count*100//total_frames}%) | Speed: {fps_proc:.1f} FPS | Hits: {total_detections}")

    cap.release()
    writer.release()

    avg_latency = total_infer_ms / max(1, frame_count)
    wall_time = time.time() - t_start_all
    print(f"[+] YOLO11n Finished: {frame_count} frames in {wall_time:.1f}s | Avg Latency: {avg_latency:.1f} ms | Total Detections: {total_detections}\n")

    return {
        "model": "YOLO11n (Selected)",
        "frames": frame_count,
        "avg_latency_ms": avg_latency,
        "total_detections": total_detections,
        "wall_time_s": wall_time,
        "output_path": output_path,
    }


def main():
    video_input = r"D:\Downloads\TestCV.mp4"
    if len(sys.argv) > 1:
        video_input = sys.argv[1]

    out_dir = r"C:\Dev\videos"
    if len(sys.argv) > 2:
        out_dir = sys.argv[2]

    os.makedirs(out_dir, exist_ok=True)
    base_name = os.path.splitext(os.path.basename(video_input))[0]

    dfine_out = os.path.join(out_dir, f"{base_name}_dfine_output.mp4")
    yolo_out = os.path.join(out_dir, f"{base_name}_yolo11n_output.mp4")

    print("\n" + "=" * 70)
    print("       MANAR SEARCH & RESCUE - DUAL BENCHMARK EVALUATION       ")
    print("=" * 70)
    print(f"Input Video:        {video_input}")
    print(f"Output Directory:   {out_dir}")
    print(f"D-FINE-N Output:    {dfine_out}")
    print(f"YOLO11n Output:     {yolo_out}")
    print("=" * 70 + "\n")

    res_dfine = run_dfine(video_input, dfine_out)
    res_yolo = run_yolo(video_input, yolo_out)

    print("\n" + "=" * 75)
    print("                       BENCHMARK COMPARISON REPORT                      ")
    print("=" * 75)
    print(f"{'Metric':<25} | {'D-FINE-N (Baseline)':<22} | {'YOLO11n (Selected)'}")
    print("-" * 75)
    print(f"{'Processed Frames':<25} | {res_dfine['frames']:<22} | {res_yolo['frames']}")
    print(f"{'Avg Inference Latency':<25} | {res_dfine['avg_latency_ms']:<6.1f} ms ({1000.0/res_dfine['avg_latency_ms']:<4.1f} FPS)   | {res_yolo['avg_latency_ms']:<6.1f} ms ({1000.0/res_yolo['avg_latency_ms']:<4.1f} FPS)")
    print(f"{'Total Detections (Hits)':<25} | {res_dfine['total_detections']:<22} | {res_yolo['total_detections']}")
    print(f"{'Total Processing Time':<25} | {res_dfine['wall_time_s']:<6.1f} s               | {res_yolo['wall_time_s']:<6.1f} s")
    print(f"{'Saved Output Video':<25} | {os.path.basename(dfine_out):<22} | {os.path.basename(yolo_out)}")
    print("=" * 75)


if __name__ == "__main__":
    main()
