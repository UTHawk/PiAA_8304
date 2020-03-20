#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

class Path {
public:
    Path(char nameFrom, char nameOut, double weightPath) : nameFrom(nameFrom), nameOut(nameOut),
                                                           weightPath(weightPath) {}

private:
    char nameFrom;
    char nameOut;
    double weightPath;
public:
    char getNameFrom() const {
        return nameFrom;
    }

    char getNameOut() const {
        return nameOut;
    }

    double getWeightPath() const {
        return weightPath;
    }
};

class Point {
public:
    Point(char name, double weight) : name(name), weight(weight) {
        secondWeight = -1;
        fromName = '!';
    }

private:
    char name;
    double weight;
    double secondWeight;
    char fromName;
public:
    char getName() const {
        return name;
    }

    double getWeight() const {
        return weight;
    }


    void setWeight(double weight) {
        Point::weight = weight;
    }

    void setSecondWeight(double secondWeight) {
        Point::secondWeight = secondWeight;
    }

    double getSecondWeight() const {
        return secondWeight;
    }

    char getFromName() const {
        return fromName;
    }

    void setFromName(char fromName) {
        Point::fromName = fromName;
    }

};

bool comp(Point a, Point b) {
        return a.getSecondWeight() < b.getSecondWeight();
}

void prepareAnswer(std::vector<Point> *vector, std::vector<char> *answer, char name) {
    for (Point point: *vector) {
        if(point.getName() == name){
            answer->emplace_back(name);
            prepareAnswer(vector, answer, point.getFromName());
            break;
        }
    }
}


bool ckeckMonotony(std::vector<Path> *paths, char endPoint){
    for (Path path: *paths) {
        char sym1 = path.getNameFrom();
        char sym2 = path.getNameOut();
        if(abs(endPoint-sym1)-abs(endPoint-sym2) > path.getWeightPath()){
            return false;
        }
    }
    return true;
}


int main() {
    std::vector<Path> vectorPath;
    vectorPath.reserve(0);
    std::vector<Point> vectorPoints;
    vectorPoints.reserve(0);

    char startPoint;
    char endPoint;

    std::cin >> startPoint;
    std::cin >> endPoint;

    char start, end;
    double weight;


    while (std::cin >> start >> end >> weight) {
        vectorPath.emplace_back(Path(start, end, weight));
    }

    std::set<char> set;

    set.insert(startPoint);
    vectorPoints.emplace_back(Point(startPoint, 0));

    for (Path path: vectorPath) {
        char from = path.getNameFrom();
        char out = path.getNameOut();
        if (set.find(from) == set.end()) {
            set.insert(from);
            vectorPoints.emplace_back(Point(from, -1));
        }
        if (set.find(out) == set.end()) {
            set.insert(out);
            vectorPoints.emplace_back(Point(out, -1));
        }
    }


    std::cout << "\nIs the function monotonous ? Answer: ";
    if(ckeckMonotony(&vectorPath, endPoint)) {
        std::cout << "Yes\n";
    } else{
        std::cout << "No\n";
    }


    std::vector<Point> queue;
    queue.reserve(0);
    queue.emplace_back(vectorPoints[0]);


    while (!queue.empty()) {
        char myPoint = queue[0].getName();
        double weightMyPoint = queue[0].getWeight();

        std::vector<Path> paths;
        paths.reserve(0);

        for (Path path: vectorPath) {
            if (path.getNameFrom() == myPoint) {

                paths.emplace_back(path);
            }
        }


        for (Path path : paths) {

            for (auto &point : vectorPoints) {
                if (path.getNameOut() == point.getName()) {

                    double sum = path.getWeightPath() + weightMyPoint;
                    double priority = sum + abs(endPoint - point.getName());
                    double weight2 = point.getWeight();

                    if (point.getWeight() == -1 || point.getWeight() > (sum)) {
                        point.setWeight(sum);
                        point.setSecondWeight(sum + abs(endPoint - point.getName()));
                        point.setFromName(myPoint);
                        queue.emplace_back(point);
                        //point.setFromPoint(myPoint);
                    }
                    break;
                }
            }
        }

        queue.erase(queue.begin());
        std::sort(queue.begin(), queue.end(), comp);

    }

    std::vector<char> answer;
    answer.resize(0);

    prepareAnswer(&vectorPoints, &answer, endPoint);

    std::reverse(answer.begin(),answer.end());
    for (char sym : answer) {
        std::cout << sym;
    }
}

