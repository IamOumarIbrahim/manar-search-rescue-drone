#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <opencv2/opencv.hpp>

#include "yolo11n_detector.hpp"

using namespace std;

const string MODEL_PATH = "models/yolo11n.onnx";

// --------------------------------------------------
// Clean Windows paths pasted with quotes
// --------------------------------------------------
string cleanPath(string path) {
    if (path.size() >= 2 && path.front() == '"' && path.back() == '"') {
        path = path.substr(1, path.size() - 2);
    }
    return path;
}

// --------------------------------------------------
// Main Video Application
// --------------------------------------------------
int main() {
    cout << "=========================================\n";
    cout << "   MANAR - YOLO11N RESCUE VISION TEST    \n";
    cout << "=========================================\n\n";
    cout << "1. Choose video path\n";
    cout << "0. Exit\n\n";
    cout << "> ";

    int option;
    if (!(cin >> option)) return 0;
    cin.ignore();

    if (option == 0) return 0;
    if (option != 1) {
        cout << "Invalid option.\n";
        return 1;
    }

    string videoPath;
    cout << "\nPaste MP4 path:\n> ";
    getline(cin, videoPath);
    videoPath = cleanPath(videoPath);

    // Verify video input
    cv::VideoCapture video(videoPath);
    if (!video.isOpened()) {
        cout << "\n[ERROR] Could not open video file: " << videoPath << "\n";
        return 1;
    }

    // Verify model existence
    if (!filesystem::exists(MODEL_PATH)) {
        cout << "\n===========================================================\n";
        cout << "[ERROR] Model file not found at: " << MODEL_PATH << "\n";
        cout << "-----------------------------------------------------------\n";
        cout << "To export the official YOLO11n ONNX model:\n";
        cout << "  1. pip install ultralytics\n";
        cout << "  2. yolo export model=yolo11n.pt format=onnx imgsz=640 dynamic=False opset=17\n";
        cout << "  3. copy yolo11n.onnx models\\yolo11n.onnx\n";
        cout << "===========================================================\n\n";
        return 1;
    }

    cout << "\nLoading YOLO11n ONNX graph via Yolo11nDetector module...\n";

    // Initialize integration detector module
    Yolo11nDetector detector(MODEL_PATH, 0.50f, 0.45f);

    if (!detector.isLoaded()) {
        cout << "[ERROR] Failed to load YOLO11n detector session.\n";
        return 1;
    }

    cout << "Model loaded successfully.\n";
    cout << "Starting detection pipeline...\n";
    cout << "Press [ESC] in the video window to stop.\n\n";

    // Ensure output directory exists
    filesystem::create_directories("output");
    string outputPath = "output/yolo11n_output.mp4";

    int width = static_cast<int>(video.get(cv::CAP_PROP_FRAME_WIDTH));
    int height = static_cast<int>(video.get(cv::CAP_PROP_FRAME_HEIGHT));
    double fps = video.get(cv::CAP_PROP_FPS);
    if (fps <= 0) fps = 30;

    int totalFrames = static_cast<int>(video.get(cv::CAP_PROP_FRAME_COUNT));

    cv::VideoWriter writer(
        outputPath,
        cv::VideoWriter::fourcc('m', 'p', '4', 'v'),
        fps,
        cv::Size(width, height)
    );

    cv::Mat frame;
    int frameNumber = 0;
    double totalInferMs = 0.0;

    while (video.read(frame)) {
        frameNumber++;
        double inferMs = 0.0;

        // Perform raw detection using the extracted module
        std::vector<PersonDetection> detections = detector.detect(frame, &inferMs);
        totalInferMs += inferMs;

        // Render annotations in identical visual styling
        cv::Mat result = frame.clone();
        for (const auto& det : detections) {
            float x1 = det.box.x;
            float y1 = det.box.y;
            float x2 = det.box.x + det.box.width;
            float y2 = det.box.y + det.box.height;

            // Bounding box (Amber/Red search & rescue accent)
            cv::rectangle(
                result,
                cv::Point(static_cast<int>(x1), static_cast<int>(y1)),
                cv::Point(static_cast<int>(x2), static_cast<int>(y2)),
                cv::Scalar(0, 0, 255),
                2
            );

            string labelText = "PERSON " + to_string(static_cast<int>(det.confidence * 100)) + "%";

            // Label background badge
            int baseLine = 0;
            cv::Size textSize = cv::getTextSize(labelText, cv::FONT_HERSHEY_SIMPLEX, 0.55, 2, &baseLine);
            int textY = max(static_cast<int>(y1) - 6, textSize.height + 4);

            cv::rectangle(
                result,
                cv::Point(static_cast<int>(x1), textY - textSize.height - 4),
                cv::Point(static_cast<int>(x1) + textSize.width + 6, textY + baseLine),
                cv::Scalar(0, 0, 255),
                cv::FILLED
            );

            cv::putText(
                result,
                labelText,
                cv::Point(static_cast<int>(x1) + 3, textY - 2),
                cv::FONT_HERSHEY_SIMPLEX,
                0.55,
                cv::Scalar(255, 255, 255),
                2
            );
        }

        // Overlay latency HUD
        string statText = "Latency: " + to_string(static_cast<int>(inferMs)) + " ms | Targets: " + to_string(detections.size());
        cv::putText(
            result,
            statText,
            cv::Point(16, 32),
            cv::FONT_HERSHEY_SIMPLEX,
            0.7,
            cv::Scalar(0, 255, 0),
            2
        );

        writer.write(result);
        cv::imshow("MANAR - YOLO11N", result);

        if (totalFrames > 0) {
            cout << "\rProcessing frame: " << frameNumber << " / " << totalFrames
                 << " (" << static_cast<int>(inferMs) << " ms)" << flush;
        } else {
            cout << "\rProcessing frame: " << frameNumber
                 << " (" << static_cast<int>(inferMs) << " ms)" << flush;
        }

        if (cv::waitKey(1) == 27) {
            cout << "\n[Interrupted by user]";
            break;
        }
    }

    video.release();
    writer.release();
    cv::destroyAllWindows();

    cout << "\n\n=========================================\n";
    cout << "Finished processing.\n";
    if (frameNumber > 0) {
        cout << "Avg Latency: " << (totalInferMs / frameNumber) << " ms/frame\n";
    }
    cout << "Annotated output saved to: " << outputPath << "\n";
    cout << "=========================================\n";

    return 0;
}
