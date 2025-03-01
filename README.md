# Graph Algorithms Implementation

## Introduction
This project provides a collection of common graph algorithms implemented for educational and practical purposes. These algorithms cover a variety of applications, such as finding the shortest path, calculating maximum flow, and determining the minimum spanning tree (MST).

## Supported Algorithms:
BFS (Breadth-First Search): A traversal algorithm that explores nodes level by level.

Dijkstra's Algorithm: Computes the shortest path between nodes in a graph with non-negative weights.

Ford-Fulkerson Algorithm: A method for computing the maximum flow in a flow network.

Kruskal's Algorithm: A greedy algorithm for finding the Minimum Spanning Tree (MST).

Prim's Algorithm: Another greedy algorithm for finding the MST.

Strongly Connected Components (SCC): Identifies strongly connected components in a directed graph.

Topological Sorting: Orders the vertices in a directed graph such that for every directed edge (u, v), vertex u comes before v.

# BFS (Breadth-First Search)

This application finds the shortest paths to all exit points in a maze using the Breadth-First Search (BFS) algorithm.

### Features
Input: The maze is read from a text file, where:
``1`` represents walkable paths,
``0`` represents walls,
``3`` represents the start point,
``2`` represents exit points.
Graph Representation: The maze is converted into a graph excluding walls, and BFS is applied to find the shortest paths to the exit points.

### Graphical Display:
```
Walls are displayed in black,
Paths in white,
The start point in blue,
Exit points in red,
Shortest paths are drawn in green.
```
### Algorithm:

Input Parsing: Reads the maze from a file and constructs the graph.

BFS: Implements BFS to find the shortest path from the start point to all exits.

Path Visualization: Draws the maze and highlights the shortest paths to exits.

![image](https://github.com/user-attachments/assets/463d6b1d-b939-479f-bb01-9583e2ce3091)

# Dijkstra Shortest Path Finder
This application uses Dijkstra's algorithm to find the shortest path between two selected points on a map, automatically loaded from an XML file.

### Features
Input: The map is automatically loaded from an XML file containing node and edge information.

Nodes represent locations or points on the map.

Edges represent the connections between nodes and may contain weights (distances or costs).

### User Interaction:
Select two points (start and end) on the map.

The program computes and displays the shortest path between these two points using Dijkstra's algorithm.

### Graphical Display:
Nodes are displayed as points.

Edges (paths between nodes) are drawn as lines connecting the nodes.

The shortest path is highlighted in green, and the start and end points are marked with special colors.

### How It Works:
Map Loading: The map is read from an XML file containing a list of nodes and edges, each with associated distances or costs.

User Selection: The user selects a start and an end point.

Dijkstra's Algorithm: The program runs Dijkstra’s algorithm to calculate the shortest path from the start to the end point.

Shortest Path Visualization: The shortest path is visually represented on the map with green color, and the start and end points are highlighted.

# Ford-Fulkerson Max Flow Algorithm
This application implements the Ford-Fulkerson algorithm to determine the maximum flow in a network and the minimum cut.

### Features
Graph Representation:

The program allows you to create a directed graph representing a flow network with nodes and edges.
Each edge has a capacity, which represents the maximum amount of flow that can pass through that edge.

Visualization:
The network is visualized on the screen, with each edge displaying its capacity.
The minimum cut edges are highlighted in a different color to distinguish them from other edges in the network.

Ford-Fulkerson Algorithm:
The algorithm computes the maximum flow from a source node to a sink node.
The program displays the calculated maximum flow and visualizes the flow through the network.

Minimum Cut:
The edges belonging to the minimum cut are displayed in a different color to indicate the set of edges that separate the source and sink in the network after the maximum flow is achieved.
### How It Works
Network Visualization:
The user can define a network of nodes and directed edges with capacities. The network is displayed in a window with edges labeled by their capacities.

Ford-Fulkerson Algorithm:
The program applies the Ford-Fulkerson algorithm to calculate the maximum flow between a source node and a sink node.
The algorithm uses augmenting paths to increase the flow in the network until no more augmenting paths can be found.

Displaying Maximum Flow:
After computing the maximum flow, the program shows the flow values on the arcs, and the total flow is displayed.

# Kruskal's Algorithm - Minimum Spanning Tree
This application implements Kruskal's algorithm to find the minimum spanning tree (MST) of a connected, undirected graph.

### Features
Graph Representation:
The program allows you to create an undirected graph, where each edge has a weight that represents the cost or distance between two nodes.

Kruskal's Algorithm:
The algorithm computes the MST by adding edges one by one, in increasing order of their weights, ensuring no cycles are formed. The process is stopped once all nodes are connected.

### How It Works
Graph Creation:
The user can define a graph with nodes and weighted edges.

Kruskal's Algorithm:
The program applies Kruskal’s algorithm to find the MST by sorting the edges in increasing order of weight.
It then adds the edges to the MST one by one while ensuring no cycle is created (using Union-Find or Disjoint Set data structure).

MST Visualization:
The program visualizes the graph and highlights the edges that are part of the MST.
The MST is displayed with the selected edges forming the minimum spanning tree.

Minimum Cut:
The algorithm also computes the minimum cut, which is the set of edges that, if removed, would disconnect the source node from the sink node.
The edges belonging to the minimum cut are drawn with a different color to visually represent the cut.

# Prim's Algorithm - Minimum Spanning Tree
This application implements Prim's algorithm to find the minimum spanning tree (MST) of a connected, undirected graph.

### Features
Graph Representation:
The program allows you to create an undirected graph, where each edge has a weight representing the cost or distance between two nodes.

Prim's Algorithm:
The algorithm computes the MST starting from an arbitrary node. It grows the MST by adding the shortest edge that connects a vertex in the MST to a vertex outside it.

### How It Works
Graph Creation:
The user can define a graph with nodes and weighted edges.

Prim's Algorithm:
The program applies Prim's algorithm starting from a source node. The algorithm adds edges to the MST by always picking the minimum weight edge that connects a node already in the MST to one outside it.
It ensures that no cycles are formed and that the graph remains connected.

MST Visualization:
The program visualizes the graph and highlights the edges that are part of the MST.
The MST is displayed with the selected edges forming the minimum spanning tree.

# Strongly Connected Components (SCC) Algorithm
This application implements an algorithm for finding Strongly Connected Components (SCC) in a directed graph. The result is a new graph where each node represents one SCC, and there are edges between the components if there are edges between the nodes of those components in the original graph.

### Features
Graph Representation:
The program creates a directed graph with nodes and edges. Each edge represents a directed connection between two nodes in the graph.

Strongly Connected Components (SCC):
The algorithm finds strongly connected components in the graph, where each SCC is a subgraph where every node is reachable from every other node within the same component.

Graph Transformation:
The new graph contains as many nodes as there are strongly connected components.
Each SCC is represented by a single node, and the label of the node will include the nodes belonging to the SCC.
The graph is updated to show the edges between these SCCs.

Visualization:
The graph is visualized, and the SCCs are represented as single nodes. Each edge between SCCs is drawn, representing the relationship between the components.
Nodes within the same SCC are grouped together, and the edges between components are represented by arrows.
### How It Works
Graph Creation:
The user can define a directed graph with nodes and edges.

Finding Strongly Connected Components:
The program uses Kosaraju's Algorithm (or Tarjan's Algorithm) to find SCCs.

The graph is processed to find all strongly connected components:
Kosaraju’s Algorithm involves two Depth-First Searches (DFS):
The first DFS is used to order the nodes based on their finishing times.
The second DFS is performed on the transpose (reverse) graph, using the order obtained from the first DFS, to discover the SCCs.

Graph Transformation:
Once the SCCs are found, a new graph is constructed with one node for each SCC.
The edges between the components are drawn based on the edges between the nodes in the original graph.

SCC Visualization:
The program visualizes the original graph as well as the new graph with SCCs.
The new graph shows nodes representing the SCCs, with edges between components when there’s a connection between nodes in different SCCs in the original graph.

# Topological Sorting Algorithm
This application implements an algorithm for Topological Sorting of a directed graph, checking whether the graph contains cycles, and handling cases where topological sorting is not possible.

### Features
Graph Representation:
The program allows you to create a directed graph with nodes and edges.

Depth-First Search (DFS):
The program uses Depth-First Search (DFS) to explore the graph and perform topological sorting.

Cycle Detection:
Before attempting topological sorting, the program checks if the graph contains any cycles.
If a cycle is detected, topological sorting is not possible, and an appropriate message is displayed.

Topological Sorting:
If the graph does not contain any cycles, the program performs a topological sort on the graph.
The sorted order of nodes is displayed and can be exported to a text file.

Graph Visualization:
A visualization of the graph is displayed after sorting, showing the nodes and edges corresponding to the topological sort.

Output:
The topological order of the graph is displayed on the screen.
The result is written to a text file.
### How It Works
1. Graph Creation:
The user can define a directed graph with nodes and edges.
 
2. Cycle Detection:
Before sorting, the program checks for cycles in the graph using a DFS-based cycle detection algorithm.
If a cycle is found, topological sorting is not possible. The program will output a message indicating that the graph contains cycles and sorting is not feasible.
 
3. Depth-First Search (DFS):
The program implements DFS for traversing the graph.
The nodes are visited in depth-first order, and a stack is used to store the topologically sorted nodes.
If all nodes are visited and no cycles are detected, the topological order is successfully determined.
 
4. Topological Sorting:
After the DFS traversal, the nodes are placed in reverse order of their finishing times.
The sorted nodes are the topologically sorted nodes of the graph.
 
5. Output:
If the graph is acyclic, the topological order is printed on the screen.
The topological order is also saved to a text file for later use.
If the graph contains cycles, a message is displayed informing the user that topological sorting is not possible.
 
6. Graph Visualization:
After determining the topological order, the program draws the graph with the nodes in the order they appear in the topological sorting.
A new window is created to show the graph with the sorted nodes.
