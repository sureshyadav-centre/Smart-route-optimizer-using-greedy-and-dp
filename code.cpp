/*
 * ============================================================
 *   SmartRoute — Route Optimizer (C++)
 *   Algorithms: Greedy | DP (Held-Karp) | Dijkstra
 * ============================================================
 */

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <climits>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <functional>
#include <cstdlib>

using namespace std;

// ============================================================
// CITY STRUCTURE
// ============================================================
struct City {
    string name;
    double x, y;
};


double dist(const City& a, const City& b) {
    return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}


void printRoute(const vector<City>& cities, const vector<int>& path,
                double cost, const string& algoName, double timeMs) {
    cout << "\n========================================\n";
    cout << "  " << algoName << "\n";
    cout << "========================================\n";
    cout << "Route:\n  ";
    for (int i = 0; i < (int)path.size(); i++) {
        cout << cities[path[i]].name;
        if (i < (int)path.size() - 1) cout << " --> ";
        if ((i + 1) % 4 == 0 && i < (int)path.size() - 1) cout << "\n  ";
    }
    cout << "\n";
    cout << "Total Distance : " << fixed << setprecision(2) << cost << " units\n";
    cout << "Time Taken     : " << fixed << setprecision(4) << timeMs << " ms\n";
    cout << "Complexity     : ";
    if (algoName.find("Greedy") != string::npos)   cout << "O(n^2)\n";
    else if (algoName.find("DP") != string::npos)  cout << "O(n^2 * 2^n)\n";
    else                                            cout << "O(n^2)\n";
}

// ============================================================
// ALGORITHM 1 — GREEDY NEAREST NEIGHBOR
// ============================================================
pair<vector<int>, double> greedyNN(const vector<City>& cities, int start) {
    int n = cities.size();
    vector<bool> visited(n, false);
    vector<int> path;
    path.push_back(start);
    visited[start] = true;
    double cost = 0;

    while ((int)path.size() < n) {
        int cur  = path.back();
        int best = -1;
        double bestDist = 1e18;

        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                double d = dist(cities[cur], cities[i]);
                if (d < bestDist) { bestDist = d; best = i; }
            }
        }
        visited[best] = true;
        cost += bestDist;
        path.push_back(best);
    }
   
    cost += dist(cities[path.back()], cities[path[0]]);
    path.push_back(path[0]);
    return {path, cost};
}

// ============================================================
// ALGORITHM 2 — DP HELD-KARP (Exact TSP)
// ============================================================
pair<vector<int>, double> heldKarp(const vector<City>& cities, int start) {
    int n = cities.size();

    if (n > 20) {
        cout << "[DP] n > 20: too large for exact DP, using Greedy instead.\n";
        return greedyNN(cities, start);
    }

    int sz = 1 << n;
    const double INF = 1e18;

    vector<vector<double>> dp(sz, vector<double>(n, INF));
    vector<vector<int>>    par(sz, vector<int>(n, -1));

    dp[1 << start][start] = 0;

    for (int mask = 0; mask < sz; mask++) {
        for (int u = 0; u < n; u++) {
            if (!(mask & (1 << u))) continue;   
            if (dp[mask][u] == INF) continue;  

            for (int v = 0; v < n; v++) {
                if (mask & (1 << v)) continue; 
                int    newMask = mask | (1 << v);
                double newCost = dp[mask][u] + dist(cities[u], cities[v]);
                if (newCost < dp[newMask][v]) {
                    dp[newMask][v] = newCost;
                    par[newMask][v] = u;
                }
            }
        }
    }

    
    int    fullMask = sz - 1;
    double best = INF;
    int    last = -1;
    for (int u = 0; u < n; u++) {
        if (u == start) continue;
        double total = dp[fullMask][u] + dist(cities[u], cities[start]);
        if (total < best) { best = total; last = u; }
    }

  
    vector<int> path;
    int mask = fullMask, cur = last;
    while (cur != -1) {
        path.push_back(cur);
        int prev = par[mask][cur];
        mask ^= (1 << cur);
        cur = prev;
    }
    reverse(path.begin(), path.end());
    path.push_back(path[0]);   
    return {path, best};
}

// ============================================================
// ALGORITHM 3 — DIJKSTRA'S SHORTEST PATH
// ============================================================
pair<vector<int>, double> dijkstra(const vector<City>& cities, int start) {
    int n = cities.size();
    vector<double> d(n, 1e18);
    vector<bool>   visited(n, false);
    d[start] = 0;

    for (int i = 0; i < n; i++) {
       
        int u = -1;
        for (int v = 0; v < n; v++)
            if (!visited[v] && (u == -1 || d[v] < d[u])) u = v;

        visited[u] = true;

     
        for (int v = 0; v < n; v++) {
            if (!visited[v]) {
                double w = dist(cities[u], cities[v]);
                if (d[u] + w < d[v]) d[v] = d[u] + w;
            }
        }
    }

    vector<int> order;
    for (int i = 0; i < n; i++) if (i != start) order.push_back(i);
    sort(order.begin(), order.end(), [&](int a, int b){ return d[a] < d[b]; });

    vector<int> path = {start};
    for (int c : order) path.push_back(c);
    path.push_back(start);   

    double cost = 0;
    for (int i = 0; i + 1 < (int)path.size(); i++)
        cost += dist(cities[path[i]], cities[path[i+1]]);
    return {path, cost};
}

// ============================================================
// COMPARISON TABLE
// ============================================================
void printComparison(const vector<tuple<string,double,string,double>>& results) {
    cout << "\n========================================\n";
    cout << "        ALGORITHM COMPARISON\n";
    cout << "========================================\n";
    cout << left
         << setw(20) << "Algorithm"
         << setw(14) << "Distance"
         << setw(16) << "Complexity"
         << setw(10) << "Time(ms)" << "\n";
    cout << "----------------------------------------\n";

    double best = 1e18;
    for (auto& [name, cost, cplx, t] : results)
        best = min(best, cost);

    for (auto& [name, cost, cplx, t] : results) {
        string mark = (abs(cost - best) < 0.01) ? " <-- BEST" : "";
        cout << left
             << setw(20) << name
             << setw(14) << fixed << setprecision(2) << cost
             << setw(16) << cplx
             << setw(10) << fixed << setprecision(4) << t
             << mark << "\n";
    }
    cout << "========================================\n";
}

// ============================================================
// MAIN
// ============================================================
int main() {
    cout << "============================================================\n";
    cout << "   SmartRoute — Route Optimizer\n";
    cout << "   Algorithms: Greedy | DP (Held-Karp) | Dijkstra\n";
    cout << "============================================================\n";

   
    vector<City> cities = {
        {"Mumbai",    100, 300},
        {"Delhi",     300, 100},
        {"Chennai",   350, 500},
        {"Kolkata",   550, 200},
        {"Bengaluru", 280, 450},
        {"Hyderabad", 300, 380},
        {"Pune",      200, 340},
        {"Ahmedabad", 150, 200},
    };

    int startCity = 0;

    cout << "\nUse default 8 Indian cities? (y/n): ";
    char ch; cin >> ch;

    if (ch == 'n' || ch == 'N') {
        cities.clear();
        int num;
        cout << "How many cities? "; cin >> num;
        cin.ignore();
        for (int i = 0; i < num; i++) {
            City c;
            cout << "\nCity " << (i+1) << " name: ";
            getline(cin, c.name);
            cout << "  X coordinate: "; cin >> c.x;
            cout << "  Y coordinate: "; cin >> c.y;
            cin.ignore();
            cities.push_back(c);
        }
        cout << "Start city index (0 to " << (int)cities.size()-1 << "): ";
        cin >> startCity;
    }

    int n = cities.size();
    cout << "\n--- Cities List ---\n";
    for (int i = 0; i < n; i++)
        cout << "  [" << i << "] " << cities[i].name
             << "  (" << cities[i].x << ", " << cities[i].y << ")\n";
    cout << "Start: " << cities[startCity].name << "\n";


    vector<tuple<string,double,string,double>> results;

    // ── 1. GREEDY ────────────────────────────────────────
    auto t1 = chrono::high_resolution_clock::now();
    auto [gPath, gCost] = greedyNN(cities, startCity);
    auto t2 = chrono::high_resolution_clock::now();
    double gTime = chrono::duration<double,milli>(t2-t1).count();
    printRoute(cities, gPath, gCost, "1. Greedy Nearest Neighbor", gTime);
    results.push_back({"Greedy NN", gCost, "O(n^2)", gTime});

    // ── 2. DP HELD-KARP ───────────────────────────────────
    t1 = chrono::high_resolution_clock::now();
    auto [dpPath, dpCost] = heldKarp(cities, startCity);
    t2 = chrono::high_resolution_clock::now();
    double dpTime = chrono::duration<double,milli>(t2-t1).count();
    printRoute(cities, dpPath, dpCost, "2. DP Held-Karp (Exact TSP)", dpTime);
    results.push_back({"DP Held-Karp", dpCost, "O(n^2 * 2^n)", dpTime});

    // ── 3. DIJKSTRA ───────────────────────────────────────
    t1 = chrono::high_resolution_clock::now();
    auto [djPath, djCost] = dijkstra(cities, startCity);
    t2 = chrono::high_resolution_clock::now();
    double djTime = chrono::duration<double,milli>(t2-t1).count();
    printRoute(cities, djPath, djCost, "3. Dijkstra's Shortest Path", djTime);
    results.push_back({"Dijkstra", djCost, "O(n^2)", djTime});

    // ── COMPARISON TABLE ──────────────────────────────────
    printComparison(results);

    cout << "\nProgram ended. Thank you!\n";
    system("pause"); 
    return 0;
}