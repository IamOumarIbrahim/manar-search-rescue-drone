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

    if (destinationsearched == true)
    {
        currentSearchLocation++;

        if (searchlocations.contains("locations") &&
            searchlocations["locations"].is_array() &&
            currentSearchLocation < searchlocations["locations"].size())
        {
            destinationsearched = false;

            lawnmowerstarted = false;
            horizontalmove = true;
            moveeast = true;
            searchrow = 0;

            double lat = searchlocations["locations"][currentSearchLocation]["latitude"];
            double lon = searchlocations["locations"][currentSearchLocation]["longitude"];

            mydrone.mydroneflight.setdestination(lat, lon);

            runtime["destination"]["latitude"] = lat;
            runtime["destination"]["longitude"] = lon;

            runtime["search"]["current_location_id"] = currentSearchLocation + 1;
            runtime["search"]["destination_searched"] = false;

            saveRuntime();

            logEvent(
                "MISSION",
                "INFO",
                "Moving to search location " + to_string(currentSearchLocation + 1)
            );

            return;
        }

        activateRTL();
        return;
    }

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
bool mission::getsearchplanlocked()
{
    return searchplanlocked;
}
void mission::setsearchplanlockedON()
{
    searchplanlocked = true;
}
void mission::setsearchplanlockedOFF()
{
    searchplanlocked = false;
}
void mission::loadSearchLocations()
{
    ifstream in("runtime/search_locations.json");
    if (in.is_open())
    {
        try
        {
            in >> searchlocations;
            if (searchlocations.contains("locked") && searchlocations["locked"].is_boolean())
            {
                searchplanlocked = searchlocations["locked"].get<bool>();
            }
        }
        catch (...)
        {
            searchlocations = json::object();
            searchlocations["locations"] = json::array();
        }
        in.close();
    }
    else
    {
        searchlocations = json::object();
        searchlocations["locations"] = json::array();
    }
}
void mission::setsearchlocations(const json& searchPlan)
{
    searchlocations = searchPlan;
}
void mission::startmission() // MANUALL LAUNCH (USED)
{
    loadSearchLocations();

    if (searchplanlocked == false ||
        !searchlocations.contains("locations") ||
        !searchlocations["locations"].is_array() ||
        searchlocations["locations"].empty())
    {
        logEvent(
            "MISSION",
            "WARN",
            "Mission start rejected | search plan not locked or empty"
        );

        return;
    }

    missionstarted = true;

    currentSearchLocation = 0;
    destinationsearched = false;

    lawnmowerstarted = false;
    horizontalmove = true;
    moveeast = true;
    searchrow = 0;

    double lat = searchlocations["locations"][0]["latitude"];
    double lon = searchlocations["locations"][0]["longitude"];

    mydrone.mydroneflight.setdestination(lat, lon);
    setdestinationconfiguredON();

    missionstatusupdater();
    mydrone.mydroneflight.printdest();
    runtime["mission"]["started"] = missionstarted; 
    runtime["destination"]["latitude"] = mydrone.mydroneflight.getdestlat();
    runtime["destination"]["longitude"] = mydrone.mydroneflight.getdestlong();

    int totalLocs = searchlocations["locations"].size();
    runtime["search"]["plan_locked"] = searchplanlocked;
    runtime["search"]["optimization_enabled"] = searchlocations.contains("optimization_enabled") ? searchlocations["optimization_enabled"].get<bool>() : false;
    runtime["search"]["planning_mode"] = searchlocations.contains("planning_mode") ? searchlocations["planning_mode"].get<string>() : "none";
    runtime["search"]["current_location_id"] = 1;
    runtime["search"]["total_locations"] = totalLocs;
    runtime["search"]["destination_searched"] = destinationsearched;
    if (searchlocations.contains("total_route_distance_m"))
    {
        runtime["search"]["planned_route_distance_m"] = searchlocations["total_route_distance_m"];
    }

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
    if (!mydrone.mydroneflight.getlaunched())
    {
        mydrone.mydroneflight.launch();
    }
    mydrone.mydroneflight.setmode(2);

    mydrone.comp.turnOffPayload();
    setdestinationhomeON();

    rescueefound = false;
    waitingforhelp = false;
    runtime["mission"]["rescueefound"] = rescueefound;
    runtime["mission"]["waitingforhelp"] = waitingforhelp;

    double X = mydrone.myhome.gethomelat();
    double Y = mydrone.myhome.gethomelon();
    mydrone.mydroneflight.setdestination(X, Y);

    json rtlPlan = json::object();
    rtlPlan["locked"] = searchplanlocked;
    rtlPlan["optimization_enabled"] = false;
    rtlPlan["planning_mode"] = "rtl_home";
    rtlPlan["origin"] = {
        {"latitude", mydrone.getdronelat()},
        {"longitude", mydrone.getdronelong()}
    };
    rtlPlan["home"] = {
        {"latitude", X},
        {"longitude", Y}
    };

    json locArray = json::array();
    locArray.push_back({
        {"id", 1},
        {"input_id", 1},
        {"latitude", X},
        {"longitude", Y}
    });
    rtlPlan["locations"] = locArray;

    double distance = mydrone.getdistfromhome();
    rtlPlan["total_route_distance_m"] = distance;

    ofstream out("runtime/search_locations.json");
    out << rtlPlan.dump(4);
    out.close();

    searchlocations = rtlPlan;
    currentSearchLocation = 0;
    destinationsearched = false;

    runtime["search"]["total_locations"] = 1;
    runtime["search"]["current_location_id"] = 1;
    runtime["search"]["destination_searched"] = false;
    runtime["search"]["planned_route_distance_m"] = distance;
    saveRuntime();

    double currentspeed = mydrone.mydroneflight.getspeed();

    double estimatedtime = 0;
    if (currentspeed > 0) {
        estimatedtime = distance / currentspeed;
    }

    logEvent(
        "MISSION",
        "INFO",
        "RTL activated | Destinations reset to home station | Estimated arrival time: " + to_string(estimatedtime) + " seconds."
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
            destinationsearched = true;

            runtime["search"]["destination_searched"] = destinationsearched;
            saveRuntime();

            logEvent(
                "MISSION",
                "INFO",
                "Search location " + to_string(currentSearchLocation + 1) + " completed"
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
        searchplanlocked = false;
        destinationsearched = false;
        currentSearchLocation = 0;
        destinationconfigured = false;

        lawnmowerstarted = false;
        horizontalmove = true;
        moveeast = true;
        searchrow = 0;

        searchlocations = json::object();
        searchlocations["locations"] = json::array();

        ofstream searchOut("runtime/search_locations.json", ios::trunc);
        searchOut << searchlocations.dump(4);
        searchOut.close();

        // RESET DESTINATION
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

        // SEARCH PROGRESS - RESET
        runtime["search"] = {
            {"plan_locked", false},
            {"optimization_enabled", false},
            {"planning_mode", "none"},
            {"current_location_id", 0},
            {"total_locations", 0},
            {"destination_searched", false},
            {"planned_route_distance_m", 0.0}
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