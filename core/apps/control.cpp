// LIBRARIES-----------------------------------------
    #include <iostream>
    #include <string>
    #include <clocale>
    #include <thread>
    #include <chrono>  
    #include <fstream>
    #include <ctime> 
    
    #include "system/shared.hpp"
    #include "system/mission.hpp"
    using namespace std;

json commands;

int lastProcessedCommandID = 0;
/* RESPONSIBILITY
    According to Section 2.4 (Responsibility Allocation) of the Version 1 specification,
    the human operator must be capable of executing all of these tasks if required

    - Defines the search area, objective, launch point, route, altitude, geofence, and safety limits.
    - Approves takeoff and may start, pause, command RTL, or manually control the aircraft.
    - Monitors flight status, map position, and sensor feeds.
    - May cancel any automatic candidate approach or verification sequence.
    - Reviews Mamba-triggered alerts and makes the final Probable rescuee determination.
    - Approves beacon, strobe, smoke, and other consequential guidance actions.
    - Shares confirmed coordinates with rescue teams and handles exceptional emergency decisions.

*/

bool readcommands()
{
    ifstream in("runtime/commands.json");

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
void changeComponent(components &comp, string component, bool enabled)
{
    int state = enabled ? 1 : 2;

    if (component == "thermal_camera")
        comp.thermal(state);

    else if (component == "rgb_camera")
        comp.rgb(state);

    else if (component == "infrared_camera")
        comp.infrared(state);

    else if (component == "fmcw_radar")
        comp.fmcw(state);

    else if (component == "speaker")
        comp.speaker(state);

    else if (component == "microphone")
        comp.microphone(state);

    else if (component == "downward_spotlight")
        comp.spotlight(state);

    else if (component == "passive_rf")
        comp.rf(state);

    else if (component == "amber_beacon")
        comp.beacon(state);

    else if (component == "white_strobe")
        comp.strobe(state);

    else if (component == "smoke_marker")
        comp.smoke(state);
    else
    {
        fout << "Unknown component: " << component << endl;
    }
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
    else if (command == "RTL")
    {
        mymission.activateRTL();
        fout << "[" << getTimestamp() << "] COMMAND: RTL executed" << endl;
    }
    else if (command == "CHANGE_DEST")
    {
        double lat = commands["arguments"]["latitude"];
        double lon = commands["arguments"]["longitude"];

        mymission.setdestinationhomeOFF();
        mymission.setdestinationconfiguredON();

        mymission.mydrone.mydroneflight.setdestination(lat,lon);

        fout << "[" << getTimestamp()
            << "] COMMAND: CHANGE_DEST executed" << endl;
    }
    else if (command == "SET_SEARCH_LOCATIONS")
    {
        json searchPlan = json::object();
        json locArray = json::array();

        int id = 1;
        for (const auto& loc : commands["arguments"]["locations"])
        {
            locArray.push_back({
                {"id", id++},
                {"latitude", loc["latitude"]},
                {"longitude", loc["longitude"]}
            });
        }

        searchPlan["locations"] = locArray;

        ofstream out("runtime/search_locations.json");
        out << searchPlan.dump(4);
        out.close();

        mymission.setsearchlocations(searchPlan);

        fout << "[" << getTimestamp()
             << "] COMMAND: SET_SEARCH_LOCATIONS executed ("
             << locArray.size() << " locations)" << endl;
    }
    else if (command == "LAUNCH_DRONE")
    {
        mymission.setwaitingforhelpOFF();
        mymission.mydrone.mydroneflight.launch();
        fout << "[" << getTimestamp() << "] COMMAND: LAUNCH_DRONE executed" << endl;
    }
    else if (command == "CHANGE_MODE")
    {
           mymission.mydrone.mydroneflight.setmode(commands["arguments"]["value"]);
           fout << "[" << getTimestamp() << "] COMMAND: CHANGE_MODE executed" << endl;
    }
    else if (command == "CHANGE_ALTITUDE")
    {
        mymission.mydrone.mydroneflight.setaltitude(commands["arguments"]["value"]);
        fout << "[" << getTimestamp() << "] COMMAND: CHANGE_ALTITUDE executed" << endl;
    }
    else if (command == "STOP_FLIGHT")
    {
        mymission.mydrone.stopflight();
        fout << "[" << getTimestamp() << "] COMMAND: STOP_FLIGHT executed" << endl;
    }
    else if (command == "CHANGE_COMPONENT")
    {
        string component = commands["arguments"]["component"];
        bool enabled = commands["arguments"]["enabled"];

        changeComponent(
            mymission.mydrone.comp,
            component,
            enabled
        );

        fout << "[" << getTimestamp()
            << "] COMMAND: CHANGE_COMPONENT executed"
            << endl;
    }
    else if (command == "CHANGE_COMPONENTS")
    {
        bool enabled = commands["arguments"]["enabled"];

        for (const auto& component :
             commands["arguments"]["components"])
        {
            changeComponent(
                mymission.mydrone.comp,
                component.get<string>(),
                enabled
            );
        }

        fout << "[" << getTimestamp()
             << "] COMMAND: CHANGE_COMPONENTS executed"
             << endl;
    }
    else if (command == "RESCUEE_FOUND")
    {
        mymission.configurerescueestate();
        fout << "[" << getTimestamp() << "] COMMAND: RESCUEE_FOUND executed" << endl;
    }
    else if (command == "TRANSMIT_INFO")
    {
        mymission.mydrone.transmitinfo();
        fout << "[" << getTimestamp() << "] COMMAND: TRANSMIT_INFO executed" << endl;
    }
    else if (command == "RESET_MISSION")
    {
        mymission.resetRuntime();
        fout << "[" << getTimestamp() << "] COMMAND: RESET_MISSION executed" << endl;

    }
    lastProcessedCommandID = commandID;

    runtime["control"]["last_processed_command"] = lastProcessedCommandID;
    saveRuntime();
}
void resetCommands()
{
    commands = json::object();

    ofstream out("runtime/commands.json", ios::trunc);
    out << commands.dump(4);
    out.close();

    lastProcessedCommandID = 0;
}
// MAIN METHOD-------------------------------------
int main()
{
    ifstream activefile("configs/activeconfig.json");

    if (!activefile.is_open())
    {
        cout << "Failed to open activeconfig.json." << endl;
        return 1;
    }

    json active;
    activefile >> active;
    
    int slot = active["slot"];

    if (slot < 1 || slot > 3)
    {
        cout << "Invalid active configuration slot." << endl;
        return 1;
    }

    string slotFolder = "configs/slot" + to_string(slot) + "/";

    string configFile = slotFolder + "config.json";
    string batteryFile = slotFolder + "batterysavemodeconfig.json";
    ifstream configfile(configFile);

    if (!configfile.is_open())
    {
        cout << "Failed to open " << configFile << endl;
        return 1;
    }

    configfile >> config;

    if (config.contains("slot_name") && config["slot_name"].is_string())
    {
        cout << "Loaded Active Slot: " << config["slot_name"].get<string>() << " (Slot " << slot << ")" << endl;
    }


    ifstream batterysavemode(batteryFile);

    if (!batterysavemode.is_open())
    {
        cout << "Failed to open " << batteryFile << endl;
        return 1;
    }

    batterysavemode >> battery;

    resetCommands();

    setlocale(LC_ALL, ".UTF-8");

    mission mymission;

    mymission.resetRuntime();

    fout << "[" << getTimestamp() << "] Control Launched" << endl;
    cout<<"Control launched."<<endl;

    while (true)
    {
        checkcommands(mymission);
        mymission.mydrone.decreasebattery(0.25);
        mymission.batterysystem();
        mymission.missionstatusupdater();

        this_thread::sleep_for(chrono::milliseconds(500));
    }


    return 0;
}