# MANAR Research Reference Library

This directory contains the curated technical research reference library for **MANAR (منار)** — a multisensor search-and-rescue UAV system.

## Library Overview & Statistics

- **Total Retained Papers**: 45
- **Peer-Reviewed Papers**: 45
- **Preprints**: 0

### Papers by Topic Area

| Topic Area | Folder | Paper Count |
|---|---|---:|
| CFAR & Adaptive Thresholding | `cfar/` | 6 |
| Passive RF & Spectrum Sensing | `passive_rf/` | 6 |
| FMCW & mmWave Radar | `fmcw_radar/` | 6 |
| Thermal & Infrared Vision | `thermal_vision/` | 5 |
| RGB & Low-Light Aerial Vision | `rgb_low_light_vision/` | 4 |
| Target Detection & Tracking | `target_detection/` | 4 |
| Multisensor Fusion Architectures | `multisensor_fusion/` | 4 |
| Acoustic Sensing & Noise Suppression | `acoustics/` | 3 |
| UAV Search & Rescue Systems | `uav_sar/` | 3 |
| Navigation & Sensor Stabilization | `navigation/` | 2 |
| Payload SWaP & Edge Compute | `payload_swap/` | 2 |

---

## Master Catalog

### Topic: CFAR

| Paper | Year | Primary Topic | Key Tags | MANAR Relevance | Score |
|---|---:|---|---|---|---:|
| [Radar CFAR Thresholding in Clutter and Multiple Target Situations](cfar/Radar_CFAR_Thresholding_in_Clutter.md) | 1983 | CFAR | OS-CFAR, adaptive-thresholding, clutter-edge | passive-rf, FMCW-radar | 10 |
| [Analysis of CFAR Processors in Nonhomogeneous Background](cfar/Analysis_of_CFAR_Processors_in_Nonhomogeneous_Background.md) | 1988 | CFAR | CA-CFAR, OS-CFAR, GO-CFAR | passive-rf, FMCW-radar | 10 |
| [Detectability Loss Due to "Greatest Of" Selection in a Cell-Averaging CFAR](cfar/Detectability_Loss_Greatest_Of_CFAR.md) | 1980 | CFAR | GO-CFAR, clutter-transitions, false-alarm-control | FMCW-radar, navigation | 9 |
| [Analysis of Some Modified Cell-Averaging CFAR Processors in Multiple-Target Situations](cfar/Analysis_Modified_Cell_Averaging_CFAR_Multiple_Targets.md) | 1982 | CFAR | SO-CFAR, smallest-of, multi-target-resolution | passive-rf, FMCW-radar | 9 |
| [Edge CA-CFAR Data Reduction for Bandwidth-Efficient Real-Time Wideband Spectrum Sensing on Low-Cost SDRs](cfar/Edge_CA_CFAR_Wideband_Spectrum_Sensing_SDR.md) | 2024 | CFAR | CA-CFAR, spectrum-sensing, SDR | passive-rf, payload-swap | 9 |
| [Adaptive Detection Mode with Threshold Control as a Function of Spatially Sampled Clutter-Level Estimates](cfar/Adaptive_Detection_Mode_Threshold_Control.md) | 1968 | CFAR | CA-CFAR, foundational-CFAR, noise-floor-estimation | passive-rf, FMCW-radar | 9 |

### Topic: passive_rf

| Paper | Year | Primary Topic | Key Tags | MANAR Relevance | Score |
|---|---:|---|---|---|---:|
| [Energy Detection of Unknown Deterministic Signals](passive_rf/Energy_Detection_of_Unknown_Signals.md) | 1967 | passive_rf | energy-detection, radiometer, chi-square-distribution | passive-rf, anomaly-detection | 10 |
| [A Survey of Spectrum Sensing Algorithms for Cognitive Radio Applications](passive_rf/Survey_Spectrum_Sensing_Algorithms_Cognitive_Radio.md) | 2009 | passive_rf | spectrum-sensing, matched-filter, cyclostationary-detection | passive-rf, anomaly-detection | 10 |
| [SNR Walls for Signal Detection](passive_rf/SNR_Walls_for_Signal_Detection.md) | 2008 | passive_rf | SNR-wall, noise-uncertainty, passive-detection-limits | passive-rf, anomaly-detection | 10 |
| [Eigenvalue-Based Spectrum Sensing Algorithms for Cognitive Radio](passive_rf/Eigenvalue_Based_Spectrum_Sensing_Cognitive_Radio.md) | 2009 | passive_rf | eigenvalue-detection, covariance-matrix, random-matrix-theory | passive-rf, anomaly-detection | 9 |
| [Localization and Tracking of Airborne Emitting Targets Using UAV Swarms](passive_rf/Localization_Tracking_Airborne_Emitting_Targets_UAV.md) | 2019 | passive_rf | RSSI-localization, airborne-rf-tracking, uav-swarm | passive-rf, target-detection | 9 |
| [UAV-Based Passive RF Sensing for Emergency Search and Rescue](passive_rf/UAV_Based_Passive_RF_Detection_and_Localization.md) | 2021 | passive_rf | passive-rf-sensing, search-and-rescue, cellular-detection | passive-rf, uav-sar | 9 |

### Topic: fmcw_radar

| Paper | Year | Primary Topic | Key Tags | MANAR Relevance | Score |
|---|---:|---|---|---|---:|
| [Micro-Doppler Effect in Radar: Phenomena, Model, and Applications](fmcw_radar/Micro_Doppler_Effect_in_Radar_Phenomena_Model_Applications.md) | 2006 | fmcw_radar | micro-doppler, radar-human-detection, spectrogram | fmcw-radar, target-detection | 10 |
| [Human Activity Classification Based on Micro-Doppler Signatures Using Artificial Neural Networks](fmcw_radar/Human_Activity_Classification_Micro_Doppler_Signatures.md) | 2009 | fmcw_radar | micro-doppler-classification, neural-network, human-radar-detection | fmcw-radar, target-detection | 9 |
| [A Review on Recent Progress in Non-Contact Vital Sign Detection Using Radar Technology](fmcw_radar/Review_Non_Contact_Vital_Sign_Detection_Radar.md) | 2013 | fmcw_radar | vital-signs, breathing-detection, heartbeat-radar | fmcw-radar, target-detection | 10 |
| [Smart Homes That Monitor Breathing and Heart Rate](fmcw_radar/Smart_Homes_Monitor_Breathing_Heart_Rate.md) | 2015 | fmcw_radar | FMCW-vital-signs, Vital-Radio, multi-person-breathing | fmcw-radar, target-detection | 9 |
| [Vibration Compensation for UAV-Mounted FMCW Radar Target Detection](fmcw_radar/Vibration_Compensation_UAV_Mounted_FMCW_Radar.md) | 2020 | fmcw_radar | airborne-radar-vibration, vibration-compensation, IMU-radar-fusion | fmcw-radar, navigation | 10 |
| [Design and Evaluation of a 24 GHz FMCW Radar Altimeter for Small Unmanned Aerial Vehicles](fmcw_radar/FMCW_Radar_Altimeter_Small_UAV.md) | 2019 | fmcw_radar | radar-altimeter, FMCW-altimetry, terrain-following | fmcw-radar, navigation | 9 |

### Topic: thermal_vision

| Paper | Year | Primary Topic | Key Tags | MANAR Relevance | Score |
|---|---:|---|---|---|---:|
| [Multispectral Pedestrian Detection: Benchmark Dataset and Baseline](thermal_vision/Multispectral_Pedestrian_Detection_Benchmark.md) | 2015 | thermal_vision | multispectral-detection, thermal-RGB-fusion, KAIST-dataset | thermal-vision, multisensor-fusion | 10 |
| [Illumination-Aware Faster R-CNN for Robust Multispectral Pedestrian Detection](thermal_vision/Illumination_Aware_Faster_RCNN_Multispectral.md) | 2019 | thermal_vision | illumination-aware, multispectral-fusion, thermal-RGB-weighting | thermal-vision, multisensor-fusion | 9 |
| [People Detection from Aerial Thermal Imagery](thermal_vision/People_Detection_Aerial_Thermal_Imagery.md) | 2014 | thermal_vision | aerial-thermal-detection, UAV-person-detection, hotspot-segmentation | thermal-vision, uav-sar | 10 |
| [Human Body Detection and Geolocation for UAV Search and Rescue Missions](thermal_vision/Human_Body_Detection_Geolocation_UAV_SAR.md) | 2008 | thermal_vision | UAV-search-and-rescue, thermal-body-detection, geolocation | thermal-vision, uav-sar | 9 |
| [Guided Attentive Feature Fusion for Multispectral Pedestrian Detection](thermal_vision/Guided_Attentional_Feature_Fusion_Multispectral.md) | 2021 | thermal_vision | attention-mechanism, feature-fusion, multispectral-detection | thermal-vision, multisensor-fusion | 9 |

### Topic: rgb_low_light_vision

| Paper | Year | Primary Topic | Key Tags | MANAR Relevance | Score |
|---|---:|---|---|---|---:|
| [VisDrone-DET2021: The Vision Meets Drone Object Detection Challenge Results](rgb_low_light_vision/VisDrone_DET2021_Object_Detection_Challenge.md) | 2021 | rgb_low_light_vision | VisDrone, UAV-object-detection, aerial-pedestrian-detection | rgb-low-light-vision, target-detection | 10 |
| [The Unmanned Aerial Vehicle Benchmark: Object Detection and Tracking](rgb_low_light_vision/Unmanned_Aerial_Vehicle_Benchmark_UAVDT.md) | 2018 | rgb_low_light_vision | UAVDT-benchmark, UAV-tracking, aerial-object-detection | rgb-low-light-vision, target-detection | 9 |
| [Zero-Reference Deep Curve Estimation for Low-Light Image Enhancement](rgb_low_light_vision/Zero_Reference_Deep_Curve_Estimation_Low_Light.md) | 2020 | rgb_low_light_vision | Zero-DCE, low-light-enhancement, real-time-enhancement | rgb-low-light-vision, payload-swap | 9 |
| [Slicing Aided Hyper Inference and Fine-Tuning for Small Object Detection](rgb_low_light_vision/Slicing_Aided_Hyper_Inference_Small_Object_Detection.md) | 2022 | rgb_low_light_vision | SAHI, slicing-inference, small-object-detection | rgb-low-light-vision, target-detection | 10 |

### Topic: target_detection

| Paper | Year | Primary Topic | Key Tags | MANAR Relevance | Score |
|---|---:|---|---|---|---:|
| [An Algorithm for Tracking Multiple Targets](target_detection/Algorithm_for_Tracking_Multiple_Targets.md) | 1979 | target_detection | MHT, multiple-hypothesis-tracking, target-confirmation | target-detection, multisensor-fusion | 10 |
| [Tracking in a Cluttered Environment With Probabilistic Data Association](target_detection/Tracking_in_Cluttered_Environment_PDAF.md) | 1975 | target_detection | PDAF, probabilistic-data-association, clutter-tracking | target-detection | 10 |
| [Particle Filter Based Track-Before-Detect Algorithm](target_detection/Particle_Filter_Based_Track_Before_Detect.md) | 2004 | target_detection | Track-Before-Detect, TBD, particle-filter | target-detection, passive-rf | 9 |
| [Multitarget Bayes Filtering via First-Order Multitarget Moments](target_detection/Multitarget_Bayes_Filtering_PHD_Filter.md) | 2003 | target_detection | PHD-filter, FISST, multi-target-tracking | target-detection, multisensor-fusion | 9 |

### Topic: multisensor_fusion

| Paper | Year | Primary Topic | Key Tags | MANAR Relevance | Score |
|---|---:|---|---|---|---:|
| [An Introduction to Multisensor Data Fusion](multisensor_fusion/Introduction_to_Multisensor_Data_Fusion.md) | 1997 | multisensor_fusion | JDL-fusion-model, data-fusion-taxonomy, sensor-level-fusion | multisensor-fusion | 10 |
| [Multisensor Data Fusion](multisensor_fusion/Multisensor_Data_Fusion_Robotics_Handbook.md) | 2008 | multisensor_fusion | bayesian-fusion, kalman-fusion, covariance-intersection | multisensor-fusion | 10 |
| [Distant Vehicle Detection Using Radar and Vision Fusion](multisensor_fusion/Distant_Vehicle_Detection_Radar_Vision_Fusion.md) | 2019 | multisensor_fusion | radar-vision-fusion, distant-target-detection, multimodal-fusion | multisensor-fusion, fmcw-radar | 9 |
| [Multimodal Target Detection and Fusion Architecture for Search and Rescue Drones](multisensor_fusion/Target_Detection_Fusion_Architecture_SAR_Drones.md) | 2022 | multisensor_fusion | multimodal-SAR-fusion, thermal-RGB-RF-fusion, decision-fusion | multisensor-fusion, uav-sar | 10 |

### Topic: acoustics

| Paper | Year | Primary Topic | Key Tags | MANAR Relevance | Score |
|---|---:|---|---|---|---:|
| [Acoustic Source Localization Using Drone Embedded Microphone Arrays](acoustics/Acoustic_Source_Localization_Drone_Microphone_Arrays.md) | 2017 | acoustics | acoustic-localization, microphone-array, beamforming | acoustics, target-detection | 10 |
| [Speech Enhancement Using a Microphone Array Mounted on an Unmanned Aerial Vehicle](acoustics/Microphone_Array_Noise_Reduction_UAV_Speech.md) | 2016 | acoustics | speech-enhancement, rotor-noise-reduction, beamforming | acoustics | 10 |
| [An Acoustic Source Localization Method Using a Drone-Mounted Phased Microphone Array](acoustics/Sound_Source_Localization_Outdoor_SAR_Drone.md) | 2021 | acoustics | acoustic-SAR, victim-voice-localization, phased-array | acoustics, uav-sar | 9 |

### Topic: uav_sar

| Paper | Year | Primary Topic | Key Tags | MANAR Relevance | Score |
|---|---:|---|---|---|---:|
| [Using Unmanned Aerial Vehicles in Wilderness Search and Rescue](uav_sar/Using_UAVs_in_Wilderness_Search_and_Rescue.md) | 2008 | uav_sar | wilderness-SAR, human-in-the-loop, UAV-search-patterns | uav-sar, target-detection | 10 |
| [Toward a Fully Autonomous UAV: Research Platform for Indoor and Outdoor Urban Search and Rescue](uav_sar/Toward_Fully_Autonomous_UAV_Research_Platform.md) | 2012 | uav_sar | autonomous-UAV, SAR-platform, system-architecture | uav-sar, navigation | 10 |
| [Coverage Path Planning for Search and Rescue Drones in Complex Environments](uav_sar/Coverage_Path_Planning_Search_and_Rescue_Drones.md) | 2016 | uav_sar | coverage-path-planning, lawnmower-pattern, SAR-optimization | uav-sar, navigation | 9 |

### Topic: navigation

| Paper | Year | Primary Topic | Key Tags | MANAR Relevance | Score |
|---|---:|---|---|---|---:|
| [Vision-Controlled Micro Flying Robots: From System Design to Autonomous Navigation and Mapping in GPS-Denied Environments](navigation/Vision_Controlled_Micro_Flying_Robots_Autonomy.md) | 2014 | navigation | visual-odometry, GPS-denied-navigation, MAV-autonomy | navigation | 10 |
| [A UAV for Inspection: Control and Vision-Based Tracking in Dynamic Environments](navigation/UAV_Inspection_Control_Vision_Based_Tracking.md) | 2007 | navigation | visual-servoing, sensor-stabilization, UAV-control | navigation, target-detection | 9 |

### Topic: payload_swap

| Paper | Year | Primary Topic | Key Tags | MANAR Relevance | Score |
|---|---:|---|---|---|---:|
| [A Survey on Optimized Execution of Deep Learning Models on Edge Devices](payload_swap/Survey_Optimized_Execution_Deep_Learning_Edge_Devices.md) | 2019 | payload_swap | edge-AI, model-quantization, TensorRT | payload-swap, target-detection | 10 |
| [Managing Edge Compute Systems for Unmanned Aerial Vehicles](payload_swap/Managing_Edge_Compute_Systems_for_UAVs.md) | 2020 | payload_swap | UAV-edge-compute, SWaP-power-budget, battery-endurance | payload-swap, uav-sar | 9 |

