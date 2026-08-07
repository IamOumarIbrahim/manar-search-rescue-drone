#include <iostream>
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

void activateRTH(){}
void displaystatus(){}
void displaycomponent(){}
void displayroute(){}
void transmitloc(){}

int main(){
    int User_Option;
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
            activateRTH();
            break;
        }
        case 1:
        {
            cout<<"Displaying mission status..."<<endl;
            displaystatus();
            break;
        }
        case 2:
        {
            cout<<"Displaying current component configuration..."<<endl;
            displaycomponent();
            break;
        }
        case 3:
        {
            cout<<"Displaying route tracker system..."<<endl;
            displayroute();
            break;
        }
        case 4:
        {
            cout<<"Transmitting current location to station..."<<endl;
            transmitloc();
            break;
        }
    }
    return 0;
}