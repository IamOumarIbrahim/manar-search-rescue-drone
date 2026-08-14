#pragma once

#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

extern json config;
extern json runtime;
extern json battery;

extern std::ofstream fout;

void saveRuntime();
std::string getTimestamp();

void logEvent(std::string subsystem,
              std::string level,
              std::string message);
              