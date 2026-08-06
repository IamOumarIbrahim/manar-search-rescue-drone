# Search-and-Rescue Drone System Concept

  

## 1. Core Architecture

  

> **Human defines and supervises the mission; deterministic autopilot flies the drone; machine learning identifies possible rescuees and requests inspection.**

  

The drone uses **supervised autonomy**:

  

- An onboard autopilot flies and stabilizes the drone.

- A human operator defines the mission, supervises the system, and can take control at any time.

- Machine learning identifies possible rescuees but does **not** directly control the motors.

- Deterministic code applies fixed equations, thresholds, timers, safety rules, and state transitions.

  

---

  

## 2. Flight and Payload Requirements

  

### 2.1 Flight Envelope

  

> **The drone shall take off, climb, and hover with its complete rescue payload at elevations up to 2,000 m AMSL, while maintaining terrain-relative flight between 10 and 120 m AGL. It shall support a 200 m vertical climb from its launch point.**

  

The drone should follow terrain rather than maintain one fixed altitude above sea level.

  

```text

Mountain rises by 100 m

        ↓

Drone climbs enough to maintain 60–120 m AGL

```

  

### 2.2 Rescue Payload

  

- RGB/day camera

- IR/night camera

- Thermal camera

- 24 GHz FMCW radar

- Passive phone-RF receiver

- Microphone array

- Onboard computer for CNN, Mamba, and software processing

- Speaker

- Beacon

- Smoke-marker mechanism

  

### 2.3 Endurance Constraint

  

For **3–4 hours of continuous flight**, the required battery would be extremely large. A single-battery, sensor-heavy multirotor is not realistically capable of a 3–4-hour mission using conventional lithium batteries.

  
| Average drone power | 3-hour battery | 4-hour battery | Approximate battery mass |
| ------------------: | -------------: | -------------: | -----------------------: |
|          **1.1 kW** |    **4.1 kWh** |    **5.5 kWh** |             **20–27 kg** |
|          **1.5 kW** |    **5.6 kWh** |    **7.5 kWh** |             **27–36 kg** |
|          **2.0 kW** |    **7.5 kWh** |     **10 kWh** |             **36–48 kg** |

---

  

## 3. Responsibility Partition

  

### 3.1 Human Operator

  

The operator:

  

- Defines the search area and mission objective.

- Draws the search area on a map.

- Selects the launch point, maximum altitude, route, geofence, and safety boundaries.

- Approves takeoff.

- Starts, pauses, or aborts the mission.

- Monitors the live map and sensor feeds.

- Reviews candidate-rescuee alerts.

- Makes the final confirmation that the target is likely a person.

- Approves close approach.

- Approves deployment of smoke, beacon, or other guidance equipment.

- Contacts rescue teams and shares confirmed coordinates.

- Takes manual control when required.

- Handles emergency landing, abort, and return-to-home decisions.

  

### 3.2 Deterministic Code

  

These functions follow fixed equations, thresholds, timers, and rules.

  

#### Flight, Navigation, and Safety

  

- Autonomous search-path generation

- GPS waypoint navigation

- Geofencing

- Obstacle avoidance

- Altitude control

- Battery monitoring

- Return-to-home logic

- Sensor health checks

- Fault detection

- Safety restrictions preventing lights or mirrors from pointing toward aircraft or roads

  

#### Sensor and Mission Coordination

  

- Day/night switching using a lux sensor

- Camera and sensor synchronization

- Speaker, strobe, mirror, and smoke-marker control

- GPS tagging of every detection

- Candidate → inspect → confirm state machine

- Detection persistence checks across several frames

- Two-sensor voting or consensus rules

- Bounding-box position matching between cameras

  

#### Audio Processing

  

- SOS buzzer timing

- English/Arabic announcement timing

- Silent listening-window timing

- Microphone-array beamforming

- Acoustic direction estimation

- Rotor-noise filtering using motor RPM

  

#### Passive RF Processing

  

- Frequency scanning

- Signal-strength measurement

- RF direction estimation from measurements taken at several drone positions

  

#### FMCW Radar Processing

  

- Range FFT

- Doppler FFT

- Range–Doppler map

- CFAR detection

- Range measurement

- Velocity measurement

- Phase-based micro-motion extraction

  

### 3.3 Machine Learning

  

These functions require trained models and annotated datasets.

  

#### Computer Vision

  

- Human detection in RGB images

- Human detection in IR/night-vision images

- Human detection in thermal images

- Detection of:

  - Standing people

  - Sitting people

  - Lying people

  - Partially buried people

  - Partially visible body parts

  - Motionless people

- Rejection of:

  - Rocks

  - Bushes

  - Animals

  - Shadows

  - Hot sand

- Multi-frame visual tracking

- Person segmentation, if added later

  

#### Audio Classification

  

- Shouting detection

- Speech or human-voice detection

- Whistle detection

- Clapping detection

- Tapping or impact-pattern detection

- Crying detection

- Separation of human sounds from wind, animals, and rotor noise

  

#### FMCW Classification

  

Deterministic DSP extracts radar features. Machine learning may then classify them as:

  

- Human movement

- Breathing-like micro-motion

- Animal movement

- Vegetation or moving debris

- Drone-vibration interference

- Unknown target

  

#### Passive RF Classification

  

Machine learning may classify received bursts as:

  

- Cellular

- Wi-Fi

- Bluetooth

- Drone telemetry

- Background infrastructure

- Unknown RF transmission

  

#### Sensor Fusion

  

A model such as Mamba can combine several seconds of observations and classify the result as:

  

```text

No target

Possible rescuee

Probable rescuee

High-priority rescuee

```

  

---

  

## 4. Guidance and Control

  

### 4.1 Deterministic Guidance Architecture

  

A Pixhawk-class autopilot runs the safety-critical flight controls.

  

```text

Operator search area

        ↓

Mission planner creates search route

        ↓

Autopilot follows GPS waypoints

        ↓

IMU + GNSS + compass + barometer stabilize the drone

        ↓

Radar altimeter maintains height above terrain

```

  

### 4.2 Navigation Sensors

  

- **GNSS/GPS:** global position

- **IMU:** roll, pitch, yaw, and acceleration

- **Compass:** heading

- **Barometer:** approximate altitude

- **Downward radar altimeter:** actual height above snow, sand, or terrain

- **Forward/side radar:** obstacle detection in fog, darkness, and dust

- **Terrain map:** anticipation of slopes and mountains

  

### 4.3 Autonomous Mission States

  

```text

1. TAKEOFF

2. CLIMB TO SEARCH HEIGHT

3. FOLLOW SEARCH PATTERN

4. DETECT POSSIBLE RESCUEE

5. SLOW DOWN AND APPROACH

6. HOVER AND VERIFY

7. ALERT OPERATOR

8. MARK LOCATION / GUIDE RESCUEE

9. CONTINUE SEARCH OR RETURN HOME

```

  

### 4.4 Search Patterns

  

- Use a lawnmower or parallel-track pattern in open desert, snowfields, and relatively flat terrain.

- Use contour-following routes in mountain areas, remaining approximately parallel to the slope.

  

### 4.5 Machine-Learning Output to the Controller

  

Machine learning provides candidate information rather than motor commands.

  

```text

Possible person detected

Confidence: 82%

Direction: 35° right

Estimated ground position: GPS coordinate

```

  

The deterministic controller then applies the mission rules.

  

```text

If detection persists:

    slow down

    approach candidate

    maintain safe altitude

    point sensors toward target

    begin verification

```

  

This keeps flight control predictable when the model produces a false detection.

  

### 4.6 Candidate-Inspection Behavior

  

When a possible rescuee is found:

  

1. Record the target’s estimated ground coordinates.

2. Reduce speed.

3. Descend to an authorized inspection altitude.

4. Hover or circle the location.

5. Point RGB/IR, thermal, and radar sensors toward the target.

6. Run the bilingual call-and-listen cycle.

7. Wait for sensor agreement or operator confirmation.

  

### 4.7 Communication and Manual Control

  

Use a dedicated encrypted command-and-control radio rather than relying solely on public cellular service.

  

The communication configuration may include:

  

- Primary long-range control radio

- Separate video/data link

- Optional LTE/5G backup where coverage exists

- Optional satellite link for remote regions

- Onboard mission storage so the drone can continue safely during brief link loss

  

### 4.8 Fail-Safe Actions

  

| Failure                     | Drone response                                           |
| --------------------------- | -------------------------------------------------------- |
| Control link briefly lost   | Hover or continue the safe route                         |
| Link not restored           | Return to home                                           |
| GNSS degraded               | Slow down and use IMU/radar positioning                  |
| Battery reaches reserve     | Stop searching and return                                |
| Dangerous wind              | Abort or land at a safe point                            |
| Sensor failure              | Disable the affected search mode and notify the operator |
| Obstacle detected           | Stop, climb, or reroute                                  |
| Terrain clearance uncertain | Hover and wait for the operator                          |
  

---

  

## 5. Detection and Verification Stack

  

A strong rescue stack consists of:

| Component                             | Role                                                     |
| ------------------------------------- | -------------------------------------------------------- |
| **Thermal camera**                    | Primary person detection                                 |
| **RGB/day camera**                    | Daytime verification                                     |
| **Low-light or IR camera**            | Night visual confirmation                                |
| **24 GHz FMCW radar**                 | Presence, range, movement, and micro-motion confirmation |
| **Speaker and microphone array**      | Response prompting and acoustic confirmation             |
| **Passive RF receiver**               | Cellular, Wi-Fi, and Bluetooth detection                 |
| **Beacon, strobe, mirror, and smoke** | Attracting attention and guiding the rescuee             |

  

### Operating Modes

  

```text

Day:

RGB camera + thermal camera → ML person detection

  

Night:

Low-light/IR camera + thermal camera → ML person detection

  

Poor visibility:

Thermal camera + 24 GHz radar → presence confirmation

  

Possible rescuee found:

Hover → radar micro-motion check → camera verification → alert rescue team

```

  

---

  

## 6. Day, Night, and Ambient-Light Switching

  

### Day Mode

  

- Thermal camera

- Daylight/RGB camera

  

### Night Mode

  

- Thermal camera

- Low-light or infrared night-vision camera

  

Use a **light sensor as the primary trigger** and local solar time only as a backup. A time-only system could incorrectly select day mode during a sandstorm, heavy fog, dark clouds, or deep shadow.

  

A raw LDR can be used, but a digital lux sensor such as a **BH1750** or **VEML7700** provides more consistent measurements.

  

|     Ambient light | Mode                                                    |
| ----------------: | ------------------------------------------------------- |
| Above **150 lux** | Daylight/RGB + thermal                                  |
|    **50–150 lux** | Transition mode: RGB + low-light/night camera + thermal |
|  Below **50 lux** | Night/low-light imaging + thermal                       |

  

### Hysteresis

  

- Switch to night mode only when ambient light remains below **50 lux for 30 seconds**.

- Switch back to day mode only when ambient light remains above **150 lux for 30 seconds**.

  

---

  

## 7. Computer Vision and Dataset

  

Computer vision with machine learning is the **main wide-area detection system**. The 24 GHz FMCW radar acts as a confirmation sensor.

  

### 7.1 Dataset Requirements

  

The image dataset should represent the drone’s actual operating conditions:

  

- Different flight altitudes

- Different camera angles

- Morning, midday, sunset, and night

- Hot and cold desert ground

- White, black, and beige clothing

- Partial burial in sand

- Fog, dust, rain, and simulated reduced visibility

- People lying completely still

- Different body sizes and poses

  

---

  

## 8. FMCW Radar

  

FMCW radar is used for:

  

- Confirmation

- Close-to-medium-range human detection

- Poor-visibility support

- Micro-motion detection while the drone is stable

  

FMCW radar does not replace:

  

- Thermal imaging

- Daylight/night cameras

- Audio hailing and listening

- Passive RF or phone-based detection

  

> **Keep FMCW active during day and night. Use low-power scanning during flight and full-resolution verification during hover.**

  

### 8.1 Operation by Mission Phase

  
| Search phase                         | FMCW operation                                                              |
| ------------------------------------ | --------------------------------------------------------------------------- |
| Normal wide-area flight              | Run continuously at a lower update rate or pulse periodically to save power |
| Camera or thermal candidate detected | Increase the radar update rate and focus on the candidate area              |
| Close-range hover                    | Use full FMCW processing for movement and breathing confirmation            |
| Battery-critical return flight       | Disable FMCW unless it is needed for obstacle avoidance                     |

  

### 8.2 Environmental Performance

  

FMCW is usually useful in:

  

- Night

- Haze

- Fog

- Dust

- Moderate sand

- Smoke

  

Detection becomes more difficult in:

  

- Heavy rain

- Dense wet snow

- Hail

- Very cluttered environments

- Strong drone vibration or motion

- Environments with moving vegetation or debris

  

Frequency-band trade-off:

  

- Lower-frequency radar generally penetrates better.

- Higher-frequency mmWave provides better resolution but is more affected by atmospheric losses and clutter.

  

A 24 GHz FMCW radar commonly provides about **250 MHz of sweep bandwidth**. Two targets separated by less than roughly **60 cm in range** may therefore be difficult to distinguish.

  

---

  

## 9. Acoustic Detection and Call-and-Listen System

  

### 9.1 Recommended Acoustic Sensor

  

Use a **4–8 microphone digital MEMS array** mounted underneath the drone.

  

```text

4–8 MEMS microphones

        ↓

Wind and rotor-noise filtering

        ↓

Beamforming / direction estimation

        ↓

ML sound classifier

        ↓

Possible rescuee direction

```

  

The useful speech-intelligibility range is approximately **300 Hz–5 kHz**. A practical processing band of **200 Hz–8 kHz** also captures whistles, claps, and many impact sounds.

  

### 9.2 Sound Classes

  

```text

shouting_for_help

normal_speech

whistle

clapping

metal_or_rock_tapping

crying

animal

wind

drone_rotor_noise

unknown

```

  

### 9.3 Microphone Placement

  

Mount the microphone array:

  

- Underneath the drone

- As far as practical from the motors

- Behind acoustic foam or wind protection

- On vibration-isolating rubber mounts

- With the microphones spaced around a circular plate

  

### 9.4 Listening Behavior

  

During the 15-second listening period:

  

- Turn the speaker and buzzer completely off.

- Hold the drone in position.

- Record continuously.

- Use known motor RPM for noise cancellation.

- Check for voice, whistles, claps, and tapping.

- Estimate the sound-arrival direction using the microphone array.

  

Create an acoustic candidate when:

  

- A human voice, whistle, clap, or tapping event is detected.

- Confidence is at least **70%**.

- The event appears twice during the listening period.

  

Then:

  

- Save the estimated direction.

- Turn or move toward that direction.

- Repeat the announcement and listening cycle.

- Inspect the location using thermal imaging, cameras, and FMCW radar.

  

### 9.5 SOS Sequence

  

### Buzzer Pattern

  

- 3 short buzzes: **0.4 s each**, with **0.3 s gaps**

- 3 long buzzes: **1.2 s each**, with **0.3 s gaps**

- 3 short buzzes: **0.4 s each**, with **0.3 s gaps**

  

The complete SOS buzzer pattern lasts **8.4 seconds**.

  

### Spoken Message

  

**English:**

  

> “Rescue drone. Shout, whistle, clap, or hit an object now.”

  

**Arabic:**

  

> هذه طائرة إنقاذ. اصرخ، صفّر، صفّق، أو اضرب أي جسم الآن

  

### Cycle Timing

  

- SOS buzzer: **8.4 seconds**

- English and Arabic messages: **12 seconds total**

  - 6 seconds in English

  - 6 seconds in Arabic

- Silent listening period: **15 seconds**

- Complete cycle: **35.4 seconds**

- Repeat the complete cycle **3 times**

  

```text

SOS buzzer

    ↓

English and Arabic spoken instruction

    ↓

15 seconds of silence and listening

    ↓

Repeat

```

  

The silent listening window is essential because continuous buzzing or speech would mask a survivor’s voice, whistle, clapping, or tapping.

  

During the listening period:

  

> No buzzing, no speaker playback, and the drone flies or hovers as steadily as possible.

  

Reference retained from the source document:

  

https://www.fema.gov/pdf/emergency/usr/appen_a.pdf

  

---

  

## 10. Passive Phone-RF Detection

  

### 10.1 Phone-Associated RF Sources Considered

  
| Source                                          | Approximate frequencies                                 | Typical detectable range                                                                                                                              |
| ----------------------------------------------- | ------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------- |
| 2G/3G/4G cellular                               | About 600 MHz–2.7 GHz                                   | A few metres with a basic RF detector; hundreds of metres to several kilometres with a sensitive receiver and suitable antenna                        |
| 5G sub-6 GHz                                    | About 600 MHz–7.1 GHz                                   | Hundreds of metres to kilometres, depending strongly on the frequency band                                                                            |
| 5G millimetre-wave on supported phones/networks | Roughly 24–40 GHz, sometimes higher depending on region | Usually tens to a few hundred metres with clear line of sight; walls, hands, and objects block it heavily                                             |
| Wi-Fi                                           | 2.4 GHz, 5 GHz, and 6 GHz                               | Normally 20–100 m indoors or outdoors; several hundred metres with line of sight and directional equipment                                            |
| Bluetooth                                       | About 2.4 GHz                                           | Commonly 5–30 m; sometimes around 100 m or more with Bluetooth Low Energy and good conditions                                                         |
| NFC                                             | 13.56 MHz                                               | Normally 1–4 cm; specialized antennas may detect activity from tens of centimetres, but not usually metres                                            |
| GPS reception                                   | Approximately 1.2–1.6 GHz                               | The phone normally receives GPS and does not transmit an ordinary GPS signal to detect                                                                |
| Internal electronics                            | Weak unintended emissions from kilohertz into gigahertz | Usually centimetres to a few metres with ordinary equipment; greater distance requires specialized laboratory receivers and shielding-free conditions |
  

### 10.2 Practical Detection Priorities

  

After filtering out weaker or impractical signals:

  
| Source                | Approximate frequencies |                                       Practical detectable range |
| --------------------- | ----------------------: | ---------------------------------------------------------------: |
| **2G/3G/4G cellular** |     **600 MHz–2.7 GHz** | Hundreds of metres to several kilometres with suitable equipment |
| **5G sub-6 GHz**      |     **600 MHz–7.1 GHz** |                                 Hundreds of metres to kilometres |
| **Wi-Fi**             |   **2.4, 5, and 6 GHz** |            Typically 20–100 m; farther with directional antennas |
| **Bluetooth**         |       **About 2.4 GHz** |                     Typically 5–30 m; occasionally 100 m or more |

  

Broad approximation order:

  

1. Start with 4G and earlier cellular signals.

2. Follow with sub-6 GHz 5G.

3. Use Wi-Fi and Bluetooth for closer-range approximation.

  

### 10.3 Receiver Options

  

| Option                 |     Coverage | Suitability                                                                        |
| ---------------------- | -----------: | ---------------------------------------------------------------------------------- |
| **HackRF One**         |  1 MHz–6 GHz | Lower-cost proof of concept                                                        |
| **USRP B205mini-i**    | 70 MHz–6 GHz | Better professional prototype; up to 56 MHz instantaneous bandwidth                |
| **AD8318 RF detector** |  1 MHz–8 GHz | Detects RF power only and cannot reliably identify a signal as coming from a phone |

  

### 10.4 Antennas

  

Do not use one small omnidirectional antenna for the full frequency range. Use at least:

  

- A **600 MHz–2.7 GHz directional antenna** for cellular signals

- A **2.4–6 GHz directional antenna** for 5G, Wi-Fi, and Bluetooth

  

### 10.5 Detection Logic

  

During flight, the receiver should:

  

1. Scan only configured phone uplink bands.

2. Ignore stronger cellular-tower downlink bands.

3. Detect short RF bursts above the local noise level.

4. Classify the signal as cellular, Wi-Fi, Bluetooth, or unknown.

5. Record drone position, heading, and received power.

6. Compare measurements from several drone positions.

7. Estimate the likely source direction and approach cautiously.

  

---

  

## 11. Multisensor Fusion and 2D-to-1D Mapping

  

### 11.1 Recommended Approach

  

- Use **YOLO/CNN models** separately on RGB, IR, and thermal images.

- Use detected bounding boxes to match the same target between cameras.

- Use **Global Average Pooling embeddings** only when building a learned sensor-fusion classifier.

- Keep FMCW range, RF strength, and acoustic direction as additional one-dimensional numerical features.

  

Role split:

  

- **CNN:** detects people separately in RGB, IR, and thermal images.

- **Mamba:** combines camera, radar, audio, RF, and drone-motion information over several consecutive seconds.

- **Deterministic controller:** decides whether the drone continues searching, approaches, hovers, or reports a rescuee.

  

### 11.2 Stage 1: Per-Sensor Processing

  

```text

RGB image     → CNN detector → boxes + confidence + feature vector

IR image      → CNN detector → boxes + confidence + feature vector

Thermal image → CNN detector → boxes + confidence + feature vector

Radar         → range, motion, and micro-motion features

Audio         → voice, whistle, and tapping features

RF receiver   → cellular, Wi-Fi, and Bluetooth features

```

  

### 11.3 Stage 2: One Feature Vector per Time Step

  

```text

[

  RGB_person_confidence,

  IR_person_confidence,

  thermal_person_confidence,

  target_x,

  target_y,

  radar_range,

  radar_motion_score,

  radar_breathing_score,

  human_voice_score,

  whistle_score,

  phone_RF_score,

  drone_altitude,

  drone_speed

]

```

  

Additional sensor-fusion inputs listed in the source include:

  

```text

Target position

Drone motion

Weather and visibility

```

  

### 11.4 Stage 3: Temporal Fusion with Mamba

  

```text

20–50 consecutive feature vectors

                ↓

             Mamba

                ↓

No target / possible rescuee / probable rescuee / high-priority rescuee

```

  

At **10 measurements per second**, 30 feature vectors represent a **3-second observation window**.

  

---

  

## 12. Candidate and Confirmation Logic

  

Two-camera agreement automatically triggers inspection. One strong, persistent detection must also trigger inspection rather than being discarded.

  

### 12.1 Candidate Trigger

  

Create a candidate when either condition is met:

  

#### Condition A: Two-Camera Agreement

  

- Two active cameras detect a person at the same location.

- Confidence is at least **0.55**.

- The detection persists for at least **3 consecutive frames**.

  

#### Condition B: Strong Single-Camera Detection

  

- One camera detects a person.

- Confidence is at least **0.85**.

- The detection persists for at least **5 consecutive frames**.

  

Then:

  

- Slow down.

- Hover.

- Point all sensors at the candidate.

- Begin close-range verification.

  

### 12.2 Rescuee Confirmation

  

Confirm a probable rescuee when any of the following occurs:

  

- Two imaging sensors continue detecting the same target.

  

**OR**

  

- One imaging sensor detects the target and FMCW detects human movement or breathing-like micro-motion.

  

**OR**

  

- The microphone detects shouting, whistling, clapping, or tapping after the announcement.

  

### Deterministic Example

  

```text

If two sensors detect the same location

for at least 3 of the last 5 frames:

    create high-priority candidate

  

If one sensor has confidence above 0.85

for at least 5 frames:

    create inspection candidate

  

If imaging detection is confirmed by radar

or an audible human response:

    mark probable rescuee

```

  

---

  

## 13. LiDAR

  

LiDAR does not help in dense fog and should remain optional.

  

---

  

## 14. Visual Attention and Guidance Systems

  

### 14.1 Light Beacon Setup

  

- A rotating or fixed **360° amber flashing beacon** for attracting attention; the final choice between rotating and fixed remains undecided.

- A directional, long-distance, high-beam white flash operating at **2 Hz**.

- A downward white spotlight for close-range inspection.

  

The setup is intended to create as much light “pollution” as possible.

  

Original image reference:

![[Pasted image 20260805165052.png]]

  

### 14.2 Rotating Heliograph-Style Mirror

  

Mirror tilt from horizontal:

$$\text{Mirror surface tilt from horizontal}

=

45 + \frac{\text{solar elevation}}{2}

$$

  
For a representative Dubai location around **25.2° N, 55.3° E**:

  

| Period          | Noon Sun elevation | Required mirror tilt |
| --------------- | -----------------: | -------------------: |
| Winter solstice |    About **41.4°** |      About **65.7°** |
| Equinoxes       |    About **64.8°** |      About **77.4°** |
| Summer solstice |    About **88.2°** |      About **89.1°** |

  
|Dubai time|Sun azimuth|Sun elevation|Mirror tilt for straight down|
|---|--:|--:|--:|
|08:00|**83°**, east|**28°**|**59°**|
|10:00|**96°**, east-southeast|**55°**|**73°**|
|12:24 solar noon|**178°**, south|**82°**|**86°**|
|14:00|**255°**, west-southwest|**67°**|**78°**|
|16:00|**272°**, west|**40°**|**65°**|
|18:00|**283°**, west-northwest|**13°**|**51°**|

  

The source leaves open whether to use the calculated tilt or a rotating **45–90° range**.

  

Reference retained from the source document:

  

https://gml.noaa.gov/grad/solcalc/azel.html

  

### 14.3 Ground Distance Covered

  

For drone altitude \(H\), the horizontal ground distance reached by the reflected flash is:

  

$$

\boxed{R = H\tan\theta}

$$

  

For wider coverage without electronic beam steering, use three small mirrors at different tilts:

  
|Mirror|Angle from vertical|Purpose|
|---|--:|---|
|Mirror 1|**40°**|Near-range flash|
|Mirror 2|**60°**|Medium-range flash|
|Mirror 3|**75°**|Far-range flash|
  

At **100 m drone altitude**, the approximate ground distances are:

  

- **40°:** 84 m

- **60°:** 173 m

- **75°:** 373 m

  

Mirror configuration:

  

- Use three **8 × 8 cm** lightweight acrylic or polycarbonate mirrors.

- Approximate total mass: **25–50 g**.

- Keep the rotation axis vertical.

- Mount each mirror at a fixed, adjustable tilt.

- Rotate at approximately **1–3 revolutions per second**.

  

### 14.4 Mirror Performance in Bad Weather

  

|Condition|Mirror usefulness|
|---|---|
|Clear daylight|**Excellent**|
|Thin haze|Good|
|Light rain or snow|Moderate|
|Dense fog|Poor|
|Heavy rain or snowstorm|Poor|
|Sandstorm|Very poor|
|Thick cloud cover|Unusable|
  

### 14.5 Smoke Marker

  

Most emergency flare guns use **red flares**.

  

Once the drone believes it is near the rescuee, it deploys a fluorescent orange smoke trail to support close-range detection.

  

| Condition          | Orange-smoke effectiveness                                                             |
| ------------------ | -------------------------------------------------------------------------------------- |
| Clear Dubai desert | **Good**, although orange can partially blend with sand when viewed against the ground |
| Snow               | **Excellent contrast**                                                                 |
| Light rain         | **Moderate**; the smoke remains visible but disperses faster                           |
| Heavy rain or hail | **Poor**; precipitation and wind break up and obscure the plume                        |
| Light fog          | **Limited**                                                                            |
| Dense fog          | **Very poor**, regardless of smoke color                                               |
| Sandstorm          | **Poor**; dust obscures it and strong wind carries it away rapidly                     |
| Night              | **Poor unless illuminated**                                                            |

  

Rotor wash may destroy the smoke trail unless the outlet is positioned behind and outside the propeller wake.

  

---

  

## 15. Open Design Decisions Retained from the Source

  

- Rotating or fixed 360° amber beacon

- Calculated heliograph tilt or a rotating 45–90° tilt range

- LiDAR retained only as an optional sensor