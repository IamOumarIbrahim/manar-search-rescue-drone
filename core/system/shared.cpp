#include "shared.hpp"

#include <chrono>
#include <ctime>
using namespace std;
json config;
json runtime;
json battery;

std::ofstream fout("runtime/logs.txt", std::ios::trunc);

std::string getTimestamp()
{
    auto now = std::chrono::system_clock::now();

    time_t currentTime =
        std::chrono::system_clock::to_time_t(now);

    tm* localTime = localtime(&currentTime);

    char buffer[20];

    strftime(
        buffer,
        sizeof(buffer),
        "%H:%M:%S",
        localTime
    );

    return buffer;
}

void saveRuntime()
{
    std::ofstream out("runtime/runtime.json");

    out << runtime.dump(4);

    out.close();
}

void logEvent(string subsystem, string level, string message)
{
    fout << "[" << getTimestamp() << "] "
         << "[" << subsystem << "] "
         << "[" << level << "] "
         << message << endl;
}