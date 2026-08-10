# References & Inspiration

These projects are used as architectural, implementation, and research references.
They are not dependencies of MANAR unless explicitly stated.

## Rescue-Drone

https://github.com/elitehadaroz/Rescue-Drone

Final-year engineering project focused on locating people in distress using a
semi-automated drone system.

### Relevant inspiration

- Separation of drone control and person-detection logic
- Human-in-the-loop rescue workflow
- Mission / route generation
- Drone state and telemetry handling
- Post-detection operator decision flow
- Search-and-rescue reporting
- Simulation and testing structure
- Overall repository organization

### MANAR differences

MANAR is being designed around:

- Deterministic C++ control logic
- Supervised autonomy
- Multisensor detection rather than vision-only detection
- Dedicated TypeScript operator GUI
- Python ML / sensor-fusion layer
- Explicit human authorization gates
- Broader payload and guidance systems
- More structured separation between control, interface, and ML layers