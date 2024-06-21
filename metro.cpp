#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <stack>
#include <climits>
#include <algorithm>
#include <random> // For random number generation

using namespace std;

class Graph
{
    unordered_map<string, unordered_map<string, int>> adjList;

public:
    void addEdge(const string &u, const string &v, int time)
    {
        adjList[u][v] = time;
        adjList[v][u] = time; // Assuming undirected graph
    }

    vector<string> dijkstra(const string &src, const string &dest)
    {
        if (adjList.find(src) == adjList.end() || adjList.find(dest) == adjList.end())
        {
            return vector<string>(); // Invalid stations
        }

        unordered_map<string, int> dist;
        unordered_map<string, string> prev;

        struct CompareDist
        {
            bool operator()(pair<int, string> const &p1, pair<int, string> const &p2)
            {
                return p1.first > p2.first;
            }
        };

        priority_queue<pair<int, string>, vector<pair<int, string>>, CompareDist> pq;

        for (const auto &node : adjList)
        {
            dist[node.first] = INT_MAX;
        }
        dist[src] = 0;
        pq.push(make_pair(0, src));

        while (!pq.empty())
        {
            string u = pq.top().second;
            pq.pop();

            if (u == dest)
                break;

            for (const auto &neighbor : adjList[u])
            {
                string v = neighbor.first;
                int weight = neighbor.second;

                if (dist[u] + weight < dist[v])
                {
                    dist[v] = dist[u] + weight;
                    prev[v] = u;
                    pq.push(make_pair(dist[v], v));
                }
            }
        }

        vector<string> path;
        for (string at = dest; at != ""; at = prev[at])
        {
            path.push_back(at);
        }
        reverse(path.begin(), path.end());

        if (path.size() == 1 && path[0] != src)
        {
            path.clear(); // No path found
        }

        return path;
    }

    vector<string> bfs(const string &src, const string &dest)
    {
        if (adjList.find(src) == adjList.end() || adjList.find(dest) == adjList.end())
        {
            return vector<string>(); // Invalid stations
        }

        unordered_map<string, bool> visited;
        unordered_map<string, string> prev;
        queue<string> q;

        visited[src] = true;
        q.push(src);

        while (!q.empty())
        {
            string u = q.front();
            q.pop();

            if (u == dest)
                break;

            for (const auto &neighbor : adjList[u])
            {
                string v = neighbor.first;
                if (!visited[v])
                {
                    visited[v] = true;
                    prev[v] = u;
                    q.push(v);
                }
            }
        }

        vector<string> path;
        for (string at = dest; at != ""; at = prev[at])
        {
            path.push_back(at);
        }
        reverse(path.begin(), path.end());

        if (path.size() == 1 && path[0] != src)
        {
            path.clear(); // No path found
        }

        return path;
    }

    void dfsHelper(const string &u, const string &dest, unordered_map<string, bool> &visited, vector<string> &path, vector<string> &result)
    {
        visited[u] = true;
        path.push_back(u);

        if (u == dest)
        {
            result = path;
        }
        else
        {
            for (const auto &neighbor : adjList[u])
            {
                if (!visited[neighbor.first])
                {
                    dfsHelper(neighbor.first, dest, visited, path, result);
                }
            }
        }

        path.pop_back();
        visited[u] = false;
    }

    vector<string> dfs(const string &src, const string &dest)
    {
        if (adjList.find(src) == adjList.end() || adjList.find(dest) == adjList.end())
        {
            return vector<string>(); // Invalid stations
        }

        unordered_map<string, bool> visited;
        vector<string> path, result;
        dfsHelper(src, dest, visited, path, result);

        if (result.size() == 1 && result[0] != src)
        {
            result.clear(); // No path found
        }

        return result;
    }

    int calculateTime(const vector<string> &path)
    {
        int totalTime = 0;
        for (size_t i = 0; i < path.size() - 1; ++i)
        {
            totalTime += adjList[path[i]][path[i + 1]];
        }
        return totalTime;
    }
};

int main()
{
    Graph metroGraph;

    // Add edges between all nodes A-Z and a-z with random values
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 20); // Random weight between 1 and 20

    // Add edges for uppercase letters 'A'-'Z'
    for (char c1 = 'A'; c1 <= 'Z'; ++c1)
    {
        for (char c2 = 'A'; c2 <= 'Z'; ++c2)
        {
            if (c1 != c2)
            {
                metroGraph.addEdge(string(1, c1), string(1, c2), dis(gen));
            }
        }
    }

    // Add edges for lowercase letters 'a'-'z'
    for (char c1 = 'a'; c1 <= 'z'; ++c1)
    {
        for (char c2 = 'a'; c2 <= 'z'; ++c2)
        {
            if (c1 != c2)
            {
                metroGraph.addEdge(string(1, c1), string(1, c2), dis(gen));
            }
        }
    }

    // Ensure connectivity between 'A' and 'z' with a direct edge
    metroGraph.addEdge("A", "z", dis(gen)); // Assign random weight between A and z

    string source, destination;
    cout << "Enter source station (A-Z, a-z): ";
    cin >> source;
    cout << "Enter destination station (A-Z, a-z): ";
    cin >> destination;

    // Validate source and destination inputs
    if (!((source >= "A" && source <= "Z") || (source >= "a" && source <= "z")) ||
        !((destination >= "A" && destination <= "Z") || (destination >= "a" && destination <= "z")))
    {
        cout << "Invalid source or destination station." << endl;
        return 1;
    }

    int choice;
    while (true)
    {
        cout << "Choose algorithm (1 - Dijkstra, 2 - BFS, 3 - DFS): ";
        cin >> choice;
        if (choice >= 1 && choice <= 3)
            break;
        cout << "Invalid choice! Please enter a number between 1 and 3." << endl;
    }

    vector<string> path;
    switch (choice)
    {
    case 1:
        path = metroGraph.dijkstra(source, destination);
        break;
    case 2:
        path = metroGraph.bfs(source, destination);
        break;
    case 3:
        path = metroGraph.dfs(source, destination);
        break;
    }

    if (!path.empty())
    {
        cout << "Path: ";
        for (const string &station : path)
        {
            cout << station << " ";
        }
        cout << endl;
        cout << "Total time: " << metroGraph.calculateTime(path) << " minutes" << endl;
    }
    else
    {
        cout << "No path found between " << source << " and " << destination << endl;
    }

    return 0;
}
