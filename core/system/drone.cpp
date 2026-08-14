#include "drone.hpp"
#include "shared.hpp"

#include <cmath>
#include <iostream>

using namespace std;

home::home(string homelocname, double lat, double lon)
{
    homelocationname = homelocname;
    latitude = lat;
    longitude = lon;
}

double home::gethomelat()
{
    return latitude;
}

double home::gethomelon()
{
    return longitude;
}

drone::drone()
    : battery(config["starting_battery_percent"]),
      myhome(
          config["home_base_name"],
          config["home_base_latitude"],
          config["home_base_longitude"]
      )
{
}

void drone::printcurrent_location()
{
    logEvent(
        "NAV",
        "INFO",
        "Current location | lat=" + to_string(latitude) +
        " | lon=" + to_string(longitude)
    );
}
void drone::transmitinfo(){
    printcurrent_location();
    runtime["mission"]["last_saved_latitude"] = getdronelat();
    runtime["mission"]["last_saved_longitude"] = getdronelong();
    saveRuntime();
    // TO BE IMPLEMENTED
}
void drone::printbattery()
{
    logEvent(
        "BATTERY",
        "INFO",
        "Battery=" + to_string(battery) + "%"
    );
}

void drone::updateDistFromHome()
{
    const double EARTL_RADIUS = 6371000.0; // meters
    const double PI = 3.14159265358979323846;

    double lat1 = latitude * PI / 180.0;
    double lon1 = longitude * PI / 180.0;

    double lat2 = myhome.gethomelat() * PI / 180.0;
    double lon2 = myhome.gethomelon() * PI / 180.0;

    double deltaLat = lat2 - lat1;
    double deltaLon = lon2 - lon1;

    double a =
        sin(deltaLat / 2.0) * sin(deltaLat / 2.0) +
        cos(lat1) * cos(lat2) *
        sin(deltaLon / 2.0) * sin(deltaLon / 2.0);

    double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));

    distfromhome = EARTL_RADIUS * c;
}
void drone::determineifclose(){
    updateDistFromHome(); 
    if (distfromhome <= config["home_arrival_radius"]){
        closefromhome = true;
    }
    else if (distfromhome > config["home_arrival_radius"])
        closefromhome = false;
}
bool drone::getclose(){
    return closefromhome;

}
void drone::printdistfromhome()
{
    updateDistFromHome();

    logEvent(
        "NAV",
        "INFO",
        "Distance to base=" + to_string(distfromhome) + " m"
    );
}
double drone::getdistfromhome(){
    updateDistFromHome();
    return distfromhome;
}
double drone::getbattery() {
    return battery;
}
void drone::savebatterymode()
{
    comp.batterySaveMode();
}
double drone::getdronelat ()
{
    return latitude;
}
double drone::getdronelong()
{
    return longitude;
}

double drone::distancebetween(double lat1, double lon1, double lat2, double lon2)
{
    const double EARTL_RADIUS = 6371000.0;
    const double PI = 3.14159265358979323846;

    lat1 = lat1 * PI / 180.0;
    lon1 = lon1 * PI / 180.0;
    lat2 = lat2 * PI / 180.0;
    lon2 = lon2 * PI / 180.0;

    double deltaLat = lat2 - lat1;
    double deltaLon = lon2 - lon1;

    double a =
        sin(deltaLat / 2.0) * sin(deltaLat / 2.0) +
        cos(lat1) * cos(lat2) *
        sin(deltaLon / 2.0) * sin(deltaLon / 2.0);

    double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));

    return EARTL_RADIUS * c;
}
bool drone::reachedDestination()
{
    double distance = distancebetween(latitude,longitude,mydroneflight.getdestlat(),mydroneflight.getdestlong()
    );

    return distance <= config["reached_radius"];
}
bool drone::destinationishome()
{
    double distance = distancebetween(mydroneflight.getdestlat(),mydroneflight.getdestlong(),
    myhome.gethomelat(),myhome.gethomelon());

    return distance <= config["reached_radius"];
}
void drone::stopflight(){
    double X = getdronelat();
    double Y = getdronelong();
    mydroneflight.setdestination(X,Y);
    transmitinfo(); // TRANSMIT INFO
    mydroneflight.stopflight();
    comp.turnOffPayload();
}
void drone::decreasebattery(double amount)
{
    if (amount <= 0 || battery <= 0)
        return;
    if (mydroneflight.getlaunched())
    {
        if (mydroneflight.getmodename() == "Hover")
        battery -= amount;
        else if (mydroneflight.getmodename() == "Inspect")
        battery = battery - 2 * amount;
        else if (mydroneflight.getmodename() == "Active")
        battery = battery - 3 * amount;
        else if (mydroneflight.getmodename() == "Quick")
        battery = battery - 5 * amount;

        if (battery < 0)
            battery = 0;
    }

    runtime["battery"]["percentage"] = battery;
    saveRuntime();

}
