
### Future CV Logic — Not Implemented Yet

D-FINE-N is currently only integrated and tested as a raw human-detection model.

Future MANAR perception logic may include:
- Candidate persistence across frames
- Handling temporarily missed detections
- Confidence history / temporal filtering
- Candidate IDs and tracking
- Association with location and other sensor evidence
- Rules for promoting a detection into a MANAR rescuee candidate

Do not implement or tune this yet. Revisit when the perception/fusion logic is designed.
## Visual Detector Decision — MANAR V1

**Selected model:** YOLO11n  
**Previous baseline:** D-FINE-N  
**Status:** Locked as MANAR V1 human-detection baseline

### Reason

YOLO11n was benchmarked against D-FINE-N using the same MANAR RGB,
infrared, and grayscale thermal test videos.

YOLO11n demonstrated:

- ~30 ms inference latency compared with ~50 ms for D-FINE-N
- Strong moving-target detection across RGB, infrared, and thermal
- Better stationary-target performance
- Better multi-target performance
- Strong false-positive and no-target behavior
- Significantly better partially concealed RGB performance
- Improved concealed IR/thermal performance during the second test round

### Important limitation

This decision applies only to the raw visual detector.

YOLO11n currently produces frame-level human detections only.

Candidate persistence, tracking, temporal confidence, missed-frame handling,
multisensor confirmation, and rescuee-classification logic are NOT yet
implemented and will be designed separately.

### Decision

YOLO11n is accepted as the MANAR V1 visual human-detection model.

D-FINE-N remains retained as the original benchmark/reference detector.