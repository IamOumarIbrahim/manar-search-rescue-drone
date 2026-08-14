#pragma once

#include "shared.hpp"

class components
{
public:
    bool thermal_camera_status;
    bool RGB_camera_status;
    bool infrared_camera_status;

    bool fmcw_status;
    bool speaker_status;
    bool microphone_status;
    bool passive_rf_status;

    bool amber_beacon_status;
    bool white_strobe_status;
    bool downward_spotlight_status;

    bool smoke_marker_status;

    const bool heliograph_mirror_status = true;

    components();

    void thermal(int X);
    void rgb(int X);
    void infrared(int X);
    void fmcw(int X);

    void speaker(int X);
    void microphone(int X);
    void rf(int X);

    void beacon(int X);
    void strobe(int X);
    void spotlight(int X);
    void smoke(int X);

    void turnOffPayload();
    void batterySaveMode();
};