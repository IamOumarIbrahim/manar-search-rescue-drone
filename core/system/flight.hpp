#pragma once

#include <string>
#include "shared.hpp"


class flight
{
private:
    double speed = 0;
    double altitude = 0;

    double destlat = config["home_base_latitude"];
    double destlon = config["home_base_longitude"];

    std::string mode = "Stall";
    bool flightlaunched = false;

public:
    void setspeed(double setspeed);
    void setaltitude(double setaltitude);

    double getspeed();
    double getaltitude();

    void stopflight();
    void launch();

    void setmodename(std::string m);
    std::string getmodename();

    void setmode(int User_Option2);

    void printflightstatus();

    void setdestination(double lat, double lon);
    void printdest();

    bool getlaunched();

    double getdestlat();
    double getdestlong();
};