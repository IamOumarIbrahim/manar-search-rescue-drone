# MANAR Behavioral Decisions

This file records owner-level behavioral decisions for the deterministic MANAR control system.

It is intentionally separate from `ROADMAP.md`:
- `ROADMAP.md` defines **what development work remains**.
- `BEHAVIOR_DECISIONS.md` defines **what the system is supposed to mean and do** when the repository or implementation is ambiguous.

---

## Locked Decisions

### BD-01 — Mission Start Comes First

**Decision:** `START_MISSION` is the first mission-lifecycle action.

`START_MISSION` does **not** require the drone to already be launched.

A valid sequence is:

```text
START_MISSION
    ↓
mission becomes active
    ↓
LAUNCH_DRONE
    ↓
flight begins
```

The mission may therefore be active while the aircraft is still on the ground.

---

### BD-02 — Starting a New Mission Resets Previous Mission State

**Decision:** MANAR V1 supports one mission at a time.

Issuing `START_MISSION` for a new mission automatically clears mission-specific state left by the previous mission, including aborted, finished, returning, rescuee-found, waiting-for-help, search progress, and related transient mission state.

A separate `RESET_MISSION` command is not required for V1.

---

### BD-03 — STOP_FLIGHT Means Safe Landing

**Decision:** `STOP_FLIGHT` means the aircraft should decelerate and land safely.

It is not automatically equivalent to `ABORT_MISSION`.

After the stop/landing completes:

- `flight.launched = false`
- `flight.speed = 0`
- `flight.altitude = 0`
- `flight.mode = "Stall"`
- `mission.enroute = false`

The mission itself may remain active.

---

### BD-04 — Relaunch During an Existing Mission Is Allowed

**Decision:** A landed/stopped aircraft may be launched again without creating a new mission.

Relaunching does not reset the active mission or destination.

This supports sequences such as:

```text
START_MISSION
    ↓
LAUNCH_DRONE
    ↓
STOP_FLIGHT
    ↓
LAUNCH_DRONE
    ↓
continue mission
```

---

### BD-05 — RTH Is a One-Time Navigation Command, Not a Destination Lock

**Decision:** `RTH` sets the current destination to the home/base coordinates.

It does not permanently lock the aircraft into returning home.

While RTH is active:

- `mission.returning = true`
- `destination = home`

If `CHANGE_DEST` is later accepted:

- the new destination overrides the home destination;
- `mission.returning` must become `false`.

This preserves the invariant that `returning == true` means the current destination is actually home.

---

### BD-06 — Manual Speed May Differ From Flight-Mode Preset

**Decision:** Flight modes provide speed presets, but the operator may manually override speed afterward.

Example:

```text
mode = "Quick"
speed = 2 m/s
```

is valid.

Changing speed manually does not need to rename or change the current flight mode.

The only hard requirement is that manual speed remains inside configured safety bounds.

---

### BD-07 — Destination May Change While Waiting for Help

**Decision:** `CHANGE_DEST` is permitted while `mission.waitingforhelp == true`.

Changing the destination does not by itself launch the aircraft or cause movement.

The aircraft remains landed until a separate action causes flight to resume.

---

### BD-08 — ABORT_MISSION Initiates RTH

**Decision:** `ABORT_MISSION` terminates the current mission objective and initiates return-to-home behavior.

The intended state transition is:

```text
ABORT_MISSION
    ↓
mission.aborted = true
mission.enroute = false
destination = home
mission.returning = true
```

If the aircraft is already airborne, it begins/continues return flight.

**Remaining detail:** behavior when abort is issued while the aircraft is already landed (for example, while waiting for help) still needs to be locked: either automatic relaunch or explicit `LAUNCH_DRONE` before movement.

---

## Hard Behavioral Invariants

These invariants should remain true after every completed command/state transition.

### Mission Lifecycle

**INV-01 — One active mission**
MANAR V1 operates one mission at a time.

**INV-02 — Mission start does not require launch**
`mission.started == true` does not imply `flight.launched == true`.

**INV-03 — New mission state is clean**
A new `START_MISSION` must not inherit aborted, finished, returning, rescuee-found, waiting-for-help, or search-progress state from the previous mission.

**INV-04 — Stopping flight does not automatically abort the mission**
`flight.launched == false` may coexist with `mission.started == true`.

**INV-05 — Enroute means actively airborne and traveling**
If `mission.enroute == true`, then `flight.launched == true`.

---

### Flight State

**INV-06 — Grounded flight state**
If `flight.launched == false`, then:

```text
flight.speed == 0
flight.altitude == 0
flight.mode == "Stall"
```

**INV-07 — STOP_FLIGHT ends in grounded state**
After `STOP_FLIGHT` completes, INV-06 must hold and `mission.enroute == false`.

**INV-08 — Relaunch preserves mission**
`LAUNCH_DRONE` during an already-active mission must not reset mission state or replace the current destination.

**INV-09 — Manual speed is bounded**
At all times:

```text
0 <= flight.speed <= config.maximum_speed
```

**INV-10 — Manual speed is independent of mode preset**
No invariant requires `flight.speed` to equal the preset associated with `flight.mode`.

---

### Destination / RTH

**INV-11 — Returning means destination is home**
If:

```text
mission.returning == true
```

then:

```text
destination == home
```

**INV-12 — RTH sets home as destination**
After a successful `RTH` transition:

```text
destination = home
mission.returning = true
```

**INV-13 — Destination override cancels returning status**
If `CHANGE_DEST` changes the destination away from home while `mission.returning == true`, then:

```text
mission.returning = false
```

RTH is therefore a command that sets navigation state, not an irreversible mode.

---

### Waiting for Help

**INV-14 — Waiting-for-help is a landed state**
If `mission.waitingforhelp == true`, the aircraft must be safely landed unless a later explicit command changes that state.

**INV-15 — Destination changes do not cause implicit launch**
Changing destination while waiting for help does not by itself launch the aircraft or begin movement.

---

### Abort

**INV-16 — Abort initiates RTH state**
After `ABORT_MISSION` is accepted:

```text
mission.aborted = true
mission.enroute = false
mission.returning = true
destination = home
```

Any later explicit `CHANGE_DEST` may override RTH according to BD-05 and INV-13.

---

### Command / Runtime Integrity

**INV-17 — Every command has an explicit result**
Every accepted command request must end as either successful or explicitly rejected. No command should be silently consumed.

**INV-18 — Runtime is authoritative and complete**
`runtime.json` must represent a complete snapshot of the control-owned state rather than a partially populated set of fields.

**INV-19 — Runtime transitions are externally consistent**
External readers must not observe a half-applied state transition.

**INV-20 — Control owns runtime persistence**
Subsystem methods may change their own internal state or return results, but the control authority is responsible for constructing and persisting the canonical runtime snapshot.

---

## Still Unresolved

### UD-01 — Component State Restrictions

This means deciding whether a component is allowed to be enabled/disabled in every system state.

Examples:

- Can the smoke marker be activated while the aircraft is on the ground?
- Can cameras be disabled during an active search?
- Can the spotlight be enabled while waiting for help?
- Can sensors be changed during RTH?

No restriction should be invented until MANAR's intended behavior requires one.

---

### UD-02 — Explicit Component Authorization

This means deciding whether certain component actions require an extra operator approval/confirmation rather than behaving like ordinary toggles.

Potential examples include:

- smoke marker deployment;
- high-intensity spotlight;
- amber beacon;
- white strobe.

This is different from a state restriction:

- **State restriction:** "This action is invalid in the current system state."
- **Authorization:** "This action is valid, but the operator must explicitly approve it."

---

### UD-03 — Abort While Already Landed

`ABORT_MISSION` initiates RTH, but the aircraft may already be landed (for example while waiting for help).

Still to decide:

1. Abort automatically relaunches the aircraft and begins RTH; or
2. Abort sets the RTH destination/state, but waits for an explicit `LAUNCH_DRONE`.

---

## Implementation Rule

When implementing Roadmap Milestones 1–4:

1. Existing code should be changed to satisfy these locked decisions and invariants.
2. Unresolved decisions should **not** be guessed by the implementation.
3. If current code conflicts with a locked decision, the locked decision is authoritative.
4. Future GUI and ML layers must follow the same behavioral contract rather than defining their own mission semantics.
