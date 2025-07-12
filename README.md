# route-planning-and-recomendation
# Route Planning and Recommendation System

An advanced C++ route planning system that supports A\* and Bellman-Ford algorithms, traffic analysis, user preferences, and dynamic JSON-based configuration.

## Features

* **City and Road Network**: Modeled via cities and directed/undirected roads.
* **Routing Algorithms**: Includes A\* Search and Enhanced Bellman-Ford.
* **Multi-Criteria Optimization**: Supports fastest, shortest, eco-friendly, safest, and cheapest routes.
* **Traffic & Weather**: Dynamic weights based on traffic, time of day, and weather.
* **Weekend and Construction Adjustments**
* **Public Transport Preferences**
* **JSON Config & Input Support**
* **Interactive CLI Menu**: Find route, compare routes, update traffic, and more.
* **Input Validation** with Levenshtein-based city name suggestions.
* **Cache**: Route caching for repeated queries.

## Getting Started

### Requirements

* C++17 or above
* nlohmann/json (included as header-only)

### Build

```
g++ -std=c++17 main.cpp -o planner
```

### Run

```
./planner
```

## File Structure

```
- main.cpp                 # Entry point and UI logic
- graph.hpp               # Graph classes and routing logic
- config.json             # System configuration
- complex_cities_graph_fully_extended.json  # Input graph data
```

## Configuration

Edit `config.json` to change system behavior (algorithm choice, risk levels, output formatting).

## Input Graph Format

Refer to `complex_cities_graph_fully_extended.json` for how to structure cities and edges.

## Example Usage

* Start the program
* Select "Find Route"
* Input start & destination cities, time of day, and weekend flag
* Get optimal route with detailed breakdown

## License

This project is open-source and free to use.




note- this was the group project earlier for dsa course but after that i have tried some improvements on my own
