#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

int main()
{
    ifstream in("config.json");
    json config;
    in >> config;
    in.close();

    string homeBaseName;


    cout << "Enter homebase name (default: "<< config["home_base_name"].get<string>()<< "): " << endl;
    getline(cin, homeBaseName);
    config["home_base_name"] = homeBaseName;


    cout << "Enter homebase latitude (default: " << config["home_base_latitude"] << "): " << endl;
    cin >> config["home_base_latitude"];


    cout << "Enter homebase longitude (default: " << config["home_base_longitude"] << "): " << endl;
    cin >> config["home_base_longitude"];


    cout << "Enter maximum altitude (default: " << config["maximum_altitude"] << "): " << endl;
    cin >> config["maximum_altitude"];


    cout << "Enter maximum speed (default: " << config["maximum_speed"] << "): " << endl;
    cin >> config["maximum_speed"];


    cout << "Enter launch altitude (default: " << config["launch_altitude"] << "): " << endl;
    cin >> config["launch_altitude"];


    cout << "Enter QUICK mode speed (default: " << config["quick_speed"] << "): " << endl;
    cin >> config["quick_speed"];


    cout << "Enter ACTIVE mode speed (default: " << config["active_speed"] << "): " << endl;
    cin >> config["active_speed"];


    cout << "Enter INSPECT mode speed (default: " << config["inspect_speed"] << "): " << endl;
    cin >> config["inspect_speed"];


    cout << "Enter HOVER mode speed (default: " << config["hover_speed"] << "): " << endl;
    cin >> config["hover_speed"];


    cout << "Enter home arrival radius (default: " << config["home_arrival_radius"] << "): " << endl;
    cin >> config["home_arrival_radius"];


    cout << "Enter battery warning percentage (default: " << config["battery_warning"] << "): " << endl;
    cin >> config["battery_warning"];


    cout << "Enter battery RTH warning percentage (default: " << config["battery_rth_warning"] << "): " << endl;
    cin >> config["battery_rth_warning"];


    cout << "Enter battery emergency RTH percentage (default: " << config["battery_emergency_rth"] << "): " << endl;
    cin >> config["battery_emergency_rth"];


    cout << "Enter battery emergency land percentage (default: " << config["battery_emergency_land"] << "): " << endl;
    cin >> config["battery_emergency_land"];


    cout << "Enter starting battery percentage (default: " << config["starting_battery_percent"] << "): " << endl;
    cin >> config["starting_battery_percent"];


    ofstream out("config.json");
    out << config.dump(4);
    out.close();


    cout << "Configuration saved." << endl;

    return 0;
}