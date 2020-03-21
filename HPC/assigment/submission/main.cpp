#include <chrono>
#include <cmath>
#include <fstream>
#include <future>
#include <iostream>
#include <set>
#include <string>
#include <thread>
#include <vector>
#include <omp.h>
#ifdef _OPENMP
#else
    #define omp_get_thread_num() 0
#endif

class coordinate {
   public:
    coordinate(int id, double x, double y, double z) : id(id), x(x), y(y), z(z){};
    coordinate() { coordinate(-1, 0, 0, 0); };

    double euclideanDistance(const coordinate& coordinate) const {
        double x = this->x - coordinate.x;
        double y = this->y - coordinate.y;
        double z = this->z - coordinate.z;
        return sqrt(x * x + y * y + z * z);
    }

    friend std::ostream& operator<<(std::ostream& out, const coordinate& coordinate);
    friend std::istream& operator>>(std::istream& in, coordinate& coordinate);

    int id;
    double x, y, z;
};

std::istream& operator>>(std::istream& is, coordinate& coordinate) {
    is >> coordinate.x >> coordinate.y >> coordinate.z;
    return is;
}

std::ostream& operator<<(std::ostream& out, const coordinate& coordinate) {
    out << coordinate.x << " " << coordinate.y << " " << coordinate.z;
    return out;
}

using ResultGrid = std::vector<std::vector<int>>;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout
            << "Wrong file input. Use ./main <coordinates_file.txt> <result_file.txt>"
            << std::endl;
        return 0;
    }
    std::ifstream inFile;
    inFile.open(argv[1]);
    std::ofstream outFile;
    outFile.open(argv[2]);

    if (!inFile.is_open() || !outFile.is_open()) {
        std::cout << "Wrong files.please check Readme file for instructions" << std::endl;
        return 0;
    }

    int neighbors, threads, coordinates;

    std::cout << "Enter number of threads: ";
    std::cin >> threads;
    std::cout << "Enter number of  neighbors the program must find per each coordinate: ";
    std::cin >> neighbors;
    inFile >> coordinates;

    std::cout << "Start processing...Please be wait......" << std::endl;
    auto startTimeFirst = std::chrono::system_clock::now();
    int perThread = coordinates / threads;
    int lastThread = perThread + coordinates % threads;
    std::vector<int> foundThreadRows(threads, perThread);
    foundThreadRows.back() = lastThread;

    std::shared_ptr<std::vector<coordinate>> ptrcoordinates(
        new std::vector<coordinate>(coordinates));
    std::shared_ptr<ResultGrid> ptrResultGrid(new ResultGrid(coordinates));

    int coordinateIndex = 1;
    for (auto& coordinate : *ptrcoordinates) {
        inFile >> coordinate;
        coordinate.id = coordinateIndex++;
    }

    auto calcNeighbours = [ptrcoordinates, ptrResultGrid, &neighbors](
                                     int indexFrom, int indexTo) mutable {
        std::set<std::pair<double, int>> minSet;
        double distance = 0;
        int coordinateIndex = -1;
        int curNeighbor = 0;

        for (int coordinate = indexFrom; coordinate < indexTo; ++coordinate) {
            minSet.clear();

            coordinateIndex = -1;
            for (auto& coordinateTo : *ptrcoordinates) {
                ++coordinateIndex;
                if (coordinate == coordinateIndex) continue;
                distance = (*ptrcoordinates)[coordinate].euclideanDistance(coordinateTo);
                minSet.insert({distance, coordinateTo.id});
            }

            curNeighbor = 0;
            for (auto setIter = minSet.begin();
                 setIter != minSet.end() && curNeighbor < neighbors;
                 ++setIter, ++curNeighbor) {
                (*ptrResultGrid)[coordinate].push_back(setIter->second);
            }
        }
    };

    std::vector<std::future<void>> found;

    int indexFirst = 0;
    for (auto& foundThreadRow : foundThreadRows) {
        found.push_back(std::async(std::launch::async,
                                           calcNeighbours, indexFirst,
                                           indexFirst + foundThreadRow));
        indexFirst += foundThreadRow;
    }

    for (auto& future : found) {
        future.get();
    }

    for (auto& neighborsList : *ptrResultGrid) {
        for (auto& neighbors : neighborsList) {
            outFile << neighbors << " ";
        }
        outFile << std::endl;
    }

    inFile.close();
    outFile.close();
    auto final = std::chrono::system_clock::now() - startTimeFirst;
    std::cout << "Done." << std::endl;
    std::cout
        << "Time: "
        << std::chrono::duration_cast<std::chrono::milliseconds>(final).count()
        << " milliseconds" << std::endl;

    return 0;
}

// g++ -std=c++17 main.cpp -o main
