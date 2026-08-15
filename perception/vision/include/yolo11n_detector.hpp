#pragma once

#include <string>
#include <vector>
#include <memory>
#include <opencv2/opencv.hpp>
#include <onnxruntime_cxx_api.h>

// ---------------------------------------------------------------------------
// MANAR Raw Visual Person Detection Record
// ---------------------------------------------------------------------------
struct PersonDetection {
    cv::Rect2f box;      // Bounding box in original frame pixel coordinates [x, y, width, height]
    float confidence;    // Detection confidence score [0.0, 1.0]
    int classId{0};      // Target class index (0 = COCO person)
};

// ---------------------------------------------------------------------------
// Reusable YOLO11n Detector Integration Module
// ---------------------------------------------------------------------------
class Yolo11nDetector {
public:
    explicit Yolo11nDetector(const std::string& modelPath = "models/yolo11n.onnx",
                             float confThreshold = 0.50f,
                             float nmsThreshold = 0.45f);
    ~Yolo11nDetector() = default;

    // Non-copyable, movable for clean resource ownership
    Yolo11nDetector(const Yolo11nDetector&) = delete;
    Yolo11nDetector& operator=(const Yolo11nDetector&) = delete;
    Yolo11nDetector(Yolo11nDetector&&) noexcept = default;
    Yolo11nDetector& operator=(Yolo11nDetector&&) noexcept = default;

    // Verification and configuration queries
    bool isLoaded() const;
    const std::string& getModelPath() const;
    float getConfidenceThreshold() const;
    float getNmsThreshold() const;
    int getInputSize() const;
    int getPersonClass() const;

    // Detects human candidates in an externally provided OpenCV frame.
    // If inferMs pointer is provided, records raw model inference latency (session.Run timer only).
    std::vector<PersonDetection> detect(const cv::Mat& frame, double* inferMs = nullptr);

private:
    std::string m_modelPath;
    int m_inputSize{640};
    float m_confThreshold{0.50f};
    float m_nmsThreshold{0.45f};
    int m_personClass{0};
    bool m_isLoaded{false};

    std::unique_ptr<Ort::Env> m_env;
    std::unique_ptr<Ort::SessionOptions> m_sessionOptions;
    std::unique_ptr<Ort::Session> m_session;

    std::string m_inputName;
    std::string m_outputName;
};
