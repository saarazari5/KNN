
#ifndef Distance_hpp
#define Distance_hpp

#include <cstdio>
#include <utility>
#include <vector>
#include <string>
#include <cmath>


enum distanceType {
     chebyshev,
     canberra,
     minkowski,
     euclidian,
     manhattan
};

class Distance {
protected:
    std::vector<double> vectorA, vectorB;
    Distance(std::vector<double> vectorA, std::vector<double> vectorB) {
        this -> vectorA = std::move(vectorA);
        this -> vectorB = std::move(vectorB);
    }
public:
    void set_vectorB(std::vector<double> vector) {
        this -> vectorB = std::move(vector);
    }

public:
    static Distance* from(distanceType type, const std::vector<double>& vectorA, const std::vector<double>& vectorB);
    static distanceType map(const std::string& s);
    static std::string map(distanceType type);
    virtual double calculate_distance() = 0;
};




class MinkowskiDistance: public Distance {
public:
    int p;
public:
    MinkowskiDistance(std::vector<double> vectorA, std::vector<double> vectorB, int p) : Distance(std::move(vectorA),
                                                                                                  std::move(vectorB)) {
        this->p = p;
    };

    double calculate_distance() override {
        double res = 0;
        for(int i = 0 ; i < vectorA.size() ; i ++) {
            res+=  pow(abs(vectorA.at(i) - vectorB.at(i)),p);
        }
        return pow(res, 1.0/p);
    }
};

class EuclidianDistance: public MinkowskiDistance {
public:
    EuclidianDistance(std::vector<double> vectorA, std::vector<double> vectorB) : MinkowskiDistance(std::move(vectorA),
                                                                                                    std::move(vectorB), 2) {}
    double calculate_distance() override {
        return MinkowskiDistance::calculate_distance();
    }
};

class CanberraDistance: public Distance {
public:
    CanberraDistance(std::vector<double> vectorA, std::vector<double> vectorB) : Distance(std::move(vectorA),
                                                                                          std::move(vectorB)) {}
    double calculate_distance() override {
        double res=0;
        for(int i = 0 ; i < vectorA.size() ; i ++) {
            double diff =  abs(vectorA.at(i) - vectorB.at(i));
            double divide = abs(vectorA.at(i)) + abs(vectorB.at(i));
            res += !divide ? 0 : diff / divide;
        }
        return res;
    }
};

class ManhattanDistance: public MinkowskiDistance {
public:
    ManhattanDistance(std::vector<double> vectorA, std::vector<double> vectorB) : MinkowskiDistance(std::move(vectorA),
                                                                                                    std::move(vectorB), 1) {}
    double calculate_distance() override {
        return MinkowskiDistance::calculate_distance();
    }
};

class ChebyshevDistance: public Distance {

public:
    ChebyshevDistance(std::vector<double> vectorA, std::vector<double> vectorB) : Distance(std::move(vectorA), std::move(vectorB)) {}
    double calculate_distance() override {
        double max = 0;
        //find the max abs between the 2 vectors.
        for(int i = 0 ; i < vectorA.size() ; i ++) {
            max = std::max(max,(double)abs(vectorA.at(i) - vectorB.at(i)));
        }
        return  max;
    }
};


Distance* Distance::from(distanceType type, const std::vector<double>& vectorA,
                         const std::vector<double>& vectorB) {
    switch (type) {
        case chebyshev:
            return new ChebyshevDistance(vectorA, vectorB);
            break;
        case canberra:
            return new CanberraDistance(vectorA, vectorB);
            break;
        case minkowski:
            return new MinkowskiDistance(vectorA, vectorB, 2);
            break;
        case euclidian:
            return new EuclidianDistance(vectorA, vectorB);
            break;
        case manhattan:
            return new ManhattanDistance(vectorA, vectorB);
            break;
    }
    return nullptr;
}

distanceType Distance::map(const std::string& s) {
    if(s == "AUC")
    {
        return euclidian;
    }
    else if(s == "MAN")
    {
        return manhattan;
    }
    else if(s == "CHB")
    {
        return chebyshev;
    }
    else if(s == "CAN")
    {
        return canberra;
    }

    return minkowski;
}

std::string Distance::map(distanceType type) {

    switch (type) {
        case chebyshev:
            return "CHB";
            break;
        case canberra:
            return "CAN";
            break;
        case minkowski:
            return "MIN";
            break;
        case euclidian:
            return "AUC";
            break;
        case manhattan:
            return "MAN";
            break;
    }

}

#endif /* Distance_hpp */

