#include "components.hpp"

#include <iostream>

using namespace std;

// CONSTRUCTOR
components::components()
{
    batterySaveMode();
}

void components::thermal(int X)
{
    if (X == 1)
    {
        thermal_camera_status = true;
        runtime["components"]["thermal_camera"] = thermal_camera_status;
        saveRuntime();

        logEvent("PAYLOAD", "INFO", "Thermal camera ON");
    }
    else if (X == 2)
    {
        thermal_camera_status = false;

        logEvent("PAYLOAD", "INFO", "Thermal camera OFF");
    }

    runtime["components"]["thermal_camera"] = thermal_camera_status;
    saveRuntime();
}

void components::rgb(int X)
{
    if (X == 1)
    {
        RGB_camera_status = true;

        logEvent("PAYLOAD", "INFO", "RGB camera ON");
    }
    else if (X == 2)
    {
        RGB_camera_status = false;

        logEvent("PAYLOAD", "INFO", "RGB camera OFF");
    }

    runtime["components"]["rgb_camera"] = RGB_camera_status;
    saveRuntime();
}

void components::infrared(int X)
{
    if (X == 1)
    {
        infrared_camera_status = true;

        logEvent("PAYLOAD", "INFO", "Infrared camera ON");
    }
    else if (X == 2)
    {
        infrared_camera_status = false;

        logEvent("PAYLOAD", "INFO", "Infrared camera OFF");
    }

    runtime["components"]["infrared_camera"] = infrared_camera_status;
    saveRuntime();
}

void components::fmcw(int X)
{
    if (X == 1)
    {
        fmcw_status = true;

        logEvent("PAYLOAD", "INFO", "FMCW radar ON");
    }
    else if (X == 2)
    {
        fmcw_status = false;

        logEvent("PAYLOAD", "INFO", "FMCW radar OFF");
    }

    runtime["components"]["fmcw_radar"] = fmcw_status;
    saveRuntime();
}

void components::speaker(int X)
{
    if (X == 1)
    {
        speaker_status = true;

        logEvent("PAYLOAD", "INFO", "Speaker ON");
    }
    else if (X == 2)
    {
        speaker_status = false;

        logEvent("PAYLOAD", "INFO", "Speaker OFF");
    }

    runtime["components"]["speaker"] = speaker_status;
    saveRuntime();
}

void components::microphone(int X)
{
    if (X == 1)
    {
        microphone_status = true;

        logEvent("PAYLOAD", "INFO", "Microphone ON");
    }
    else if (X == 2)
    {
        microphone_status = false;

        logEvent("PAYLOAD", "INFO", "Microphone OFF");
    }

    runtime["components"]["microphone"] = microphone_status;
    saveRuntime();
}

void components::rf(int X)
{
    if (X == 1)
    {
        passive_rf_status = true;

        logEvent("PAYLOAD", "INFO", "Passive RF ON");
    }
    else if (X == 2)
    {
        passive_rf_status = false;

        logEvent("PAYLOAD", "INFO", "Passive RF OFF");
    }

    runtime["components"]["passive_rf"] = passive_rf_status;
    saveRuntime();
}

void components::beacon(int X)
{
    if (X == 1)
    {
        amber_beacon_status = true;

        logEvent("PAYLOAD", "INFO", "Amber beacon ON");
    }
    else if (X == 2)
    {
        amber_beacon_status = false;

        logEvent("PAYLOAD", "INFO", "Amber beacon OFF");
    }

    runtime["components"]["amber_beacon"] = amber_beacon_status;
    saveRuntime();
}

void components::strobe(int X)
{
    if (X == 1)
    {
        white_strobe_status = true;

        logEvent("PAYLOAD", "INFO", "White strobe ON");
    }
    else if (X == 2)
    {
        white_strobe_status = false;

        logEvent("PAYLOAD", "INFO", "White strobe OFF");
    }

    runtime["components"]["white_strobe"] = white_strobe_status;
    saveRuntime();
}

void components::spotlight(int X)
{
    if (X == 1)
    {
        downward_spotlight_status = true;

        logEvent("PAYLOAD", "INFO", "Spotlight ON");
    }
    else if (X == 2)
    {
        downward_spotlight_status = false;

        logEvent("PAYLOAD", "INFO", "Spotlight OFF");
    }

    runtime["components"]["downward_spotlight"] = downward_spotlight_status;
    saveRuntime();
}

void components::smoke(int X)
{
    if (X == 1)
    {
        smoke_marker_status = true;

        logEvent("PAYLOAD", "INFO", "Smoke marker ON");
    }
    else if (X == 2)
    {
        smoke_marker_status = false;

        logEvent("PAYLOAD", "INFO", "Smoke marker OFF");
    }

    runtime["components"]["smoke_marker"] = smoke_marker_status;
    saveRuntime();
}

void components::turnOffPayload()
{
    logEvent("PAYLOAD", "INFO", "Turning off all components");

    int var = 2;

    thermal(var);
    rgb(var);
    infrared(var);
    fmcw(var);
    speaker(var);
    microphone(var);
    rf(var);
    beacon(var);
    strobe(var);
    spotlight(var);
    smoke(var);
}

void components::batterySaveMode()
{
    thermal(battery["thermal_camera"]);
    rgb(battery["rgb_camera"]);
    infrared(battery["infrared_camera"]);
    fmcw(battery["fmcw_radar"]);
    rf(battery["passive_rf"]);
    microphone(battery["microphone"]);
    speaker(battery["speaker"]);
    beacon(battery["amber_beacon"]);
    strobe(battery["white_strobe"]);
    spotlight(battery["downward_spotlight"]);
    smoke(battery["smoke_marker"]);
}