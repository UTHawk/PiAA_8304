#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>

char begin, end;

struct Edge {
    char end{}; //куда входит ребро
    int bandwidth{}; //max пропуск. способность
    int flow{}; //поток в сети
    bool is_reverse = false; //обратное ли
};


struct Vertex {  //здесь все дети вершины
    std::vector<Edge> destinations;
};

std::map<char, Vertex> dictOfVertex;  //словарь вершин
std::map<char, bool> visited;

void input(){
    int N;

    std::cin >> N; //кол-во ориентированных ребер графа

    std::cin >> begin;  //исток

    std::cin >> end;  //сток

    Vertex tmp = Vertex();
    char startVertex, endVertex;
    int length;
    Edge edge{};

    for(int i = 0; i < N; ++i){
        std::cin >> startVertex >> endVertex >> length;

        bool f = false;
        for(auto& k : dictOfVertex[startVertex].destinations) {   //прохождение по всем детям вершины
            if(k.end == endVertex ){   //если уже есть конец
                k = {endVertex, length, 0, false};
                f = true;
                break;
            }
        }
        if(f) //все заполнено
            continue;

        if(dictOfVertex.find(startVertex) == dictOfVertex.end()) { //если нет начала
            edge = {endVertex, length, 0};
            tmp.destinations.push_back(edge);  //доб. ребро в массив детей вершины
            dictOfVertex[startVertex] = tmp; // вершину доб. в словарь вершин
            visited[startVertex] = false;
        }
        else { //если есть начало
            edge = {endVertex, length, 0};
            dictOfVertex[startVertex].destinations.push_back(edge); //добавляем ребро к уже существ.
        }

        tmp.destinations.clear();

        if(startVertex != begin) { //если не исток
            if(dictOfVertex.find(endVertex) == dictOfVertex.end()) {  //если нет конца
                edge = {startVertex, length, 0, true};
                tmp.destinations.push_back(edge);  //доб. ребро в массив детей вершины
                dictOfVertex[endVertex] = tmp;  // вершину доб. в словарь вершин
                visited[startVertex] = false;
            }
            else{  //если конец есть
                edge = {startVertex, length, 0, true};
                dictOfVertex[startVertex].destinations.push_back(edge);  //добавляем ребро к уже существ.
            }
            tmp.destinations.clear();

        }
    }

    for(auto& i : dictOfVertex) {
        std::sort(i.second.destinations.begin(), i.second.destinations.end(), [](Edge e1, Edge e2){return e1.bandwidth < e2.bandwidth;});
    }
}



void input(std::string argv) {
    int N;
    std::ifstream fin;
    fin.open(argv);

    fin >> N; //кол-во ориентированных ребер графа

    fin >> begin;  //исток

    fin >> end;  //сток

    Vertex tmp = Vertex();
    char startVertex, endVertex;
    int length;
    Edge edge{};

    for(int i = 0; i < N; ++i){
        fin >> startVertex >> endVertex >> length;

        bool f = false;
        for(auto& k : dictOfVertex[startVertex].destinations) {   //прохождение по всем детям вершины
            if(k.end == endVertex ){   //если уже есть конец
                k = {endVertex, length, 0, false};
                f = true;
                break;
            }
        }
        if(f) //все заполнено
            continue;

        if(dictOfVertex.find(startVertex) == dictOfVertex.end()) { //если нет начала
            edge = {endVertex, length, 0};
            tmp.destinations.push_back(edge);  //доб. ребро в массив детей вершины
            dictOfVertex[startVertex] = tmp; // вершину доб. в словарь вершин
            visited[startVertex] = false;
        }
        else { //если есть начало
            edge = {endVertex, length, 0};
            dictOfVertex[startVertex].destinations.push_back(edge); //добавляем ребро к уже существ.
        }

        tmp.destinations.clear();

        if(startVertex != begin) { //если не исток
            if(dictOfVertex.find(endVertex) == dictOfVertex.end()) {  //если нет конца
                edge = {startVertex, length, 0, true};
                tmp.destinations.push_back(edge);  //доб. ребро в массив детей вершины
                dictOfVertex[endVertex] = tmp;  // вершину доб. в словарь вершин
                visited[startVertex] = false;
            }
            else{  //если конец есть
                edge = {startVertex, length, 0, true};
                dictOfVertex[startVertex].destinations.push_back(edge);  //добавляем ребро к уже существ.
            }
            tmp.destinations.clear();

        }
    }

    for(auto& i : dictOfVertex) {
        std::sort(i.second.destinations.begin(), i.second.destinations.end(), [](Edge e1, Edge e2){return e1.bandwidth < e2.bandwidth;});
    }
}



int bfs(char currVertex /*текущая вершина*/, int c_min /*минимальный поток в сети*/, std::string tab, std::ostream& fout) {
    fout<<tab<<"Текущая вершина "<<currVertex<<"\n";
    if(currVertex == end){ //конец
        fout<<tab<<"Текущая вершина - сток\n";
        return c_min;
    }

    visited[currVertex] = true; //обознач. посещенной
    fout<<tab<<"Цикл по детям "<<currVertex<<"\n";
    for(auto& edge : dictOfVertex[currVertex].destinations) {
        fout<<tab<<"Ребро ведет в "<<edge.end<<"; П/С пути "<< edge.bandwidth << "; поток через ребро "<< edge.flow <<"\n";
        if(!visited[edge.end]){
            fout<<tab<<"Ребро не посещенное\n";
        }else{
            fout<<tab<<"Ребро посещенное\n";
        }
        if (edge.end != currVertex) { //не цикл
            if (!visited[edge.end] && (edge.flow < edge.bandwidth && !edge.is_reverse)) {
    //если не посещенная и поток в сети меньше макс. пропуск. способности и не обратное
                fout<<tab<<"Ребро не обратное\n";
                int flow = bfs(edge.end, std::min(c_min, edge.bandwidth - edge.flow), tab+" ", fout);
                //запуск. функц. с этим ребенком и мин. из тек. мин. потока в сети и разностью между макс. пропуск. способ. и потоком.
                if (flow > 0) {
                    edge.flow += flow;
                    fout<<tab<<"Возвращаемое значение "<< flow <<"\n";
                    return flow;
                }
            } else if (!visited[edge.end] && (edge.is_reverse && edge.flow > 0)) {
                //если не посещенная, обратная и поток больше 0
                fout<<tab<<"Ребро обратное\n";
                int flow = bfs(edge.end, std::min(c_min, edge.flow), tab+" ", fout);
                //запуск. функц. с этим ребенком и минимумом из тек. мин. потока в сети и потоком.
                if (flow > 0) {
                    edge.flow -= flow;
                    fout<<tab<<"Возвращаемое значение "<< flow <<"\n";
                    return flow;
                }
            }
        }
    }
    return 0;
}

void output(int flow){
    std::cout << flow << std::endl;
    for(auto& i : dictOfVertex) {
        for(auto& j : i.second.destinations) {
            if(!j.is_reverse)
                std::cout << i.first << " " << j.end << " " << j.flow << std::endl;
        }
    }
}
void output(int flow, std::string argv){
    std::ofstream fout;
    fout.open(argv, std::ios_base::out | std::ios_base::app);
    fout << flow << std::endl;
    for(auto& i : dictOfVertex) {
        for(auto& j : i.second.destinations) {
            if(!j.is_reverse)
                fout << i.first << " " << j.end << " " << j.flow << std::endl;
        }
    }
}


int main(int argc, char *argv[]) {
    if(argv[1]!=NULL && (std::string(argv[1]) != "res.txt")){
        input(std::string(argv[1])); //из файла
    }else if(argv[1]==NULL || (std::string(argv[1]) == "res.txt")){
        input(); //из консоли
    }


    if(argv[1]==NULL){
        int flow = 0;
        int iterationResult = 0;
        //std::cout<<"Максимальный поток " <<flow<< "\n";
        while (true) {
            iterationResult = bfs(begin, 1000, " ", std::cout);
            if(iterationResult <= 0)
                break;
            for (auto& i : visited)
                i.second = false;
            flow += iterationResult;
            //std::cout<<"Максимальный поток " <<flow<< "\n";

        }

        for(auto& i : dictOfVertex) {
            std::sort(i.second.destinations.begin(), i.second.destinations.end(), [](Edge e1, Edge e2){return e1.end < e2.end;});
        }
        output(flow);

    }
    else if(std::string(argv[1]) == "res.txt"){
        std::ofstream fout;
        fout.open(argv[1]);
        int flow = 0;
        int iterationResult = 0;
        //std::cout<<"Максимальный поток " <<flow<< "\n";
        while (true) {
            iterationResult = bfs(begin, 1000, " ", fout);
            if(iterationResult <= 0)
                break;
            for (auto& i : visited)
                i.second = false;
            flow += iterationResult;
            //std::cout<<"Максимальный поток " <<flow<< "\n";

        }
        fout.close();
        for(auto& i : dictOfVertex) {
            std::sort(i.second.destinations.begin(), i.second.destinations.end(), [](Edge e1, Edge e2){return e1.end < e2.end;});
        }
        output(flow, std::string(argv[1]));
    }else{
        if(argv[2]==NULL){
            int flow = 0;
            int iterationResult = 0;
            //std::cout<<"Максимальный поток " <<flow<< "\n";
            while (true) {
                iterationResult = bfs(begin, 1000, " ", std::cout);
                if(iterationResult <= 0)
                    break;
                for (auto& i : visited)
                    i.second = false;
                flow += iterationResult;
                //std::cout<<"Максимальный поток " <<flow<< "\n";

            }

            for(auto& i : dictOfVertex) {
                std::sort(i.second.destinations.begin(), i.second.destinations.end(), [](Edge e1, Edge e2){return e1.end < e2.end;});
            }
            output(flow);
        }
        if(argv[2]!=NULL){
            std::ofstream fout;
            fout.open(argv[2]);
            int flow = 0;
            int iterationResult = 0;
            //std::cout<<"Максимальный поток " <<flow<< "\n";
            while (true) {
                iterationResult = bfs(begin, 1000, " ", fout);
                if(iterationResult <= 0)
                    break;
                for (auto& i : visited)
                    i.second = false;
                flow += iterationResult;
                //std::cout<<"Максимальный поток " <<flow<< "\n";

            }

            for(auto& i : dictOfVertex) {
                std::sort(i.second.destinations.begin(), i.second.destinations.end(), [](Edge e1, Edge e2){return e1.end < e2.end;});
            }
            fout.close();
            output(flow, std::string(argv[2]));
        }
    }

    return 0;
}
