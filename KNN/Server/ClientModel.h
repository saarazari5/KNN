
#include "KNN/Flower.h"
#include "KNN/Distance.hpp"

#ifndef ClientModel_h
#define ClientModel_h


struct ClientModel {
    explicit ClientModel(int identifier):identifier(identifier){};
    int identifier;
    std::vector<Flower> trainFlower;
    std::vector<Flower> testFlower;
    std::vector<std::string> k_closets;
    
    int k = 5;
    distanceType type = euclidian;
};

#endif /* ClientModel_h */
