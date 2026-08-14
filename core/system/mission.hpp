#pragma once

#include "drone.hpp"

class mission
{
private:
    bool rescueefound = false;
    bool missionstarted = false;
    bool missionfinished = false;

    bool enroute = false;
    bool isdestinationhome = false;

    bool batterySaveTriggered = false;
    bool rtlWarningTriggered = false;
    bool emergencyRTL = false;
    bool emergencyLandTriggered = false;

    bool waitingforhelp = false;

    bool lawnmowerstarted = false;
    bool horizontalmove = true;
    bool moveeast = true;

    int searchrow = 0;

public:
    drone mydrone;

    void missionstatusupdater();

    void configurerescueestate();

    void startmission();

    void batterysystem();

    void printflightpath();

    bool getmissionstarted();

    void setdestinationhomeON();
    void setdestinationhomeOFF();

    void checksearchlocation();

    void lawnmower();

    void setwaitingforhelpOFF();

    void resetRuntime();
};