#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <opencv2/opencv.hpp>

#include "yolo11n_detector.hpp"

using namespace std;

const string MODEL_PATH = "models/yolo11n.onnx";

// --------------------------------------------------
// Clean Windows paths/URLs pasted with quotes
// --------------------------------------------------
string cleanPath(string path) {
    if (path.size() >= 2 && path.front() == '"' && path.back() == '"') {
        path = path.substr(1, path.size() - 2);
    }
    return path;
}

// --------------------------------------------------
// Main Live Camera Application
// --------------------------------------------------
int main() {
    cout << "=========================================\n";
    cout << "   MANAR - YOLO11N LIVE STREAM FEED      \n";
    cout << "=========================================\n\n";
    cout << "1. Default Live Camera (Index 0)\n";
    cout << "2. Custom Camera Index or RTSP URL\n";
    cout << "0. Exit\n\n";
    cout << "> ";

    int option;
    if (!(cin >> option)) return 0;
    cin.ignore();

    if (option == 0) return 0;

    cv::VideoCapture cap;
    if (option == 1) {
        cout << "\nOpening default camera [Device 0]...\n";
        cap.open(0);
    } else if (option == 2) {
        cout << "\nEnter camera index (e.g. 0, 1) or stream URL (rtsp://...):\n> ";
        string inputSource;
        getline(cin, inputSource);
        inputSource = cleanPath(inputSource);

        bool isNumeric = !inputSource.empty() && inputSource.find_first_not_of("0123456789") == string::npos;
        if (isNumeric) {
            cap.open(stoi(inputSource));
        } else {
            cap.open(inputSource);
        }
    } else {
        cout << "Invalid option.\n";
        return 1;
    }

    if (!cap.isOpened()) {
        cout << "\n[ERROR] Could not open live capture source.\n";
        return 1;
    }

    // Check model
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
    cout << "Live inference running... Press [ESC] in window to exit.\n\n";

    // Create output folder for live recording
    filesystem::create_directories("output");
    string outputPath = "output/yolo11n_live_capture.mp4";

    int width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    int height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    double fps = cap.get(cv::CAP_PROP_FPS);
    if (fps <= 0 || fps > 120) fps = 30.0;

    cv::VideoWriter writer(
        outputPath,
        cv::VideoWriter::fourcc('m', 'p', '4', 'v'),
        fps,
        cv::Size(width, height)
    );

    cv::Mat frame;
    int frameCount = 0;

    while (cap.read(frame)) {
        if (frame.empty()) break;
        frameCount++;

        double inferMs = 0.0;

        // Perform raw detection
        std::vector<PersonDetection> detections = detector.detect(frame, &inferMs);
        int personCount = static_cast<int>(detections.size());

        cv::Mat result = frame.clone();
        for (const auto& det : detections) {
            float x1 = det.box.x;
            float y1 = det.box.y;
            float x2 = det.box.x + det.box.width;
            float y2 = det.box.y + det.box.height;

            // Draw bounding box (Amber/Red search & rescue accent)
            cv::rectangle(
                result,
                cv::Point(static_cast<int>(x1), static_cast<int>(y1)),
                cv::Point(static_cast<int>(x2), static_cast<int>(y2)),
                cv::Scalar(0, 0, 255),
                2
            );

            string labelText = "PERSON " + to_string(static_cast<int>(det.confidence * 100)) + "%";

            // Draw label background badge
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

        // HUD Diagnostics Overlay
        string hudLatency = "Inference: " + to_string(static_cast<int>(inferMs)) + " ms";
        string hudTargets = "Human Candidates: " + to_string(personCount);

        cv::rectangle(result, cv::Point(10, 10), cv::Point(310, 72), cv::Scalar(0, 0, 0), cv::FILLED);
        cv::putText(result, hudLatency, cv::Point(18, 34), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 255, 0), 2);
        cv::putText(result, hudTargets, cv::Point(18, 58), cv::FONT_HERSHEY_SIMPLEX, 0.6, (personCount > 0 ? cv::Scalar(0, 0, 255) : cv::Scalar(200, 200, 200)), 2);

        writer.write(result);
        cv::imshow("MANAR - YOLO11N Live Feed", result);

        cout << "\rLive Frame: " << frameCount
             << " | Latency: " << static_cast<int>(inferMs) << " ms"
             << " | Humans Detected: " << personCount << "   " << flush;

        if (cv::waitKey(1) == 27) {
            cout << "\n[User Stopped Live Stream]";
            break;
        }
    }

    cap.release();
    writer.release();
    cv::destroyAllWindows();

    cout << "\n\nLive session completed. Stream saved to: " << outputPath << "\n";
    return 0;
}
