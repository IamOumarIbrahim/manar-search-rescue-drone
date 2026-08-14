#include "route_optimizer.hpp"
#include <cmath>
#include <limits>

const double EARTH_RADIUS_METERS = 6371000.0;
const double PI = 3.14159265358979323846;

double haversineDistanceMeters(const SearchLocation& loc1, const SearchLocation& loc2)
{
    double lat1Rad = loc1.latitude * PI / 180.0;
    double lat2Rad = loc2.latitude * PI / 180.0;
    double dLat = (loc2.latitude - loc1.latitude) * PI / 180.0;
    double dLon = (loc2.longitude - loc1.longitude) * PI / 180.0;

    double a = std::sin(dLat / 2.0) * std::sin(dLat / 2.0) +
               std::cos(lat1Rad) * std::cos(lat2Rad) *
               std::sin(dLon / 2.0) * std::sin(dLon / 2.0);

    double c = 2.0 * std::atan2(std::sqrt(a), std::sqrt(1.0 - a));

    return EARTH_RADIUS_METERS * c;
}

OptimizedRoute optimizeRouteGreedy(
    SearchLocation start,
    const std::vector<SearchLocation>& locations,
    SearchLocation home
)
{
    OptimizedRoute result;
    if (locations.empty())
    {
        result.totalDistance = 0.0;
        return result;
    }

    std::vector<SearchLocation> unvisited = locations;
    SearchLocation current = start;
    double accumulatedDistance = 0.0;

    while (!unvisited.empty())
    {
        size_t nearestIndex = 0;
        double minDistance = std::numeric_limits<double>::max();

        for (size_t i = 0; i < unvisited.size(); ++i)
        {
            double dist = haversineDistanceMeters(current, unvisited[i]);
            if (dist < minDistance)
            {
                minDistance = dist;
                nearestIndex = i;
            }
        }

        accumulatedDistance += minDistance;
        current = unvisited[nearestIndex];
        result.locations.push_back(current);

        unvisited.erase(unvisited.begin() + nearestIndex);
    }

    // Add distance from final search location to home
    accumulatedDistance += haversineDistanceMeters(current, home);

    result.totalDistance = accumulatedDistance;
    return result;
}

double calculateRouteDistance(
    SearchLocation start,
    const std::vector<SearchLocation>& locations,
    SearchLocation home
)
{
    if (locations.empty())
    {
        return 0.0;
    }

    SearchLocation current = start;
    double accumulatedDistance = 0.0;

    for (const auto& loc : locations)
    {
        accumulatedDistance += haversineDistanceMeters(current, loc);
        current = loc;
    }

    accumulatedDistance += haversineDistanceMeters(current, home);
    return accumulatedDistance;
}
