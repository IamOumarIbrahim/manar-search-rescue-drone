# NEXT STEPS

- define clearly what is MISSION:
    - remove abort mission entirely
    - keep RTH 
> [x] Fixed at: 13/08


---
- change RTH:
    - name change from RTH to RTL
    - RTL needs to run as a on-demand command
    - RTL is not a persistent runtime operation
    - RTL only defines the destination location

> [x] Fixed at: 13/08

---
- speed:
    - remove manual speed alteration.
    - configure mode speeds.
> [x] Fixed at: TBD
---
- extended configuration states:
    - allow operator to save multiple config states for different environments
    - different configs may include: day/night, dry/rainy/snowy/hail, sandstorm/thunderstorm/snowstorm/fog

> [x] Fixed at: TBD
---


- battery:
    - define battery save-mode
    - allow user to config battery save-mode operations in config

> [X] Fixed at: TBD
---
- runtime.json definition:
    - runtime will only snapshot the on-demand:
        - Full component status
        - Full mission state
        - Diagnostics
        - Configuration
        - Detailed battery information
        - RF history
        - Logs
> [ ] Fixed at: TBD
---
- mission start
    - mission start clears any transient old information like destinations, rescuee statuses and resets configuration to the select config. (ADD PROTECTION )
    - mission start doesn't need the drone to be launched
    - configuring the components and destination and others doesn't need the drone to be launched
> [ ] Fixed at: TBD

- search locations:
    - allow the operator to set multiple locations to be searched
    - save the locations as an ordered list
    - allow the control system to move through the locations sequentially
> [ ] Fixed at: TBD
---
# Laid off til later
    - calculate time until CRITICAL through current power usage statistics
    - multi-rescuee support:
        - allow for more than one rescuee
        - allow different states for different rescuees
        - allow operator to set different statuses to different rescuees
> [ ] V2
