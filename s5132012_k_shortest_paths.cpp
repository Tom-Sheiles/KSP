#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

using namespace std;

// Class to define a particular instance of a node in a graph. 
class GraphNodes{
    
    private:
        int numberOfEdges;
        
    public:
        char name;
        int distance;
        pair<char, int> previous;
        std::vector <std::pair<GraphNodes*, int> > edges;
    
        GraphNodes(char Name){
            this->name = Name;
            this->numberOfEdges = 0;
            this->distance = 999;
        }
        
        void addEdge(GraphNodes* destination, int weight){
            edges.push_back(make_pair(destination, weight));
            this-> numberOfEdges += 1;
            return;
        }
        
        int getEdgeNumber(){return numberOfEdges;}
    
};


// Initializes the names and edge weights of the nodes in the graph
std::vector<GraphNodes> initalizeGraph(string inputs[], int numberOfInputs, int *nodesN, int *edgesN){
        
    int numberOfNodes = inputs[0][0] - '0';
    int numberOfEdges = inputs[0][2] - '0';
    *nodesN = numberOfNodes;
    *edgesN = numberOfEdges;
    char uniqueNodes[numberOfNodes];
    int uniqueNodesN = 0;
    bool exists = false;
    std::vector<GraphNodes> nodes;
    
    // Initializes the Graph Nodes and adds each unique node to the array
    for(int i = 1; i < numberOfInputs; i++){
        for(int j = 0; j < numberOfNodes; j++){
            
            if(inputs[i][0] == uniqueNodes[j]){
                //cout << inputs[i][0] << " exists" << endl;
                exists = true;
            }
        }
        if(!exists){
            nodes.push_back(GraphNodes(inputs[i][0]));
            uniqueNodes[uniqueNodesN] = inputs[i][0];
            uniqueNodesN++;
            cout << "Added " << inputs[i][0] << " to Graph" << endl;
        }
        exists = false;
        
        for(int j = 0; j < numberOfNodes; j++){
            
            if(inputs[i][2] == uniqueNodes[j]){
                //cout << inputs[i][2] << " exists" << endl;
                exists = true;
            }
        }
        
        if(!exists){
            nodes.push_back(GraphNodes(inputs[i][2]));
            uniqueNodes[uniqueNodesN] = inputs[i][2];
            uniqueNodesN++;
            cout << "Added " << inputs[i][2] << " to Graph" << endl;
        }
        exists = false;
    }
    
    
    // Initializes the edge weights for all nodes in the graph
    for(int i = 1; i < numberOfInputs; i++){
        GraphNodes *origin;
        GraphNodes *destination;
        for(int j = 0; j < numberOfNodes; j++){
            if(inputs[i][0] == nodes[j].name)
                origin = &nodes[j];
        }
        for(int j = 0; j < numberOfNodes; j++){
            if(inputs[i][2] == nodes[j].name)
                destination = &nodes[j];
        }
        
        origin->addEdge(destination, (inputs[i][4] - '0'));
    }
    
    return nodes;
}

// Calculate the node with the lowest distance from the origin node
GraphNodes findLowestDistance(std::vector<GraphNodes> nodes, int *index){
    
    int lowest = nodes[0].distance;
    GraphNodes lowestNode = nodes[0];
    for(int i = 0; (unsigned)i <= nodes.size() - 1; i++){
        if(nodes[i].distance <= lowest){
            lowest = nodes[i].distance;
            lowestNode = nodes[i];
            *index = i;
        }
    }
    
    return lowestNode;
}


// Locates the position of the node at location current step in the edges of the node at location nextLowest in the nodeQueue
int getEdgeNodeIndex(std::vector<GraphNodes> nodeQueue, int nextLowest, int currentStep){
    
    int index = -1;
    
    char name =  nodeQueue[nextLowest].edges[currentStep].first->name;
    for(int i = 0; (unsigned)i < nodeQueue.size(); i++){
        if(nodeQueue[i].name == name){
            index = i;
        }
    }
    
    if(index == -1){
        cout << "Could not find node " << nodeQueue[nextLowest].edges[currentStep].first->name << endl;
        exit(1);
    }
    
    return index;
}


int getNodeIndex(std::vector<GraphNodes> nodeQueue, char label){
    
    for(int i = 0; (unsigned)i < nodeQueue.size(); i++){
        if(nodeQueue[i].name == label)
            return i;
    }
    return -1;
}


// Calculates the shortest path from the source node to the target
std::vector<GraphNodes> Dijkstra(std::vector<GraphNodes> graph, GraphNodes source, GraphNodes goal, std::vector<std::vector<char> > *routeChars, int numberOfNodes, int numberOfEdges){
    
    std::vector<GraphNodes> nodeQueue;
    std::vector<GraphNodes> previous;
    std::vector<GraphNodes> route;
    std::vector<char> routeNames;
    GraphNodes nextLowest = source;
    int sourceIndex = getNodeIndex(graph, source.name);
    int goalIndex = getNodeIndex(graph, goal.name);
    int nextLowestIndex = 0;
    
    // Initialize node weights
    for(int i = 0; i < numberOfNodes; i++){
        graph[i].distance = INT_MAX;
        //graph[i].previous = NULL;
        nodeQueue.push_back(graph[i]);
    }
    
    //Initialize the source node weight to 0
    nodeQueue[sourceIndex].distance = 0;
    
    
    // While there remains unvisited nodes, find the node with the next lowest cost to move to and add that to the queue
    while(!nodeQueue.empty()){
        nextLowest = findLowestDistance(nodeQueue, &nextLowestIndex);
        route.push_back(nextLowest);
        if(nextLowest.name == goal.name){
            break;
        }
        //nodeQueue.erase (nodeQueue.begin() + nextLowestIndex);
        
        for(int i = 0; i < nextLowest.getEdgeNumber(); i++){
            
            int neighbourIndex = getEdgeNodeIndex(nodeQueue, nextLowestIndex, i);
            
            if(nodeQueue[neighbourIndex].distance > (nextLowest.distance + nextLowest.edges[i].second)){
                
                nodeQueue[neighbourIndex].distance = (nextLowest.distance + nextLowest.edges[i].second);
                nodeQueue[neighbourIndex].previous.first = nextLowest.name;
                nodeQueue[neighbourIndex].previous.second = nextLowest.edges[i].second;
            }
        } 
        nodeQueue.erase (nodeQueue.begin() + nextLowestIndex);
    }
    
    goalIndex = getNodeIndex(route, goal.name);
    routeNames.push_back(route[goalIndex].name);
    GraphNodes next = route[goalIndex];
    
    while(true){
        next = route[getNodeIndex(route, next.previous.first)];
        routeNames.push_back(next.name);
        
        if(next.name == source.name)
            break;
    }
    std::reverse(routeNames.begin(), routeNames.end());
    routeChars->push_back(routeNames);
    
    return route;
    
}

std::vector<char> slice(std::vector<char> input, int start, int end){
    
    vector<char>::const_iterator first = input.begin() + start;
    vector<char>::const_iterator last = input.begin() + end + 1;
    
    std::vector<char> sliced(first, last);
    return sliced;
}


std::vector<GraphNodes> removeNextEdge(std::vector<GraphNodes> graph, char spur, std::vector<char> route){
    
    int spurIndex;
    int nextIndex;
    char nextName;
    
    for(int i = 0; i < route.size(); i++){
        if(spur == route[i]){
             spurIndex = i;
             nextIndex = spurIndex + 1;
             nextName = route[nextIndex];
        }
    }
    
    spurIndex = getNodeIndex(graph, spur);
    
    for(int i = 0; i < graph[spurIndex].edges.size(); i++){
        if(graph[spurIndex].edges[i].first->name == nextName)
            nextIndex = i;
    }
    
    graph[spurIndex].edges[nextIndex].second = INT_MAX;
    
    return graph;
}


float findPathLength(std::vector<char> route, std::vector<GraphNodes> graph){
    
    /*for(int i = 0; (unsigned)i < route.size(); i++){
        
    }*/
    
}


void KSP(std::vector<GraphNodes> nodes, int numberOfNodes, int numberOfEdges, int k){
    
    int i = 1;
    i += 2;
    
    std::vector<std::vector<char> > fastestRoutes;
    // TODO: Update the init function to get the source and sink and k
    std::vector<GraphNodes> fastestRoute = Dijkstra(nodes, nodes[0], nodes[5], &fastestRoutes, numberOfNodes, numberOfEdges);
    std::vector<GraphNodes> nextFastestRoutes;
    
    std::vector<GraphNodes> next = nodes;
    std::vector<std::vector<char> > nextRouteName;
    float nextRoutesSizes[INT_MAX];

    // Calculate the k shortest paths using yens algorithm
    for(int i = 1; i < k; i++){
        for(int j = 0; (unsigned)j < fastestRoutes[i - 1].size() - 1; j++){
            std::vector<GraphNodes> graphCopy = nodes;

            char spurNode = fastestRoutes[i - 1][j];
            std::vector<char> rootPath = slice(fastestRoutes[i - 1], 0, j - 1);
            
            graphCopy = removeNextEdge(graphCopy, spurNode, fastestRoutes[0]);
            /*for(int p = 0; (unsigned)p < fastestRoutes.size(); p++){
                if(rootPath == slice(fastestRoutes[p], 0, j)){
                    graphCopy[p].edges[j].second = INT_MAX;
                }
            }*/
            
            GraphNodes spurNodeObject = graphCopy[getNodeIndex(graphCopy, spurNode)];
            next = Dijkstra(graphCopy, spurNodeObject, nodes[5], &nextRouteName, numberOfNodes, numberOfEdges);
            nextRouteName[j].insert(nextRouteName[j].begin(), rootPath.begin(), rootPath.end());
             /* for each path p in A: 
              * // Remove the links that are ppart of the previous shortest paths which share the same root
              *     if rootPath = p.nodes(0, j)
              *         remove p.edge(i, i+1)*/
             
            /* spurpath = dijk(spurNode, goal)
             * totalpath = rootpath + spurPath 
             * nextFastes.append(totalPath)*/
        }
        //for(int k = 0; (unsigned)k < nextRouteName.size() - 1; k++){
            //nextRoutesSizes[k] = findPathLength(nextRouteName[k], nodes);
       // }
       // cout << "";
    }
    
    return;
}


int main(int argc, char *argv[]){
    
    string fileName;
    
    if(argc > 1){
        fileName = argv[1];
        fileName = fileName.append("\\input.txt");
        cout << fileName << endl;
        
    }else{cout << "No valid file input" << endl;}
    
    ifstream file(fileName.c_str());
    int numberOfInputs = 0;

    // Retrieve size of input file
    if(file.is_open()){
        string line;
        while(getline(file, line))
            ++numberOfInputs;
        file.clear();
        file.seekg(0);
    }else{
        cout << "Unable to open file" << endl;
    }
    
    // Retrieve data from file
    string inputArray[numberOfInputs];
    string line;
    int i = 0;
    while(getline(file, line)){
        inputArray[i] = line;
        i++;
    }
    
    for(int i = 0; i < numberOfInputs; i++){
        cout << inputArray[i] << endl;
    }
    
    int numberOfNodes = 0;
    int numberOfEdges = 0;
    std::vector<GraphNodes> nodes = initalizeGraph(inputArray, numberOfInputs, &numberOfNodes, &numberOfEdges);
    std::vector<GraphNodes> test;
    KSP(nodes, numberOfNodes, numberOfEdges, 3);
        
    file.close();
}