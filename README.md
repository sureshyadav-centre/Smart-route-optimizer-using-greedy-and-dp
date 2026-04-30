# 🗺️ SmartRoute – AI-Powered Route Optimizer

> A visual demonstration of classic algorithmic strategies applied to the **Travelling Salesman Problem (TSP)** and shortest-path routing.

---

## 📌 Algorithms Implemented

| Algorithm | Type | Time Complexity | Use Case |
|---|---|---|---|
| **Greedy Nearest Neighbor** | Greedy | O(n²) | Fast approximate TSP |
| **Held-Karp (DP/TSP)** | Dynamic Programming | O(n² · 2ⁿ) | Optimal TSP solution |
| **Dijkstra's Algorithm** | Graph / Greedy | O(n²) | Single-source shortest path |

---

## 🚀 Features

- **Interactive Graph Canvas** – Click to add nodes, drag to reposition
- **Real-time Algorithm Visualization** – Watch the route animate step-by-step
- **Algorithm Comparison Table** – Compare all 3 algorithms side-by-side
- **Random City Generator** – Instantly generate Indian city graphs
- **Cost & Complexity Display** – See Big-O complexity for each approach

---

## 🛠️ How to Run

```bash
# Clone the repo
git clone https://github.com/YOUR_USERNAME/smartroute-optimizer

# Open in browser (no build step needed!)
open index.html
```

Or just double-click `index.html` — it's pure HTML/CSS/JS with zero dependencies!

---

## 📂 Project Structure

```
smartroute-optimizer/
│
├── index.html          # Main app (self-contained)
├── README.md           # This file
└── screenshots/        # Demo screenshots
```

---

## 🧠 Algorithm Details

### 1. Greedy Nearest Neighbor
```
Start at node S
While unvisited nodes remain:
    Move to closest unvisited neighbor
Return to S
```
- **Pros**: Very fast, easy to implement
- **Cons**: May be up to 25% worse than optimal

### 2. Dynamic Programming – Held-Karp (TSP)
```
dp[mask][i] = min cost to visit nodes in 'mask', ending at i
Base: dp[1<<start][start] = 0
Transition: dp[mask|(1<<v)][v] = min(dp[mask][u] + dist(u,v))
```
- **Pros**: Guarantees optimal solution
- **Cons**: Exponential space — practical for n ≤ 15

### 3. Dijkstra's Shortest Path
```
dist[start] = 0, all others = ∞
While unvisited nodes:
    Pick u with min dist
    For each neighbor v:
        relax dist[v] = min(dist[v], dist[u] + w(u,v))
```
- **Pros**: Optimal shortest paths from source
- **Cons**: Doesn't minimize total tour length

---

## 👥 Team Members

| Name | Roll No |
|---|---|
| Member 1 | - |
| Member 2 | - |
| Member 3 | - |
| Member 4 | - |
| Member 5 | - |

---

## 📊 Sample Results

Running on 8 Indian cities:
- **Greedy**: 1842 px (0.003ms)
- **DP (Optimal)**: 1756 px (0.12ms)  ← Best route
- **Dijkstra**: 1923 px (0.002ms)

---

## 🏫 Submitted To
**CCC (Computer Coding Club)**  
Deadline: 30th April

---

*Built with pure HTML, CSS & Vanilla JavaScript — no frameworks, no libraries.*
