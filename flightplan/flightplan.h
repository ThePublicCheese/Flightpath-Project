#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map> //how fancy, I'm learning
#include <algorithm>
#include <tuple> //someday, a man has to learn how to tuple.
#include "stack.h"
#include "linkedListFlight.h"
//flight plan class with adjacency list

class FlightPlan {

private:
    unordered_map<string, LinkedList*> adjacencyList;

public:
    FlightPlan() {}

    void addFlight(string origin, string destination, double cost, double time) {
        if (adjacencyList.find(origin) == adjacencyList.end()) {
            adjacencyList[origin] = new LinkedList();
        }
        adjacencyList[origin]->addNode(destination, cost, time);
    }

    //implement your backtracking algorithm here?
    
    void findPaths(string currentCity, string destination, Stack& pathStack, unordered_map<string, bool>& visited, vector<string>& currentPath, double& minCost, double& minTime, vector<vector<string>>& allPaths) {
        //check if the current city is in the visited map and if it has been visited
        if (visited.find(currentCity) == visited.end()) {
            throw std::runtime_error("Current city not found in visited map");
        }
        if (visited[currentCity]) {
            throw std::runtime_error("Current city already visited");
        }

        //make the city visited and push our city into stack
        visited[currentCity] = true;
        pathStack.push(currentCity);
        currentPath.push_back(currentCity);

        //pathStack.display();

        if (currentCity == destination) {
            allPaths.push_back(currentPath);
        }

        //check possible destinations from the current city
        else {
            LinkedList* destinations = adjacencyList[currentCity];
            LinkedListNode* current = destinations->getHead();

            while (current != nullptr) {
                if (!visited[current->getCity()]) {
                    //recurcively
                    findPaths(current->getCity(), destination, pathStack, visited, currentPath, minCost, minTime, allPaths);
                }
                current = current->getNext();
            }
        }

        //backtracking
        pathStack.pop();
        currentPath.pop_back();
        visited[currentCity] = false;
    }

    //print out path for readability
    void printPath(const vector<string>& path) {
        for (size_t i = 0; i < path.size(); ++i) {
            cout << path[i];
            if (i < path.size() - 1) {
                cout << " -> ";
            }
        }
        cout << "\n";
    }
    //compare function
    bool comparePaths(const vector<string>& path1, const vector<string>& path2, char data) {
        double calc1 = calculatePathData(path1, data);
        double calc2 = calculatePathData(path2, data);
        return calc1 < calc2;
    }

    /*
    double calculatePathData(const vector<string>& path, char data) {
        double totalTime = 0.0;
        double totalCost = 0.0;

        for (size_t i = 0; i < path.size() - 1; ++i) {
            string currentStop = path[i];
            string nextStop = path[i + 1];
            if (data == 'T') {
                double legTime = getTime(adjacencyList, currentStop, nextStop);
                totalTime += legTime;
            }
            if (data == 'C') {
                // Retrieve cost and time for the current leg of the journey
                double legCost = getCost(adjacencyList, currentStop, nextStop);
                totalCost += legCost;
            }
        }

        if (data == 'T') {
            return totalTime;
        }
        if (data == 'C') {
            return totalCost;
        }
    }*/
    double calculatePathData(const vector<string>& path, char data) {
        double totalTime = 0.0;
        double totalCost = 0.0;

        for (size_t i = 0; i < path.size() - 1; ++i) {
            string currentStop = path[i];
            string nextStop = path[i + 1];
            double legTime = getTime(adjacencyList, currentStop, nextStop);
            double legCost = getCost(adjacencyList, currentStop, nextStop);

            totalTime += legTime;
            totalCost += legCost;
        }

        if (data == 'T') {
            return totalTime;
        }
        else if (data == 'C') {
            return totalCost;
        }
        else {
            //if not 'T' or 'C'
            throw std::runtime_error("Invalid data type");
        }
    }

    void shortestPaths(string origin, string destination, unordered_map<string, bool>& visited, char data) {
        Stack pathStack;
        vector<string> currentPath;
        vector<vector<string>> allPaths;

        double minCost = -1.0;
        double minTime = -1.0;

        findPaths(origin, destination, pathStack, visited, currentPath, minCost, minTime, allPaths);

        //get ready for output with a funky switch statement
        //print shortest paths with nice output
        switch (data) {

        case '@': //magic case for testing: UNTESTED
            sort(allPaths.begin(), allPaths.end(), [this](const vector<string>& path1, const vector<string>& path2) {
                return comparePaths(path1, path2, 'T');
                });

            //why did i choose to use sort
            sort(allPaths.begin(), allPaths.end(), [this](const vector<string>& path1, const vector<string>& path2) {
                return comparePaths(path1, path2, 'C');
                });

            cout << "\n\n" << "------------------------------------------------------------\n"
                << "Shortest Paths from " << origin << " to " << destination << "when considering both time and cost " << ":\n\n";

            for (size_t i = 0; i < min(static_cast<size_t>(3), allPaths.size()); ++i) {
                printPath(allPaths[i]);
                cout << "Time: " << calculatePathData(allPaths[i], 'T') << "\n";
                cout << "Cost: " << calculatePathData(allPaths[i], 'C') << "\n\n";
                //cout << "Cost: " << calculatePathCost(allPaths[i]) << "\n";
            }

            cout << "\n" << "------------------------------------------------------------\n";
            break;

        case 'C':
            sort(allPaths.begin(), allPaths.end(), [this](const vector<string>& path1, const vector<string>& path2) {
                return comparePaths(path1, path2, 'C');
                });

            cout << "\n\n" << "------------------------------------------------------------\n"
                << "Shortest Paths from " << origin << " to " << destination << " when considering cost " << ":\n\n";

            for (size_t i = 0; i < min(static_cast<size_t>(3), allPaths.size()); ++i) {
                printPath(allPaths[i]);
                cout << "Cost: " << calculatePathData(allPaths[i], 'C') << "\n\n";
            }

            cout << "\n" << "------------------------------------------------------------\n";
            break;


        case 'T':
            sort(allPaths.begin(), allPaths.end(), [this](const vector<string>& path1, const vector<string>& path2) {
                return comparePaths(path1, path2, 'T');
                });

            cout << "\n\n" << "------------------------------------------------------------\n"
                << "Shortest Paths from " << origin << " to " << destination << " when considering time " << ":\n\n";

            for (size_t i = 0; i < min(static_cast<size_t>(3), allPaths.size()); ++i) {
                printPath(allPaths[i]);
                cout << "Time: " << calculatePathData(allPaths[i], 'T') << "\n\n";
            }

            cout << "\n" << "------------------------------------------------------------\n";
            break;


            /*
                //this function is the devil.

                if (data == 'T') {
                    sort(allPaths.begin(), allPaths.end(), [this](const vector<string>& path1, const vector<string>& path2) {
                        return comparePaths(path1, path2, 'T');
                        });
                }
                if (data == 'C') {
                    sort(allPaths.begin(), allPaths.end(), [this](const vector<string>& path1, const vector<string>& path2, char data) {
                        return comparePaths(path1, path2, 'C');
                        });
                }
                // Print the top 3 shortest paths
                cout << "Shortest Paths from " << origin << " to " << destination << " when considering "<< data << ":\n";
                for (size_t i = 0; i < min(static_cast<size_t>(3), allPaths.size()); ++i) {
                    printPath(allPaths[i]);
                    cout << "Time: " << calculatePathData(allPaths[i], data) << "\n";
                }
            */

        }
    }

    //adjList, curCity, nextCity
    double getCost(const unordered_map<string, LinkedList*>& adjacencyList, string currentCity, string nextCity) {
        //check if the origin city exists
        unordered_map<string, LinkedList*>::const_iterator it = adjacencyList.find(currentCity);
        if (it == adjacencyList.end()) {
            // Handle the case where the origin city is not found
            throw std::runtime_error("Origin city not found in adjacency list");
        }

        LinkedListNode* current = it->second->getHead();
        while (current != nullptr) {
            if (current->getCity() == nextCity) {
                //assume the cost is associated with the next city in the linked list
                return current->getCost();
            }
            current = current->getNext();
        }

        //handle the case where the destination is not found
        throw std::runtime_error("Destination not found for getCost");
    }

    //adjList, curCity, nextCity
    double getTime(const unordered_map<string, LinkedList*>& adjacencyList, string currentCity, string nextCity) {
        //check if the origin city exists, if it does not throw an error
        unordered_map<string, LinkedList*>::const_iterator it = adjacencyList.find(currentCity);
        if (it == adjacencyList.end()) {
            throw std::runtime_error("Origin city not found in adjacency list");
        }

        LinkedListNode* current = it->second->getHead();
        while (current != nullptr) {
            if (current->getCity() == nextCity) {
                return current->getTime();
            }
            current = current->getNext();
        }
        //errcheck
        throw std::runtime_error("Destination not found for getTime");
    }

    ///getlist
    unordered_map<string, LinkedList*> getAdjacencyList() {
        return adjacencyList;
    }
};