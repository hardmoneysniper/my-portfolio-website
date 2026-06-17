// Project Identifier: 5949F553E20B650AB0FB2266D3C0822B13D248B0
#include <iomanip>
#include <iostream>
#include <getopt.h>
#include <vector>
#include <cmath>
#include <queue>
#include <algorithm>
#include <numeric>

using namespace std;

// --------------------------------------------------------------------------------------
// from genPerms.cpp
// template <typename T>
// void genPerms(vector<T> &path, size_t permLength) {
//   if (permLength == path.size()) {
//   // Do something with the path
//     return;
//   }  // if ..complete path

//   if (!promising(path, permLength)) {
//     return;
//   }  // if ..not promising

//   for (size_t i = permLength; i < path.size(); ++i) {
//     swap(path[permLength], path[i]);
//     genPerms(path, permLength + 1);
//     swap(path[permLength], path[i]);
//   }  // for ..unpermuted elements
// };  // genPerms()
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// Part A Related Implementations
struct Edge {
    int from;
    int to;
    double weight;

    Edge(int from, int to, double weight) : from(from), to(to), weight(weight) {} 
};

bool operator<(const Edge& lhs, const Edge& rhs) {
    return lhs.weight < rhs.weight;
}

struct LocationPA {
    int x;
    int y;
    char area; // 'L' for land, 'S' for sea, 'C' for coastlines

    LocationPA(int x_coord, int y_coord, char area_type)
        : x(x_coord), y(y_coord), area(area_type) {}

    double squaredDistanceTo(const LocationPA& other) const {
        double dx = static_cast<double>(x) - static_cast<double>(other.x);
        double dy = static_cast<double>(y) - static_cast<double>(other.y);
        return dx * dx + dy * dy;
    }
};

class DistanceCalculator {
public:
    double operator()(const LocationPA& loc1, const LocationPA& loc2) const {
        if (isValidConnection(loc1, loc2)) {
            return loc1.squaredDistanceTo(loc2); // Return squared distance, no need for sqrt
        } else {
            return numeric_limits<double>::infinity(); // Return "infinity" for invalid connections
        }
    }

private:
    bool isValidConnection(const LocationPA& loc1, const LocationPA& loc2) const {
        // if (loc1.area == loc2.area) {
        //     return true;  // Allow connection within the same area
        // } else if ((loc1.area == 'L' && loc2.area == 'C') || (loc1.area == 'C' && loc2.area == 'L') 
        //         || (loc1.area == 'S' && loc2.area == 'C') || (loc1.area == 'C' && loc2.area == 'S')) {
        //     return true;  // Allow connection from land to coastline
        // } else {
        //     return false;  // Invalid connection between land and sea without coastline
        // }
        if (loc1.area == loc2.area) {
            return true; // Same area
        }
        if (loc1.area == 'C' || loc2.area == 'C') {
            return true; // Connection involving coastline
        }
        return false; // Invalid connection
    }
};

class PartA_MST {
public:
    PartA_MST(const vector<LocationPA>& locations, const DistanceCalculator& distanceCalculator)
        : locations(locations), distanceCalculator(distanceCalculator) {}

    void calculateMST() {
        primMST();
    }

private:
    const vector<LocationPA>& locations;
    const DistanceCalculator& distanceCalculator;
    int n = static_cast<int>(locations.size());

    struct VertexInfo {
        bool k; // has the vertex been visited
        double d; // minimal edge weight to this vertex
        int p; // parent vertex

        VertexInfo() : k(false), d(numeric_limits<double>::infinity()), p(-1) {}
    };

    void primMST() {
        vector<VertexInfo> vertexInfo(n);

        vertexInfo[0].d = 0.0;

        double totalWeight = 0.0;
        vector<Edge> mstEdges;

        for (int count = 0; count < n; ++count) {
            int u = 1;
            double minDist = numeric_limits<double>::infinity();
            
            for (int i = 0; i < n; ++i) {
                if (!vertexInfo[i].k && vertexInfo[i].d < minDist) {
                    minDist = vertexInfo[i].d;
                    u = i;
                }
            }

            if (u == -1) {
                cerr << "Cannot construct MST\n";
                exit(1);
            }

            vertexInfo[u].k = true;
            totalWeight += sqrt(minDist);

            if (vertexInfo[u].p != -1) {
                mstEdges.emplace_back(vertexInfo[u].p, u, minDist);
            }

            for (int v = 0; v < n; ++v) {
                if (!vertexInfo[v].k) {
                    double weight = distanceCalculator(locations[u], locations[v]);
                    if (weight < vertexInfo[v].d) {
                        vertexInfo[v].d = weight;
                        vertexInfo[v].p = u;
                    }
                }
            }
        }

        if (static_cast<int>(mstEdges.size()) != n - 1) {
            cerr << "Cannot construct MST\n";
            exit(1);
        }

        printMST(totalWeight, mstEdges);
    }

    void printMST(double totalWeight, const vector<Edge>& edges) {
        cout << totalWeight << "\n";
        for (const auto& edge : edges) {
            cout << min(edge.from, edge.to) << " " << max(edge.from, edge.to) << "\n";
        }
    }
};

// --------------------------------------------------------------------------------------
// Part B Related Implementations

struct LocationPBC {
    int x;
    int y;

    LocationPBC(int x_coord, int y_coord)
        : x(x_coord), y(y_coord) {}

    double distanceTo(const LocationPBC& other) const {
        double dx = static_cast<double>(x) - static_cast<double>(other.x);
        double dy = static_cast<double>(y) - static_cast<double>(other.y);
        
        return sqrt(dx * dx + dy * dy);
    }
};

class PartB_FastTSP {
public: 
    PartB_FastTSP(const vector<LocationPBC>& locations) : locations(locations), n(static_cast<int>(locations.size())) {}

    void solveTSP() {
        vector<int> tour = furthestInsertionHeuristic();
        printTour(tour);
    }

    vector<int> PartC_Access_FIH() {
        return furthestInsertionHeuristic();
    }

    double getLength() {
        return totalTripLength;
    }

private: 
    const vector<LocationPBC>& locations;
    int n; // size
    double totalTripLength = 0;

    vector<int> furthestInsertionHeuristic() {
        // Step 1. Start with a sub-graph consisting of node i only.
        // Step 2. Find node r such that cir is maximal and form sub-tour i-r-i.
        // Step 3. (Selection step) Given a sub-tour, find node r not in the sub-tour farthest from any node in the sub-tour; i.e. with maximal crj
        // Step 4. (Insertion step) Find the arc (i, j) in the sub-tour which minimizes cir + crj - cij . Insert r between i and j.
        // Step 5. If all the nodes are added to the tour, stop. Else go to step 3

        vector<int> tour;
        vector<bool> visited(n, false);
        vector<double> minDistToTour(n, numeric_limits<double>::infinity());

        // step 1: start with the first vertex and its nearest neighbor
        int start = 0;
        visited[start] = true;

        // Step 2: Find the farthest node from the start node
        int furthest = -1;
        double maxDist = -1.0;

        for (int i = 1; i < n; ++i) {
            double dist = locations[start].distanceTo(locations[i]);
            if (dist > maxDist) {
                maxDist = dist;
                furthest = i;
            }
            minDistToTour[i] = dist;
        }

        visited[furthest] = true;
        tour.push_back(start);
        tour.push_back(furthest);
        tour.push_back(start); // close the sub-tour

        // cout << totalTripLength << "\n";
        totalTripLength += 2 * maxDist;
        // cout << totalTripLength << "\n";

        for (int i = 0; i < n; ++i) {
            if (!visited[i]) {
                // dist from unvisited nodes to the furthest node
                double dist = locations[furthest].distanceTo(locations[i]); 
                if (dist < minDistToTour[i]) {
                    minDistToTour[i] = dist;
                }
            }
        }

        // step 3: Repeat until all nodes are added
        while (static_cast<int>(tour.size()) - 1 < n) {
            int nextNode = -1;
            double maxDistToTour = -1.0;

            // (Selection step) Find the node farthest from any node in the sub-tour
            for (int i = 0; i < n; ++i) {
                if (!visited[i]) {
                    if (minDistToTour[i] > maxDistToTour) {
                        maxDistToTour = minDistToTour[i];
                        nextNode = i;
                    }
                }
            }

            visited[nextNode] = true;

            // (Insertion step) Find the best place to insert the new node
            int bestInsertPos = -1;
            double bestCostIncrease = numeric_limits<double>::infinity();

            for (size_t i = 0; i < tour.size() - 1; ++i) {
                int current = tour[i];
                int next = tour[i + 1];

                double costIncrease =   locations[current].distanceTo(locations[nextNode])
                                      + locations[nextNode].distanceTo(locations[next])
                                      - locations[current].distanceTo(locations[next]);
                
                if (costIncrease < bestCostIncrease) {
                    bestCostIncrease = costIncrease;
                    bestInsertPos = static_cast<int>(i + 1);
                }
            }

            // Insert the new node into the tour
            tour.insert(tour.begin() + bestInsertPos, nextNode);

            totalTripLength += bestCostIncrease;
            // cout << totalTripLength << "\n";

            for (int i = 0; i < n; ++i) {
                if (!visited[i]) {
                    double dist = locations[nextNode].distanceTo(locations[i]);
                    if (dist < minDistToTour[i]) {
                        minDistToTour[i] = dist;
                    }
                }
            }
        }

        return tour;
    }

    void printTour(const vector<int>& tour) const {
        // cout << fixed << setprecision(2) << tourLength << endl;
        cout << totalTripLength << "\n";

        for (int i = 0; i < n; ++i) {
            cout << tour[i] << " ";
        }
        cout << "\n";
    }

};

// --------------------------------------------------------------------------------------
// Part C Related Implementations
class DistanceCalculator_PartC {
public:
    double operator()(const LocationPBC& loc1, const LocationPBC& loc2) const {
        return loc1.distanceTo(loc2);
    }
};

class PartC_OPTTSP {
public:
    PartC_OPTTSP(const vector<LocationPBC>& locations, const DistanceCalculator_PartC& distanceCalculator, PartB_FastTSP& partB)
        : locations(locations), distanceCalculator(distanceCalculator), n(locations.size()), 
        minTourCost(numeric_limits<double>::infinity()), currentPathCost(0.0) {

        path = partB.PartC_Access_FIH();
        if (!path.empty() && path.front() == path.back()) {
            path.pop_back();
        }
        bestTour = path;
        minTourCost = partB.getLength();

        distanceMatrix.resize(n, vector<double>(n, 0.0));
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = i + 1; j < n; ++j) {
                double dist = distanceCalculator(locations[i], locations[j]);
                distanceMatrix[i][j] = dist;
                distanceMatrix[j][i] = dist;
            }
        }
    }

    void optimizeTSP() {
        // start generating permutations from the second position
        genPerms(1);
        printSolution();
    }

private:
    const vector<LocationPBC>& locations;
    const DistanceCalculator_PartC& distanceCalculator;
    size_t n; // number of locations
    double minTourCost;     
    double currentPathCost; // running total           
    vector<int> bestTour; // best path
    vector<int> path;     
    vector<vector<double>> distanceMatrix;        
    
    struct VertexInfo {
        bool k; // has the vertex been visited
        double d; // minimal edge weight to this vertex
        int p; // parent vertex

        VertexInfo() : k(false), d(numeric_limits<double>::infinity()), p(-1) {}
    };

    void genPerms(size_t permLength) {
        if (permLength == path.size()) {
            evaluateTour();
            return;
        }

        if (!promising(permLength)) {
            return;
        }

        for (size_t i = permLength; i < path.size(); ++i) {
            swap(path[permLength], path[i]);

            if (permLength > 0) {
                currentPathCost += distanceMatrix[path[permLength - 1]][path[permLength]];
            }

            genPerms(permLength + 1);

            if (permLength > 0) {
                currentPathCost -= distanceMatrix[path[permLength - 1]][path[permLength]];
            }

            swap(path[permLength], path[i]);
        }
    }

    void evaluateTour() {
        double tourCost = currentPathCost + distanceMatrix[path.back()][path[0]];

        if (tourCost < minTourCost) {
            minTourCost = tourCost;
            bestTour = path;
        }
    }

    bool promising(size_t permLength) {
        if (currentPathCost >= minTourCost)
            return false;

        size_t n_unvisited = n - permLength;
        if (n_unvisited == 0)
            return true; // All nodes are visited

        // Compute MST cost over unvisited nodes
        double mstCost = computeMSTCost(permLength);

        int currentNode = path[permLength - 1];
        int startNode = path[0];

        // Compute minimum edge from current node to unvisited nodes
        double minEdgeCurrentToUnvisited = numeric_limits<double>::infinity();
        for (size_t i = permLength; i < n; ++i) {
            int unvisitedNode = path[i];
            double dist = distanceMatrix[currentNode][unvisitedNode];
            if (dist < minEdgeCurrentToUnvisited)
                minEdgeCurrentToUnvisited = dist;
        }

        // Compute minimum edge from unvisited nodes back to the start node
        double minEdgeUnvisitedToStart = numeric_limits<double>::infinity();
        for (size_t i = permLength; i < n; ++i) {
            int unvisitedNode = path[i];
            double dist = distanceMatrix[unvisitedNode][startNode];
            if (dist < minEdgeUnvisitedToStart)
                minEdgeUnvisitedToStart = dist;
        }

        // Calculate lower bound
        double lowerBound = currentPathCost + minEdgeCurrentToUnvisited + mstCost + minEdgeUnvisitedToStart;

        return lowerBound < minTourCost;
    }

    double computeMSTCost(size_t permLength) { // basically just copying off part A
        size_t numUnvisited = n - permLength;
        if (numUnvisited == 0)
            return 0.0;

        vector<int> unvisitedIndices(numUnvisited);
        for (size_t i = 0; i < numUnvisited; ++i) {
            unvisitedIndices[i] = path[permLength + i];
        }

        int numNodes = static_cast<int>(numUnvisited);
        vector<VertexInfo> vertexInfo(numNodes);

        // Initialize vertexInfo
        for (int i = 0; i < numNodes; ++i) {
            vertexInfo[i].d = numeric_limits<double>::infinity();
            vertexInfo[i].k = false;
            vertexInfo[i].p = -1;
        }

        vertexInfo[0].d = 0.0;

        double totalWeight = 0.0;

        for (int count = 0; count < numNodes; ++count) {
            int u = -1;
            double minDist = numeric_limits<double>::infinity();

            for (int i = 0; i < numNodes; ++i) {
                if (!vertexInfo[i].k && vertexInfo[i].d < minDist) {
                    minDist = vertexInfo[i].d;
                    u = i;
                }
            }

            if (u == -1) {
                cerr << "Cannot construct MST\n";
                exit(1);
            }

            vertexInfo[u].k = true;
            totalWeight += vertexInfo[u].d;

            int uGlobalIndex = unvisitedIndices[u];
            for (int v = 0; v < numNodes; ++v) {
                if (!vertexInfo[v].k) {
                    int vGlobalIndex = unvisitedIndices[v];
                    double weight = distanceMatrix[uGlobalIndex][vGlobalIndex];
                    if (weight < vertexInfo[v].d) {
                        vertexInfo[v].d = weight;
                        vertexInfo[v].p = u;
                    }
                }
            }
        }

        return totalWeight;
    }

    void printSolution() const {
        cout << minTourCost << "\n";
        for (size_t i = 0; i < bestTour.size(); ++i) {
            cout << bestTour[i] << " ";
        }
        cout << "\n";
    }
};

// --------------------------------------------------------------------------------------
// main function

void print_help() {
    cout << "Usage: poke [OPTIONS]\n"
        << "Options:\n"
        << "  -m, --mode {MST|FASTTSP|OPTTSP}  Specify the mode to run\n"
        << "  -h, --help                       Print this help message\n";
}

int main(int argc, char *argv[]) {
    ios_base::sync_with_stdio(false); 
    cout << setprecision(2); //Always show 2 decimal places
    cout << fixed; //Disable scientific notation for large numbers

    opterr = 0;
    int choice = 0;
    int index = 0;
    string mode;
    
    static struct option long_options[] = {
        {"help", no_argument, nullptr, 'h'},
        {"mode", required_argument, nullptr, 'm'},
        {nullptr, 0, nullptr, '\0'}
    };
    
    while ((choice = getopt_long(argc, argv, "m:h", long_options, &index)) != -1) {
        switch (choice) {
        case 'h':
            print_help();
            exit(0);
            break;

        case 'm':
            mode = optarg;
            if (mode != "MST" && mode != "FASTTSP" && mode != "OPTTSP") {
                cerr << "Error: Invalid mode\n";
                exit(1);
            }
            break;

        default:
            cerr << "Error: Invalid command line option\n";
            exit(1);
        } // switch ..choice
    } // while

    if (mode.empty()) {
        cerr << "Error: No mode specified\n";
        exit(1);
    }

    // Part A
    if (mode == "MST") {
        // Parse input locations
        int numLocations;
        cin >> numLocations;
        vector<LocationPA> locations;
        locations.reserve(numLocations);

        for (int i = 0; i < numLocations; ++i) {
            int x, y;
            char area;
            cin >> x >> y;
            if (x < 0 && y < 0) {
                area = 'S';
            } else if ((x < 0 && y == 0) || (x == 0 && y <= 0)) {
                area = 'C';
            } else {
                area = 'L';
            }
            locations.emplace_back(x, y, area);
        }

        DistanceCalculator distanceCalculator;
        PartA_MST partA(locations, distanceCalculator);
        partA.calculateMST();
    }

    // Part B
    if (mode == "FASTTSP") {
        // Parse input locations
        int numLocations;
        cin >> numLocations;
        vector<LocationPBC> locations;
        locations.reserve(numLocations);

        for (int i = 0; i < numLocations; ++i) {
            int x, y;
            cin >> x >> y;
            locations.emplace_back(x, y);
        }

        PartB_FastTSP partB(locations);
        partB.solveTSP();
    }

    // Part C
    if (mode == "OPTTSP") {
        int numLocations;
        cin >> numLocations;
        vector<LocationPBC> locations;
        locations.reserve(numLocations);

        for (int i = 0; i < numLocations; ++i) {
            int x, y;
            cin >> x >> y;
            locations.emplace_back(x, y);
        }

        DistanceCalculator_PartC distanceCalculator;
        PartB_FastTSP partB(locations);
        PartC_OPTTSP partC(locations, distanceCalculator, partB);
        // PartC_OPTTSP partC(locations, partB);
        partC.optimizeTSP();
    }

}