#include "yolo11n_detector.hpp"

#include <iostream>
#include <vector>
#include <array>
#include <chrono>
#include <algorithm>
#include <filesystem>

// ---------------------------------------------------------------------------
// Constructor & Model Initialization
// ---------------------------------------------------------------------------
Yolo11nDetector::Yolo11nDetector(const std::string& modelPath,
                                 float confThreshold,
                                 float nmsThreshold)
    : m_modelPath(modelPath),
      m_inputSize(640),
      m_confThreshold(confThreshold),
      m_nmsThreshold(nmsThreshold),
      m_personClass(0),
      m_isLoaded(false)
{
    if (!std::filesystem::exists(m_modelPath)) {
        std::cerr << "[Yolo11nDetector] Model file not found at: " << m_modelPath << std::endl;
        return;
    }

    try {
        m_env = std::make_unique<Ort::Env>(ORT_LOGGING_LEVEL_WARNING, "MANAR_YOLO11N_DETECTOR");
        m_sessionOptions = std::make_unique<Ort::SessionOptions>();
        m_sessionOptions->SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);

#ifdef _WIN32
        std::wstring modelPathWide(m_modelPath.begin(), m_modelPath.end());
        m_session = std::make_unique<Ort::Session>(*m_env, modelPathWide.c_str(), *m_sessionOptions);
#else
        m_session = std::make_unique<Ort::Session>(*m_env, m_modelPath.c_str(), *m_sessionOptions);
#endif

        Ort::AllocatorWithDefaultOptions allocator;
        auto inputNameAlloc = m_session->GetInputNameAllocated(0, allocator);
        auto outputNameAlloc = m_session->GetOutputNameAllocated(0, allocator);
        m_inputName = inputNameAlloc.get();
        m_outputName = outputNameAlloc.get();

        m_isLoaded = true;
    }
    catch (const std::exception& e) {
        std::cerr << "[Yolo11nDetector] Initialization failed: " << e.what() << std::endl;
        m_isLoaded = false;
    }
}

bool Yolo11nDetector::isLoaded() const {
    return m_isLoaded;
}

const std::string& Yolo11nDetector::getModelPath() const {
    return m_modelPath;
}

float Yolo11nDetector::getConfidenceThreshold() const {
    return m_confThreshold;
}

float Yolo11nDetector::getNmsThreshold() const {
    return m_nmsThreshold;
}

int Yolo11nDetector::getInputSize() const {
    return m_inputSize;
}

int Yolo11nDetector::getPersonClass() const {
    return m_personClass;
}

// ---------------------------------------------------------------------------
// Frame Detection
// ---------------------------------------------------------------------------
std::vector<PersonDetection> Yolo11nDetector::detect(const cv::Mat& frame, double* inferMs) {
    if (!m_isLoaded || frame.empty() || !m_session) {
        return {};
    }

    int originalWidth = frame.cols;
    int originalHeight = frame.rows;

    float ratio = std::min(
        static_cast<float>(m_inputSize) / originalWidth,
        static_cast<float>(m_inputSize) / originalHeight
    );

    int newWidth = static_cast<int>(originalWidth * ratio);
    int newHeight = static_cast<int>(originalHeight * ratio);

    int padX = (m_inputSize - newWidth) / 2;
    int padY = (m_inputSize - newHeight) / 2;

    // 1. Aspect-ratio preserving resize
    cv::Mat resized;
    cv::resize(frame, resized, cv::Size(newWidth, newHeight));

    // 2. Letterbox pad to 640x640
    cv::Mat inputImage(m_inputSize, m_inputSize, CV_8UC3, cv::Scalar(0, 0, 0));
    resized.copyTo(inputImage(cv::Rect(padX, padY, newWidth, newHeight)));

    // 3. BGR -> RGB conversion
    cv::cvtColor(inputImage, inputImage, cv::COLOR_BGR2RGB);

    // 4. Create NCHW float32 normalized tensor [0.0, 1.0]
    std::vector<float> inputData(3 * m_inputSize * m_inputSize);
    int index = 0;
    for (int c = 0; c < 3; c++) {
        for (int y = 0; y < m_inputSize; y++) {
            for (int x = 0; x < m_inputSize; x++) {
                inputData[index++] = inputImage.at<cv::Vec3b>(y, x)[c] / 255.0f;
            }
        }
    }

    std::vector<int64_t> imageShape = { 1, 3, m_inputSize, m_inputSize };

    Ort::MemoryInfo memoryInfo = Ort::MemoryInfo::CreateCpu(
        OrtArenaAllocator,
        OrtMemTypeDefault
    );

    Ort::Value imageTensor = Ort::Value::CreateTensor<float>(
        memoryInfo,
        inputData.data(),
        inputData.size(),
        imageShape.data(),
        imageShape.size()
    );

    const char* inputNames[] = { m_inputName.c_str() };
    const char* outputNames[] = { m_outputName.c_str() };

    // 5. Run inference with strict latency timing around session.Run
    auto tStart = std::chrono::high_resolution_clock::now();

    auto outputs = m_session->Run(
        Ort::RunOptions{nullptr},
        inputNames,
        &imageTensor,
        1,
        outputNames,
        1
    );

    auto tEnd = std::chrono::high_resolution_clock::now();
    if (inferMs) {
        *inferMs = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
    }

    // 6. Parse raw YOLO11 tensor output
    float* rawOutput = outputs[0].GetTensorMutableData<float>();
    auto shapeInfo = outputs[0].GetTensorTypeAndShapeInfo();
    auto shape = shapeInfo.GetShape();

    // Standard YOLO output: [1, 84, 8400] or transposed [1, 8400, 84]
    int dimensions = 0;
    int anchors = 0;
    bool isTransposed = false;

    if (shape.size() == 3) {
        if (shape[1] < shape[2]) {
            // [1, 84, 8400]
            dimensions = static_cast<int>(shape[1]);
            anchors = static_cast<int>(shape[2]);
            isTransposed = false;
        } else {
            // [1, 8400, 84]
            anchors = static_cast<int>(shape[1]);
            dimensions = static_cast<int>(shape[2]);
            isTransposed = true;
        }
    }

    std::vector<cv::Rect2d> candidateBoxes;
    std::vector<float> candidateScores;

    for (int i = 0; i < anchors; i++) {
        float cx = 0.0f, cy = 0.0f, w = 0.0f, h = 0.0f, personScore = 0.0f;

        if (!isTransposed) {
            cx = rawOutput[0 * anchors + i];
            cy = rawOutput[1 * anchors + i];
            w  = rawOutput[2 * anchors + i];
            h  = rawOutput[3 * anchors + i];
            personScore = rawOutput[(4 + m_personClass) * anchors + i];
        } else {
            cx = rawOutput[i * dimensions + 0];
            cy = rawOutput[i * dimensions + 1];
            w  = rawOutput[i * dimensions + 2];
            h  = rawOutput[i * dimensions + 3];
            personScore = rawOutput[i * dimensions + (4 + m_personClass)];
        }

        if (personScore < m_confThreshold) continue;

        float bx = cx - (w / 2.0f);
        float by = cy - (h / 2.0f);

        candidateBoxes.push_back(cv::Rect2d(bx, by, w, h));
        candidateScores.push_back(personScore);
    }

    // 7. Non-Maximum Suppression (NMS)
    std::vector<int> nmsIndices;
    cv::dnn::NMSBoxes(candidateBoxes, candidateScores, m_confThreshold, m_nmsThreshold, nmsIndices);

    // 8. Inverse letterbox coordinate mapping
    std::vector<PersonDetection> detections;
    detections.reserve(nmsIndices.size());

    for (int idx : nmsIndices) {
        const auto& box = candidateBoxes[idx];
        float score = candidateScores[idx];

        float x1 = (box.x - padX) / ratio;
        float y1 = (box.y - padY) / ratio;
        float x2 = (box.x + box.width - padX) / ratio;
        float y2 = (box.y + box.height - padY) / ratio;

        x1 = std::max(0.0f, std::min(x1, static_cast<float>(originalWidth - 1)));
        y1 = std::max(0.0f, std::min(y1, static_cast<float>(originalHeight - 1)));
        x2 = std::max(0.0f, std::min(x2, static_cast<float>(originalWidth - 1)));
        y2 = std::max(0.0f, std::min(y2, static_cast<float>(originalHeight - 1)));

        float boxW = std::max(0.0f, x2 - x1);
        float boxH = std::max(0.0f, y2 - y1);

        detections.push_back(PersonDetection{
            cv::Rect2f(x1, y1, boxW, boxH),
            score,
            m_personClass
        });
    }

    return detections;
}
