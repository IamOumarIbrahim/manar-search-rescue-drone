cmake_minimum_required(VERSION 3.16)
project(MANAR_ComputerVision CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------
# Dependencies: OpenCV & ONNX Runtime
# ---------------------------------------------------------------------------

# Find OpenCV
find_package(OpenCV REQUIRED)
message(STATUS "Found OpenCV: ${OpenCV_VERSION}")
message(STATUS "OpenCV include dirs: ${OpenCV_INCLUDE_DIRS}")

# Find ONNX Runtime (allow ONNXRUNTIME_ROOT_DIR from cache or environment)
if(NOT DEFINED ONNXRUNTIME_ROOT_DIR)
    if(DEFINED ENV{ONNXRUNTIME_ROOT_DIR})
        set(ONNXRUNTIME_ROOT_DIR $ENV{ONNXRUNTIME_ROOT_DIR})
    elseif(DEFINED ENV{ORT_HOME})
        set(ONNXRUNTIME_ROOT_DIR $ENV{ORT_HOME})
    endif()
endif()

find_path(ONNXRUNTIME_INCLUDE_DIR onnxruntime_cxx_api.h
    PATHS
        ${ONNXRUNTIME_ROOT_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/onnxruntime/include
        /usr/include/onnxruntime
        /usr/local/include/onnxruntime
)

find_library(ONNXRUNTIME_LIB NAMES onnxruntime onnxruntime.lib
    PATHS
        ${ONNXRUNTIME_ROOT_DIR}/lib
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/onnxruntime/lib
        /usr/lib
        /usr/local/lib
)

if(NOT ONNXRUNTIME_INCLUDE_DIR OR NOT ONNXRUNTIME_LIB)
    message(WARNING "ONNX Runtime not found automatically. Please set -DONNXRUNTIME_ROOT_DIR=<path_to_ort>")
endif()

# ---------------------------------------------------------------------------
# Executable Targets
# ---------------------------------------------------------------------------

# 1. Video File Inference
add_executable(dfine_video dfine_video.cpp)

target_include_directories(dfine_video PRIVATE
    ${OpenCV_INCLUDE_DIRS}
    ${ONNXRUNTIME_INCLUDE_DIR}
)

target_link_libraries(dfine_video PRIVATE
    ${OpenCV_LIBS}
    ${ONNXRUNTIME_LIB}
)

set_target_properties(dfine_video PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)

# 2. Live Camera / Stream Feed Inference
add_executable(dfine_live dfine_live.cpp)

target_include_directories(dfine_live PRIVATE
    ${OpenCV_INCLUDE_DIRS}
    ${ONNXRUNTIME_INCLUDE_DIR}
)

target_link_libraries(dfine_live PRIVATE
    ${OpenCV_LIBS}
    ${ONNXRUNTIME_LIB}
)

set_target_properties(dfine_live PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)
