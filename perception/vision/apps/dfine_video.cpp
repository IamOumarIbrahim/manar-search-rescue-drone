#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <chrono>
#include <filesystem>
#include <opencv2/opencv.hpp>
#include <onnxruntime_cxx_api.h>

using namespace std;

// Model and Detection Configuration
const string MODEL_PATH = "models/dfine_n.onnx";
const int INPUT_SIZE = 640;
const float CONFIDENCE = 0.50f;
const int PERSON_CLASS = 0; // COCO class 0 = person

const char* input_names[] = { "images", "orig_target_sizes" };
const char* output_names[] = { "labels", "boxes", "scores" };

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
// Run D-FINE on a single video frame
// --------------------------------------------------
cv::Mat detect(Ort::Session& session, const cv::Mat& frame, double& inferMs) {
    int originalWidth = frame.cols;
    int originalHeight = frame.rows;

    float ratio = min(
        static_cast<float>(INPUT_SIZE) / originalWidth,
        static_cast<float>(INPUT_SIZE) / originalHeight
    );

    int newWidth = static_cast<int>(originalWidth * ratio);
    int newHeight = static_cast<int>(originalHeight * ratio);

    int padX = (INPUT_SIZE - newWidth) / 2;
    int padY = (INPUT_SIZE - newHeight) / 2;

    // Resize frame while keeping aspect ratio
    cv::Mat resized;
    cv::resize(frame, resized, cv::Size(newWidth, newHeight));

    // Pad to 640x640 letterbox
    cv::Mat inputImage(INPUT_SIZE, INPUT_SIZE, CV_8UC3, cv::Scalar(0, 0, 0));
    resized.copyTo(inputImage(cv::Rect(padX, padY, newWidth, newHeight)));

    // BGR -> RGB
    cv::cvtColor(inputImage, inputImage, cv::COLOR_BGR2RGB);

    // Create NCHW tensor (float32 [0.0, 1.0])
    vector<float> inputData(3 * INPUT_SIZE * INPUT_SIZE);
    int index = 0;
    for (int c = 0; c < 3; c++) {
        for (int y = 0; y < INPUT_SIZE; y++) {
            for (int x = 0; x < INPUT_SIZE; x++) {
                inputData[index++] = inputImage.at<cv::Vec3b>(y, x)[c] / 255.0f;
            }
        }
    }

    vector<int64_t> imageShape = { 1, 3, INPUT_SIZE, INPUT_SIZE };
    vector<int64_t> sizeShape = { 1, 2 };
    vector<int64_t> originalSize = { INPUT_SIZE, INPUT_SIZE };

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

    Ort::Value sizeTensor = Ort::Value::CreateTensor<int64_t>(
        memoryInfo,
        originalSize.data(),
        originalSize.size(),
        sizeShape.data(),
        sizeShape.size()
    );

    array<Ort::Value, 2> inputs = { move(imageTensor), move(sizeTensor) };

    // Measure inference time
    auto tStart = chrono::high_resolution_clock::now();

    auto outputs = session.Run(
        Ort::RunOptions{nullptr},
        input_names,
        inputs.data(),
        2,
        output_names,
        3
    );

    auto tEnd = chrono::high_resolution_clock::now();
    inferMs = chrono::duration<double, milli>(tEnd - tStart).count();

    // Parse model outputs
    int64_t* labels = outputs[0].GetTensorMutableData<int64_t>();
    float* boxes = outputs[1].GetTensorMutableData<float>();
    float* scores = outputs[2].GetTensorMutableData<float>();

    auto scoreShape = outputs[2].GetTensorTypeAndShapeInfo().GetShape();
    size_t detectionCount = scoreShape[1];

    cv::Mat result = frame.clone();

    // Draw detections
    int personFound = 0;
    for (size_t i = 0; i < detectionCount; i++) {
        if (scores[i] < CONFIDENCE) continue;
        if (labels[i] != PERSON_CLASS) continue;

        personFound++;

        // Un-letterbox coordinates back to original frame
        float x1 = (boxes[i * 4] - padX) / ratio;
        float y1 = (boxes[i * 4 + 1] - padY) / ratio;
        float x2 = (boxes[i * 4 + 2] - padX) / ratio;
        float y2 = (boxes[i * 4 + 3] - padY) / ratio;

        // Clamp to image dimensions
        x1 = max(0.0f, min(x1, static_cast<float>(originalWidth - 1)));
        y1 = max(0.0f, min(y1, static_cast<float>(originalHeight - 1)));
        x2 = max(0.0f, min(x2, static_cast<float>(originalWidth - 1)));
        y2 = max(0.0f, min(y2, static_cast<float>(originalHeight - 1)));

        // Draw bounding box (Amber/Red search & rescue accent)
        cv::rectangle(
            result,
            cv::Point(static_cast<int>(x1), static_cast<int>(y1)),
            cv::Point(static_cast<int>(x2), static_cast<int>(y2)),
            cv::Scalar(0, 0, 255),
            2
        );

        string labelText = "PERSON " + to_string(static_cast<int>(scores[i] * 100)) + "%";
        
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

    // Overlay inference time info
    string statText = "Latency: " + to_string(static_cast<int>(inferMs)) + " ms | Targets: " + to_string(personFound);
    cv::putText(
        result,
        statText,
        cv::Point(16, 32),
        cv::FONT_HERSHEY_SIMPLEX,
        0.7,
        cv::Scalar(0, 255, 0),
        2
    );

    return result;
}

// --------------------------------------------------
// Main
// --------------------------------------------------
int main() {
    cout << "=========================================\n";
    cout << "   MANAR - D-FINE-N RESCUE VISION TEST   \n";
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
        cout << "\n[ERROR] Model file not found at: " << MODEL_PATH << "\n";
        cout << "Please place dfine_n.onnx in the models/ directory.\n";
        return 1;
    }

    cout << "\nLoading D-FINE-N ONNX graph...\n";

    // Initialize ONNX Runtime Session
    Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "MANAR");
    Ort::SessionOptions sessionOptions;
    sessionOptions.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);

#ifdef _WIN32
    wstring modelPathWide(MODEL_PATH.begin(), MODEL_PATH.end());
    Ort::Session session(env, modelPathWide.c_str(), sessionOptions);
#else
    Ort::Session session(env, MODEL_PATH.c_str(), sessionOptions);
#endif

    cout << "Model loaded successfully.\n";
    cout << "Starting detection pipeline...\n";
    cout << "Press [ESC] in the video window to stop.\n\n";

    // Ensure output folder exists
    filesystem::create_directories("output");
    string outputPath = "output/output.mp4";

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
        cv::Mat result = detect(session, frame, inferMs);
        totalInferMs += inferMs;

        writer.write(result);
        cv::imshow("MANAR - D-FINE-N", result);

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
