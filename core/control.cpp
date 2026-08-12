// LIBRARIES-----------------------------------------
    #include <iostream>
    #include <string>
    #include <clocale>
    #include <cmath>
    #include <iomanip>
    #include <thread>
    #include <chrono>  
    #include <fstream>
    #include <ctime> 
    #include <cstdio>
    #include <nlohmann/json.hpp>
    #include <cctype>
    #include <sstream>
    using namespace std;



using json = nlohmann::json;

json config;
json runtime;
json commands;

int lastProcessedCommandID = 0;
ofstream fout("logs.txt");
/* RESPONSIBILITY
    According to Section 2.4 (Responsibility Allocation) of the Version 1 specification,
    the human operator must be capable of executing all of these tasks if required

    - Defines the search area, objective, launch point, route, altitude, geofence, and safety limits.
    - Approves takeoff and may start, pause, abort, command RTH, or manually control the aircraft.
    - Monitors flight status, map position, and sensor feeds.
    - May cancel any automatic candidate approach or verification sequence.
    - Reviews Mamba-triggered alerts and makes the final Probable rescuee determination.
    - Approves beacon, strobe, smoke, and other consequential guidance actions.
    - Shares confirmed coordinates with rescue teams and handles exceptional emergency decisions.

*/


string getTimestamp()
{
    auto now = chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(now);

    tm* localTime = localtime(&currentTime);

    char buffer[20];

    strftime(buffer, sizeof(buffer), "%H:%M:%S", localTime);

    return buffer;
}
bool parseCoordinates(string input, double &lat, double &lon)
{
    // DECIMAL:
    // 25.336421042699826, 55.344470601876345

    if (input.find(',') != string::npos)
    {
        try
        {
            size_t comma = input.find(',');

            lat = stod(input.substr(0, comma));
            lon = stod(input.substr(comma + 1));

            lat = round(lat * 1000000.0) / 1000000.0;
            lon = round(lon * 1000000.0) / 1000000.0;

            return true;
        }
        catch (...)
        {
            return false;
        }
    }

    // DMS:
    // 25°13'10.91"N 55°27'13.60"E

    string cleaned = input;

    // Replace every separator with a space.
    // Keep only numbers, decimal points, minus signs, and N/S/E/W.
    for (char &c : cleaned)
    {
        if (!isdigit((unsigned char)c) &&
            c != '.' &&
            c != '-' &&
            c != 'N' && c != 'n' &&
            c != 'S' && c != 's' &&
            c != 'E' && c != 'e' &&
            c != 'W' && c != 'w')
        {
            c = ' ';
        }
    }

    stringstream ss(cleaned);

    double latDeg, latMin, latSec;
    double lonDeg, lonMin, lonSec;
    char latDirection, lonDirection;

    if (!(ss >> latDeg >> latMin >> latSec >> latDirection
            >> lonDeg >> lonMin >> lonSec >> lonDirection))
    {
        return false;
    }

    lat = latDeg + latMin / 60.0 + latSec / 3600.0;
    lon = lonDeg + lonMin / 60.0 + lonSec / 3600.0;

    if (latDirection == 'S' || latDirection == 's')
        lat = -lat;

    if (lonDirection == 'W' || lonDirection == 'w')
        lon = -lon;

    lat = round(lat * 1000000.0) / 1000000.0;
    lon = round(lon * 1000000.0) / 1000000.0;

    return true;
}
void saveRuntime()
{
    ofstream out("runtime.json");
    out << runtime.dump(4);
    out.close();
}
bool readcommands()
{
    ifstream in("commands.json");

    if (!in.is_open())
        return false;

    try
    {
        in >> commands;
        in.close();

        return true;
    }
    catch (...)
    {
        in.close();

        return false;
    }
}



// CLASSES-------------------------------------------
class flight
{
    private:
        double speed = 0;
        double altitude = 0;
        double destlat = config["home_base_latitude"];  // default latitude of the destination 
        double destlon = config["home_base_longitude"];  // default longitude of the destination 
        string mode = "Stall";
        bool flightlaunched = false;
    public:
        void setspeed(double setspeed)
        {
            if ((setspeed <= config["maximum_speed"]) && (setspeed >= 0)){
                speed = setspeed;
                fout<<"----------------------------------"<<endl;
                runtime["flight"]["speed"] = speed;
                saveRuntime();
                fout<<"Speed goal manually set to "<<speed<<"m/s"<<endl;
            }
        }
        void setaltitude(double setaltitude)
        {
            if ((setaltitude <= config["maximum_altitude"]) && (setaltitude >= 0)){
                
                altitude = setaltitude;
                runtime["flight"]["altitude"] = altitude;
                saveRuntime();
                fout<<"----------------------------------"<<endl;
                fout<<"Altitude goal manually set to "<<altitude<<"m."<<endl;
            }
        }
        double getspeed()
        {
            return speed;
        }
        double getaltitude()
        {
            return altitude;
        }
        void stopflight()
        {
            fout<<"----------------------------------"<<endl;
            fout<<"Descending and reducing speed..."<<endl;
            flightlaunched = false;
            
            speed = 0;
            altitude = 0;
            fout << "Successfully stopped." << endl;
            mode = "Stall";
            runtime["flight"]["launched"] = flightlaunched;
            runtime["flight"]["speed"] = speed;
            runtime["flight"]["altitude"] = altitude;
            runtime["flight"]["mode"] = mode;
            saveRuntime();
        }
        void launch()
        {
            fout<<"----------------------------------"<<endl;
            fout<<"Initating launch..."<<endl;
            flightlaunched = true;
            fout<<"Climbing to "<<config["launch_altitude"]<<" m."<<endl;
            altitude = config["launch_altitude"];
            fout<<"Successfully launched, drone hovering at "<<config["launch_altitude"]<<" m altitude."<<endl;
            mode = "Hover";
            runtime["flight"]["launched"] = flightlaunched;
            runtime["flight"]["speed"] = speed;
            runtime["flight"]["altitude"] = altitude;
            runtime["flight"]["mode"] = mode;
            saveRuntime();
        }
        void setmodename(string m)
        {
            mode = m;
            runtime["flight"]["mode"] = mode;
            saveRuntime();
        }
        string getmodename()
        {
            return mode;
        }
        void setmode(int User_Option2)
        {
            if (User_Option2 == 1)
            {
                fout<<"Flight mode: Quick -- Enabled"<<endl;
                setspeed(config["quick_speed"]);
                setmodename("Quick");
            }
            else if (User_Option2 == 2)
            {
                fout<<"Flight mode: Active -- Enabled"<<endl;
                setspeed(config["active_speed"]);
                setmodename("Active");
            }
            else if (User_Option2 == 4)
            {
                fout<<"Flight mode: Hover -- Enabled"<<endl;
                setspeed(config["hover_speed"]);
                setmodename("Hover");
            }
            else if (User_Option2 == 3)
            {
                fout<<"Flight mode: Inspect -- Enabled"<<endl;
                setspeed(config["inspect_speed"]);
                setmodename("Inspect");
            }
        }
        void printflightstatus()
        {
            string stat;
            fout<<"----------------------------------"<<endl;
            fout<<"Current Speed: "<<getspeed()<<" m/s"<<endl;
            fout<<"Current Altitude: "<<getaltitude()<<" m"<<endl;
            fout<<"Current Mode: "<<getmodename()<<endl;
            if (getlaunched())
            {
                stat = "Launched";
            }
            else{
                stat = "Stalled";
            }
            fout<<"Current Status: "<<stat<<endl;
        }
        void setdestination(double lat,double lon)
        {
            destlat = lat;
            destlon = lon;

            runtime["destination"]["latitude"] = destlat;
            runtime["destination"]["longitude"] = destlon;
            saveRuntime();

            fout << "Destination set! Routing to "
                << fixed << setprecision(6)
                << destlat << " ° N " << destlon << " ° E" << endl
                << "- Google Maps: https://www.google.com/maps?q="
                << destlat << "," << destlon << endl;
        }
        void printdest()
        {
            fout<<"Current destination: "<<fixed << setprecision(6)<<destlat<<" ° N "<<destlon<<" ° E"<<endl<<
            "- Google Maps: https://www.google.com/maps?q="<<destlat<<","<<destlon<< endl;
        }
        bool getlaunched(){
            return flightlaunched;
        }
        double getdestlat()
        {
            return destlat;
        }
        double getdestlong()
        {
            return destlon;
        }
};
class components 
{
    // COMPONENT VARIABLES
        public:
            bool thermal_camera_status,RGB_camera_status,infrared_camera_status;
            bool fmcw_status,speaker_status,microphone_status,passive_rf_status;
            bool amber_beacon_status,white_strobe_status,downward_spotlight_status;
            bool smoke_marker_status;
            const bool heliograph_mirror_status = true;
    // CONSTRUCTOR
        components() {
            thermal_camera_status = false;
            RGB_camera_status = false;
            infrared_camera_status = false;
            fmcw_status = false;
            speaker_status = false;
            microphone_status = false;
            passive_rf_status = false;
            amber_beacon_status = false;
            white_strobe_status = false;
            downward_spotlight_status = false;
            smoke_marker_status = false;
        }
    // CONFIGURATION METHODS (USED)
        void thermal(int X){

            if (X == 1){
                thermal_camera_status = true;
                runtime["components"]["thermal_camera"] = thermal_camera_status;
                saveRuntime();

                fout << "[" << getTimestamp() << "] COMPONENTS: thermal camera ON" << endl;
            }

            else if (X == 2){
                thermal_camera_status = false;
                fout<<"Confirmed thermal camera OFF"<<endl;
            }
            runtime["components"]["thermal_camera"] = thermal_camera_status; saveRuntime();
        }
        void rgb(int X){

            if (X == 1){
                RGB_camera_status = true;
                fout<<"Confirmed RGB camera ON"<<endl;
            }

            else if (X == 2){
                RGB_camera_status = false;
                fout<<"Confirmed RGB camera OFF"<<endl;
            }
            runtime["components"]["rgb_camera"] = RGB_camera_status; saveRuntime();
        }
        void infrared(int X){
            if (X == 1){
                infrared_camera_status = true;
                fout<<"Confirmed infrared camera ON"<<endl;
            }

            else if (X == 2){
                infrared_camera_status = false;
                fout<<"Confirmed infrared camera OFF"<<endl;
            }
            runtime["components"]["infrared_camera"] = infrared_camera_status; saveRuntime();
        }
        void fmcw(int X){
            if (X == 1){
                fmcw_status = true;
                fout<<"Confirmed FMCW radar ON"<<endl;
            }

            else if (X == 2){
                fmcw_status = false;
                fout<<"Confirmed FMCW radar OFF"<<endl;
            }
            runtime["components"]["fmcw_radar"] = fmcw_status; saveRuntime();
        }
        void speaker(int X){
            if (X == 1){
                speaker_status = true;
                fout<<"Confirmed speaker ON"<<endl;
            }
            else if (X == 2){
                speaker_status = false;
                fout<<"Confirmed speaker OFF"<<endl;
            }
            runtime["components"]["speaker"] = speaker_status; saveRuntime();
        }
        void microphone(int X){
            if (X == 1){
                microphone_status = true;
                fout<<"Confirmed microphone ON"<<endl;
            }
            else if (X == 2){
                microphone_status = false;
                fout<<"Confirmed microphone OFF"<<endl;
            }
            runtime["components"]["microphone"] = microphone_status; saveRuntime();
        }
        void rf(int X){
            if (X == 1){
                passive_rf_status = true;
                fout<<"Confirmed passive rf ON"<<endl;
            }
            else if (X == 2){
                passive_rf_status = false;
                fout<<"Confirmed passive rf OFF"<<endl;
            }
            runtime["components"]["passive_rf"] = passive_rf_status; saveRuntime();
        }     
        void beacon(int X){
            if (X == 1){
                amber_beacon_status = true;
                fout<<"Confirmed amber beacon ON"<<endl;
            }
            else if (X == 2){
                amber_beacon_status = false;
                fout<<"Confirmed amber beacon OFF"<<endl;
            }
            runtime["components"]["amber_beacon"] = amber_beacon_status; saveRuntime();
        }                
        void strobe(int X){
            if (X == 1){
                white_strobe_status = true;
                fout<<"Confirmed white strobe ON"<<endl;
            }
            else if (X == 2){
                white_strobe_status = false;
                fout<<"Confirmed white strobe OFF"<<endl;
            }
            runtime["components"]["white_strobe"] = white_strobe_status; saveRuntime();
        }                
        void spotlight(int X){
            if (X == 1){
                downward_spotlight_status = true;
                fout<<"Confirmed spotlight ON"<<endl;
            }
            else if (X == 2){
                downward_spotlight_status = false;
                fout<<"Confirmed spotlight OFF"<<endl;
            }
            runtime["components"]["downward_spotlight"] = downward_spotlight_status; saveRuntime();
        }         
        void smoke(int X){
            if (X == 1){
                smoke_marker_status = true;
                fout<<"Confirmed smoke marker ON"<<endl;
            }
            else if (X == 2){
                smoke_marker_status = false;
                fout<<"Confirmed smoke marker OFF"<<endl;
            }
            runtime["components"]["smoke_marker"] = smoke_marker_status; saveRuntime();
        }       
};
class home 
{
    private:
        string homelocationname;
        double latitude ,longitude;
    public:
        home(string homelocname,double lat,double lon){
           homelocationname = homelocname;
           latitude = lat;
           longitude = lon;
        }
        double gethomelat(){
            return latitude;
        }
        double gethomelon(){
            return longitude;
        }

};
class drone 
{
    private:
        double latitude = 0;    // latitude of the drone 
        double longitude = 0;   // longitude of the drone 
        double battery = config["starting_battery_percent"];   // Battery of drone default
        double distfromhome = 1000; // safe default
        bool  closefromhome = false;
        bool savebattery = false; // TO BE IMPLEMENTED
    public:
        components comp;
        flight mydroneflight;
        home myhome{config["home_base_name"], config["home_base_latitude"], config["home_base_longitude"]};
    void printcurrent_location(){
        fout<<"----------------------------------"<<endl;
        fout<<"Current drone location: "<<fixed << setprecision(6)<<latitude<<" ° N "<<longitude<<" ° E"<<endl<<
        "- Google Maps: https://www.google.com/maps?q="<<latitude<<","<<longitude<< endl;
    }
    void transmitinfo(){
        printcurrent_location();
        runtime["mission"]["last_saved_latitude"] = getdronelat();
        runtime["mission"]["last_saved_longitude"] = getdronelong();
        saveRuntime();
        // TO BE IMPLEMENTED
    }
    void printbattery(){
        fout<<"Battery: "<<battery<<"%"<<endl;
    }
    void updateDistFromHome()
    {
        const double EARTH_RADIUS = 6371000.0; // meters
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

        distfromhome = EARTH_RADIUS * c;
    }
    void determineifclose(){
        updateDistFromHome(); 
        if (distfromhome <= config["home_arrival_radius"]){
            closefromhome = true;
        }
        else if (distfromhome > config["home_arrival_radius"])
            closefromhome = false;
    }
    bool getclose(){
        return closefromhome;
    
    }
    void printdistfromhome(){
        updateDistFromHome();
        fout<<"Current distance to base: "<<fixed<<setprecision(0)<<distfromhome<<" meters."<<endl;
    }
    double getdistfromhome(){
        updateDistFromHome();
        return distfromhome;
    }
    double getbattery() {
        return battery;
    }
    void savebatterymode(int num)
    {
        if (num == 0 )
        {
            savebattery = false;
        }
        else if (num == 1)
        {
            savebattery = true;
        }
    }
    double getdronelat ()
    {
        return latitude;
    }
    double getdronelong()
    {
        return longitude;
    }
    void landwhereyouare()
    {
        double X = getdronelat();
        double Y = getdronelong();
        mydroneflight.setdestination(X,Y);
        transmitinfo(); // TRANSMIT INFO
        savebatterymode(1); // SAVE BATTERY
        mydroneflight.stopflight(); // LAND DRONE
    }
    double distancebetween(double lat1, double lon1, double lat2, double lon2)
    {
        const double EARTH_RADIUS = 6371000.0;
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

        return EARTH_RADIUS * c;
    }
    bool reachedDestination()
    {
        double distance = distancebetween(latitude,longitude,mydroneflight.getdestlat(),mydroneflight.getdestlong()
        );

        return distance <= config["reached_radius"];
    }
    bool destinationishome()
    {
        double distance = distancebetween(mydroneflight.getdestlat(),mydroneflight.getdestlong(),
        myhome.gethomelat(),myhome.gethomelon());

        return distance <= config["reached_radius"];
    }
};
class mission 
{
    private:
        bool rescueefound = false; // if found then transmit location, hover in that place and save battery till manual help
                                        // if not found generate report and RTH if battery 
        bool missionstarted = false;
        bool missionabort = false;
        bool missionfinished = false;
        bool enroute = false;
        bool returning = false;
        bool emergencyrth = false;
        bool waitingforhelp = false;
        bool lawnmowerstarted = false;
        bool horizontalmove = true;
        bool moveeast = true;
        int searchrow = 0;
    public:
        drone mydrone;

        void missionstatusupdater() // CONTROL UNIT
        {
            if (missionstarted == false)
            {
                return;
            }


            // RETURNING / RTH HAS PRIORITY
            if (returning == true)
            {
                enroute = false;

                runtime["mission"]["returning"] = returning;
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


            // MISSION ABORTED
            if (missionabort == true)
            {
                returning = true;
                enroute = false;

                runtime["mission"]["returning"] = returning;
                runtime["mission"]["enroute"] = enroute;
                saveRuntime();

                return;
            }


            // WAITING FOR HELP
            if (waitingforhelp == true)
            {

                return;
            }


            // RESCUEE FOUND
            if (rescueefound == true)
            {
                enroute = false;
                returning = false;
                waitingforhelp = true;

                runtime["mission"]["enroute"] = enroute;
                runtime["mission"]["returning"] = returning;
                runtime["mission"]["waitingforhelp"] = waitingforhelp;
                saveRuntime();

                mydrone.landwhereyouare();
                mydrone.transmitinfo();
                return;
            }


            // NORMAL MISSION
            enroute = true;

            runtime["mission"]["enroute"] = enroute;
            runtime["mission"]["returning"] = returning;
            saveRuntime();

            checksearchlocation();
        }
        
    
        void configurerescueestate() // MANUALLY SET BY THE OPERATOR (USED)
        {
            if (missionstarted == true)
            {
                rescueefound = true;
                runtime["mission"]["rescueefound"] = rescueefound; saveRuntime();
                missionstatusupdater();     
            }

        }

        void startmission() // MANUALL LAUNCH (USED)
        {
            missionstarted = true;
            missionstatusupdater();
            fout<<"Missions successfully started. Drone is currently enroute to:"<<endl;
            mydrone.mydroneflight.printdest();
            runtime["mission"]["started"] = missionstarted; 
            runtime["destination"]["latitude"] = mydrone.mydroneflight.getdestlat();
            runtime["destination"]["longitude"] = mydrone.mydroneflight.getdestlong();
            saveRuntime();
        }
        void abortmission() // MANUALL ABORT (USED)
        {
            missionabort = true;
            missionstatusupdater();
            fout<<"Mission is aborted."<<endl;
            runtime["mission"]["aborted"] = missionabort;
            runtime["destination"]["latitude"] = mydrone.mydroneflight.getdestlat();
            runtime["destination"]["longitude"] = mydrone.mydroneflight.getdestlong(); 
            saveRuntime(); 
        }
        void batterysystem() // DETERMINISTIC BATTERY BEHAVIOUR
        {
            if (mydrone.getbattery() <= config["battery_emergency_rth"])
            {
                if (rescueefound == false){
                    emergencyrth = true;
                    mydrone.transmitinfo();
                    fout<<"[Battery: CRITICAL] -- Enabling emergency rth"<<endl;
                    if (mydrone.getbattery() <= config["battery_emergency_land"]) // HAUL FLIGHT OPERATION, LAND SAFELY
                    {
                        fout<<"[Battery: CRITICAL] -- Transmitting current location..."<<endl;
                        mydrone.transmitinfo();
                        fout<<"[Battery: CRITICAL] -- Landing safely..."<<endl;
                        mydrone.mydroneflight.stopflight();
                        fout<<"[Battery: CRITICAL] -- Transmitting current location..."<<endl;
                        mydrone.transmitinfo();
                    }
                }
            }
            if (mydrone.getbattery() <= config["battery_rth_warning"])
            {
                fout<<"Battery is currently under "<<config["battery_rth_warning"]<<"%, Transmitting current location... Please consider ENABLING RTH."<<endl;
                mydrone.transmitinfo();
            }
            if (mydrone.getbattery() <= config["battery_warning"])
            {
                fout<<"Battery is currently under "<<config["battery_warning"]<<"%, Consider enabling energy save-mode."<<endl;
            }


        }

        void printflightpath()
        {
            if (returning == true)
            {
                fout<<"Current flight status: Returning.\n";
            }
            else if (enroute == true)
            {
                fout<<"Current flight status: Enroute.\n";
            }
            else if (waitingforhelp == true)
            {
                fout<<"Current flight status: Waiting for help.\n";
            }
        }
        bool getmissionstarted()
        {
            return missionstarted;
        }
        void setreturningON()
        {
            returning = true;
            runtime["mission"]["returning"] = returning; saveRuntime();
        }
        void checksearchlocation()
        {
            if (rescueefound == true)
                return;

            if (returning == true)
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
        void lawnmower()
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
                    returning = true;
                    enroute = false;

                    runtime["mission"]["returning"] = returning;
                    runtime["mission"]["enroute"] = enroute;
                    saveRuntime();

                    double X, Y;

                    X = mydrone.myhome.gethomelat();
                    Y = mydrone.myhome.gethomelon();

                    mydrone.mydroneflight.setdestination(X, Y);

                    fout << "Lawnmower search completed. Returning to base." << endl;

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

};
// TERMINAL METHODS----------------------------------
void activateRTH(drone &mydrone, home &myhome, mission &mymission)
{
    fout << "[" << getTimestamp() << "] RTH: disabled -> enabled" << endl;

    mymission.setreturningON();
    double X,Y;
    X = myhome.gethomelat();
    Y = myhome.gethomelon();
    mydrone.mydroneflight.setdestination(X,Y);
    double currentspeed = mydrone.mydroneflight.getspeed();
    double distance = mydrone.getdistfromhome();
    runtime["destination"]["latitude"] = mydrone.mydroneflight.getdestlat();
    runtime["destination"]["longitude"] = mydrone.mydroneflight.getdestlong();
    saveRuntime();
    double estimatedtime = 0;
        if (currentspeed > 0){
             estimatedtime = distance / currentspeed;
        }
        fout<<"Estimated arrival time: "<<estimatedtime<<" seconds."<<endl;
    
        fout<<"----------------------------------"<<endl;

}

void checkcommands(mission &mymission)
{
    if (!readcommands())
        return;

    if (!commands.contains("id"))
        return;

    if (!commands.contains("command"))
        return;


    int commandID = commands["id"];


    // Already processed this command.
    if (commandID <= lastProcessedCommandID)
        return;


    string command = commands["command"];


    if (command == "START_MISSION")
    {
        mymission.startmission();

        fout << "[" << getTimestamp() << "] COMMAND: START_MISSION executed" << endl;
    }
    else if (command == "RTH")
    {
        activateRTH(mymission.mydrone,mymission.mydrone.myhome,mymission);
        fout << "[" << getTimestamp() << "] COMMAND: RTH executed" << endl;
    }
    else if (command == "CHANGE_DEST")
    {
        double lat = commands["arguments"]["latitude"];
        double lon = commands["arguments"]["longitude"];
        mymission.mydrone.mydroneflight.setdestination(lat,lon);
        fout << "[" << getTimestamp() << "] COMMAND: CHANGE_DEST executed" << endl;
    }
    else if (command == "LAUNCH_DRONE")
    {
        mymission.mydrone.mydroneflight.launch();
        fout << "[" << getTimestamp() << "] COMMAND: LAUNCH_DRONE executed" << endl;
    }
    else if (command == "CHANGE_MODE")
    {
           mymission.mydrone.mydroneflight.setmode(commands["arguments"]["value"]);
           fout << "[" << getTimestamp() << "] COMMAND: CHANGE_MODE executed" << endl;
    }
    else if (command == "CHANGE_SPEED")
    {
        mymission.mydrone.mydroneflight.setspeed(commands["arguments"]["value"]);
        fout << "[" << getTimestamp() << "] COMMAND: CHANGE_SPEED executed" << endl;
    }
    else if (command == "CHANGE_ALTITUDE")
    {
        mymission.mydrone.mydroneflight.setaltitude(commands["arguments"]["value"]);
        fout << "[" << getTimestamp() << "] COMMAND: CHANGE_ALTITUDE executed" << endl;
    }
    else if (command == "STOP_FLIGHT")
    {
        mymission.mydrone.mydroneflight.stopflight();
        fout << "[" << getTimestamp() << "] COMMAND: STOP_FLIGHT executed" << endl;
    }
    else if (command == "CHANGE_COMPONENT")
    {
        if (commands["arguments"]["enabled"] == true)
        {
            if (commands["arguments"]["component"] == "thermal_camera")
            {
                mymission.mydrone.comp.thermal(1);
            }
            else if (commands["arguments"]["component"] == "rgb_camera")
            {
                mymission.mydrone.comp.rgb(1);
            }
            else if (commands["arguments"]["component"] == "infrared_camera")
            {
                mymission.mydrone.comp.infrared(1);
            }
            else if (commands["arguments"]["component"] == "fmcw_radar")
            {
                mymission.mydrone.comp.fmcw(1);
            }
            else if (commands["arguments"]["component"] == "speaker")
            {
                mymission.mydrone.comp.speaker(1);
            }
            else if (commands["arguments"]["component"] == "microphone")
            {
                mymission.mydrone.comp.microphone(1);
            }
            else if (commands["arguments"]["component"] == "downward_spotlight")
            {
                mymission.mydrone.comp.spotlight(1);
            }
            else if (commands["arguments"]["component"] == "passive_rf")
            {
                mymission.mydrone.comp.rf(1);
            }
            else if (commands["arguments"]["component"] == "amber_beacon")
            {
                mymission.mydrone.comp.beacon(1);
            }
            else if (commands["arguments"]["component"] == "white_strobe")
            {
                mymission.mydrone.comp.strobe(1);
            }
            else if (commands["arguments"]["component"] == "smoke_marker")
            {
                mymission.mydrone.comp.smoke(1);
            }
        }
        else
        {
            if (commands["arguments"]["component"] == "thermal_camera")
            {
                mymission.mydrone.comp.thermal(2);
            }
            else if (commands["arguments"]["component"] == "rgb_camera")
            {
                mymission.mydrone.comp.rgb(2);
            }
            else if (commands["arguments"]["component"] == "infrared_camera")
            {
                mymission.mydrone.comp.infrared(2);
            }
            else if (commands["arguments"]["component"] == "fmcw_radar")
            {
                mymission.mydrone.comp.fmcw(2);
            }
            else if (commands["arguments"]["component"] == "speaker")
            {
                mymission.mydrone.comp.speaker(2);
            }
            else if (commands["arguments"]["component"] == "microphone")
            {
                mymission.mydrone.comp.microphone(2);
            }
            else if (commands["arguments"]["component"] == "downward_spotlight")
            {
                mymission.mydrone.comp.spotlight(2);
            }
            else if (commands["arguments"]["component"] == "passive_rf")
            {
                mymission.mydrone.comp.rf(2);
            }
            else if (commands["arguments"]["component"] == "amber_beacon")
            {
                mymission.mydrone.comp.beacon(2);
            }
            else if (commands["arguments"]["component"] == "white_strobe")
            {
                mymission.mydrone.comp.strobe(2);
            }
            else if (commands["arguments"]["component"] == "smoke_marker")
            {
                mymission.mydrone.comp.smoke(2);
            }
        }
        fout << "[" << getTimestamp() << "] COMMAND: CHANGE_COMPONENT executed" << endl;
    }
    else if (command == "RESCUEE_FOUND")
    {
        mymission.configurerescueestate();
        fout << "[" << getTimestamp() << "] COMMAND: RESCUEE_FOUND executed" << endl;
    }

    else if (command == "ABORT_MISSION")
    {
        mymission.abortmission();
        activateRTH(mymission.mydrone,mymission.mydrone.myhome,mymission);
        fout << "[" << getTimestamp() << "] COMMAND: ABORT_MISSION executed" << endl;
    }
    else if (command == "TRANSMIT_INFO")
    {
        mymission.mydrone.transmitinfo();
        fout << "[" << getTimestamp() << "] COMMAND: TRANSMIT_INFO executed" << endl;
    }
    lastProcessedCommandID = commandID;

    runtime["control"]["last_processed_command"] = lastProcessedCommandID;
    saveRuntime();
}
// MAIN METHOD-------------------------------------
int main()
{
    ifstream configfile("config.json"); 
    configfile >> config;

    runtime = json::object();
    saveRuntime();

    setlocale(LC_ALL, ".UTF-8");

    mission mymission;

    fout << "[" << getTimestamp() << "] Control Launched" << endl;
    cout<<"Control launched."<<endl;

    while (true)
    {
        checkcommands(mymission);

        mymission.missionstatusupdater();
        this_thread::sleep_for(chrono::seconds(1));

        
    }


    return 0;
}