#include "mission.hpp"
#include "shared.hpp"

#include <cmath>
#include <iostream>

using namespace std;

void activateRTL(drone &mydrone, home &myhome, mission &mymission);

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
            activateRTL(mydrone, mydrone.myhome, *this);
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
void mission::checksearchlocation()
{
    if (rescueefound == true)
        return;

    if (isdestinationhome == true)
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
                runtime = json::object();

                // CONTROL
                runtime["control"]["last_processed_command"] = 0;

                // MISSION
                runtime["mission"]["started"] = missionstarted;
                runtime["mission"]["finished"] = missionfinished;
                runtime["mission"]["enroute"] = enroute;
                runtime["mission"]["isdestinationhome"] = isdestinationhome;
                runtime["mission"]["waitingforhelp"] = waitingforhelp;
                runtime["mission"]["rescueefound"] = rescueefound;

                runtime["mission"]["last_saved_latitude"] = 0;
                runtime["mission"]["last_saved_longitude"] = 0;

                // FLIGHT
                runtime["flight"]["launched"] =
                    mydrone.mydroneflight.getlaunched();

                runtime["flight"]["speed"] =
                    mydrone.mydroneflight.getspeed();

                runtime["flight"]["altitude"] =
                    mydrone.mydroneflight.getaltitude();

                runtime["flight"]["mode"] =
                    mydrone.mydroneflight.getmodename();

                // DESTINATION
                runtime["destination"]["latitude"] =
                    mydrone.mydroneflight.getdestlat();

                runtime["destination"]["longitude"] =
                    mydrone.mydroneflight.getdestlong();

                // DRONE LOCATION
                runtime["drone"]["latitude"] =
                    mydrone.getdronelat();

                runtime["drone"]["longitude"] =
                    mydrone.getdronelong();

                // BATTERY
                runtime["battery"]["percentage"] =
                    mydrone.getbattery();

                runtime["battery"]["mode"] = "normal";

                // COMPONENTS
                runtime["components"]["thermal_camera"] =
                    mydrone.comp.thermal_camera_status;

                runtime["components"]["rgb_camera"] =
                    mydrone.comp.RGB_camera_status;

                runtime["components"]["infrared_camera"] =
                    mydrone.comp.infrared_camera_status;

                runtime["components"]["fmcw_radar"] =
                    mydrone.comp.fmcw_status;

                runtime["components"]["speaker"] =
                    mydrone.comp.speaker_status;

                runtime["components"]["microphone"] =
                    mydrone.comp.microphone_status;

                runtime["components"]["passive_rf"] =
                    mydrone.comp.passive_rf_status;

                runtime["components"]["amber_beacon"] =
                    mydrone.comp.amber_beacon_status;

                runtime["components"]["white_strobe"] =
                    mydrone.comp.white_strobe_status;

                runtime["components"]["downward_spotlight"] =
                    mydrone.comp.downward_spotlight_status;

                runtime["components"]["smoke_marker"] =
                    mydrone.comp.smoke_marker_status;

                saveRuntime();
            
    }