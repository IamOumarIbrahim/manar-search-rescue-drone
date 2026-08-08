// LIBRARIES-----------------------------------------
    #include <iostream>
    #include <string>
    #include <clocale>
    #include <cmath>
    using namespace std;
// LIBRARIES-----------------------------------------

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
class components {
    public:
        bool thermal_camera_status,RGB_camera_status,infrared_camera_status;
        bool fmcw_status,speaker_status,microphone_status,passive_rf_status;
        bool amber_beacon_status,white_strobe_status,downward_spotlight_status;
        bool smoke_marker_status;
        const bool heliograph_mirror_status = true;
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
        double destlat = 20;  // latitude of the destination 
        double destlon = 100;  // longitude of the destination 
        double battery = 100;   // Battery of drone default
        double distfromhome = 1000; // safe default
        bool  closefromhome = false;
    public:
        components comp;
        home myhome{"Base Station", 20.0, 100.0};
    void printcurrent_location(){
        cout<<"Current drone location: "<<latitude<<" ° N "<<longitude<<" ° E"<<endl;
    }
    void transmitinfo(){
        // TO BE IMPLEMENTED
    }
    void setdestination(double lat,double lon){
        destlat = lat;
        destlon = lon;
        cout<<"Destination set! Routing to "<<destlat<<" ° N "<<destlon<<" ° E"<<endl;
    }
    void printdest(){
        cout<<"Current destination is: "<<destlat<<" ° N "<<destlon<<" ° E"<<endl;
    }
    void printbattery(){
        cout<<battery<<"%"<<endl;
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
};
class mission {
    private:
        bool rescueefound = false;
        bool missionsuccess = false;
    public:
        drone mydrone;
        void missionstatus(){
            mydrone.determineifclose();

            if ((rescueefound == true) && (mydrone.getclose() == true)){
                missionsuccess = true;
                cout << "----------------------------------" << endl;
                cout << "Mission Success! Rescuee has been found and the drone is close by to the home station. Well done." << endl;
            }
        }
        void setrescueefound(){
            rescueefound = true;
        }


};
// CLASSES-------------------------------------------

// GLOBAL VARIABLES----------------------------------
bool return_main = true;
int return_menu_int;
// GLOBAL VARIABLES----------------------------------

// TERMINAL METHODS----------------------------------
void activateRTH(drone &mydrone, home &myhome){
    return_menu_int = 0;
    double X,Y;
    mydrone.printcurrent_location();
    X = myhome.gethomelat();
    Y = myhome.gethomelon();
    mydrone.setdestination(X,Y);
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
    
    cout<<"----------------------------------"<<endl;
    while (return_menu_int != 10)
    {
    cout<<"Enter 10 to return to menu."<<endl;
    cin>>return_menu_int;
    if (return_menu_int == 10)
        return_main = true;
    }
}
void displaycomponent(drone mydrone){
    return_menu_int = 0;
    mydrone.comp.printcomp();
    cout<<"----------------------------------"<<endl;
    while (return_menu_int != 10)
    {
    cout<<"Enter 10 to return to menu."<<endl;
    cin>>return_menu_int;
    if (return_menu_int == 10)
        return_main = true;
    }
}
void displayroute(drone mydrone){
    return_menu_int = 0;
    mydrone.printdest();
    cout<<"----------------------------------"<<endl;
    while (return_menu_int != 10)
    {
    cout<<"Enter 10 to return to menu."<<endl;
    cin>>return_menu_int;
    if (return_menu_int == 10)
        return_main = true;
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
// TERMINAL METHODS--------------------------------

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
        cout<<"- ENTER 1 FOR MISSION STATUS"<<endl;
        cout<<"- ENTER 2 FOR COMPONENTS"<<endl;
        cout<<"- ENTER 3 TO DISPLAY ROUTE STATUS"<<endl;
        cout<<"- ENTER 4 TO TRANSMIT LOCATION"<<endl;
        cout<<"- ENTER 0 TO ACTIVATE RTH-MODE"<<endl;
        cout<<"- ENTER 10 TO EXIT TERMINAL"<<endl;
        cout<<"----------------------------------"<<endl;
        cin>>User_Option;
            while (((User_Option < 0) || (User_Option >= 5)) && (User_Option != 10))  
            {
            cout<<"----------------------------------"<<endl;
            cout<<"PLEASE RETRY AND ENTER ONE OF THE FOLLOWING OPTIONS:"<<endl;
            cout<<"- ENTER 1 FOR MISSION STATUS"<<endl;
            cout<<"- ENTER 2 FOR COMPONENTS"<<endl;
            cout<<"- ENTER 3 TO DISPLAY ROUTE STATUS"<<endl;
            cout<<"- ENTER 4 TO TRANSMIT LOCATION"<<endl;
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
                displaystatus(mymission.mydrone);
                break;
            }
            case 2:
            {
                cout<<"Displaying current component configuration..."<<endl;
                displaycomponent(mymission.mydrone); // Done
                break;
            }
            case 3:
            {
                cout<<"Displaying route tracker system..."<<endl;
                displayroute(mymission.mydrone);
                break;
            }
            case 4:
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
// MAIN METHOD-------------------------------------