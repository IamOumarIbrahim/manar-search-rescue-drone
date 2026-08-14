"""
MANAR Computer Vision - Test Harness & Pipeline Verifier
Supports both official D-FINE ONNX exports and HuggingFace ONNX model formats.
"""

import os
import sys
import time
import cv2
import numpy as np
import onnxruntime as ort

MODEL_PATH = os.path.join(os.path.dirname(__file__), "models", "dfine_n.onnx")
OUTPUT_DIR = os.path.join(os.path.dirname(__file__), "output")
INPUT_SIZE = 640
CONFIDENCE_THRESHOLD = 0.40
PERSON_CLASS = 0


def clean_path(path: str) -> str:
    path = path.strip()
    if path.startswith('"') and path.endswith('"'):
        path = path[1:-1]
    return path


def letterbox_image(image: np.ndarray, target_size: int = 640):
    """
    Resize and pad image to target_size while preserving aspect ratio.
    Returns: padded_image, ratio, (pad_x, pad_y), (new_w, new_h)
    """
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


def run_pipeline(video_path: str, model_path: str = MODEL_PATH):
    print("=" * 45)
    print("   MANAR D-FINE-N VISION PIPELINE TEST   ")
    print("=" * 45)

    if not os.path.exists(model_path):
        print(f"\n[!] Model file not found at: {model_path}")
        return

    # Load ONNX Runtime session
    opts = ort.SessionOptions()
    opts.graph_optimization_level = ort.GraphOptimizationLevel.ORT_ENABLE_ALL
    session = ort.InferenceSession(model_path, sess_options=opts, providers=["CPUExecutionProvider"])
    
    input_names = [i.name for i in session.get_inputs()]
    output_names = [o.name for o in session.get_outputs()]
    print(f"[+] Loaded D-FINE-N model: {os.path.basename(model_path)}")
    print(f"    Inputs:  {input_names}")
    print(f"    Outputs: {output_names}")

    cap = cv2.VideoCapture(video_path)
    if not cap.isOpened():
        print(f"[!] Error: Could not open video '{video_path}'")
        return

    os.makedirs(OUTPUT_DIR, exist_ok=True)
    output_path = os.path.join(OUTPUT_DIR, "output.mp4")

    width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
    height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
    fps = cap.get(cv2.CAP_PROP_FPS) or 30.0
    total_frames = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))

    fourcc = cv2.VideoWriter_fourcc(*"mp4v")
    writer = cv2.VideoWriter(output_path, fourcc, fps, (width, height))

    print(f"\n[+] Processing '{video_path}' ({width}x{height} @ {fps:.1f} FPS)")
    print("[+] Live view window active. Press [ESC] or 'q' to stop.")

    frame_idx = 0
    total_infer_ms = 0.0

    while cap.isOpened():
        ret, frame = cap.read()
        if not ret:
            break

        frame_idx += 1
        t0 = time.perf_counter()

        # 1. Letterbox preprocessing
        input_img, ratio, pad_x, pad_y, new_w, new_h = letterbox_image(frame, INPUT_SIZE)

        # 2. RGB + Normalize + NCHW float32
        rgb = cv2.cvtColor(input_img, cv2.COLOR_BGR2RGB)
        tensor = (rgb.astype(np.float32) / 255.0).transpose(2, 0, 1)
        tensor = np.expand_dims(tensor, axis=0)  # [1, 3, 640, 640]

        # 3. Model Inference
        if "orig_target_sizes" in input_names:
            # Official D-FINE exported graph format
            orig_sizes = np.array([[INPUT_SIZE, INPUT_SIZE]], dtype=np.int64)
            feed = {input_names[0]: tensor, "orig_target_sizes": orig_sizes}
            out = session.run(None, feed)
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
            # DETR / HuggingFace graph format (logits + pred_boxes)
            feed = {input_names[0]: tensor}
            out = session.run(None, feed)
            
            # Map outputs
            logits = out[0][0] if len(out[0].shape) == 3 else out[1][0]
            boxes = out[1][0] if len(out[1].shape) == 3 else out[0][0]
            
            # Sigmoid probabilities
            probs = sigmoid(logits)
            pred_classes = np.argmax(probs, axis=-1)
            pred_scores = np.max(probs, axis=-1)

            detections = []
            for cls_idx, score, box in zip(pred_classes, pred_scores, boxes):
                if score >= CONFIDENCE_THRESHOLD and int(cls_idx) == PERSON_CLASS:
                    cx, cy, bw, bh = box
                    # Map from [0,1] normalized to 640x640 letterbox coordinates
                    bx1 = (cx - bw / 2.0) * INPUT_SIZE
                    by1 = (cy - bh / 2.0) * INPUT_SIZE
                    bx2 = (cx + bw / 2.0) * INPUT_SIZE
                    by2 = (cy + bh / 2.0) * INPUT_SIZE

                    # Map from letterbox to original frame coordinates
                    x1 = max(0, min(width - 1, (bx1 - pad_x) / ratio))
                    y1 = max(0, min(height - 1, (by1 - pad_y) / ratio))
                    x2 = max(0, min(width - 1, (bx2 - pad_x) / ratio))
                    y2 = max(0, min(height - 1, (by2 - pad_y) / ratio))
                    detections.append((x1, y1, x2, y2, float(score)))

        t1 = time.perf_counter()
        infer_ms = (t1 - t0) * 1000.0
        total_infer_ms += infer_ms

        # 4. Render Annotations
        result = frame.copy()
        for x1, y1, x2, y2, score in detections:
            # Red bounding box (SAR rescue accent)
            cv2.rectangle(result, (int(x1), int(y1)), (int(x2), int(y2)), (0, 0, 255), 2)

            # Target badge
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
        cv2.imshow("MANAR - D-FINE-N Rescue Vision", result)

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
    print("\n\n" + "=" * 45)
    print("FINISHED PROCESSING")
    print(f"Total Frames: {frame_idx}")
    print(f"Avg Latency:  {avg_ms:.1f} ms/frame ({1000.0 / max(1e-5, avg_ms):.1f} FPS)")
    print(f"Saved Output: {output_path}")
    print("=" * 45)


def make_sample_video(path: str = "sample_test.mp4", num_frames: int = 120):
    """Generates a synthetic 720p search & rescue test video with moving subject."""
    w, h = 1280, 720
    writer = cv2.VideoWriter(path, cv2.VideoWriter_fourcc(*"mp4v"), 30.0, (w, h))

    for i in range(num_frames):
        # Landscape background (grass / terrain)
        frame = np.full((h, w, 3), (34, 100, 34), dtype=np.uint8)

        # Moving person marker
        px = int(200 + (i / num_frames) * 800)
        py = int(300 + np.sin(i / 10.0) * 50)

        # Draw person figure
        cv2.circle(frame, (px, py - 30), 15, (180, 180, 180), -1)  # Head
        cv2.rectangle(frame, (px - 15, py - 15), (px + 15, py + 40), (220, 50, 50), -1)  # Body
        cv2.line(frame, (px - 10, py + 40), (px - 10, py + 80), (50, 50, 50), 6)  # Left leg
        cv2.line(frame, (px + 10, py + 40), (px + 10, py + 80), (50, 50, 50), 6)  # Right leg

        # Add camera OSD
        cv2.putText(frame, "MANAR EO SENSOR SIM", (20, 40), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (255, 255, 255), 2)
        cv2.putText(frame, f"REC [O] Frame {i+1:04d}", (w - 240, 40), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2)

        writer.write(frame)

    writer.release()
    print(f"[OK] Generated synthetic sample video: {path}")


if __name__ == "__main__":
    if len(sys.argv) > 1 and sys.argv[1] == "--make-sample":
        make_sample_video("sample_test.mp4")
    elif len(sys.argv) > 1:
        run_pipeline(clean_path(sys.argv[1]))
    else:
        sample_path = "sample_test.mp4"
        if not os.path.exists(sample_path):
            make_sample_video(sample_path)
        run_pipeline(sample_path)
