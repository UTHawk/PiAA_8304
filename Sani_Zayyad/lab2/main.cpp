#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <set>

struct VertexInfo {
    std::vector<std::pair<char, int>> neighbors;
    int length;
};

bool cmp(const std::pair<char,int> &a, const std::pair<char,int> &b) {
    if(a.second == b.second) return a.first > b.first;
    return a.second < b.second;
}

int G(char from, std::map<char, VertexInfo> &graphDict) {
    return graphDict[from].length;
}

int H(char from, char endvertex) {
    return abs(from - endvertex);
}

int F(char from, char endvertex, std::map<char, VertexInfo>& graphDict){
    return G(from, graphDict) + H(from, endvertex);
}

char minVertex(std::set<char> & open, char endvertex, std::map<char, VertexInfo>& graphDict) {
    int min = INT_MAX;
    char curr = 0;
    for(char i: open)
        if(F(i, endvertex, graphDict) < min){
            min = F(i, endvertex, graphDict);
            curr = i;
        }
    return curr;
}

std::string constructPath(std::map<char,char> &from,char startVertex, char endVertex) {
    std::string path = {endVertex};
    char curr = endVertex;
    while (curr != startVertex) {
        curr = from[curr];
        path += curr;
    }
    std::reverse(path.begin(), path.end());
    return path;
}

void astarAlgo(char startVertex, char endVertex,std::map<char, VertexInfo>& graphDict) {
    char currentVertex = startVertex;
    std::set<char> closed;
    std::set<char> open = {startVertex};
    std::map<char,char> from;
    
    while(!open.empty()) {

        
        std::cout << "* поиск необработанной вершины для проведения релаксации *" << std::endl;
        currentVertex = minVertex(open,endVertex,graphDict);
        std::cout << "* вершина для релаксации -- <" <<currentVertex << "> *" << std::endl;

        
        //currentvertex = end , algorithm ends
        if(currentVertex == endVertex){//endGame
            std::cout << "_________________________________" << std::endl;
            std::cout << "Optima path : ";
            std::cout << constructPath(from, startVertex, endVertex) << std::endl;
            return;
        }
        
        open.erase(currentVertex);
        closed.insert(currentVertex);
        
        std::cout << "* релаксация ребер, исходящих из вершины *" << std::endl;
        for(auto neighbour : graphDict[currentVertex].neighbors) {
            bool tentative_is_better;
            if(closed.find(neighbour.first) != closed.end())//if it has been visited
                continue;
            
            std::cout << " ** путь из "<< currentVertex << " в " << neighbour.first << " ** " << std::endl;
            
            int tentative_g_score = G(currentVertex, graphDict) + neighbour.second;
            if(open.find(neighbour.first) == open.end()){
                open.insert(neighbour.first);
                tentative_is_better = true;
            }
            else {
                tentative_is_better = tentative_g_score < G(neighbour.first, graphDict);
            }
            
            if(tentative_is_better) {
                from[neighbour.first] = currentVertex;
                std::cout << " ** успешная релаксация: новая длина -> " << tentative_g_score << " ** " << std::endl;
                graphDict[neighbour.first].length = tentative_g_score;
            }
            std::cout << "* конец релаксации *" << std::endl;
        }
    }
    std::cout << "No way" << std::endl;
}
bool monotone(std::map<char, VertexInfo> vertices,char endVertex){
    for(auto vertex : vertices){
        char vertice1 = vertex.first;
        for (int i = 0 ; i < vertex.second.neighbors.size(); ++i) {
            char vertice2 = vertex.second.neighbors[i].first;
            double weight = vertex.second.neighbors[i].second;
            if(abs(endVertex-vertice1) - abs(endVertex-vertice2) > weight){
                return false;
            }
        }
    }
    return true;
}

int main() {
    char startVertex, endVertex;
    
    std::cin >> startVertex >> endVertex;
    std::map<char, VertexInfo> graphDict;
    
    char from, to;
    float length;
    while(std::cin >> from >> to >> length) {
        if(length == -1)
            break;
        graphDict[from].neighbors.push_back({to,length});
    }
        
    
    auto time = clock();
    std::cout << "_________AStar Algorith with Monotone function________" << std::endl;
    astarAlgo(startVertex, endVertex, graphDict);
    if(monotone(graphDict, endVertex)){
        std::cout<< "The heuristic function of the graph is Monotone\n";
        
    }else{
        std::cout << "Not Monotone\n";
    }
    std::cout << "Time taken : " << (double)(clock() - time) / CLOCKS_PER_SEC << std::endl;
    std::cout << "Algorith complexity G(V,E) : O(|V| * |V| + E) ~ O(V * V)" << std::endl;
    std::cout << "where V - Vertices and E - edges" << std::endl;
    return 0;
}
