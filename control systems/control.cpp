#include <iostream>
#include <string>
using namespace std;

/*
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

// GLOBAL VARIABLES----------------------------------
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
class drone {
    private:
        string current_location_coordinates;
    public:
        components comp;
    void printcurrent_location(){
        cout<<"Current drone location: "<<current_location_coordinates<<endl;
    }
    void transmitinfo(){
        // TO BE IMPLEMENTED
    }
    drone(){
        current_location_coordinates = "";
    }
};
bool return_main = true;
int return_menu_int;
// GLOBAL VARIABLES----------------------------------

void activateRTH(drone mydrone){}
void displaystatus(drone mydrone){}
void displaycomponent(drone mydrone){
    return_menu_int = 0;
    mydrone.comp.printcomp();
    while (return_menu_int != 10)
    {
    cout<<"Enter 10 to return to menu."<<endl;
    cin>>return_menu_int;
    if (return_menu_int == 10)
        return_main = true;
    }
}
void displayroute(drone mydrone){}
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

int main(){
    drone mydrone;
    int User_Option;
    while (return_main == true){
    cout<<"----------------------------------"<<endl;
    cout<<"WELCOME TO MANAR HUMAN CONTROL SYSTEM"<<endl;
    cout<<"----------------------------------"<<endl;
    cout<<"PLEASE SELECT ONE OF THE FOLLOWING OPTIONS:"<<endl;
    cout<<"- ENTER 1 FOR MISSION STATUS"<<endl;
    cout<<"- ENTER 2 FOR COMPONENTS"<<endl;
    cout<<"- ENTER 3 TO ALTER ROUTE"<<endl;
    cout<<"- ENTER 4 TO TRANSMIT LOCATION"<<endl;
    cout<<"- ENTER 0 TO ACTIVATE RTH-MODE"<<endl;
    cout<<"----------------------------------"<<endl;
    cin>>User_Option;
        while ((User_Option < 0) || (User_Option >= 5))  
        {
        cout<<"----------------------------------"<<endl;
        cout<<"PLEASE RETRY AND ENTER ONE OF THE FOLLOWING OPTIONS:"<<endl;
        cout<<"- ENTER 1 FOR MISSION STATUS"<<endl;
        cout<<"- ENTER 2 FOR COMPONENTS"<<endl;
        cout<<"- ENTER 3 TO ALTER ROUTE"<<endl;
        cout<<"- ENTER 4 TO TRANSMIT LOCATION"<<endl;
        cout<<"- ENTER 0 TO ACTIVATE RTH-MODE"<<endl;
        cout<<"----------------------------------"<<endl;
        cin>>User_Option;
        }  
    switch (User_Option){
        case 0:
        {
            cout<<"Activating Return-To-Home mode..."<<endl;
            activateRTH(mydrone);
            break;
        }
        case 1:
        {
            cout<<"Displaying mission status..."<<endl;
            displaystatus(mydrone);
            break;
        }
        case 2:
        {
            cout<<"Displaying current component configuration..."<<endl;
            displaycomponent(mydrone); // Done
            break;
        }
        case 3:
        {
            cout<<"Displaying route tracker system..."<<endl;
            displayroute(mydrone);
            break;
        }
        case 4:
        {
            cout<<"Transmitting current location to station..."<<endl;
            transmitloc(mydrone);
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