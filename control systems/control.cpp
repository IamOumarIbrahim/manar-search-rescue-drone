// LIBRARIES-----------------------------------------
    #include <iostream>
    #include <string>
    #include <clocale>
    #include <cmath>
    #include <iomanip>
    using namespace std;


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
// CLASSES-------------------------------------------
class flight{
    private:
        double speed = 0;
        double altitude = 0;
        double destlat = 25.276987;  // latitude of the destination 
        double destlon = 55.296249;  // longitude of the destination 
    public:
        void setspeed(double setspeed){
            if ((setspeed <= 15) && (setspeed >= 0)){
                if (speed < setspeed)
                {
                    while (speed < setspeed)
                    {
                        speed = speed + 1;
                    }
                }
                else if (speed > setspeed)
                {
                    while (speed > setspeed)
                    {
                        speed = speed - 1;
                    }
                }
            cout<<"----------------------------------"<<endl;
            cout<<"Speed manually set to "<<speed<<endl;
            }
        }
        void setaltitude(double setaltitude){
            if ((setaltitude <= 2000) && (setaltitude >= 0)){
                if (altitude < setaltitude)
                {
                    while (altitude < setaltitude)
                    {
                        altitude = altitude + 1;
                    }
                }
                else if (altitude > setaltitude)
                {
                    while (altitude > setaltitude)
                    {
                        altitude = altitude - 1;
                    }
                }
            cout<<"----------------------------------"<<endl;
            cout<<"Altitude manually set to "<<altitude<<endl;
            }
        }
        double getspeed()
        {
            return speed;
        }
        double getaltitude(){
            return altitude;
        }
        void stopflight(){
            cout<<"----------------------------------"<<endl;
            cout<<"Descending and reducing speed..."<<endl;
            while(speed > 0){
                speed = speed - 1;
            }
            while(altitude > 0){
                altitude = altitude - 1;
            }
            cout << "Successfully stopped." << endl;
        }
        void launch(){
            cout<<"----------------------------------"<<endl;
            cout<<"Initating launch..."<<endl;
            cout<<"Climbing to 10m..."<<endl;
            while (altitude < 10){
                altitude = altitude + 1;
            }
            cout<<"Successfully launched, drone hovering at 10m altitude"<<endl;
        }
        void setmode(int User_Option2){
            if (User_Option2 == 1)
            {
                cout<<"Flight mode: Quick -- Enabled"<<endl;
                setspeed(15);
            }
            else if (User_Option2 == 2)
            {
                cout<<"Flight mode: Active -- Enabled"<<endl;
                setspeed(5);
            }
            else if (User_Option2 == 4)
            {
                cout<<"Flight mode: Hover -- Enabled"<<endl;
                setspeed(0);
            }
            else if (User_Option2 == 3)
            {
                cout<<"Flight mode: Inspect -- Enabled"<<endl;
                setspeed(1);
            }
        }
        void printflightstatus(){
            cout<<"----------------------------------"<<endl;
            cout<<"Current Speed: "<<getspeed()<<" m/s"<<endl;
            cout<<"Current Altitude: "<<getaltitude()<<" m"<<endl;
        }
        void setdestination(double lat,double lon){
            destlat = lat;
            destlon = lon;
            cout<<"Destination set! Routing to "<< fixed << setprecision(6)<<destlat<<" ° N "<<destlon<<" ° E"<<endl<<
            "- Google Maps: https://www.google.com/maps?q="<<destlat<<","<<destlon<< endl;
        }
        void printdest(){
            cout<<"Current destination: "<<fixed << setprecision(6)<<destlat<<" ° N "<<destlon<<" ° E"<<endl<<
            "- Google Maps: https://www.google.com/maps?q="<<destlat<<","<<destlon<< endl;
        }
};
class components {
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
class home {
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
class drone {
    private:
        double latitude = 0;    // latitude of the drone 
        double longitude = 0;   // longitude of the drone 
        double battery = 100;   // Battery of drone default
        double distfromhome = 1000; // safe default
        bool  closefromhome = false;

    public:
        components comp;
        flight mydroneflight;
        home myhome{"Base Station", 25.276987, 55.296249};
    void printcurrent_location(){
        cout<<"Current drone location: "<<fixed << setprecision(6)<<latitude<<" ° N "<<longitude<<" ° E"<<endl<<
        "- Google Maps: https://www.google.com/maps?q="<<latitude<<","<<longitude<< endl;
    }
    void transmitinfo(){
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
        if (distfromhome <= 100){
            closefromhome = true;
        }
        else if (distfromhome > 100)
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

};
class mission {
    private:
        bool rescueefound = false;
        bool missionsuccess = false;
        bool missionstarted = false;
        bool missionabort = false;
    public:
        drone mydrone;
        void missionstatus(){
            mydrone.determineifclose();
            if ((missionstarted == true) && (missionabort == true)) {
                missionsuccess = false;
                cout << "----------------------------------" << endl;
                cout<< "Mission Failed! Mission has been aborted."<<endl;
            }
            else if ((missionstarted == true) && (missionabort == false)){
                if ((rescueefound == true) && (mydrone.getclose() == true)){
                    missionsuccess = true;
                    cout << "----------------------------------" << endl;
                    cout << "Mission Success! Rescuee has been found and the drone is close by to the home station. Well done." << endl;
                }
                else if ((rescueefound != true) && (mydrone.getclose() == true)) {
                    missionsuccess = false;
                    cout << "----------------------------------" << endl;
                    cout << "Mission Failed! Rescuee has NOT been found but the drone is close by to the home station." << endl;
                }
                else if ((rescueefound != true) && (mydrone.getclose() != true)) {
                    missionsuccess = false;
                    cout << "----------------------------------" << endl;
                    cout << "Mission Failed! Rescuee has NOT been found and the drone is far from the home station." << endl;
                }
                else if ((rescueefound == true) && (mydrone.getclose() != true)) {
                    missionsuccess = true;
                    cout << "----------------------------------" << endl;
                    cout << "Mission Success! Rescuee has been found but the drone is far from the home station." << endl;
                }
            }
            else if ((missionstarted == false) && (missionabort == false))
            {
                cout << "----------------------------------" << endl;
                cout<<"Mission has not started yet."<<endl;
            }

        }
        void configurerescueestate(){
            rescueefound = true;
        }
        void startmission(){
            missionstarted = true;
            cout<<"Missions successfully started."<<endl;
        }
        void abortmission(){
            missionabort = true;
            cout<<"Mission is aborted."<<endl;
        }


};


// GLOBAL VARIABLES----------------------------------
bool return_main = true;
int return_menu_int;
// TERMINAL METHODS----------------------------------
void activateRTH(drone &mydrone, home &myhome){
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
void displaystatus(drone mydrone){
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
void configurecomponents(drone &mydrone){
    return_menu_int = 0;
    int User_Option = -1;
    int User_Option2 = -1;
    cout<<"----------------------------------"<<endl;
    cout << "Select a payload option:\n"<< "1. Thermal\n"<< "2. RGB\n"<< "3. Infrared\n"<< "4. FMCW\n"<< "5. Speaker\n"<< "6. Mic\n"<< "7. RF\n"<< "8. Beacon\n"
    << "9. Strobe\n"<< "10. Spotlight\n"<< "11. Smoke\n\n"<< "Enter your choice (1-11): ";
    cin>>User_Option;
    cout<<"----------------------------------"<<endl;
    cout << "Select an option:\n"<< "1. TURN IT ON\n"<< "2. TURN IT OFF\n"<< "Enter your choice (1-2): ";
    cin>>User_Option2;

    if (User_Option2 == 1){
        switch (User_Option){
            case 1:
                cout << "Initializing Thermal camera..." << endl;
                mydrone.comp.thermal(User_Option2);
                break;
            case 2:
                cout << "Initializing RGB camera..." << endl;
                mydrone.comp.rgb(User_Option2);
                break;
            case 3:
                cout << "Initializing Infrared camera..." << endl;
                mydrone.comp.infrared(User_Option2);
                break;
            case 4:
                cout << "Initializing FMCW radar..." << endl;
                mydrone.comp.fmcw(User_Option2);
                break;
            case 5:
                cout << "Activating Speaker..." << endl;
                mydrone.comp.speaker(User_Option2);
                break;
            case 6:
                cout << "Activating Microphone..." << endl;
                mydrone.comp.microphone(User_Option2);
                break;
            case 7:
                cout << "Enabling RF module..." << endl;
                mydrone.comp.rf(User_Option2);
                break;
            case 8:
                cout << "Activating Beacon..." << endl;
                mydrone.comp.beacon(User_Option2);
                break;
            case 9:
                cout << "Activating Strobe light..." << endl;
                mydrone.comp.strobe(User_Option2);
                break;
            case 10:
                cout << "Turning on Spotlight..." << endl;
                mydrone.comp.spotlight(User_Option2);
                break;
            case 11:
                cout << "Deploying Smoke system..." << endl;
                mydrone.comp.smoke(User_Option2);
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
                break;
            case 2:
                cout << "Shutting down RGB camera..." << endl;
                mydrone.comp.rgb(User_Option2);
                break;
            case 3:
                cout << "Shutting down Infrared camera..." << endl;
                mydrone.comp.infrared(User_Option2);
                break;
            case 4:
                cout << "Shutting down FMCW radar..." << endl;
                mydrone.comp.fmcw(User_Option2);
                break;
            case 5:
                cout << "Deactivating Speaker..." << endl;
                mydrone.comp.speaker(User_Option2);
                break;
            case 6:
                cout << "Deactivating Microphone..." << endl;
                mydrone.comp.microphone(User_Option2);
                break;
            case 7:
                cout << "Disabling RF module..." << endl;
                mydrone.comp.rf(User_Option2);
                break;
            case 8:
                cout << "Deactivating Beacon..." << endl;
                mydrone.comp.beacon(User_Option2);
                break;
            case 9:
                cout << "Deactivating Strobe light..." << endl;
                mydrone.comp.strobe(User_Option2);
                break;
            case 10:
                cout << "Turning off Spotlight..." << endl;
                mydrone.comp.spotlight(User_Option2);
                break;
            case 11:
                cout << "Shutting down Smoke system..." << endl;
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
void displaycomponent(drone &mydrone){
    return_menu_int = 0;
    mydrone.comp.printcomp();

    while (return_menu_int != 10)
    {
    cout<<"Select an option."<<endl<<"- 2: Configure components"<<endl<<"- 10: Return to Main Menu."<<endl<<"Enter your choice (2, 10)..."<<endl;
    cin>>return_menu_int;
    if (return_menu_int == 10)
        return_main = true;
    else if (return_menu_int == 2){
        configurecomponents(mydrone);
        break;
    }

    }
}
void configureroute(drone &mydrone){
    return_menu_int = 0;
    int User_Option = -1;
    int User_Option2 = -1;
    double setspeed = 0;
    double setaltitude = 0;
    cout<<"----------------------------------"<<endl;
    cout << "Select an option:\n"<< "1. Set mode\n"<< "2. Set speed\n"<< "3. Set altitude\n"<< "4. Launch\n"<< "5. Stop flight\n";
    cout << "Enter your choice (1-5): ";
    cin>>User_Option;
    switch (User_Option){
        case 1:
            {
                cout<<"Enter the type of mode:\n"<<"1. Quick\n"<<"2. Active\n"<<"3. Inspect\n"<<"4. Hover\n";
                cin>>User_Option2;
                mydrone.mydroneflight.setmode(User_Option2);
                break;
            }
        case 2:
            {
                cout<<"Enter speed between 0-15m/s:\n";
                cin>>setspeed;
                mydrone.mydroneflight.setspeed(setspeed);
                break;
            }
        case 3:
            {
                cout<<"Enter altitude between 0-2000m:\n";
                cin>>setaltitude;
                mydrone.mydroneflight.setaltitude(setaltitude);
                break;
            }
        case 4:
            {
                mydrone.mydroneflight.launch();
                break;
            }
        case 5:
            {
                mydrone.mydroneflight.stopflight();
                break;
            }
        default:
            {
                cout<<"Please retry, only enter options (1-5)\n";
                break;
            }

    }
}
void displayroute(drone &mydrone){
    return_menu_int = 0;
    mydrone.printcurrent_location();
    mydrone.mydroneflight.printdest();
    mydrone.printdistfromhome();
    mydrone.mydroneflight.printflightstatus();
    cout<<"----------------------------------"<<endl;
    while (return_menu_int != 10)
    {
    cout<<"Select an option."<<endl<<"- 2: Configure flight options"<<endl<<"- 10: Return to Main Menu."<<endl<<"Enter your choice (2, 10)..."<<endl;
    cin>>return_menu_int;
    if (return_menu_int == 10)
        return_main = true;
    else if (return_menu_int == 2){
        configureroute(mydrone);
        break;
    }
    }
}
void transmitloc(drone mydrone){
    return_menu_int = 0;
    mydrone.printcurrent_location();
    mydrone.transmitinfo();
    cout<<"----------------------------------"<<endl;
    while (return_menu_int != 10)
    {
    cout<<"Enter 10 to return to menu."<<endl;
    cin>>return_menu_int;
    if (return_menu_int == 10)
        return_main = true;
    }
}
void configuremission(mission &mymission){
    return_menu_int = 0;
    string answer;
    int confirmation;
    int User_Option;
    // IMPLEMENTATION
    cout<<"----------------------------------"<<endl;
    cout << "Select an option:\n"<< "1. Launch mission\n"<< "2. Configure rescuee status\n"<< "0. Abort mission (RTH)\n";
    cout << "Enter your choice (1,2,0): ";
    cin>>User_Option;
    switch (User_Option)
    {
        case 1:
            {   
                cout<<"Starting mission..."<<endl;
                mymission.startmission();
                break;
            }
        case 2:
            {
                cout<<"Has a rescuee been found? (Y/N) "<<endl;
                cin>>answer;
                if ((answer == "Y" )||(answer == "y")){
                    mymission.configurerescueestate();
                    cout<<"Rescuee state set to found."<<endl;
                }
                break;
            }
        case 0:
            {
                cout<<"ARE YOU SURE YOU WANT TO ABORT THIS MISSION? ENTER 10 IF YOU WANT TO ABORT."<<endl;
                cin>>confirmation;
                if (confirmation == 10)
                {
                    cout<<"Aborting mission..."<<endl;
                    mymission.abortmission();
                }
                else{
                    cout<<"Mission has NOT been aborted."<<endl;
                }
                break;
            }
        default:
            {
                cout<<"Please either enter 1 or 2 or 0..."<<endl;
                break;
            }
    }
    // RETURN TO MENU
    cout<<"----------------------------------"<<endl;
    while (return_menu_int != 10)
    {
    cout<<"Enter 10 to return to menu."<<endl;
    cin>>return_menu_int;
    if (return_menu_int == 10)
        return_main = true;
    }
}

// MAIN METHOD-------------------------------------
int main(){
    setlocale(LC_ALL, ".UTF-8"); // DEGREE---------
    mission mymission; // START MISSION------------
    int User_Option = -1; // DEFAULT OPTION--------
    while (return_main == true){
        cout<<"----------------------------------"<<endl;
        cout<<"WELCOME TO MANAR HUMAN CONTROL SYSTEM"<<endl;
        cout<<"----------------------------------"<<endl;
        cout<<"PLEASE SELECT ONE OF THE FOLLOWING OPTIONS:"<<endl;
        cout<<"- ENTER 1 TO DISPLAY MISSION STATUS"<<endl;
        cout<<"- ENTER 2 TO CONFIGURE MISSION"<<endl;
        cout<<"- ENTER 3 FOR COMPONENTS"<<endl;
        cout<<"- ENTER 4 TO DISPLAY ROUTE STATUS"<<endl;
        cout<<"- ENTER 5 TO TRANSMIT LOCATION"<<endl;
        cout<<"- ENTER 0 TO ACTIVATE RTH-MODE"<<endl;
        cout<<"- ENTER 10 TO EXIT TERMINAL"<<endl;
        cout<<"----------------------------------"<<endl;
        cin>>User_Option;
            while (((User_Option < 0) || (User_Option >= 6)) && (User_Option != 10))  
            {
            cout<<"----------------------------------"<<endl;
            cout<<"PLEASE RETRY AND ENTER ONE OF THE FOLLOWING OPTIONS:"<<endl;
            cout<<"- ENTER 1 TO DISPLAY MISSION STATUS"<<endl;
            cout<<"- ENTER 2 TO CONFIGURE MISSION"<<endl;
            cout<<"- ENTER 3 FOR COMPONENTS"<<endl;
            cout<<"- ENTER 4 TO DISPLAY ROUTE STATUS"<<endl;
            cout<<"- ENTER 5 TO TRANSMIT LOCATION"<<endl;
            cout<<"- ENTER 0 TO ACTIVATE RTH-MODE"<<endl;
            cout<<"- ENTER 10 TO EXIT TERMINAL"<<endl;
            cout<<"----------------------------------"<<endl;
            cin>>User_Option;
            }  
        switch (User_Option){
            case 0:
            {
                cout<<"Activating Return-To-Home mode..."<<endl;
                activateRTH(mymission.mydrone,mymission.mydrone.myhome);
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
                displayroute(mymission.mydrone);
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
                return 0;
                break;
            }
        }
    }
    return 0;
}
