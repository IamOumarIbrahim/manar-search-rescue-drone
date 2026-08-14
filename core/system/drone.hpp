#pragma once

#include <string>

#include "flight.hpp"
#include "components.hpp"


class home
{
private:
    std::string homelocationname;
    double latitude;
    double longitude;

public:
    home(std::string homelocname, double lat, double lon);

    double gethomelat();
    double gethomelon();
};


class drone
{
private:
    double latitude = 0;
    double longitude = 0;

    double battery;

    double distfromhome = 1000;
    bool closefromhome = false;

public:
    components comp;
    flight mydroneflight;
    home myhome;

    drone();

    void printcurrent_location();
    void transmitinfo();

    void printbattery();

    void updateDistFromHome();
    void determineifclose();

    bool getclose();

    void printdistfromhome();
    double getdistfromhome();

    double getbattery();

    void savebatterymode();

    double getdronelat();
    double getdronelong();

    double distancebetween(
        double lat1,
        double lon1,
        double lat2,
        double lon2
    );

    bool reachedDestination();
    bool destinationishome();

    void stopflight();

    void decreasebattery(double amount);
};