#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

int main()
{
    // LOAD CONFIG
    ifstream in("config.json");
    json config;
    in >> config;
    in.close();

    // LOAD BATTERY SAVE MODE CONFIG
    ifstream batteryin("batterysavemodeconfig.json");
    json batterysavemode;
    batteryin >> batterysavemode;
    batteryin.close();


    string homeBaseName;


    cout << "Enter homebase name (default: "
         << config["home_base_name"].get<string>() << "): " << endl;
    getline(cin, homeBaseName);
    config["home_base_name"] = homeBaseName;


    cout << "Enter homebase latitude (default: "
         << config["home_base_latitude"] << "): " << endl;
    cin >> config["home_base_latitude"];


    cout << "Enter homebase longitude (default: "
         << config["home_base_longitude"] << "): " << endl;
    cin >> config["home_base_longitude"];


    cout << "Enter maximum altitude (default: "
         << config["maximum_altitude"] << "): " << endl;
    cin >> config["maximum_altitude"];


    cout << "Enter maximum speed (default: "
         << config["maximum_speed"] << "): " << endl;
    cin >> config["maximum_speed"];


    cout << "Enter launch altitude (default: "
         << config["launch_altitude"] << "): " << endl;
    cin >> config["launch_altitude"];


    cout << "Enter QUICK mode speed (default: "
         << config["quick_speed"] << "): " << endl;
    cin >> config["quick_speed"];


    cout << "Enter ACTIVE mode speed (default: "
         << config["active_speed"] << "): " << endl;
    cin >> config["active_speed"];


    cout << "Enter INSPECT mode speed (default: "
         << config["inspect_speed"] << "): " << endl;
    cin >> config["inspect_speed"];


    cout << "Enter HOVER mode speed (default: "
         << config["hover_speed"] << "): " << endl;
    cin >> config["hover_speed"];


    cout << "Enter home arrival radius (default: "
         << config["home_arrival_radius"] << "): " << endl;
    cin >> config["home_arrival_radius"];


    cout << "Enter battery warning percentage (default: "
         << config["battery_warning"] << "): " << endl;
    cin >> config["battery_warning"];


    cout << "Enter battery rtl warning percentage (default: "
         << config["battery_rtl_warning"] << "): " << endl;
    cin >> config["battery_rtl_warning"];


    cout << "Enter battery emergency rtl percentage (default: "
         << config["battery_emergency_rtl"] << "): " << endl;
    cin >> config["battery_emergency_rtl"];


    cout << "Enter battery emergency land percentage (default: "
         << config["battery_emergency_land"] << "): " << endl;
    cin >> config["battery_emergency_land"];


    cout << "Enter starting battery percentage (default: "
         << config["starting_battery_percent"] << "): " << endl;
    cin >> config["starting_battery_percent"];


    // BATTERY SAVE MODE CONFIGURATION
    cout << "\n----------------------------------" << endl;
    cout << "BATTERY SAVE MODE CONFIGURATION" << endl;
    cout << "1 = ON, 2 = OFF" << endl;
    cout << "----------------------------------" << endl;


    cout << "Thermal camera (default: "
         << batterysavemode["thermal_camera"] << "): " << endl;
    cin >> batterysavemode["thermal_camera"];


    cout << "RGB camera (default: "
         << batterysavemode["rgb_camera"] << "): " << endl;
    cin >> batterysavemode["rgb_camera"];


    cout << "Infrared camera (default: "
         << batterysavemode["infrared_camera"] << "): " << endl;
    cin >> batterysavemode["infrared_camera"];


    cout << "FMCW radar (default: "
         << batterysavemode["fmcw_radar"] << "): " << endl;
    cin >> batterysavemode["fmcw_radar"];


    cout << "Speaker (default: "
         << batterysavemode["speaker"] << "): " << endl;
    cin >> batterysavemode["speaker"];


    cout << "Microphone (default: "
         << batterysavemode["microphone"] << "): " << endl;
    cin >> batterysavemode["microphone"];


    cout << "Passive RF (default: "
         << batterysavemode["passive_rf"] << "): " << endl;
    cin >> batterysavemode["passive_rf"];


    cout << "Amber beacon (default: "
         << batterysavemode["amber_beacon"] << "): " << endl;
    cin >> batterysavemode["amber_beacon"];


    cout << "White strobe (default: "
         << batterysavemode["white_strobe"] << "): " << endl;
    cin >> batterysavemode["white_strobe"];


    cout << "Downward spotlight (default: "
         << batterysavemode["downward_spotlight"] << "): " << endl;
    cin >> batterysavemode["downward_spotlight"];


    cout << "Smoke marker (default: "
         << batterysavemode["smoke_marker"] << "): " << endl;
    cin >> batterysavemode["smoke_marker"];


    // SAVE NORMAL CONFIG
    ofstream out("config.json");
    out << config.dump(4);
    out.close();


    // SAVE BATTERY SAVE MODE CONFIG
    ofstream batteryout("batterysavemodeconfig.json");
    batteryout << batterysavemode.dump(4);
    batteryout.close();


    cout << "\nConfiguration saved." << endl;
    cout << "Battery save mode configuration saved." << endl;

    return 0;
}