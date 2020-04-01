#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <stack>
#include <set>


//struct Vertex;
struct VertexI;

/*struct Child{
    Child(Vertex* child, double cost) : child(child), cost(cost) { }
    Vertex* child;
    double cost;
};

struct Vertex{
    Vertex(char name) : name(name), from(nullptr) {}
    char name;
    std::vector <Child*> children;
    double g;
    Vertex* from;
};*/

struct ChildI{
    ChildI(VertexI* child, double cost) : child(child), cost(cost) { }
    VertexI* child;
    double cost;
};

struct VertexI{
    VertexI(int name) : name(name){}
    int name;
    std::vector <ChildI*> children;
    double g;
    double f;
};

int h(VertexI* vertex1, VertexI* vertex2) { //эвр. ф. - близость символов
    return abs(vertex1->name - vertex2->name);
}

VertexI* vertexWithMinF(std::set<VertexI*> open) {
    VertexI* vertexWithMinF = nullptr;

    for (auto i : open) {
        if (!vertexWithMinF) {
            vertexWithMinF = i;
        }
        else if (vertexWithMinF->f > i->f) {
            vertexWithMinF = i;
        }
        else if (vertexWithMinF->f == i->f) {
            if (vertexWithMinF->name < i->name) {
                vertexWithMinF = i;
            }
        }
    }
    return vertexWithMinF;
}

std::string printResA(VertexI* end, std::map<VertexI*, VertexI*> from){
    std::string res = "";

    std::stack<int> stack;

    while (from.find(end) != from.end()) { //формируем стек с конца
        stack.push(end->name);
        end = from[end];
    }

    stack.push(end->name);
    int top = stack.top();
    if(top<0){
        res.push_back('-');
        top = -top;
    }
    res.push_back(top + '0');
    stack.pop();
    while (!stack.empty()) {
        top = stack.top();
        if(top<0){
            res.push_back('-');
            top = -top;
        }
        res.push_back(top + '0');
        stack.pop();
    }
    return res;
}

std::string aStar(VertexI* start, VertexI* end) {
    std::set<VertexI*> open; //раскрыты
    std::set<VertexI*> closed; //нужно обработать
    std::map<VertexI*, VertexI*> from; //необходимые
    //заполняем св-ва вершины старт
    start->g = 0; //стоимость пути от начальной
    start->f = start->g + h(start, end); //эвр. оценка

    open.insert(start); //обработана

    while (!open.empty()) {
        VertexI* current = vertexWithMinF(open); //вершина с самой низкой оценкой

        if (current == end) {
            return printResA(current, from);
        }

        open.erase(open.find(current)); //уд. из очереди
        closed.insert(current); //доб. в список обработанных

        for (auto child : current->children) { //проверяем каждого соседа
            if (closed.find(current) != closed.end()) {
                double tmpChildG = current->g + child->cost;

                if (child->child->g > tmpChildG || closed.find(child->child) == closed.end())  {
                    from[child->child] = current;
                    child->child->g = tmpChildG;
                    child->child->f = child->child->g + h(child->child, end);

                    if (open.find(child->child) == open.end()) {
                        open.insert(child->child);
                    }
                }
            }


        }
    }
    return "Путь не найден";
}


/*
std::string printResGreedy(Vertex* end)
{
    std::string res;
    Vertex* curr = end;
    std::stack<char> stack;

    while (curr) {
        stack.push(curr->name);
        curr = curr->from;
    }

    while (!stack.empty()) {
        res.push_back( stack.top());
        stack.pop();
    }
    return res;
}

std::string greedy(Vertex* start, Vertex* end){
    Vertex* curr = start;
    std::set<Vertex*> closed;

    while (curr) {
        if (curr == end) { //нашли конец
            return printResGreedy(curr);
        }

        closed.insert(curr);  //добавляем в просмотренные
        Vertex* minChild = nullptr;

        for (auto i : curr->children) { //ищем минимум
            i->child->g = i->cost;

            if (!minChild && closed.find(i->child) == closed.end()) { //если не открывали
                minChild = i->child;
            }
            else if (!minChild) {
                continue;
            }
            else if (i->child->g < minChild->g &&
                     closed.find(i->child) == closed.end()) { //проверка на минимальность
                minChild = i->child;
            }
        }

        if (minChild) { //если нашли
            minChild->from = curr;
            curr = minChild;
        }
        else {  //если не нашли, возврат назад
            curr = curr->from;
        }

        if (!curr) {
            return "Ошибка ввода";

        }
    }
    return "Выход";
}*/

int main(){
    int startVertex;
    int endVertex;

    std::cin >> startVertex >> endVertex;

    VertexI* start = new VertexI(startVertex);
    VertexI* end = new VertexI(endVertex);

    std::map<int, VertexI*> connectionMap;
    connectionMap[start->name] = start;
    connectionMap[end->name] = end;

    int firstVertex;
    int secondVertex;
    double length = 0;

    while (std::cin >> firstVertex >> secondVertex >> length) {
//       std::cin >> firstVertex >> secondVertex >> length;


        if (connectionMap.find(firstVertex) != connectionMap.end() &&
                    connectionMap.find(secondVertex) != connectionMap.end()) { //оба есть - устанавливаем связь
                    ChildI* child = new ChildI(connectionMap[secondVertex], length);
                    connectionMap[firstVertex]->children.push_back(child);
                }
                else if (connectionMap.find(firstVertex) != connectionMap.end()) {  //есть родитель - доб. ребенка, устанавл. связь
                    VertexI* node = new VertexI(secondVertex);
                    connectionMap[secondVertex] = node;

                    ChildI* child = new ChildI(connectionMap[secondVertex], length);
                    connectionMap[firstVertex]->children.push_back(child);
                }
                else if (connectionMap.find(secondVertex) != connectionMap.end()) { //есть ребенок - доб. реб., уст.связь
                    VertexI* node = new VertexI(firstVertex);
                    connectionMap[firstVertex] = node;

                    ChildI* child = new ChildI(connectionMap[secondVertex], length);
                    node->children.push_back(child);
                }
                else { //ничего нет - добавляем
                    VertexI* node = new VertexI(firstVertex);
                    connectionMap[firstVertex] = node;

                    VertexI* node2 = new VertexI(secondVertex);
                    connectionMap[secondVertex] = node2;

                    ChildI* child = new ChildI(connectionMap[secondVertex], length);
                    connectionMap[firstVertex]->children.push_back(child);
                }
    }
    /*
    Vertex* start = new Vertex(startVertex);
    Vertex* end = new Vertex(endVertex);

    std::map<int, Vertex*> connectionMap;
    connectionMap[start->name] = start;
    connectionMap[end->name] = end;

    char firstVertex;
    char secondVertex;
    double length = 0;

    while (!std::cin.eof()) {
        std::cin >> firstVertex >> secondVertex >> length;

        if (connectionMap.find(firstVertex) != connectionMap.end() &&
                        connectionMap.find(secondVertex)  != connectionMap.end()) { //оба есть - устанавливаем связь
                    Child* child = new Child(connectionMap[secondVertex], length);
                    connectionMap[firstVertex]->children.push_back(child);
                }
                else if (connectionMap.find(firstVertex) != connectionMap.end()) {  //есть родитель - доб. ребенка, устанавл. связь
                    Vertex* node = new Vertex(secondVertex);
                    connectionMap[secondVertex] = node;

                    Child* child = new Child(connectionMap[secondVertex], length);
                    connectionMap[firstVertex]->children.push_back(child);
                }
                else if (connectionMap.find(secondVertex) != connectionMap.end()) { //есть ребенок - доб. реб., уст.связь
                    Vertex* node = new Vertex(firstVertex);
                    connectionMap[firstVertex] = node;

                    Child* child = new Child(connectionMap[secondVertex], length);
                    node->children.push_back(child);
                }
                else { //ничего нет - добавляем
                    Vertex* node = new Vertex(firstVertex);
                    connectionMap[firstVertex] = node;

                    Vertex* node2 = new Vertex(secondVertex);
                    connectionMap[secondVertex] = node2;

                    Child* child = new Child(connectionMap[secondVertex], length);
                    connectionMap[firstVertex]->children.push_back(child);
                }
    }*/
    //std::string way = greedy(start,end);
    std::string way = aStar(start, end);

    std::cout << way;
}
