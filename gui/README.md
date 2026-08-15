# MANAR Ground Control Station (GCS) GUI

This directory is reserved for the upcoming **MANAR Operator Ground Control Station (GCS)** interface.

## Planned Capabilities
- **Interactive Geospatial Map**: Live drone telemetry, GPS trajectory visualization, and search grid overlay.
- **Sensor Feeds & Detections**: Low-latency video streaming with real-time YOLO11n / D-FINE-N bounding box overlays.
- **Multimodal State Monitor**: Instantaneous fusion bus readouts (Radar, Acoustics, RF anomalies, and Mamba SSM temporal confidence).
- **Supervised Autonomy Controls**: Search location dispatch, greedy route generation preview, override controls, and Return-to-Launch (RTL) trigger.

## Planned Tech Stack
- **Framework**: React / TypeScript / Vite
- **Mapping**: Mapbox GL / Leaflet
- **Streaming**: WebSocket / gRPC-Web bridge to the C++ Core telemetry bus
