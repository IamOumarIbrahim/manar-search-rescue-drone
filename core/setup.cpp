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

    cout << "Enter homebase name: " << endl;
    getline(cin, homeBaseName);
    config["home_base_name"] = homeBaseName;

    cout << "Enter homebase latitude: " << endl;
    cin >> config["home_base_latitude"];

    cout << "Enter homebase longitude: " << endl;
    cin >> config["home_base_longitude"];

    cout << "Enter maximum altitude: " << endl;
    cin >> config["maximum_altitude"];

    cout << "Enter maximum speed: " << endl;
    cin >> config["maximum_speed"];

    cout << "Enter launch altitude: " << endl;
    cin >> config["launch_altitude"];

    cout << "Enter QUICK mode speed: " << endl;
    cin >> config["quick_speed"];

    cout << "Enter ACTIVE mode speed: " << endl;
    cin >> config["active_speed"];

    cout << "Enter INSPECT mode speed: " << endl;
    cin >> config["inspect_speed"];

    cout << "Enter HOVER mode speed: " << endl;
    cin >> config["hover_speed"];

    cout << "Enter home arrival radius: " << endl;
    cin >> config["home_arrival_radius"];

    cout << "Enter battery warning percentage: " << endl;
    cin >> config["battery_warning"];

    cout << "Enter battery RTH warning percentage: " << endl;
    cin >> config["battery_rth_warning"];

    cout << "Enter battery emergency RTH percentage: " << endl;
    cin >> config["battery_emergency_rth"];

    cout << "Enter battery emergency land percentage: " << endl;
    cin >> config["battery_emergency_land"];

    cout << "Enter starting battery percentage: " << endl;
    cin >> config["starting_battery_percent"];


    ofstream out("config.json");
    out << config.dump(4);
    out.close();


    cout << "Configuration saved." << endl;

    return 0;
}