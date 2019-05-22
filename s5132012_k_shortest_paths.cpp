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
    for(int i = 0; (unsigned)i < nodes.size() - 1; i++){
        if(nodes[i].distance < lowest){
            lowest = nodes[i].distance;
            lowestNode = nodes[i];
            *index = i;
        }
    }
    
    return lowestNode;
}


// Locates the position of the node at location current step in the edges of the node at location nextLowest in the nodeQueue
int getNodeIndex(std::vector<GraphNodes> nodeQueue, int nextLowest, int currentStep){
    
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


// Calculates the shortest path from the source node to the target
void Dijkstra(std::vector<GraphNodes> graph, GraphNodes source, int numberOfNodes, int numberOfEdges){
    
    std::vector<GraphNodes> nodeQueue;
    std::vector<GraphNodes> previous;
    std::vector<GraphNodes> route;
    GraphNodes nextLowest = source;
    int nextLowestIndex = 0;
    
    // Initialize node weights
    for(int i = 0; i < numberOfNodes; i++){
        graph[i].distance = INT_MAX;
        //graph[i].previous = NULL;
        nodeQueue.push_back(graph[i]);
    }
    
    //Initialize the source node weight to 0
    nodeQueue[0].distance = 0;
    
    
    // While there remains unvisited nodes, find the node with the next lowest cost to move to and add that to the queue
    while(!nodeQueue.empty()){
        nextLowest = findLowestDistance(nodeQueue, &nextLowestIndex);
        route.push_back(nextLowest);
        //nodeQueue.erase (nodeQueue.begin() + nextLowestIndex);
        
        for(int i = 0; i < nextLowest.getEdgeNumber(); i++){
            
            int neighbourIndex = getNodeIndex(nodeQueue, nextLowestIndex, i);
            
            if(nodeQueue[neighbourIndex].distance > (nextLowest.distance + nextLowest.edges[i].second)){
                
                nodeQueue[neighbourIndex].distance = (nextLowest.distance + nextLowest.edges[i].second);
                nodeQueue[neighbourIndex].previous.first = nextLowest.name;
                nodeQueue[neighbourIndex].previous.second = nextLowest.edges[i].second;
            }
            
            /*int alternate = nextLowest.distance + nextLowest.edges[i].second;
            
            if(alternate < nextLowest.edges[i].first->distance){
                nextLowest.edges[i].first->distance = alternate;
                previous.push_back(nextLowest);*/
            
        } 
        nodeQueue.erase (nodeQueue.begin() + nextLowestIndex);
    }
    
    cout << "finished" << endl;
    
}


void KSP(std::vector<GraphNodes> nodes, int numberOfNodes, int numberOfEdges){
    
    Dijkstra(nodes, nodes[0], numberOfNodes, numberOfEdges);
    
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
    KSP(nodes, numberOfNodes, numberOfEdges);
        
    file.close();
}