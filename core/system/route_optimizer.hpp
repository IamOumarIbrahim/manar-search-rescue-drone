#pragma once

#include <vector>

struct SearchLocation
{
    int input_id = 0;
    double latitude = 0.0;
    double longitude = 0.0;
};

struct OptimizedRoute
{
    std::vector<SearchLocation> locations;
    double totalDistance = 0.0;
};

double haversineDistanceMeters(const SearchLocation& loc1, const SearchLocation& loc2);

OptimizedRoute optimizeRouteGreedy(
    SearchLocation start,
    const std::vector<SearchLocation>& locations,
    SearchLocation home
);

double calculateRouteDistance(
    SearchLocation start,
    const std::vector<SearchLocation>& locations,
    SearchLocation home
);
