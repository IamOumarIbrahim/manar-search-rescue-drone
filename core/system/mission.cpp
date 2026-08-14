#include "mission.hpp"
#include "shared.hpp"

#include <cmath>
#include <iostream>

using namespace std;

void mission::missionstatusupdater() // CONTROL UNIT
{
    if (missionstarted == false)
    {
        return;
    }


    // RETURNING / RTL HAS PRIORITY
    if (isdestinationhome == true)
    {
        enroute = false;
        waitingforhelp = false;
        runtime["mission"]["waitingforhelp"] = waitingforhelp;
        runtime["mission"]["isdestinationhome"] = isdestinationhome;
        runtime["mission"]["enroute"] = enroute;
        saveRuntime();

        mydrone.determineifclose();

        if (mydrone.getclose() == true)
        {
            missionfinished = true;

            runtime["mission"]["finished"] = missionfinished;
            saveRuntime();
        }

        return;
    }




    // WAITING FOR HELP
    if (waitingforhelp == true)
    {

        return;
    }

    if (rescueefound == true)
    {
        enroute = false;
        isdestinationhome = false;

        mydrone.stopflight();

        waitingforhelp = true;

        runtime["mission"]["enroute"] = enroute;
        runtime["mission"]["isdestinationhome"] = isdestinationhome;
        runtime["mission"]["waitingforhelp"] = waitingforhelp;
        saveRuntime();

        return;
    }


    // NORMAL MISSION
    enroute = true;

    runtime["mission"]["enroute"] = enroute;
    runtime["mission"]["isdestinationhome"] = isdestinationhome;
    saveRuntime();

    checksearchlocation();
}
void mission::configurerescueestate() // MANUALLY SET BY THE OPERATOR (USED)
{
    if (missionstarted == true)
    {
        rescueefound = true;
        runtime["mission"]["rescueefound"] = rescueefound; saveRuntime();
        missionstatusupdater();     
    }

}
void mission::startmission() // MANUALL LAUNCH (USED)
{
    missionstarted = true;
    missionstatusupdater();
    mydrone.mydroneflight.printdest();
    runtime["mission"]["started"] = missionstarted; 
    runtime["destination"]["latitude"] = mydrone.mydroneflight.getdestlat();
    runtime["destination"]["longitude"] = mydrone.mydroneflight.getdestlong();
    saveRuntime();
}
void mission::batterysystem()
{
    if (waitingforhelp == true)
        return;

    if (mydrone.getbattery() <= config["battery_emergency_land"])
    {
        if (emergencyLandTriggered == false)
        {
            emergencyLandTriggered = true;
            runtime["battery"]["mode"] = "emergencyLandTriggered";
            saveRuntime();

            logEvent(
                "BATTERY",
                "CRITICAL",
                "Emergency landing triggered"
            );

            mydrone.stopflight();

            logEvent(
                "BATTERY",
                "CRITICAL",
                "Transmitting current location"
            );

            mydrone.transmitinfo();
        }
    }

    else if (mydrone.getbattery() <= config["battery_emergency_rtl"])
    {
        if (emergencyRTL == false)
        {
            emergencyRTL = true;
            runtime["battery"]["mode"] = "emergencyRTL";

            logEvent(
                "BATTERY",
                "CRITICAL",
                "Emergency RTL triggered"
            );

            mydrone.transmitinfo();
            activateRTL();
        }
    }

    else if (mydrone.getbattery() <= config["battery_rtl_warning"])
    {
        if (rtlWarningTriggered == false)
        {
            rtlWarningTriggered = true;
            runtime["battery"]["mode"] = "rtlWarningTriggered";

            logEvent(
                "BATTERY",
                "WARN",
                "Battery below " +
                config["battery_rtl_warning"].dump() +
                "% | RTL recommended"
            );

            mydrone.transmitinfo();
        }
    }

    else if (mydrone.getbattery() <= config["battery_warning"])
    {
        if (batterySaveTriggered == false)
        {
            batterySaveTriggered = true;
            runtime["battery"]["mode"] = "batterySaveTriggered";

            logEvent(
                "BATTERY",
                "WARN",
                "Battery below " +
                config["battery_warning"].dump() +
                "% | battery-save mode triggered"
            );

            mydrone.savebatterymode();
        }
    }
}
void mission::printflightpath()
{
    if (isdestinationhome == true)
    {
        logEvent(
            "MISSION",
            "INFO",
            "Flight status: Returning"
        );
    }
    else if (enroute == true)
    {
        logEvent(
            "MISSION",
            "INFO",
            "Flight status: Enroute"
        );
    }
    else if (waitingforhelp == true)
    {
        logEvent(
            "MISSION",
            "INFO",
            "Flight status: Waiting for help"
        );
    }
}
bool mission::getmissionstarted()
{
    return missionstarted;
}
void mission::setdestinationhomeON()
{
    isdestinationhome = true;
    runtime["mission"]["isdestinationhome"] = isdestinationhome;
    saveRuntime();
}
void mission::setdestinationhomeOFF()
{
    isdestinationhome = false;
    runtime["mission"]["isdestinationhome"] = isdestinationhome;
    saveRuntime();
}
void mission::setdestinationconfiguredON()
{
    destinationconfigured = true;
}
void mission::activateRTL()
{
    mydrone.comp.turnOffPayload();
    setdestinationhomeON();
    double X = mydrone.myhome.gethomelat();
    double Y = mydrone.myhome.gethomelon();
    mydrone.mydroneflight.setdestination(X, Y);

    double currentspeed = mydrone.mydroneflight.getspeed();
    double distance = mydrone.getdistfromhome();

    double estimatedtime = 0;
    if (currentspeed > 0) {
        estimatedtime = distance / currentspeed;
    }

    logEvent(
        "MISSION",
        "INFO",
        "Estimated arrival time: " + to_string(estimatedtime) + " seconds."
    );
}
void mission::checksearchlocation()
{
    if (rescueefound == true)
        return;

    if (isdestinationhome == true)
        return;

    if (destinationconfigured == false)
        return;

    // Do NOT start lawnmower operation if destination is home.
    if (mydrone.destinationishome())
        return;

    // Drone has reached the search destination.
    if (mydrone.reachedDestination())
    {
        lawnmower();
    }
}
void mission::lawnmower()
{
    const double PI = 3.14159265358979323846;

    double searchwidth = config["search_width"];
    double searchheight = config["search_height"];
    double rowspacing = config["row_spacing"];

    double currentlat = mydrone.getdronelat();
    double currentlon = mydrone.getdronelong();

    double metersperlat = 111320.0;
    double metersperlon =
        111320.0 * cos(currentlat * PI / 180.0);

    double nextlat = currentlat;
    double nextlon = currentlon;

    int maxrows = searchheight / rowspacing;


    // FIRST TIME:
    // Move from search center to southwest corner.
    if (lawnmowerstarted == false)
    {
        lawnmowerstarted = true;

        nextlat =
            currentlat - ((searchheight / 2.0) / metersperlat);

        nextlon =
            currentlon - ((searchwidth / 2.0) / metersperlon);
    }


    // HORIZONTAL PART
    else if (horizontalmove == true)
    {
        if (moveeast == true)
        {
            nextlon =
                currentlon + (searchwidth / metersperlon);
        }
        else
        {
            nextlon =
                currentlon - (searchwidth / metersperlon);
        }

        horizontalmove = false;
    }


    // VERTICAL PART
    else
    {
        // ENTIRE SEARCH AREA HAS BEEN COMPLETED
        if (searchrow >= maxrows)
        {
            isdestinationhome = true;
            enroute = false;

            runtime["mission"]["isdestinationhome"] = isdestinationhome;
            runtime["mission"]["enroute"] = enroute;
            saveRuntime();

            double X, Y;

            X = mydrone.myhome.gethomelat();
            Y = mydrone.myhome.gethomelon();

            mydrone.mydroneflight.setdestination(X, Y);

            logEvent(
                "MISSION",
                "INFO",
                "Lawnmower search completed | returning to base"
            );

            return;
        }


        nextlat =
            currentlat + (rowspacing / metersperlat);

        searchrow++;

        moveeast = !moveeast;
        horizontalmove = true;
    }


    mydrone.mydroneflight.setdestination(nextlat, nextlon);
}
void mission::setwaitingforhelpOFF()
    {
        waitingforhelp = false;
        runtime["mission"]["waitingforhelp"] = waitingforhelp;
        saveRuntime();
    }
void mission::resetRuntime()
{
        // PRESERVE CONTROL-SESSION / PHYSICAL STATE
        int currentCommandID = 0;
        string currentBatteryMode = "normal";

        if (runtime.contains("control") &&
            runtime["control"].contains("last_processed_command"))
        {
            currentCommandID =
                runtime["control"]["last_processed_command"];
        }

        if (runtime.contains("battery") &&
            runtime["battery"].contains("mode"))
        {
            currentBatteryMode =
                runtime["battery"]["mode"];
        }


        // RESET MISSION STATE
        missionstarted = false;
        missionfinished = false;
        enroute = false;
        isdestinationhome = false;
        waitingforhelp = false;
        rescueefound = false;

        // RESET SEARCH STATE
        lawnmowerstarted = false;
        horizontalmove = true;
        moveeast = true;
        searchrow = 0;

        // RESET DESTINATION
        destinationconfigured = false;
        // Current aircraft position becomes the neutral destination.
        mydrone.mydroneflight.setdestination(
            mydrone.getdronelat(),
            mydrone.getdronelong()
        );

        // RESET MISSION COMPONENT STATE
        mydrone.comp.batterySaveMode();


        // REBUILD CURRENT RUNTIME STATE
        runtime = json::object();


        // CONTROL - PRESERVED
        runtime["control"] = {
            {"last_processed_command", currentCommandID}
        };


        // MISSION - RESET
        runtime["mission"] = {
            {"started", missionstarted},
            {"finished", missionfinished},
            {"enroute", enroute},
            {"isdestinationhome", isdestinationhome},
            {"waitingforhelp", waitingforhelp},
            {"rescueefound", rescueefound},
            {"last_saved_latitude", 0},
            {"last_saved_longitude", 0}
        };


        // FLIGHT - PRESERVED
        runtime["flight"] = {
            {"launched", mydrone.mydroneflight.getlaunched()},
            {"speed", mydrone.mydroneflight.getspeed()},
            {"altitude", mydrone.mydroneflight.getaltitude()},
            {"mode", mydrone.mydroneflight.getmodename()}
        };


        // DESTINATION - RESET TO CURRENT POSITION
        runtime["destination"] = {
            {"latitude", mydrone.mydroneflight.getdestlat()},
            {"longitude", mydrone.mydroneflight.getdestlong()}
        };


        // DRONE LOCATION - PRESERVED
        runtime["drone"] = {
            {"latitude", mydrone.getdronelat()},
            {"longitude", mydrone.getdronelong()}
        };


        // BATTERY - PRESERVED
        runtime["battery"] = {
            {"percentage", mydrone.getbattery()},
            {"mode", currentBatteryMode}
        };


        // COMPONENTS - RESET
        runtime["components"] = {
            {"thermal_camera", mydrone.comp.thermal_camera_status},
            {"rgb_camera", mydrone.comp.RGB_camera_status},
            {"infrared_camera", mydrone.comp.infrared_camera_status},
            {"fmcw_radar", mydrone.comp.fmcw_status},
            {"speaker", mydrone.comp.speaker_status},
            {"microphone", mydrone.comp.microphone_status},
            {"passive_rf", mydrone.comp.passive_rf_status},
            {"amber_beacon", mydrone.comp.amber_beacon_status},
            {"white_strobe", mydrone.comp.white_strobe_status},
            {"downward_spotlight", mydrone.comp.downward_spotlight_status},
            {"smoke_marker", mydrone.comp.smoke_marker_status}
        };


        saveRuntime();

        logEvent(
            "MISSION",
            "INFO",
            "Mission runtime reset | physical aircraft state preserved"
        );
}