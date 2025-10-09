#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <optional>
#include <stdexcept>

template <typename T>
class Graph {
private:
    std::unordered_map<T, std::unordered_set<T>> adjacency_list;

    // Backtrack from end to start using parent pointers
    std::vector<T> reconstruct_path(
        const std::unordered_map<T, T>& parent,
        const T& start,
        const T& end
    ) const {
        std::vector<T> path;
        T current = end;
        
        while (current != start) {
            path.push_back(current);
            auto it = parent.find(current);
            if (it == parent.end()) {
                return {};
            }
            current = it->second;
        }
        path.push_back(start);
        std::reverse(path.begin(), path.end());
        
        return path;
    }

public:
    Graph() = default;

    bool add_node(const T& data) {
        if (adjacency_list.find(data) != adjacency_list.end()) {
            return false;
        }
        adjacency_list[data] = std::unordered_set<T>();
        return true;
    }

    bool add_edge(const T& node1_data, const T& node2_data) {
        if (adjacency_list.find(node1_data) == adjacency_list.end()) {
            throw std::runtime_error("Source node does not exist");
        }
        if (adjacency_list.find(node2_data) == adjacency_list.end()) {
            throw std::runtime_error("Destination node does not exist");
        }

        adjacency_list[node1_data].insert(node2_data);
        return true;
    }

    bool add_undirected_edge(const T& node1_data, const T& node2_data) {
        add_edge(node1_data, node2_data);
        add_edge(node2_data, node1_data);
        return true;
    }

    std::vector<T> get_neighbors(const T& data) const {
        auto it = adjacency_list.find(data);
        if (it == adjacency_list.end()) {
            throw std::runtime_error("Node does not exist");
        }
        
        return std::vector<T>(it->second.begin(), it->second.end());
    }

    // BFS guarantees shortest path (by number of edges)
    std::vector<T> bfs(const T& start, const T& end) const {
        if (adjacency_list.find(start) == adjacency_list.end()) {
            throw std::runtime_error("Start node does not exist");
        }
        if (adjacency_list.find(end) == adjacency_list.end()) {
            throw std::runtime_error("End node does not exist");
        }

        if (start == end) {
            return {start};
        }

        std::queue<T> queue;
        std::unordered_set<T> visited;
        std::unordered_map<T, T> parent;

        queue.push(start);
        visited.insert(start);

        while (!queue.empty()) {
            T current = queue.front();
            queue.pop();

            if (current == end) {
                return reconstruct_path(parent, start, end);
            }

            auto it = adjacency_list.find(current);
            if (it != adjacency_list.end()) {
                for (const T& neighbor : it->second) {
                    if (visited.find(neighbor) == visited.end()) {
                        visited.insert(neighbor);
                        parent[neighbor] = current;
                        queue.push(neighbor);
                    }
                }
            }
        }

        return {};
    }

    // DFS finds any path (not necessarily shortest, unlike BFS)
    std::vector<T> dfs(const T& start, const T& end) const {
        if (adjacency_list.find(start) == adjacency_list.end()) {
            throw std::runtime_error("Start node does not exist");
        }
        if (adjacency_list.find(end) == adjacency_list.end()) {
            throw std::runtime_error("End node does not exist");
        }

        if (start == end) {
            return {start};
        }

        std::stack<T> stack;
        std::unordered_set<T> visited;
        std::unordered_map<T, T> parent;

        stack.push(start);
        visited.insert(start);

        while (!stack.empty()) {
            T current = stack.top();
            stack.pop();

            if (current == end) {
                return reconstruct_path(parent, start, end);
            }

            auto it = adjacency_list.find(current);
            if (it != adjacency_list.end()) {
                for (const T& neighbor : it->second) {
                    if (visited.find(neighbor) == visited.end()) {
                        visited.insert(neighbor);
                        parent[neighbor] = current;
                        stack.push(neighbor);
                    }
                }
            }
        }

        return {};
    }

    size_t size() const {
        return adjacency_list.size();
    }

    bool has_node(const T& data) const {
        return adjacency_list.find(data) != adjacency_list.end();
    }

    bool has_edge(const T& node1_data, const T& node2_data) const {
        auto it = adjacency_list.find(node1_data);
        if (it == adjacency_list.end()) {
            return false;
        }
        return it->second.find(node2_data) != it->second.end();
    }

    std::vector<T> get_all_nodes() const {
        std::vector<T> nodes;
        nodes.reserve(adjacency_list.size());
        for (const auto& pair : adjacency_list) {
            nodes.push_back(pair.first);
        }
        return nodes;
    }
};

#endif // GRAPH_HPP

