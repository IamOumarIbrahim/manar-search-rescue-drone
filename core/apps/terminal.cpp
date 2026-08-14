// LIBRARIES 
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
    #include <sstream>
    #include <cctype>
    #include <vector>
    #include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;
ofstream fout("runtime/logs.txt");
json commands;
json runtime;
json config;
bool return_main = true;
int commandID = 0;

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

bool parseComponentSelection(string input, vector<int>& selected)
{
    stringstream ss(input);
    string token;

    bool alreadySelected[12] = {false};

    while (getline(ss, token, ','))
    {
        try
        {
            size_t dash = token.find('-');

            if (dash != string::npos)
            {
                int start = stoi(token.substr(0, dash));
                int end = stoi(token.substr(dash + 1));

                if (start < 1 || end > 11 || start > end)
                    return false;

                for (int i = start; i <= end; i++)
                {
                    if (!alreadySelected[i])
                    {
                        selected.push_back(i);
                        alreadySelected[i] = true;
                    }
                }
            }
            else
            {
                int option = stoi(token);

                if (option < 1 || option > 11)
                    return false;

                if (!alreadySelected[option])
                {
                    selected.push_back(option);
                    alreadySelected[option] = true;
                }
            }
        }
        catch (...)
        {
            return false;
        }
    }

    return !selected.empty();
}
void loadlastcommandid() // DONT TOUCH
{
    ifstream in("runtime/commands.json");

    if (in.is_open())
    {
        json oldcommands;

        try
        {
            in >> oldcommands;

            if (oldcommands.contains("id"))
            {
                commandID = oldcommands["id"];
            }
        }
        catch (...)
        {
            commandID = 0;
        }

        in.close();
    }
}
void savecommands() // DONT TOUCH
{
    ofstream out("runtime/commands.json");
    out << commands.dump(4);
    out.close();
}

void sendcommand(string command, string component, bool enabled)
{
    commands.clear();
    commandID++;

    commands["id"] = commandID;
    commands["command"] = command;

    commands["arguments"]["component"] = component;
    commands["arguments"]["enabled"] = enabled;

    savecommands();
}
void sendcommand(
    string command,
    vector<string> components,
    bool enabled
)
{
    commands.clear();
    commandID++;

    commands["id"] = commandID;
    commands["command"] = command;

    commands["arguments"]["components"] = components;
    commands["arguments"]["enabled"] = enabled;

    savecommands();
}
void sendcommand(string command, double value)
{
    commands.clear();
    commandID++;

    commands["id"] = commandID;
    commands["command"] = command;
    commands["arguments"]["value"] = value;

    savecommands();
}
void sendcommand(string command, double lat, double lon)
{
    commands.clear();
    commandID++;

    commands["id"] = commandID;
    commands["command"] = command;

    commands["arguments"]["latitude"] = lat;
    commands["arguments"]["longitude"] = lon;

    savecommands();
}
void sendcommand(string command) // DONT TOUCH
{
    commands.clear();
    commandID++;

    commands["id"] = commandID;
    commands["command"] = command;

    savecommands();
}


void readruntime() // DONT TOUCH
{
    ifstream in("runtime/runtime.json");

    if (in.is_open())
    {
        in >> runtime;
        in.close();
    }
}


int main()
{
    ifstream activefile("configs/activeconfig.json");
    json active;
    activefile >> active;
    activefile.close();

    int slot = active["slot"];

    if (slot < 1 || slot > 3)
    {
        cout << "Invalid active configuration slot." << endl;
        return 1;
    }

    string configFile =
        "configs/slot" + to_string(slot) + "/config.json";

    ifstream configfile(configFile);
    configfile >> config;
    configfile.close();

    setlocale(LC_ALL, ".UTF-8");

    int User_Option3 = -1;
    loadlastcommandid();

    while (return_main == true)
    {
        this_thread::sleep_for(chrono::seconds(1));
        cout << "----------------------------------" << endl;
        cout << "WELCOME TO MANAR HUMAN CONTROL SYSTEM" << endl;
        if (config.contains("slot_name") && config["slot_name"].is_string())
        {
            cout << "ACTIVE SLOT: " << config["slot_name"].get<string>() << " (Slot " << slot << ")" << endl;
        }
        cout << "----------------------------------" << endl;

        cout << "- 1. DISPLAY RUNTIME STATUS" << endl;
        cout << "- 2. START MISSION" << endl;
        cout << "- 3. SET DESTINATION" << endl;
        cout << "- 4. LAUNCH DRONE" << endl;
        cout << "- 5. CONFIGURE FLIGHT OPTIONS" << endl;
        cout << "- 6. CONFIGURE COMPONENTS" << endl;
        cout << "- 7. FOUND RESCUEE?" << endl;
        cout << "- 8. TRANSMIT LOCATION" << endl;
        cout << "----------------------------------" << endl;

        cout << "- 0. ACTIVATE RTL" << endl;
        cout << "- 10. EXIT TERMINAL" << endl;

        cout << "----------------------------------" << endl;
        cout << "Select an option: ";
        cin >> User_Option3;


        switch (User_Option3)
        {
            case 0:
            {
                string confirmation;

                cout << "Activate RTL? (Y/N): ";
                cin >> confirmation;

                if ((confirmation == "Y") || (confirmation == "y"))
                {
                    sendcommand("RTL");
                    cout << "RTL command sent." << endl;
                }

                break;
            }


            case 1:
            {
                readruntime();

                cout << runtime.dump(4) << endl;

                break;
            }

            case 2:
            {
                readruntime();

                if (runtime["mission"]["started"] == true)
                {
                    string confirmation;

                    cout << "A mission is already active." << endl;
                    cout << "Starting a new mission will reset current mission state." << endl;
                    cout << "Continue? (Y/N): ";

                    cin >> confirmation;

                    if (confirmation == "Y" || confirmation == "y")
                    {
                        sendcommand("RESET_MISSION");
                        cout << "Reset mission command sent." << endl;
                    }
                    else
                    {
                        cout << "Start mission cancelled." << endl;
                    }
                }
                else
                {
                    sendcommand("START_MISSION");
                    cout << "Start mission command sent." << endl;
                }

                break;
            }
            case 3:
            {
                string coordinateinput;
                double destlatset, destlongset;

                cout << "Enter destination coordinates:\n";
                cout << "Examples:\n";
                cout << "25.336421, 55.344471\n";
                cout << "25°13'05.40\"N 55°27'09.67\"E\n";

                getline(cin >> ws, coordinateinput);

                if (parseCoordinates(coordinateinput, destlatset, destlongset))
                {
                    sendcommand("CHANGE_DEST",destlatset,destlongset);
                    cout<<"Change destination request sent."<<endl;

                }
                else
                {
                    cout << "Invalid coordinate format." << endl;
                }

                break;
            }
            case 4:
            {
                sendcommand("LAUNCH_DRONE");
                cout<<"Launch drone request sent."<<endl;
                break;
            }
            case 5:
            {
                int User_Option = -1;
                int User_Option2 = -1;
                
                double setaltitude = 0;
                cout<<"1. Set mode\n"<<  "2. Set altitude\n"<< "0. Stop flight\n";
                cin>>User_Option;
                switch (User_Option)
                {
                    case 1:
                        {
                            cout<<"----------------------------------"<<endl;
                            cout<<"1. Quick\n"<<"2. Active\n"<<"3. Inspect\n"<<"4. Hover\n";
                            cin>>User_Option2;

                            if (User_Option2 == 1)
                            {
                                sendcommand("CHANGE_MODE", 1);
                                cout<<"Change mode request sent."<<endl;
                            }
                            else if (User_Option2 == 2)
                            {
                                sendcommand("CHANGE_MODE", 2);
                                cout<<"Change mode request sent."<<endl;
                            }
                            else if (User_Option2 == 3)
                            {
                                sendcommand("CHANGE_MODE", 3);
                                cout<<"Change mode request sent."<<endl;
                            }
                            else if (User_Option2 == 4)
                            {
                                sendcommand("CHANGE_MODE", 4);
                                cout<<"Change mode request sent."<<endl;
                            }
                            else
                            {
                                cout<<"Invalid flight mode option."<<endl;
                            }
                            break;
                        }
                    case 2:
                        {
                            cout<<"Enter altitude between 0-"<<config["maximum_altitude"]<<"m"<<endl;
                            cin>>setaltitude;
                            sendcommand("CHANGE_ALTITUDE",setaltitude);
                            cout<<"Change altitude request sent."<<endl;
                            break;
                        }
                    case 0:
                        {
                            sendcommand("STOP_FLIGHT");
                            cout<<"Stop flight request sent."<<endl;
                            break;
                        }

                }
                break;
            }
            case 6:
            {
                string input;
                int User_Option2 = -1;

                vector<int> selected;
                vector<string> selectedComponents;

                vector<string> componentNames = {
                    "",
                    "thermal_camera",
                    "rgb_camera",
                    "infrared_camera",
                    "fmcw_radar",
                    "speaker",
                    "microphone",
                    "passive_rf",
                    "amber_beacon",
                    "white_strobe",
                    "downward_spotlight",
                    "smoke_marker"
                };

                cout << "Select payload components:\n"
                     << "1. Thermal\n"
                     << "2. RGB\n"
                     << "3. Infrared\n"
                     << "4. FMCW\n"
                     << "5. Speaker\n"
                     << "6. Mic\n"
                     << "7. RF\n"
                     << "8. Beacon\n"
                     << "9. Strobe\n"
                     << "10. Spotlight\n"
                     << "11. Smoke\n\n"
                     << "Examples: 1-5,9 or 1,3,7\n"
                     << "Select components: ";

                cin >> input;

                if (!parseComponentSelection(input, selected))
                {
                    cout << "Invalid component selection." << endl;
                    break;
                }

                cout << "----------------------------------" << endl;

                cout << "Select state:\n"
                     << "1. TURN ON\n"
                     << "2. TURN OFF\n"
                     << "Select an option (1-2): ";

                cin >> User_Option2;

                if (User_Option2 != 1 && User_Option2 != 2)
                {
                    cout << "Invalid component state." << endl;
                    break;
                }

                for (int option : selected)
                {
                    selectedComponents.push_back(
                        componentNames[option]
                    );
                }

                bool enabled = (User_Option2 == 1);

                sendcommand(
                    "CHANGE_COMPONENTS",
                    selectedComponents,
                    enabled
                );

                cout << "Component change request sent." << endl;

                break;
            }
            case 7:
            {
                sendcommand("RESCUEE_FOUND");
                cout<<"Rescuee found request sent"<<endl;
                break;
            }
            case 8:
            {
                sendcommand("TRANSMIT_INFO");
                cout<<"Transmit info request sent"<<endl;
                break;
            }
            case 10:
            {
                cout << "Exiting terminal." << endl;

                return 0;
            }
            default:
            {
                cout << "Invalid option." << endl;

                break;
            }
        }
    }

    return 0;
}