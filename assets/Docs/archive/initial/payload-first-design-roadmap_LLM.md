You should **not** start by choosing an airframe and squeezing MANAR into it, and you also shouldn't blindly insist that every component must stay.

For this project, the right direction is:

> **Mission → required capabilities → payload → aircraft → iterate payload/aircraft together.**

Your enthusiasm for the multisensor idea is actually part of MANAR's identity, so I would preserve the **concept of many complementary sensors**. But each component now has to earn its place through mass, power, volume, usefulness, and integration.

Here's exactly how I'd do the next phase, sequentially.

### 1. Freeze the mission before touching hardware

Write one page called something like:

```text
MANAR V1 Engineering Requirements
```

Define only measurable requirements:

```text
Mission:
Search and rescue

Aircraft:
Multirotor
Supervised autonomy
1 operator

Target:
Locate / verify stranded human

Search duration target:
≥ 1 hour active search

Operating altitude:
10–120 m AGL

Environment:
day / night
desert
mountain
poor visibility

Payload capabilities:
visual
thermal
radar
audio
RF
signaling
```

Don't select models or manufacturers yet.

This becomes the thing every later decision has to satisfy.

---

### 2. Turn your component list into **capabilities**

Don't think:

```text
RGB camera
thermal camera
FMCW
microphone
...
```

Think:

```text
Human visual detection
    → RGB

Heat detection
    → thermal

Night confirmation
    → low-light / IR

Motion / breathing detection
    → FMCW

Voice response
    → speaker + microphone

Electronic-device detection
    → passive RF

Rescuee guidance
    → beacon / strobe / spotlight / smoke
```

This is important because later you may discover two components solve essentially the same problem.

Then you can remove one **without damaging the mission**.

---

### 3. Make every component justify itself

Create a table.

| Component | Why MANAR needs it | Mass | Power | Size | Data rate | Priority |
| --------- | ------------------ | ---: | ----: | ---: | --------: | -------- |
| Thermal   | detect human heat  |    ? |     ? |    ? |         ? | Critical |
| RGB       | identification     |    ? |     ? |    ? |         ? | Critical |
| IR        | night imaging      |    ? |     ? |    ? |         ? | High     |
| FMCW      | motion/breathing   |    ? |     ? |    ? |         ? | High     |
| RF        | device detection   |    ? |     ? |    ? |         ? | Medium   |
| Smoke     | marking            |    ? |     ? |    ? |         ? | ?        |

For now, leave unknown values as `?`.

This table becomes your main engineering document.

---

### 4. Research **real candidate hardware**

Now you finally start looking at actual components.

Not necessarily buying them.

For every subsystem, find **one or two physically real devices** that could plausibly perform the job.

Then record:

```text
dimensions
mass
voltage
current / wattage
interface
operating temperature
price
```

The purpose isn't shopping.

It's preventing fantasy numbers.

If a real thermal module weighs 35 g and consumes 2 W, your simulation should be somewhere around reality—not a fictional 2 g sensor consuming 0.01 W.

---

### 5. Build the complete payload budget

Now add everything.

Example conceptually:

```text
RGB                         80 g
Thermal                     40 g
IR                          90 g
FMCW                        60 g
RF hardware                180 g
Microphones                 40 g
Speaker                     80 g
Spotlight                   70 g
Beacon                      30 g
Smoke system               250 g
Computer                    250 g
Autopilot                    50 g
GNSS / IMU / radar etc.     ...
--------------------------------
Payload total               XXXX g
```

Do the same for **power**.

At this point you finally know what kind of aircraft MANAR needs to carry.

---

### 6. Only now size the drone

This answers your original question.

**The drone gets designed around the mission payload.**

But not completely.

You'll discover something like:

```text
Desired payload = 3.2 kg

Aircraft required to carry it for 1 hour
= enormous / impractical
```

Then you loop backward.

Maybe:

```text
smoke system
250 g → 90 g redesign

RF system
400 g → 180 g

separate RGB + IR cameras
→ combined module

spotlight
120 W → unacceptable
→ smaller unit
```

So it becomes:

```text
PAYLOAD
   ↓
AIRCRAFT
   ↓
"This is too heavy."
   ↓
PAYLOAD REFINEMENT
   ↓
AIRCRAFT REFINEMENT
```

That's engineering iteration.

---

### 7. Choose the propulsion architecture

Once you know approximate takeoff mass, work out:

```text
number of motors
propeller diameter
motor thrust
required thrust-to-weight ratio
battery voltage
ESC rating
maximum current
```

You don't need CFD yet.

You need enough math that someone can look at the design and say:

> "Yes, those motors could physically lift that machine."

---

### 8. Solve the battery problem properly

This is probably going to be one of MANAR's hardest constraints.

Calculate:

```text
hover power
+
computing power
+
sensor power
+
communications
+
lighting when active
```

Then:

```text
battery energy (Wh)
÷
average power (W)
≈
theoretical endurance
```

Then reduce it with realistic reserve/margin assumptions.

You may discover your one-hour requirement forces major design changes.

That's exactly the sort of discovery you want **before writing more software**.

---

### 9. Design the physical layout

Now make a crude block layout before Blender.

Top:

```text
GNSS
communications
```

Front:

```text
RGB
thermal
IR
FMCW
```

Bottom:

```text
radar altimeter
spotlight
```

Sides:

```text
RF antennas
```

Center:

```text
battery
computer
flight controller
```

Then ask:

* Is anything blocking camera FOV?
* Are antennas interfering with each other?
* Is the center of gravity reasonable?
* Is the smoke outlet near prop wash?
* Can thermal sensors see motor heat?
* Can microphones hear anything over propellers?
* Where does cooling airflow go?

This is where MANAR starts becoming a real machine rather than a list of sensors.

---

### 10. Make a wiring/interface architecture

Before Blender detail, define:

```text
BATTERY
   ↓
power distribution
   ├── ESCs
   ├── flight controller
   ├── compute
   ├── cameras
   ├── radar
   ├── RF
   └── auxiliary systems
```

And separately:

```text
Compute
 ├── USB
 ├── Ethernet
 ├── UART
 ├── SPI
 └── CAN
```

Determine approximately how every component communicates.

This becomes hugely useful later when you're implementing the software.

Because you'll know whether a simulated device represents:

```text
camera stream
serial sensor
CAN device
digital switch
network device
```

---

### 11. Produce the first **MANAR hardware specification**

At this point you should be able to produce something like:

```text
MANAR V1 SIMULATED AIRCRAFT

Configuration:
Hexacopter

Dimensions:
xxxx × xxxx × xxxx mm

Estimated MTOW:
x.x kg

Payload:
x.x kg

Battery:
xxS / xxxx mAh / xxx Wh

Estimated hover power:
xxx W

Estimated endurance:
xx minutes

Compute:
...

Sensors:
...

Communications:
...

Propulsion:
...

Estimated thrust:
...

Safety margin:
...
```

That document is the thing I would call your **hardware freeze**.

Not permanent forever.

Just:

> "This is the machine MANAR software currently represents."

---

### 12. Then Blender

Now Blender has engineering constraints.

You're no longer sculpting a futuristic drone.

You know:

```text
motor diameter
propeller diameter
battery dimensions
camera dimensions
sensor positions
computer dimensions
antenna locations
frame dimensions
```

Ideally your Blender model becomes close enough that somebody could use it as the basis for CAD.

One correction to your ambition, though: **"manufacturable within hours" isn't a realistic target from a simulation-only project.** A real aircraft needs CAD tolerances, structural analysis, fasteners, wiring, thermal management, EMC, vibration isolation, fabrication drawings, sourcing, assembly procedures, and physical validation.

A much better target is:

> **MANAR should be specified deeply enough that a competent hardware team could begin building a prototype without having to redesign the entire system from scratch.**

That's achievable and still extremely impressive.

---

## Your return-to-C++ gate

Come back to C++ when you have these five artifacts:

```text
1. MANAR engineering requirements
2. component capability + hardware table
3. mass and power budget
4. frozen V1 aircraft specification
5. component/interface diagram
```

You **do not** need:

```text
finished Blender model
CFD
perfect structural analysis
trained ML
PCB design
manufacturing drawings
```

before returning.

Then your C++ work becomes grounded:

```text
"thermal ON"
```

is no longer just a boolean.

You know what thermal module is being simulated, what it consumes, what data it produces, how often it produces it, and what happens to the rest of the aircraft when it is enabled.

That is the point where I'd send you back into `components.cpp`.
