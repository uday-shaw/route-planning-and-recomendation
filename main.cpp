#include <queue>
#include <cmath>
#include "json.hpp"
using json = nlohmann::json;

class Graph {
protected:
    unordered_map<int, vector<Edge>> adjList;
    unordered_map<int, City> cities;
};
void InteractiveInterface::exportAsCSV() {
    cout << "📁 Exporting route as CSV... (stub)\n";
    // TODO: actual logic
}

void InteractiveInterface::exportAsJSON() {
    cout << "📁 Exporting route as JSON... (stub)\n";
    // TODO: actual logic
}

void InteractiveInterface::exportAsGPX() {
    cout << "📁 Exporting route as GPX... (stub)\n";
    // TODO: actual logic
}


struct Node {
    int id;
    int gCost; // Distance from start
    int hCost; // Heuristic distance to goal
    int fCost() const { return gCost + hCost; }
    int parent;
    
    bool operator>(const Node& other) const {
        return fCost() > other.fCost();
    }
};

class EnhancedGraph : public Graph {
private:
    // Calculate heuristic distance using coordinates
    int calculateHeuristic(int from, int to) const {
        const auto& fromCity = cities.at(from);
        const auto& toCity = cities.at(to);
        
        int dx = fromCity.x - toCity.x;
        int dy = fromCity.y - toCity.y;
        return static_cast<int>(std::sqrt(dx * dx + dy * dy) * 10); // Scale factor
    }

public:
    vector<int> aStarPathfinding(int start, int goal, vector<int>& parent, 
                                TimeOfDay timeOfDay, bool isWeekend = false) {
        priority_queue<Node, vector<Node>, greater<Node>> openSet;
        unordered_map<int, int> gScore;
        unordered_set<int> closedSet;
        
        // Initialize
        for (const auto& city : cities) {
            gScore[city.first] = numeric_limits<int>::max();
        }
        gScore[start] = 0;
        
        openSet.push({start, 0, calculateHeuristic(start, goal), -1});

        parent.assign(cities.size(), -1);
        
        while (!openSet.empty()) {
            Node current = openSet.top();
            openSet.pop();
            
            if (current.id == goal) {
                // Reconstruct path
                vector<int> distances(cities.size(), numeric_limits<int>::max());
                distances[goal] = current.gCost;
                return distances;
            }
            
            if (closedSet.count(current.id)) continue;
            closedSet.insert(current.id);
            
            // Check neighbors
            if (adjList.count(current.id)) {
                for (const auto& edge : adjList.at(current.id)) {
                    if (closedSet.count(edge.v)) continue;
                    
                    int totalWeight = edge.getTotalWeight(timeOfDay, isWeekend);
                    if (totalWeight == numeric_limits<int>::max()) continue;
                    
                    int tentativeGScore = gScore[current.id] + totalWeight;
                    
                    if (tentativeGScore < gScore[edge.v]) {
                        gScore[edge.v] = tentativeGScore;
                        parent[edge.v] = current.id;
                        
                        openSet.push({
                            edge.v,
                            tentativeGScore,
                            calculateHeuristic(edge.v, goal),
                            0,
                            current.id
                        });
                    }
                }
            }
        }
        
        return vector<int>(cities.size(), numeric_limits<int>::max());
    }
};
// Precomputed Distance Matrix
//Cache frequently used routes:
class RouteCache {
private:
    unordered_map<string, unordered_map<string, int>> cache;
    
    string generateKey(int start, int end, TimeOfDay time, bool weekend) const {
        return to_string(start) + "_" + to_string(end) + "_" + 
               to_string(static_cast<int>(time)) + "_" + to_string(weekend);
    }

public:
    bool hasRoute(int start, int end, TimeOfDay time, bool weekend) const {
        string key = generateKey(start, end, time, weekend);
        return cache.count(key) > 0;
    }
    
    int getDistance(int start, int end, TimeOfDay time, bool weekend) const {
        string key = generateKey(start, end, time, weekend);
        return cache.at(key).count("distance") ? cache.at(key).at("distance") : -1;
    }
    
    void cacheRoute(int start, int end, TimeOfDay time, bool weekend, int distance) {
        string key = generateKey(start, end, time, weekend);
        cache[key]["distance"] = distance;
    }
};
//2. Advanced Features
//Multi-Criteria Optimization
//Support different optimization goals:
enum class OptimizationGoal {
    SHORTEST_DISTANCE,
    FASTEST_TIME,
    LOWEST_COST,
    SAFEST_ROUTE,
    ECO_FRIENDLY
};

struct RoutePreferences {
    OptimizationGoal primaryGoal = OptimizationGoal::SHORTEST_DISTANCE;
    bool avoidTolls = false;
    bool avoidHighways = false;
    bool preferPublicTransport = false;
    int maxAccidentRisk = 80; // Percentage
    vector<int> avoidCities;
    vector<int> mustVisitCities;
};

class MultiCriteriaRouter {
private:
    int calculateWeightedCost(const Edge& edge, TimeOfDay timeOfDay, 
                             bool isWeekend, const RoutePreferences& prefs) const {
        int baseCost = edge.getTotalWeight(timeOfDay, isWeekend);
        
        switch (prefs.primaryGoal) {
            case OptimizationGoal::FASTEST_TIME:
                return baseCost - (edge.speedLimit / 10); // Favor higher speed limits
            
            case OptimizationGoal::LOWEST_COST:
                return baseCost + edge.toll;
            
            case OptimizationGoal::SAFEST_ROUTE:
                return baseCost + static_cast<int>(edge.accidentRisk * 100);
            
            case OptimizationGoal::ECO_FRIENDLY:
                if (edge.publicTransport) return baseCost - 20;
                if (edge.roadType == RoadType::Highway) return baseCost + 10;
                return baseCost;
            
            default:
                return baseCost;
        }
    }

public:
    vector<int> findOptimalRoute(int start, int end, TimeOfDay timeOfDay,
                                bool isWeekend, const RoutePreferences& prefs) {
        // Implementation with preference-based weights
        // ... (similar to A* but with weighted costs)
    }
};
//Real-Time Traffic Integration
//Add support for dynamic traffic updates:
class TrafficMonitor {
private:
    unordered_map<string, int> realTimeDelays; // edge_id -> delay_minutes
    chrono::system_clock::time_point lastUpdate;

public:
    void updateTrafficData(const string& edgeId, int delayMinutes) {
        realTimeDelays[edgeId] = delayMinutes;
        lastUpdate = chrono::system_clock::now();
    }
    
    int getCurrentDelay(int u, int v) const {
        string edgeId = to_string(u) + "_" + to_string(v);
        return realTimeDelays.count(edgeId) ? realTimeDelays.at(edgeId) : 0;
    }
    
    bool isDataFresh(chrono::minutes maxAge = chrono::minutes(15)) const {
        auto now = chrono::system_clock::now();
        return (now - lastUpdate) < maxAge;
    }
};
// 3. Robust Error Handling & Validation
// Input Validation Framework

class InputValidator {
public:
    struct ValidationResult {
        bool isValid;
        string errorMessage;
        vector<string> warnings;
    };
    
    static ValidationResult validateCity(const string& cityName, 
                                       const unordered_map<string, int>& nameToId) {
        ValidationResult result;
        
        if (cityName.empty()) {
            result.isValid = false;
            result.errorMessage = "City name cannot be empty";
            return result;
        }
        
        if (nameToId.find(cityName) == nameToId.end()) {
            result.isValid = false;
            result.errorMessage = "City '" + cityName + "' not found";
            
            // Suggest similar city names
            vector<string> suggestions = findSimilarCities(cityName, nameToId);
            if (!suggestions.empty()) {
                result.errorMessage += ". Did you mean: ";
                for (size_t i = 0; i < suggestions.size() && i < 3; ++i) {
                    result.errorMessage += suggestions[i];
                    if (i < suggestions.size() - 1) result.errorMessage += ", ";
                }
                result.errorMessage += "?";
            }
            return result;
        }
        
        result.isValid = true;
        return result;
    }
    
private:
    static vector<string> findSimilarCities(const string& input, 
                                           const unordered_map<string, int>& nameToId) {
        vector<pair<string, int>> similarities;
        
        for (const auto& pair : nameToId) {
            int distance = calculateLevenshteinDistance(input, pair.first);
            if (distance <= 2) { // Allow up to 2 character differences
                similarities.push_back({pair.first, distance});
            }
        }
        
        sort(similarities.begin(), similarities.end(), 
             [](const auto& a, const auto& b) { return a.second < b.second; });
        
        vector<string> result;
        for (const auto& sim : similarities) {
            result.push_back(sim.first);
        }
        return result;
    }
    
    static int calculateLevenshteinDistance(const string& s1, const string& s2) {
        // Implementation of Levenshtein distance algorithm
        vector<vector<int>> dp(s1.length() + 1, vector<int>(s2.length() + 1));
        
        for (int i = 0; i <= s1.length(); i++) dp[i][0] = i;
        for (int j = 0; j <= s2.length(); j++) dp[0][j] = j;
        
        for (int i = 1; i <= s1.length(); i++) {
            for (int j = 1; j <= s2.length(); j++) {
                if (s1[i-1] == s2[j-1]) {
                    dp[i][j] = dp[i-1][j-1];
                } else {
                    dp[i][j] = 1 + min({dp[i-1][j], dp[i][j-1], dp[i-1][j-1]});
                }
            }
        }
        
        return dp[s1.length()][s2.length()];
    }
};
// 4. Configuration Management
// Settings Framework
struct SystemSettings {
    // Performance settings
    bool useAStarAlgorithm = true;
    bool enableRouteCache = true;
    int maxCacheSize = 1000;
    
    // Display settings
    bool showDetailedOutput = true;
    bool colorCodeOutput = false;
    string distanceUnit = "km";
    string currencySymbol = "$";
    
    // Safety settings
    double maxAcceptableRisk = 0.8;
    bool avoidConstructionZones = false;
    
    // Load from JSON configuration file
    void loadFromFile(const string& configFile) {
        ifstream file(configFile);
        if (file.is_open()) {
            json config;
            file >> config;
            
            useAStarAlgorithm = config.value("useAStarAlgorithm", true);
            enableRouteCache = config.value("enableRouteCache", true);
            maxCacheSize = config.value("maxCacheSize", 1000);
            showDetailedOutput = config.value("showDetailedOutput", true);
            distanceUnit = config.value("distanceUnit", "km");
            currencySymbol = config.value("currencySymbol", "$");
            maxAcceptableRisk = config.value("maxAcceptableRisk", 0.8);
            avoidConstructionZones = config.value("avoidConstructionZones", false);
        }
    }
    
    void saveToFile(const string& configFile) const {
        json config;
        config["useAStarAlgorithm"] = useAStarAlgorithm;
        config["enableRouteCache"] = enableRouteCache;
        config["maxCacheSize"] = maxCacheSize;
        config["showDetailedOutput"] = showDetailedOutput;
        config["distanceUnit"] = distanceUnit;
        config["currencySymbol"] = currencySymbol;
        config["maxAcceptableRisk"] = maxAcceptableRisk;
        config["avoidConstructionZones"] = avoidConstructionZones;
        
        ofstream file(configFile);
        file << config.dump(4);
    }
};
// 5. Advanced User Interface
// Interactive Menu System
class InteractiveInterface {
private:
    SystemSettings settings;
    EnhancedGraph* graph;
    RouteCache cache;
    TrafficMonitor trafficMonitor;

public:
    void runInteractiveSession() {
        cout << "\n🚗 Advanced Route Planning System v2.0\n";
        cout << "======================================\n";
        
        while (true) {
            displayMainMenu();
            int choice = getUserChoice();
            
            switch (choice) {
                case 1: handleRouteSearch(); break;
                case 2: handleMultipleDestinations(); break;
                case 3: handleRouteComparison(); break;
                case 4: handleTrafficUpdate(); break;
                case 5: handleSettings(); break;
                case 6: handleDataExport(); break;
                case 7: displayStatistics(); break;
                case 0: 
                    cout << "Thank you for using the Route Planning System!\n";
                    return;
                default:
                    cout << "❌ Invalid option. Please try again.\n";
            }
        }
    }

private:
    void displayMainMenu() {
        cout << "\n📋 Main Menu:\n";
        cout << "1. 🔍 Find Route\n";
        cout << "2. 🗺️  Plan Multi-Destination Trip\n";
        cout << "3. ⚖️  Compare Routes\n";
        cout << "4. 🚦 Update Traffic Information\n";
        cout << "5. ⚙️  Settings\n";
        cout << "6. 📤 Export Route Data\n";
        cout << "7. 📊 View Statistics\n";
        cout << "0. 🚪 Exit\n";
        cout << "\nSelect option: ";
    }
    
    void handleRouteComparison() {
        cout << "\n⚖️ Route Comparison Tool\n";
        cout << "========================\n";
        
        string start, end, timeStr;
        cout << "Enter start city: "; cin >> start;
        cout << "Enter destination: "; cin >> end;
        cout << "Time of day (morning/afternoon/evening): "; cin >> timeStr;
        
        // Validate inputs
        auto startValidation = InputValidator::validateCity(start, graph->getNameToIdMap());
        auto endValidation = InputValidator::validateCity(end, graph->getNameToIdMap());
        
        if (!startValidation.isValid || !endValidation.isValid) {
            if (!startValidation.isValid) cout << "❌ " << startValidation.errorMessage << "\n";
            if (!endValidation.isValid) cout << "❌ " << endValidation.errorMessage << "\n";
            return;
        }
        
        TimeOfDay timeOfDay = graph->parseTime(timeStr);
        
        // Compare different optimization strategies
        vector<OptimizationGoal> goals = {
            OptimizationGoal::SHORTEST_DISTANCE,
            OptimizationGoal::FASTEST_TIME,
            OptimizationGoal::LOWEST_COST,
            OptimizationGoal::SAFEST_ROUTE
        };
        
        cout << "\n📊 Route Comparison Results:\n";
        cout << "┌─────────────────┬──────────┬──────────┬─────────┬─────────────┐\n";
        cout << "│ Optimization    │ Distance │ Time Est │ Cost    │ Safety Score│\n";
        cout << "├─────────────────┼──────────┼──────────┼─────────┼─────────────┤\n";
        
        for (auto goal : goals) {
            RoutePreferences prefs;
            prefs.primaryGoal = goal;
            
            // Calculate route with different preferences
            // Display comparison table
            string goalName = getGoalName(goal);
            cout << "│ " << left << setw(15) << goalName << " │ ";
            // ... format and display results
        }
        
        cout << "└─────────────────┴──────────┴──────────┴─────────┴─────────────┘\n";
    }
    
    void handleDataExport() {
        cout << "\n📤 Export Route Data\n";
        cout << "===================\n";
        cout << "1. Export as CSV\n";
        cout << "2. Export as JSON\n";
        cout << "3. Export as GPX (GPS format)\n";
        cout << "Select format: ";
        
        int format;
        cin >> format;
        
        switch (format) {
            case 1: exportAsCSV(); break;
            case 2: exportAsJSON(); break;
            case 3: exportAsGPX(); break;
            default: cout << "❌ Invalid format\n";
        }
    }
};
// 6. Memory Management & Performance
// Smart Pointers and RAII
class GraphManager {
private:
    unique_ptr<EnhancedGraph> graph;
    unique_ptr<RouteCache> cache;
    unique_ptr<TrafficMonitor> monitor;
    
public:
    GraphManager(int vertexCount) :
        graph(make_unique<EnhancedGraph>(vertexCount)),
        cache(make_unique<RouteCache>()),
        monitor(make_unique<TrafficMonitor>()) {}
    
    // Automatic cleanup through RAII
    ~GraphManager() = default;
    
    // Move semantics for efficiency
    GraphManager(GraphManager&&) = default;
    GraphManager& operator=(GraphManager&&) = default;
    
    // Delete copy operations to prevent expensive copying
    GraphManager(const GraphManager&) = delete;
    GraphManager& operator=(const GraphManager&) = delete;
};
// 7. Testing Framework
// Unit Tests
#ifdef TESTING_ENABLED
#include <cassert>

class RouteSystemTester {
public:
    static void runAllTests() {
        testBasicRouting();
        testTrafficFactors();
        testClosedRoads();
        testInputValidation();
        testCaching();
        cout << "✅ All tests passed!\n";
    }
    
private:
    static void testBasicRouting() {
        // Create test graph
        EnhancedGraph testGraph(5);
        // Add test cities and edges
        // Verify expected routes
        
        vector<int> parent;
        auto distances = testGraph.aStarPathfinding(0, 4, parent, TimeOfDay::Morning);
        assert(distances[4] != numeric_limits<int>::max());
        cout << "✓ Basic routing test passed\n";
    }
    
    static void testInputValidation() {
        unordered_map<string, int> testMap = {{"CityA", 0}, {"CityB", 1}};
        
        auto result = InputValidator::validateCity("CityA", testMap);
        assert(result.isValid);
        
        auto invalidResult = InputValidator::validateCity("InvalidCity", testMap);
        assert(!invalidResult.isValid);
        
        cout << "✓ Input validation test passed\n";
    }
};
#endif
