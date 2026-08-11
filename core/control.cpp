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
    #include <nlohmann/json.hpp>
    using namespace std;



using json = nlohmann::json;

json config;

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
            cout<<"----------------------------------"<<endl;
            cout<<"Speed goal manually set to "<<speed<<"m/s"<<endl;
            }
        }
        void setaltitude(double setaltitude)
        {
            if ((setaltitude <= config["maximum_altitude"]) && (setaltitude >= 0)){
                altitude = setaltitude;
            cout<<"----------------------------------"<<endl;
            cout<<"Altitude goal manually set to "<<altitude<<"m."<<endl;
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
            cout<<"----------------------------------"<<endl;
            cout<<"Descending and reducing speed..."<<endl;
            flightlaunched = false;
            this_thread::sleep_for(chrono::seconds(1));
            speed = 0;
            altitude = 0;
            cout << "Successfully stopped." << endl;
            mode = "Stall";
        }
        void launch()
        {
            cout<<"----------------------------------"<<endl;
            cout<<"Initating launch..."<<endl;
            flightlaunched = true;
            cout<<"Climbing to "<<config["launch_altitude"]<<" m."<<endl;
            altitude = config["launch_altitude"];
            cout<<"Successfully launched, drone hovering at "<<config["launch_altitude"]<<" m altitude."<<endl;
            mode = "Hover";
        }
        void setmodename(string m)
        {
            mode = m;
        }
        string getmodename()
        {
            return mode;
        }
        void setmode(int User_Option2)
        {
            if (User_Option2 == 1)
            {
                cout<<"Flight mode: Quick -- Enabled"<<endl;
                setspeed(config["quick_speed"]);
                setmodename("Quick");
            }
            else if (User_Option2 == 2)
            {
                cout<<"Flight mode: Active -- Enabled"<<endl;
                setspeed(config["active_speed"]);
                setmodename("Active");
            }
            else if (User_Option2 == 4)
            {
                cout<<"Flight mode: Hover -- Enabled"<<endl;
                setspeed(config["hover_speed"]);
                setmodename("Hover");
            }
            else if (User_Option2 == 3)
            {
                cout<<"Flight mode: Inspect -- Enabled"<<endl;
                setspeed(config["inspect_speed"]);
                setmodename("Inspect");
            }
        }
        void printflightstatus()
        {
            string stat;
            cout<<"----------------------------------"<<endl;
            cout<<"Current Speed: "<<getspeed()<<" m/s"<<endl;
            cout<<"Current Altitude: "<<getaltitude()<<" m"<<endl;
            cout<<"Current Mode: "<<getmodename()<<endl;
            if (getlaunched())
            {
                stat = "Launched";
            }
            else{
                stat = "Stalled";
            }
            cout<<"Current Status: "<<stat<<endl;
        }
        void setdestination(double lat,double lon)
        {
            destlat = lat;
            destlon = lon;
            cout<<"Destination set! Routing to "<< fixed << setprecision(6)<<destlat<<" ° N "<<destlon<<" ° E"<<endl<<
            "- Google Maps: https://www.google.com/maps?q="<<destlat<<","<<destlon<< endl;
        }
        void printdest()
        {
            cout<<"Current destination: "<<fixed << setprecision(6)<<destlat<<" ° N "<<destlon<<" ° E"<<endl<<
            "- Google Maps: https://www.google.com/maps?q="<<destlat<<","<<destlon<< endl;
        }
        bool getlaunched(){
            return flightlaunched;
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
    // PRINT METHODS
        void printcomp(){
            this_thread::sleep_for(chrono::seconds(1));
            cout<<"----------------------------------"<<endl;
            cout<<"Thermal camera: "<<thermal_camera_status<<endl;
            cout<<"RGB camera: "<<RGB_camera_status<<endl;
            cout<<"Low light/Infrared: "<<infrared_camera_status<<endl;
            cout<<"24Ghz FMCW: "<<fmcw_status<<endl;
            cout<<"Speaker: "<<speaker_status<<endl;
            cout<<"Microphone: "<<microphone_status<<endl;
            cout<<"Passive RF: "<<passive_rf_status<<endl;
            cout<<"Amber beacon: "<<amber_beacon_status<<endl;
            cout<<"White strobe: "<<white_strobe_status<<endl;
            cout<<"Downward spotlight: "<<downward_spotlight_status<<endl;
            cout<<"Heliograph mirrors: "<<heliograph_mirror_status<<endl;
            cout<<"Smoke marker: "<<smoke_marker_status<<endl;
            cout<<"----------------------------------"<<endl;
        }

    // CONFIGURATION METHODS
        void thermal(int X){

            if (X == 1){
                thermal_camera_status = true;
                cout<<"Confirmed thermal camera ON"<<endl;
            }

            else if (X == 2){
                thermal_camera_status = false;
                cout<<"Confirmed thermal camera OFF"<<endl;
            }
        }
        void rgb(int X){

            if (X == 1){
                RGB_camera_status = true;
                cout<<"Confirmed RGB camera ON"<<endl;
            }

            else if (X == 2){
                RGB_camera_status = false;
                cout<<"Confirmed RGB camera OFF"<<endl;
            }
        }
        void infrared(int X){
            if (X == 1){
                infrared_camera_status = true;
                cout<<"Confirmed infrared camera ON"<<endl;
            }

            else if (X == 2){
                infrared_camera_status = false;
                cout<<"Confirmed infrared camera OFF"<<endl;
            }
        }
        void fmcw(int X){
            if (X == 1){
                fmcw_status = true;
                cout<<"Confirmed FMCW radar ON"<<endl;
            }

            else if (X == 2){
                fmcw_status = false;
                cout<<"Confirmed FMCW radar OFF"<<endl;
            }
        }
        void speaker(int X){
            if (X == 1){
                speaker_status = true;
                cout<<"Confirmed speaker ON"<<endl;
            }
            else if (X == 2){
                speaker_status = false;
                cout<<"Confirmed speaker OFF"<<endl;
            }
        }
        void microphone(int X){
            if (X == 1){
                microphone_status = true;
                cout<<"Confirmed microphone ON"<<endl;
            }
            else if (X == 2){
                microphone_status = false;
                cout<<"Confirmed microphone OFF"<<endl;
            }
        }
        void rf(int X){
            if (X == 1){
                passive_rf_status = true;
                cout<<"Confirmed passive rf ON"<<endl;
            }
            else if (X == 2){
                passive_rf_status = false;
                cout<<"Confirmed passive rf OFF"<<endl;
            }
        }     
        void beacon(int X){
            if (X == 1){
                amber_beacon_status = true;
                cout<<"Confirmed amber beacon ON"<<endl;
            }
            else if (X == 2){
                amber_beacon_status = false;
                cout<<"Confirmed amber beacon OFF"<<endl;
            }
        }                
        void strobe(int X){
            if (X == 1){
                white_strobe_status = true;
                cout<<"Confirmed white strobe ON"<<endl;
            }
            else if (X == 2){
                white_strobe_status = false;
                cout<<"Confirmed white strobe OFF"<<endl;
            }
        }                
        void spotlight(int X){
            if (X == 1){
                downward_spotlight_status = true;
                cout<<"Confirmed spotlight ON"<<endl;
            }
            else if (X == 2){
                downward_spotlight_status = false;
                cout<<"Confirmed spotlight OFF"<<endl;
            }
        }         
        void smoke(int X){
            if (X == 1){
                smoke_marker_status = true;
                cout<<"Confirmed smoke marker ON"<<endl;
            }
            else if (X == 2){
                smoke_marker_status = false;
                cout<<"Confirmed smoke marker OFF"<<endl;
            }
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
        double battery = 100;   // Battery of drone default
        double distfromhome = 1000; // safe default
        bool  closefromhome = false;
        bool savebattery = false; // TO BE IMPLEMENTED
    public:
        components comp;
        flight mydroneflight;
        home myhome{config["home_base_name"], config["home_base_latitude"], config["home_base_longitude"]};
    void printcurrent_location(){
        cout<<"----------------------------------"<<endl;
        this_thread::sleep_for(chrono::seconds(1));
        cout<<"Current drone location: "<<fixed << setprecision(6)<<latitude<<" ° N "<<longitude<<" ° E"<<endl<<
        "- Google Maps: https://www.google.com/maps?q="<<latitude<<","<<longitude<< endl;
    }
    void transmitinfo(){
        printcurrent_location();
        // TO BE IMPLEMENTED
    }
    void printbattery(){
        cout<<"Battery: "<<battery<<"%"<<endl;
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
        cout<<"Current distance to base: "<<fixed<<setprecision(0)<<distfromhome<<" meters."<<endl;
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
};
class mission 
{
    private:
        bool rescueefound = false; // if found then transmit location, hover in that place and save battery till manual help
                                        // if not found generate report and RTH if battery 
        bool missionsuccess = false;
        bool missionstarted = false;
        bool missionabort = false;
        bool missionfinished = false;
        bool enroute = false;
        bool returning = false;
        bool emergencyrth = false;
        bool checkedlocation = false;
        bool waitingforhelp = false;
    public:
        drone mydrone;
        void missionstatus() // DISPLAY UNIT
        {   
            missionstatusupdater();
            this_thread::sleep_for(chrono::seconds(1));
            if (rescueefound == true)
            {
                cout << "----------------------------------" << endl;
                cout<<"A rescuee has been found."<<endl;
            }
            else
            {
                cout << "----------------------------------" << endl;               
                cout<<"No rescuees has been found yet."<<endl;
            }
            mydrone.determineifclose();
            if (missionstarted == false)
                {
                    cout<<"Mission has not started yet."<<endl;
                }
            else if ((missionstarted == true) && (missionabort == true)) 
                {
                    missionsuccess = false;
                    cout<< "Mission Failed! Mission has been aborted."<<endl;
                }
            else if ((missionstarted == true) && (missionabort == false))
            {
                if (enroute == true)
                {
                    cout<< "Mission ongoing. Drone is on route."<<endl;
                }
                else if (returning == true)
                {
                    cout<< "Mission ongoing. Drone is returning to base."<<endl;
                }
                else if (waitingforhelp == true)
                {
                    cout<<"Mission ongoing. Manual help is underway..."<<endl;
                }
            }
        }
        void missionstatusupdater() // CONTROL UNIT
        {   
            if (missionstarted == true)
            {
                if (missionabort == false)
                {
                    if (checkedlocation == false)
                        {
                            enroute = true;
                            returning = false;
                            if (rescueefound == true)
                            {
                                mydrone.transmitinfo(); // TRANSMIT INFO
                                mydrone.mydroneflight.setmode(4); // SET TO HOVER
                                mydrone.savebatterymode(1); // SAVE BATTERY
                                mydrone.mydroneflight.stopflight(); // LAND DRONE
                                enroute = false;
                                waitingforhelp = true;
                            }

                        }
                    else if (checkedlocation == true)
                        {
                            if (rescueefound == false)
                            {
                                returning = true; 
                                enroute = false; 
                                double X,Y;
                                X = mydrone.myhome.gethomelat();
                                Y = mydrone.myhome.gethomelon();
                                mydrone.mydroneflight.setdestination(X,Y);

                            }
                            else if (rescueefound == true)
                            {
                                waitingforhelp = true;
                                mydrone.transmitinfo(); // TRANSMIT INFO
                                mydrone.mydroneflight.setmode(4); // SET TO HOVER
                                mydrone.savebatterymode(1); // SAVE BATTERY
                                mydrone.mydroneflight.stopflight(); // LAND DRONE
                            }

                        }
                    if (returning == true) 
                    {
                        mydrone.determineifclose();
                        if (mydrone.getclose() == true)
                        {
                            missionfinished = true;
                        }
                    }
                }
                else if (missionabort == true)
                {
                    returning = true;
                    enroute = false;
                    if (returning == true) 
                    {
                        mydrone.determineifclose();
                        if (mydrone.getclose() == true)
                        {
                            missionfinished = true;
                        }
                    }
                }
            }
        }
        void configurerescueestate() // MANUALLY SET BY THE OPERATOR
        {
            if (missionstarted == true)
            {
                rescueefound = true;
                missionstatusupdater();     
            }

        }
        void startmission() // MANUALL LAUNCH
        {
            missionstarted = true;
            missionstatusupdater();
            cout<<"Missions successfully started. Drone is currently enroute"<<endl;
        }
        void abortmission() // MANUALL ABORT
        {
            missionabort = true;
            missionstatusupdater();
            cout<<"Mission is aborted."<<endl;
        }
        void batterysystem() // DETERMINISTIC BATTERY BEHAVIOUR
        {
            if (mydrone.getbattery() <= config["battery_emergency_rth"])
            {
                if (rescueefound == false){
                    emergencyrth = true;
                    mydrone.transmitinfo();
                    cout<<"[Battery: CRITICAL] -- Enabling emergency rth"<<endl;
                    if (mydrone.getbattery() <= config["battery_emergency_land"]) // HAUL FLIGHT OPERATION, LAND SAFELY
                    {
                        cout<<"[Battery: CRITICAL] -- Transmitting current location..."<<endl;
                        mydrone.transmitinfo();
                        cout<<"[Battery: CRITICAL] -- Landing safely..."<<endl;
                        mydrone.mydroneflight.stopflight();
                        cout<<"[Battery: CRITICAL] -- Transmitting current location..."<<endl;
                        mydrone.transmitinfo();
                    }
                }
            }
            if (mydrone.getbattery() <= config["battery_rth_warning"])
            {
                cout<<"Battery is currently under "<<config["battery_rth_warning"]<<"%, Transmitting current location... Please consider ENABLING RTH."<<endl;
                mydrone.transmitinfo();
            }
            if (mydrone.getbattery() <= config["battery_warning"])
            {
                cout<<"Battery is currently under "<<config["battery_warning"]<<"%, Consider enabling energy save-mode."<<endl;
            }


        }
        void setcheckedloc(string ans)
        {   
            if (missionstarted == true)
            {
                if ((ans == "Y")||(ans == "y"))
                {
                    checkedlocation = true;
                    cout<<"Location has been checked."<<endl;
                }

                else if ((ans == "N")||(ans == "n"))
                {
                    checkedlocation = false;
                    cout<<"Location has NOT been checked."<<endl;
                }
            }

            missionstatusupdater();
        }
        void printflightpath()
        {
            if (returning == true)
            {
                cout<<"Current flight status: Returning.\n";
            }
            else if (enroute == true)
            {
                cout<<"Current flight status: Enroute.\n";
            }
            else if (waitingforhelp == true)
            {
                cout<<"Current flight status: Waiting for help.\n";
            }
        }
        bool getmissionstarted(){
            return missionstarted;
        }
};
// GLOBAL VARIABLES----------------------------------
bool return_main = true;
int return_menu_int;
// TERMINAL METHODS----------------------------------
void activateRTH(drone &mydrone, home &myhome)
{
    fout << "[" << getTimestamp() << "] RTH: disabled -> enabled" << endl;
    return_menu_int = 0;
    double X,Y;
    mydrone.printcurrent_location();
    X = myhome.gethomelat();
    Y = myhome.gethomelon();
    mydrone.mydroneflight.setdestination(X,Y);
    double currentspeed = mydrone.mydroneflight.getspeed();
    double distance = mydrone.getdistfromhome();
    double estimatedtime = 0;
        if (currentspeed > 0){
             estimatedtime = distance / currentspeed;
        }
        cout<<"Estimated arrival time: "<<estimatedtime<<" seconds."<<endl;
    cout<<"----------------------------------"<<endl;
    while (return_menu_int != 10)
    {
    cout<<"Enter 10 to return to menu."<<endl;
    cin>>return_menu_int;
    if (return_menu_int == 10)
        return_main = true;
    }
}
void displaystatus(drone mydrone)
{
    return_menu_int = 0;
    //ENTER IMPLEMENTATION
    mydrone.printbattery();
    mydrone.printdistfromhome();
    cout<<"----------------------------------"<<endl;
    while (return_menu_int != 10)
    {
    cout<<"Enter 10 to return to menu."<<endl;
    cin>>return_menu_int;
    if (return_menu_int == 10)
        return_main = true;
    }
}
void configurecomponents(drone &mydrone)
{
    return_menu_int = 0;
    int User_Option = -1;
    int User_Option2 = -1;
    cout<<"----------------------------------"<<endl;
    this_thread::sleep_for(chrono::seconds(1));
    cout << "Select a payload option:\n"<< "1. Thermal\n"<< "2. RGB\n"<< "3. Infrared\n"<< "4. FMCW\n"<< "5. Speaker\n"<< "6. Mic\n"<< "7. RF\n"<< "8. Beacon\n"
    << "9. Strobe\n"<< "10. Spotlight\n"<< "11. Smoke\n\n"<< "Select an option (1-11): ";
    cin>>User_Option;
    cout<<"----------------------------------"<<endl;
    this_thread::sleep_for(chrono::seconds(1));
    cout << "Select an option:\n"<< "1. TURN IT ON\n"<< "2. TURN IT OFF\n"<< "Select an option (1-2): ";
    cin>>User_Option2;

    if (User_Option2 == 1){
        switch (User_Option){
            case 1:
                cout << "Initializing Thermal camera..." << endl;
                mydrone.comp.thermal(User_Option2);
                fout << "[" << getTimestamp() << "] COMPONENTS: thermal camera ON" << endl;
                break;
            case 2:
                cout << "Initializing RGB camera..." << endl;
                mydrone.comp.rgb(User_Option2);
                fout << "[" << getTimestamp() << "] COMPONENTS: RGB camera ON" << endl;
                break;
            case 3:
                cout << "Initializing Infrared camera..." << endl;
                mydrone.comp.infrared(User_Option2);
                fout << "[" << getTimestamp() << "] COMPONENTS: Infrared camera ON" << endl;
                break;
            case 4:
                cout << "Initializing FMCW radar..." << endl;
                mydrone.comp.fmcw(User_Option2);
                fout << "[" << getTimestamp() << "] COMPONENTS: FMCW radar ON" << endl;
                break;
            case 5:
                cout << "Activating Speaker..." << endl;
                mydrone.comp.speaker(User_Option2);
                fout << "[" << getTimestamp() << "] COMPONENTS: speaker ON" << endl;
                break;
            case 6:
                cout << "Activating Microphone..." << endl;
                mydrone.comp.microphone(User_Option2);
                fout << "[" << getTimestamp() << "] COMPONENTS: microphone ON" << endl;
                break;
            case 7:
                cout << "Enabling RF module..." << endl;
                mydrone.comp.rf(User_Option2);
                fout << "[" << getTimestamp() << "] COMPONENTS: rf module ON" << endl;
                break;
            case 8:
                cout << "Activating Beacon..." << endl;
                mydrone.comp.beacon(User_Option2);
                fout << "[" << getTimestamp() << "] COMPONENTS: beacon ON" << endl;
                break;
            case 9:
                cout << "Activating Strobe light..." << endl;
                mydrone.comp.strobe(User_Option2);
                fout << "[" << getTimestamp() << "] COMPONENTS: strobe ON" << endl;
                break;
            case 10:
                cout << "Turning on Spotlight..." << endl;
                mydrone.comp.spotlight(User_Option2);
                fout << "[" << getTimestamp() << "] COMPONENTS: spotlight ON" << endl;
                break;
            case 11:
                cout << "Deploying Smoke system..." << endl;
                mydrone.comp.smoke(User_Option2);
                fout << "[" << getTimestamp() << "] COMPONENTS: smoke ON" << endl;
                break;
            default:
                cout << "Invalid selection. Please choose a number between 1 and 11." << endl;
                break;
        }
    }
    else if (User_Option2 == 2){
        switch (User_Option){
            case 1:
                cout << "Shutting down Thermal camera..." << endl;
                mydrone.comp.thermal(User_Option2);
                fout << "[" << getTimestamp() << "] COMPONENTS: thermal camera OFF " << endl;
                break;
            case 2:
                cout << "Shutting down RGB camera..." << endl;
                mydrone.comp.rgb(User_Option2);
                fout << "[" << getTimestamp() << "] COMPONENTS: RGB camera OFF " << endl;
                break;
            case 3:
                cout << "Shutting down Infrared camera..." << endl;
                mydrone.comp.infrared(User_Option2);
                fout << "[" << getTimestamp() << "] COMPONENTS: Infrared camera OFF " << endl;
                break;
            case 4:
                cout << "Shutting down FMCW radar..." << endl;
                fout << "[" << getTimestamp() << "] COMPONENTS: FMCW radar OFF " << endl;
                mydrone.comp.fmcw(User_Option2);
                break;
            case 5:
                cout << "Deactivating Speaker..." << endl;
                fout << "[" << getTimestamp() << "] COMPONENTS: speaker OFF " << endl;
                mydrone.comp.speaker(User_Option2);
                break;
            case 6:
                cout << "Deactivating Microphone..." << endl;
                fout << "[" << getTimestamp() << "] COMPONENTS: microphone OFF " << endl;
                mydrone.comp.microphone(User_Option2);
                break;
            case 7:
                cout << "Disabling RF module..." << endl;
                fout << "[" << getTimestamp() << "] COMPONENTS: RF module OFF " << endl;
                mydrone.comp.rf(User_Option2);
                break;
            case 8:
                cout << "Deactivating Beacon..." << endl;
                fout << "[" << getTimestamp() << "] COMPONENTS: beacon OFF " << endl;
                mydrone.comp.beacon(User_Option2);
                break;
            case 9:
                cout << "Deactivating Strobe light..." << endl;
                fout << "[" << getTimestamp() << "] COMPONENTS: strobe OFF " << endl;
                mydrone.comp.strobe(User_Option2);
                break;
            case 10:
                cout << "Turning off Spotlight..." << endl;
                fout << "[" << getTimestamp() << "] COMPONENTS: spotlight OFF " << endl;
                mydrone.comp.spotlight(User_Option2);
                break;
            case 11:
                cout << "Shutting down Smoke system..." << endl;
                fout << "[" << getTimestamp() << "] COMPONENTS: smoke OFF " << endl;
                mydrone.comp.smoke(User_Option2);
                break;
            default:
                cout << "Invalid selection. Please choose a number between 1 and 11." << endl;
                break;
        }
    }
    cout<<"----------------------------------"<<endl;
    while (return_menu_int != 10)
    {
    cout<<"Enter 10 to return to menu."<<endl;
    cin>>return_menu_int;
    if (return_menu_int == 10)
        return_main = true;
    }
}
void displaycomponent(drone &mydrone)
{
    return_menu_int = 0;
    mydrone.comp.printcomp();
    this_thread::sleep_for(chrono::seconds(1));
    while (return_menu_int != 10)
    {
    
    cout<<"Select an option."<<endl<<"- 2: Configure components"<<endl<<"- 10: Return to Main Menu."<<endl<<"Select an option (2, 10)..."<<endl;
    cin>>return_menu_int;
    if (return_menu_int == 10)
        return_main = true;
    else if (return_menu_int == 2){
        configurecomponents(mydrone);
        break;
    }

    }
}
void configureroute(drone &mydrone)
{
    return_menu_int = 0;
    int User_Option = -1;
    int User_Option2 = -1;
    double setspeed = 0;
    double setaltitude = 0;
    cout<<"----------------------------------"<<endl;
    cout << "Select an option:\n"<< "1. Set mode\n"<< "2. Set speed\n"<< "3. Set altitude\n"<< "4. Launch\n"<< "5. Stop flight\n"<<"6. Set target location\n"<<"10. Return to menu.\n";
    cout << "Select an option (1-6,10): ";
    cin>>User_Option;
    switch (User_Option){
        case 1:
            {
                if (mydrone.mydroneflight.getlaunched())
                {
                    cout<<"Enter the type of mode:\n"<<"1. Quick\n"<<"2. Active\n"<<"3. Inspect\n"<<"4. Hover\n";
                    cin>>User_Option2;
                    string currentmode = mydrone.mydroneflight.getmodename();
                    mydrone.mydroneflight.setmode(User_Option2);
                    fout << "[" << getTimestamp() << "] FLIGHT: mode configured " <<currentmode<<" -> "<<mydrone.mydroneflight.getmodename() << endl;
                }
                else
                {
                    cout<<"Flight has NOT been launched yet, cannot modify mode."<<endl;
                }
 
                break;
            }
        case 2:
            {
                if (mydrone.mydroneflight.getlaunched())
                {
                    cout<<"Enter speed between 0-"<<config["maximum_speed"]<<"m/s."<<endl;
                    cin>>setspeed;
                    double currentsp = mydrone.mydroneflight.getspeed();
                    mydrone.mydroneflight.setspeed(setspeed);
                    fout << "[" << getTimestamp() << "] FLIGHT: speed configured " << currentsp <<"m/s -> "<< setspeed<<"m/s."<< endl;
                }
                else
                {
                    cout<<"Flight has NOT been launched yet, cannot modify speed."<<endl;
                }
                break;
            }
        case 3:
            {
                if (mydrone.mydroneflight.getlaunched())
                {
                    cout<<"Enter altitude between 0-"<<config["maximum_altitude"]<<"m"<<endl;
                    cin>>setaltitude;
                    double currental = mydrone.mydroneflight.getaltitude();
                    mydrone.mydroneflight.setaltitude(setaltitude);
                    fout << "[" << getTimestamp() << "] FLIGHT: altitude configured" << currental <<"m -> "<< setaltitude<<"m."<< endl;
                }
                else
                {
                    cout<<"Flight has NOT been launched yet, cannot modify altitude."<<endl;
                }
                break;
            }
        case 4:
            {
                if (!mydrone.mydroneflight.getlaunched())
                {
                    mydrone.mydroneflight.launch();
                    fout << "[" << getTimestamp() << "] FLIGHT: launched" << endl;
                }
                else
                {
                    cout<<"Flight already launched!"<<endl;
                }
                break;
            }
        case 5:
            {
                if (mydrone.mydroneflight.getlaunched())
                {
                    mydrone.mydroneflight.stopflight();
                    fout << "[" << getTimestamp() << "] FLIGHT: stopped" << endl;
                }
                else
                {
                    cout<<"Flight already stopped!"<<endl;
                }
                break;
            }
        case 6:
            {
                double destlatset,destlongset;
                cout<<"Enter the destination latitude (Up to 6 decimal places):\n";
                cin>>destlatset;
                cout<<"Enter the destination longtitude (Up to 6 decimal places):\n";
                cin>>destlongset;
                mydrone.mydroneflight.setdestination(destlatset,destlongset);
                break;
            }
        case 10:
            {
                return_main = true;
                break;
            }
        default:
            {
                cout<<"Please retry, only enter options (1-5)\n";
                break;
            }

    }
}
void displayroute(drone &mydrone, mission &mymission)
{
    return_menu_int = 0;

    mydrone.printcurrent_location();
    mydrone.mydroneflight.printdest();
    mydrone.printdistfromhome();
    mydrone.mydroneflight.printflightstatus();
    mymission.printflightpath();
    cout<<"----------------------------------"<<endl;
    while (return_menu_int != 10)
    {
    cout<<"Select an option."<<endl<<"- 2: Configure flight options"<<endl<<"- 10: Return to Main Menu."<<endl<<"Select an option (2, 10)..."<<endl;
    cin>>return_menu_int;
    if (return_menu_int == 10)
        return_main = true;
    else if (return_menu_int == 2){
        configureroute(mydrone);
        break;
    }
    }
}
void transmitloc(drone mydrone)
{
    return_menu_int = 0;
    mydrone.transmitinfo();
    fout << "[" << getTimestamp() << "] INFO: transmitted" << endl;
    cout<<"----------------------------------"<<endl;
    while (return_menu_int != 10)
    {
    cout<<"Enter 10 to return to menu."<<endl;
    cin>>return_menu_int;
    if (return_menu_int == 10)
        return_main = true;
    }
}
void configuremission(mission &mymission)
{
    return_menu_int = 0;
    string returntohomeans = "";
    string answer;
    string User_Option2;
    int confirmation;
    int User_Option;
    // IMPLEMENTATION
    cout<<"----------------------------------"<<endl;
    this_thread::sleep_for(chrono::seconds(1));
    cout << "Select an option:\n"<< "1. Launch mission\n"<< "2. Configure rescuee status\n"<<"3. Configure location checked status\n"<< "0. Abort mission (RTH)\n"<<"10. Return to menu\n";
    cout << "Select an option (1,2,0,10): ";
    cin>>User_Option;
    switch (User_Option)
    {
        case 1:
            {   
                cout<<"Starting mission..."<<endl;
                mymission.startmission();
                fout << "[" << getTimestamp() << "] MISSION: stalled -> launched" << endl;
                break;
            }
        case 2:
            {
                cout<<"Has a rescuee been found? (Y/N) "<<endl;
                cin>>answer;
                if ((answer == "Y" )||(answer == "y")){
                    mymission.configurerescueestate();
                    cout<<"Rescuee state set to found."<<endl;
                    this_thread::sleep_for(chrono::seconds(1));
                    fout << "[" << getTimestamp() << "] RESCUEE: lost -> found" << endl;
                }
                break;
            }
        case 0:
            {   
                cout<<"ARE YOU SURE YOU WANT TO ABORT THIS MISSION? ENTER 10 IF YOU WANT TO ABORT."<<endl;
                cin>>confirmation;
                if (confirmation == 10)
                {
                    if (mymission.getmissionstarted()){
                        fout << "[" << getTimestamp() << "] MISSION: ongoing -> aborted" << endl;
                        cout<<"Aborting mission..."<<endl;
                        mymission.abortmission();
                        cout<<"Return to home? (Y/n)"<<endl;
                        cin>>returntohomeans;
                        if ((returntohomeans == "Y") || (returntohomeans == "y"))
                        {
                            cout<<"Activating Return-To-Home mode..."<<endl;
                            activateRTH(mymission.mydrone,mymission.mydrone.myhome);
                            fout << "[" << getTimestamp() << "] RTH: disabled -> enabled" << endl;
                        }
                        else
                        {
                            cout<<"Return to home has NOT been activated."<<endl;
                        }
                    }
                    else {
                        cout<<"Cannot abort a mission that did NOT start."<<endl;
                    }

                }
                else{
                    cout<<"Mission has NOT been aborted."<<endl;
                }
                break;
            }
        case 10:
            {
                return_main = true;
                break;
            }
        case 3:
            {   cout<<"Has the drone checked the location? (Y/n)?\n";
                cin>>User_Option2;
                cout<<"Changing status..."<<endl;
                mymission.setcheckedloc(User_Option2);
                fout << "[" << getTimestamp() << "] LOCATION: unchecked -> checked" << endl;
            }
        default:
            {
                cout<<"Please either enter 1 or 2 or 0..."<<endl;
                break;
            }
    }
}

// MAIN METHOD-------------------------------------
int main()
{
    ifstream configfile("config.json"); 
    configfile >> config;
    setlocale(LC_ALL, ".UTF-8"); // DEGREE---------
    mission mymission; // START MISSION------------
    int User_Option = -1; // DEFAULT OPTION--------
    string confirmationrth = "";
    fout << "[" << getTimestamp() << "] Terminal started" << endl;
    while (return_main == true){
        this_thread::sleep_for(chrono::seconds(1));
        cout<<"----------------------------------"<<endl;
        cout<<"WELCOME TO MANAR HUMAN CONTROL SYSTEM"<<endl;
        cout<<"----------------------------------"<<endl;
        cout<<"Select an option (1-5,0,10):"<<endl;
        cout<<"- 1. DISPLAY MISSION STATUS"<<endl;
        cout<<"- 2. CONFIGURE MISSION"<<endl;
        cout<<"- 3. COMPONENTS"<<endl;
        cout<<"- 4. DISPLAY ROUTE STATUS"<<endl;
        cout<<"- 5. TRANSMIT LOCATION"<<endl;
        cout<<"- 0. ACTIVATE RTH-MODE"<<endl;
        cout<<"- 10. EXIT TERMINAL"<<endl;
        cout<<"----------------------------------"<<endl;
        cin>>User_Option;
            while (((User_Option < 0) || (User_Option >= 6)) && (User_Option != 10))  
            {
            this_thread::sleep_for(chrono::seconds(1));
            cout<<"----------------------------------"<<endl;
            cout<<"Select an option (1-5,0,10):"<<endl;
            cout<<"- 1. DISPLAY MISSION STATUS"<<endl;
            cout<<"- 2. CONFIGURE MISSION"<<endl;
            cout<<"- 3. COMPONENTS"<<endl;
            cout<<"- 4. DISPLAY ROUTE STATUS"<<endl;
            cout<<"- 5. TRANSMIT LOCATION"<<endl;
            cout<<"- 0. ACTIVATE RTH-MODE"<<endl;
            cout<<"- 10. EXIT TERMINAL"<<endl;
            cout<<"----------------------------------"<<endl;
            cin>>User_Option;
            }  
        switch (User_Option){
            case 0:
            {
                cout<<"ARE YOU SURE YOU WANT TO ACTIVATE RETURN TO HOME OPERATION? (Y/N): "<<endl;
                cin>>confirmationrth;
                if ((confirmationrth == "y")||(confirmationrth =="Y"))
                {
                    cout<<"Activating Return-To-Home mode..."<<endl;
                    activateRTH(mymission.mydrone,mymission.mydrone.myhome);
                }
                else 
                {
                    cout<<"RTH has NOT been activated."<<endl;
                }
                break;
            }
            case 1:
            {
                cout<<"Displaying mission status..."<<endl;
                mymission.missionstatus();
                displaystatus(mymission.mydrone);
                break;
            }
            case 2:
            {
                cout<<"Launching mission configuration..."<<endl;
                configuremission(mymission); // Done
                break;
            }
            case 3:
            {
                cout<<"Displaying current component configuration..."<<endl;
                displaycomponent(mymission.mydrone); // Done
                break;
            }
            case 4:
            {
                cout<<"Displaying route tracker system..."<<endl;
                displayroute(mymission.mydrone,mymission);
                break;
            }
            case 5:
            {
                cout<<"Transmitting current location to station..."<<endl;
                transmitloc(mymission.mydrone);
                break;
            }
            case 10:
            {
                cout<<"Exiting terminal now..."<<endl;
                fout << "[" << getTimestamp() << "] Exiting terminal" << endl;
                return 0;
                break;
            }
        }
    }
    return 0;
}
