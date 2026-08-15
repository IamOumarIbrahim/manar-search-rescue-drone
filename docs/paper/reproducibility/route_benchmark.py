"""Fixed-seed reference evaluation for the MANAR route heuristic.

This script evaluates nearest-neighbor ordering followed by best-improvement
2-opt refinement.  The 2-opt result is a stronger local-search baseline, not a
global optimum.  Timing is deliberately omitted because this Python reference
does not measure the C++ implementation used by MANAR.
"""

from __future__ import annotations

import argparse
import csv
import math
import random
import statistics
import sys
from dataclasses import dataclass


EARTH_RADIUS_M = 6_371_000.0
DEFAULT_SEED = 20_260_815
DEFAULT_TRIALS = 100
TARGET_COUNTS = (5, 8, 10, 15, 20)


@dataclass(frozen=True)
class Point:
    latitude: float
    longitude: float


def haversine_m(a: Point, b: Point) -> float:
    lat_a = math.radians(a.latitude)
    lat_b = math.radians(b.latitude)
    delta_lat = math.radians(b.latitude - a.latitude)
    delta_lon = math.radians(b.longitude - a.longitude)
    chord = (
        math.sin(delta_lat / 2.0) ** 2
        + math.cos(lat_a)
        * math.cos(lat_b)
        * math.sin(delta_lon / 2.0) ** 2
    )
    return 2.0 * EARTH_RADIUS_M * math.atan2(
        math.sqrt(chord), math.sqrt(1.0 - chord)
    )


def closed_route_length_m(route: list[Point], home: Point) -> float:
    path = [home, *route, home]
    return sum(haversine_m(a, b) for a, b in zip(path, path[1:]))


def nearest_neighbor(points: list[Point], start: Point) -> list[Point]:
    unvisited = list(points)
    route: list[Point] = []
    current = start
    while unvisited:
        index = min(
            range(len(unvisited)),
            key=lambda candidate: haversine_m(current, unvisited[candidate]),
        )
        current = unvisited.pop(index)
        route.append(current)
    return route


def two_opt_best_improvement(route: list[Point], home: Point) -> tuple[list[Point], float]:
    current = list(route)
    current_length = closed_route_length_m(current, home)
    while True:
        best_route = current
        best_length = current_length
        for first in range(len(current) - 1):
            for last in range(first + 1, len(current)):
                candidate = (
                    current[:first]
                    + list(reversed(current[first : last + 1]))
                    + current[last + 1 :]
                )
                candidate_length = closed_route_length_m(candidate, home)
                if candidate_length < best_length - 1e-9:
                    best_route = candidate
                    best_length = candidate_length
        if best_length >= current_length - 1e-9:
            return current, current_length
        current = best_route
        current_length = best_length


def random_targets(
    rng: random.Random,
    count: int,
    center: Point,
    square_width_m: float,
) -> list[Point]:
    half_width = square_width_m / 2.0
    meters_per_degree_lat = 111_320.0
    meters_per_degree_lon = meters_per_degree_lat * math.cos(
        math.radians(center.latitude)
    )
    return [
        Point(
            center.latitude
            + rng.uniform(-half_width, half_width) / meters_per_degree_lat,
            center.longitude
            + rng.uniform(-half_width, half_width) / meters_per_degree_lon,
        )
        for _ in range(count)
    ]


def evaluate(seed: int, trials: int) -> list[dict[str, float | int]]:
    rng = random.Random(seed)
    home = Point(25.2048, 55.2708)
    rows: list[dict[str, float | int]] = []
    for count in TARGET_COUNTS:
        greedy_lengths: list[float] = []
        refined_lengths: list[float] = []
        excess_percentages: list[float] = []
        for _ in range(trials):
            targets = random_targets(rng, count, home, square_width_m=5_000.0)
            greedy_route = nearest_neighbor(targets, home)
            greedy_length = closed_route_length_m(greedy_route, home)
            _, refined_length = two_opt_best_improvement(greedy_route, home)
            greedy_lengths.append(greedy_length)
            refined_lengths.append(refined_length)
            excess_percentages.append((greedy_length / refined_length - 1.0) * 100.0)
        rows.append(
            {
                "targets": count,
                "trials": trials,
                "greedy_mean_m": statistics.mean(greedy_lengths),
                "two_opt_mean_m": statistics.mean(refined_lengths),
                "relative_excess_mean_pct": statistics.mean(excess_percentages),
                "relative_excess_sd_pct": statistics.stdev(excess_percentages),
                "relative_excess_max_pct": max(excess_percentages),
            }
        )
    return rows


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--seed", type=int, default=DEFAULT_SEED)
    parser.add_argument("--trials", type=int, default=DEFAULT_TRIALS)
    args = parser.parse_args()
    rows = evaluate(args.seed, args.trials)
    writer = csv.DictWriter(sys.stdout, fieldnames=list(rows[0].keys()))
    writer.writeheader()
    writer.writerows(rows)


if __name__ == "__main__":
    main()
