#ifndef KMEAN_H
#define KMEAN_H
#include <vector>
#include "Image.h"
#include <random>
#include "Segmentation.h"

struct Cluster{
    std::vector<double> mean;
    std::vector<std::pair<int, int>> index;
    Cluster(const int &);
    Cluster(const Cluster &);
    void updateMean(const Image &, std::uniform_int_distribution<int> &, std::default_random_engine &);
    void addValue(const int, const int);
    void apply(Image &);

    friend bool operator ==(const Cluster &, const Cluster &);
};

class KMean : public Segmentation<Cluster>{
public:
    KMean(Image &, const unsigned int = 2);
    std::vector<Cluster> & compute();
    Image apply(std::vector<Cluster> &);
private:
    void updateValues();
};




#endif
