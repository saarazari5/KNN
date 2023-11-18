

#ifndef KNN_hpp
#define KNN_hpp
#include <map>
#include "../ClientModel.h"
#include <vector>
#include <cstdio>
#include <algorithm>

class KNN {
public:
    KNN();
    std::vector<std::string> start_knn(ClientModel* client);
private:
    std::map<std::string,int> k_closets;
    
    void initialize_k_closets() {
        k_closets.clear();
    }
    
    std::string knn(const Flower& flower,int k, distanceType type ,std::vector<Flower> data);
};

using namespace std;
KNN::KNN() {
    initialize_k_closets();
}

std::vector<std::string> KNN::start_knn(ClientModel* client) {
    vector<string> res;
    for(const Flower& test : (client -> testFlower)) {
        string r = knn(test, client -> k ,client -> type ,client -> trainFlower);
        initialize_k_closets();
        if (r.empty()) {continue;}
        res.push_back(r);
    }
    return res;
}

std::string KNN::knn(const Flower& flower, int k ,distanceType type, std::vector<Flower> data ) {
    auto it = std::stable_partition(data.begin(), data.end(),
                                    [flower](const Flower& f) { return f.vector.size() == flower.vector.size(); });

    data.erase(it, data.end());
    if (data.empty()) { return ""; }

    Distance* distance = Distance::from(type, flower.vector, data.begin() -> vector);

    std::sort(data.begin(), data.end(), [distance](const Flower &f1, const Flower &f2) {
        distance -> set_vectorB(f1.vector);
        double d1 = distance -> calculate_distance();
        distance -> set_vectorB(f2.vector);
        double d2 = distance -> calculate_distance();
        return d1 < d2;
    });

    for(int i = 0 ; i < k ; i ++) {
        k_closets[data.at(i).str]++ ;
    }

    unsigned int max = 0;
    std::string max_flower;
    for (auto const& element : k_closets)
    {
        if(max < element.second) {
            max = element.second;
            max_flower = element.first;
        }
    }
    return max_flower;
}
#endif /* KNN_hpp */
