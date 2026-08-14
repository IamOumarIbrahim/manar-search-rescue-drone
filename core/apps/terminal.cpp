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
                        sendcommand("START_MISSION");
                        cout << "Start mission command sent." << endl;
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
                int User_Option = -1;
                int User_Option2 = -1;

                cout << "Select a payload option:\n"
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
                    << "Select an option (1-11): ";

                cin >> User_Option;

                cout << "----------------------------------" << endl;

                cout << "Select an option:\n"
                    << "1. TURN IT ON\n"
                    << "2. TURN IT OFF\n"
                    << "Select an option (1-2): ";

                cin >> User_Option2;

                switch (User_Option)
                {
                    case 1:
                    {
                        if (User_Option2 == 1)
                        {
                            sendcommand("CHANGE_COMPONENT", "thermal_camera", true);
                            cout<<"Change thermal camera request sent."<<endl;
                        }
                        else if (User_Option2 == 2)
                        {
                            sendcommand("CHANGE_COMPONENT", "thermal_camera", false);
                            cout<<"Change thermal camera request sent."<<endl;
                        }

                        break;
                    }

                    case 2:
                    {
                        if (User_Option2 == 1)
                        {
                            sendcommand("CHANGE_COMPONENT", "rgb_camera", true);
                            cout<<"Change RGB camera request sent."<<endl;
                        }
                        else if (User_Option2 == 2)
                        {
                            sendcommand("CHANGE_COMPONENT", "rgb_camera", false);
                            cout<<"Change RGB camera request sent."<<endl;
                        }

                        break;
                    }

                    case 3:
                    {
                        if (User_Option2 == 1)
                        {
                            sendcommand("CHANGE_COMPONENT", "infrared_camera", true);
                            cout<<"Change infrared camera request sent."<<endl;
                        }
                        else if (User_Option2 == 2)
                        {
                            sendcommand("CHANGE_COMPONENT", "infrared_camera", false);
                            cout<<"Change infrared camera request sent."<<endl;
                        }

                        break;
                    }

                    case 4:
                    {
                        if (User_Option2 == 1)
                        {
                            sendcommand("CHANGE_COMPONENT", "fmcw_radar", true);
                            cout<<"Change FMCW radar request sent."<<endl;
                        }
                        else if (User_Option2 == 2)
                        {
                            sendcommand("CHANGE_COMPONENT", "fmcw_radar", false);
                            cout<<"Change FMCW radar request sent."<<endl;
                        }

                        break;
                    }

                    case 5:
                    {
                        if (User_Option2 == 1)
                        {
                            sendcommand("CHANGE_COMPONENT", "speaker", true);
                            cout<<"Change speaker request sent."<<endl;
                        }
                        else if (User_Option2 == 2)
                        {
                            sendcommand("CHANGE_COMPONENT", "speaker", false);
                            cout<<"Change speaker request sent."<<endl;
                        }

                        break;
                    }

                    case 6:
                    {
                        if (User_Option2 == 1)
                        {
                            sendcommand("CHANGE_COMPONENT", "microphone", true);
                            cout<<"Change microphone request sent."<<endl;
                        }
                        else if (User_Option2 == 2)
                        {
                            sendcommand("CHANGE_COMPONENT", "microphone", false);
                            cout<<"Change microphone request sent."<<endl;
                        }

                        break;
                    }

                    case 7:
                    {
                        if (User_Option2 == 1)
                        {
                            sendcommand("CHANGE_COMPONENT", "passive_rf", true);
                            cout<<"Change passive RF request sent."<<endl;
                        }
                        else if (User_Option2 == 2)
                        {
                            sendcommand("CHANGE_COMPONENT", "passive_rf", false);
                            cout<<"Change passive RF request sent."<<endl;
                        }

                        break;
                    }

                    case 8:
                    {
                        if (User_Option2 == 1)
                        {
                            sendcommand("CHANGE_COMPONENT", "amber_beacon", true);
                            cout<<"Change amber beacon request sent."<<endl;
                        }
                        else if (User_Option2 == 2)
                        {
                            sendcommand("CHANGE_COMPONENT", "amber_beacon", false);
                            cout<<"Change amber beacon request sent."<<endl;
                        }

                        break;
                    }

                    case 9:
                    {
                        if (User_Option2 == 1)
                        {
                            sendcommand("CHANGE_COMPONENT", "white_strobe", true);
                            cout<<"Change white strobe request sent."<<endl;
                        }
                        else if (User_Option2 == 2)
                        {
                            sendcommand("CHANGE_COMPONENT", "white_strobe", false);
                            cout<<"Change white strobe request sent."<<endl;
                        }

                        break;
                    }

                    case 10:
                    {
                        if (User_Option2 == 1)
                        {
                            sendcommand("CHANGE_COMPONENT", "downward_spotlight", true);
                            cout<<"Change downward spotlight request sent."<<endl;
                        }
                        else if (User_Option2 == 2)
                        {
                            sendcommand("CHANGE_COMPONENT", "downward_spotlight", false);
                            cout<<"Change downward spotlight request sent."<<endl;
                        }

                        break;
                    }

                    case 11:
                    {
                        if (User_Option2 == 1)
                        {
                            sendcommand("CHANGE_COMPONENT", "smoke_marker", true);
                            cout<<"Change smoke marker request sent."<<endl;
                        }
                        else if (User_Option2 == 2)
                        {
                            sendcommand("CHANGE_COMPONENT", "smoke_marker", false);
                            cout<<"Change smoke marker request sent."<<endl;
                        }

                        break;
                    }

                    default:
                    {
                        cout << "Invalid payload option." << endl;
                        break;
                    }
                }

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