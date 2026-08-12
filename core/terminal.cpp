#include <iostream>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

json commands;
json runtime;

bool return_main = true;
int commandID = 0;


void loadlastcommandid()
{
    ifstream in("commands.json");

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
void savecommands()
{
    ofstream out("commands.json");
    out << commands.dump(4);
    out.close();
}


void sendcommand(string command)
{
    commandID++;

    commands["id"] = commandID;
    commands["command"] = command;

    savecommands();
}


void readruntime()
{
    ifstream in("runtime.json");

    if (in.is_open())
    {
        in >> runtime;
        in.close();
    }
}


int main()
{
    int User_Option = -1;
    loadlastcommandid();
    while (return_main == true)
    {
        cout << "----------------------------------" << endl;
        cout << "WELCOME TO MANAR HUMAN CONTROL SYSTEM" << endl;
        cout << "----------------------------------" << endl;

        cout << "- 1. DISPLAY RUNTIME STATUS" << endl;
        cout << "- 2. START MISSION" << endl;
        cout << "- 0. ACTIVATE RTH" << endl;
        cout << "- 10. EXIT TERMINAL" << endl;

        cout << "----------------------------------" << endl;

        cin >> User_Option;


        switch (User_Option)
        {
            case 0:
            {
                string confirmation;

                cout << "Activate RTH? (Y/N): ";
                cin >> confirmation;

                if ((confirmation == "Y") || (confirmation == "y"))
                {
                    sendcommand("RTH");
                    cout << "RTH command sent." << endl;
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
                sendcommand("START_MISSION");

                cout << "Start mission command sent." << endl;

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