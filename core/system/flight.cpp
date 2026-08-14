#include "flight.hpp"

#include <iostream>
#include <iomanip>

using namespace std;

void flight::setspeed(double setspeed)
{
    if ((setspeed <= config["maximum_speed"]) && (setspeed >= 0))
    {
        speed = setspeed;

        runtime["flight"]["speed"] = speed;
        saveRuntime();

        logEvent(
            "FLIGHT",
            "INFO",
            "Speed set | speed=" + to_string(speed) + " m/s"
        );
    }
}

void flight::setaltitude(double setaltitude)
{
    if ((setaltitude <= config["maximum_altitude"]) && (setaltitude >= 0))
    {
        altitude = setaltitude;

        runtime["flight"]["altitude"] = altitude;
        saveRuntime();

        logEvent(
            "FLIGHT",
            "INFO",
            "Altitude set | altitude=" + to_string(altitude) + " m"
        );
    }
}

double flight::getspeed()
{
    return speed;
}

double flight::getaltitude()
{
    return altitude;
}

void flight::stopflight()
{
    flightlaunched = false;

    speed = 0;
    altitude = 0;
    mode = "Stall";

    runtime["flight"]["launched"] = flightlaunched;
    runtime["flight"]["speed"] = speed;
    runtime["flight"]["altitude"] = altitude;
    runtime["flight"]["mode"] = mode;
    saveRuntime();

    logEvent(
        "FLIGHT",
        "INFO",
        "Flight stopped | speed=0 m/s | altitude=0 m | mode=Stall"
    );
}

void flight::launch()
{
    flightlaunched = true;
    altitude = config["launch_altitude"];
    mode = "Hover";

    runtime["flight"]["launched"] = flightlaunched;
    runtime["flight"]["speed"] = speed;
    runtime["flight"]["altitude"] = altitude;
    runtime["flight"]["mode"] = mode;
    saveRuntime();

    logEvent(
        "FLIGHT",
        "INFO",
        "Drone launched | altitude=" + to_string(altitude) +
        " m | mode=Hover"
    );
}

void flight::setmodename(string m)
{
    mode = m;

    runtime["flight"]["mode"] = mode;
    saveRuntime();
}

string flight::getmodename()
{
    return mode;
}

void flight::setmode(int User_Option2)
{
    if (User_Option2 == 1)
    {
        setspeed(config["quick_speed"]);
        setmodename("Quick");
    }
    else if (User_Option2 == 2)
    {
        setspeed(config["active_speed"]);
        setmodename("Active");
    }
    else if (User_Option2 == 3)
    {
        setspeed(config["inspect_speed"]);
        setmodename("Inspect");
    }
    else if (User_Option2 == 4)
    {
        setspeed(config["hover_speed"]);
        setmodename("Hover");
    }

    logEvent(
        "FLIGHT",
        "INFO",
        "Mode set | mode=" + mode +
        " | speed=" + to_string(speed) + " m/s"
    );
}

void flight::printflightstatus()
{
    string stat;

    if (getlaunched())
        stat = "Launched";
    else
        stat = "Stalled";

    logEvent(
        "FLIGHT",
        "INFO",
        "Status | state=" + stat +
        " | mode=" + getmodename() +
        " | speed=" + to_string(getspeed()) +
        " m/s | altitude=" + to_string(getaltitude()) + " m"
    );
}

void flight::setdestination(double lat, double lon)
{
    destlat = lat;
    destlon = lon;

    runtime["destination"]["latitude"] = destlat;
    runtime["destination"]["longitude"] = destlon;
    saveRuntime();

    logEvent(
        "NAV",
        "INFO",
        "Destination set | lat=" + to_string(destlat) +
        " | lon=" + to_string(destlon)
    );
}

void flight::printdest()
{
    logEvent(
        "NAV",
        "INFO",
        "Current destination | lat=" + to_string(destlat) +
        " | lon=" + to_string(destlon)
    );
}

bool flight::getlaunched()
{
    return flightlaunched;
}

double flight::getdestlat()
{
    return destlat;
}

double flight::getdestlong()
{
    return destlon;
}