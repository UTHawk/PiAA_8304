#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>


struct VertexInfo{
    int capacity;
    int flow;
};

struct Vertex {
    std::vector<std::pair<char, VertexInfo>> destinations;
    bool visited;
    char parentVertex;
};


char selectAvailabevertex(std::vector<std::pair<char, VertexInfo>> const& destinations,std::map<char, Vertex> graph){
    char availableVertex = 0;
    
    for(auto i : destinations){
        if(!graph[i.first].visited && i.second.capacity > 0) {
            availableVertex = i.first;
        }
    }
    return availableVertex;
}

std::string dfsAugmentedPath(std::map<char, Vertex> graph, char source, char sink){
    char currentVertex = source;
    std::string path = {source};
    
    char selectedVertex = selectAvailabevertex(graph[currentVertex].destinations, graph);
    //iterative DFS
    while(currentVertex != sink && !(currentVertex == source && selectedVertex == 0)){
        
        std::cout << "* Vertex to transverse * -> " << currentVertex << std::endl;

        graph[currentVertex].visited = true;
        
        if(selectedVertex != 0) {
            char parentVertex = currentVertex;
            currentVertex = selectedVertex;
            graph[currentVertex].parentVertex = parentVertex;
            path += currentVertex;
        }
        else {
            path.erase(path.end()-1);
            currentVertex = graph[currentVertex].parentVertex;
        }
        
        selectedVertex = selectAvailabevertex(graph[currentVertex].destinations, graph);
        if(currentVertex == sink)
            std::cout << "* Vertex to transverse * -> " << currentVertex << std::endl;

    }
    
    return path;
}


int minCapacity(std::string augPath, std::map<char, Vertex> residualGraph){
    int min = INT_MAX;
    
    while(augPath.size() > 1){
        for(auto vertex : residualGraph[augPath[0]].destinations)
            if(vertex.first == augPath[1] && vertex.second.capacity < min) {
                min = vertex.second.capacity;
            }
        augPath.erase(augPath.begin());
    }
    
    return min;
}

void updateResidualGraph(std::string augPath, std::map<char, Vertex>& residualGraph){
    int min = minCapacity(augPath,residualGraph);
    std::string path = augPath;
    std::cout << "## Min Capacity in path* -> " << min << "\n";

    while(augPath.size() > 1){
        for(auto& vertex : residualGraph[augPath[0]].destinations)
            if(vertex.first == augPath[1]) {
                vertex.second.flow += min;
                vertex.second.capacity -= vertex.second.flow;
                
                for(auto& j:residualGraph[augPath[1]].destinations) {
                    if(j.first == augPath[0]) {
                        j.second.flow -= min;
                        j.second.capacity -= j.second.flow;
                    }
                }
            }
        augPath.erase(augPath.begin());
    }
    std::cout << "Aug path : " << path << " updated successfully******\n\n";

    
}

bool cmp(std::pair<char, VertexInfo> const& a, std::pair<char, VertexInfo> const& b) {
    return a.first < b.first;
}

void maxFlow(std::map<char, Vertex> graph, char source, char sink){
    
    std::string augPath = dfsAugmentedPath(graph,source,sink);

    while (augPath != std::string(1,source)) {
        std::cout<<"Updating Augmented path: "<<augPath<<"\n";
        updateResidualGraph(augPath,graph);
        augPath = dfsAugmentedPath(graph,source,sink);
    }
    
    //max flow going out from source = the flow coming to sink
    int maxFlowOut = 0;
    for(auto i : graph[source].destinations){
        maxFlowOut += i.second.flow;
    }
    std::cout << maxFlowOut <<std::endl;
    
    for(auto vertex : graph) {
         std::sort(vertex.second.destinations.begin(),vertex.second.destinations.end(),cmp);
        for(auto dest : vertex.second.destinations)
            std::cout << vertex.first << " " << dest.first << " " << std::max(0,dest.second.flow) << std::endl;
     }

}

int main(){
    
    int N;
    char source,sink;
    std::map<char, Vertex> residualGraph;
    
    std::cin >> N;
    std::cin >>source;
    std::cin>> sink;
    char from,to;
    int capacity;
    for(int i = 0; i < N; ++i) {
        int flow = 0;
        std::cin >> from >> to >> capacity;
        residualGraph[from].destinations.push_back({to,{capacity,flow}});
    }
    
    maxFlow(residualGraph,source,sink);
    
    return 0;
}
