#include "KMean.h"
#include <algorithm> 

Cluster::Cluster(const int & channel){
    this->mean = std::vector<double> (channel, 0);
    this->index = std::vector<std::pair<int, int>>(0);
}

Cluster::Cluster(const Cluster & cluster){
    this->mean = cluster.mean;
    this->index = cluster.index;
}

void Cluster::updateMean(const Image & image, std::uniform_int_distribution<int> & distribution, std::default_random_engine & generator){
    mean = std::vector<double>(mean.size(), 0);
    if(index.size() > 0)
        for(int i = 0; i < image.getBytes_per_pixel(); i++){
            for(int x = 0; x < index.size(); x++)
                mean[i] += image[index[x].first][index[x].second][i];
        mean[i]/=(double)(index.size());
    } else {
        for(int i = 0; i < image.getBytes_per_pixel(); i++)
            mean[i] = distribution(generator);
    }
    index.clear();
}

void Cluster::addValue(const int x, const int y){
    this->index.push_back({x, y});
}

void Cluster::apply(Image & image){
    for(int x = 0; x < index.size(); x++)
        for(int i = 0; i < mean.size(); i++)
            image[index[x].first][index[x].second][i] = mean[i];
}

KMean::KMean(Image & image, const unsigned int k ) : Segmentation(image, k){
}

std::vector<Cluster> & KMean::compute(){
    std::default_random_engine generator;
    generator.seed(time(nullptr));
    std::uniform_int_distribution<int> distribution(0, 255);

    for(int cl = 0; cl < this->data.size(); cl++)
        for(int cha = 0; cha < this->image.getBytes_per_pixel(); cha++)
            this->data[cl].mean[cha] = distribution(generator);

    std::vector<Cluster> clusters_n1;

    int e = 0;
    while(clusters_n1 != this->data && e++ < 25){
        //Asignement
        this->updateValues();
        clusters_n1 = this->data;
        //Update mean cluster
        for(int i = 0; i < this->data.size(); i++) this->data[i].updateMean(image, distribution, generator);
        std::cout << "Step : " << e << std::endl;
    }
    this->updateValues();
    return this->data;
}

void KMean::updateValues(){
    std::vector<int>tmp(this->data.size());
    for(int x = 0; x < this->image.getSize_pixel_x(); x++){
        for(int y = 0; y < this->image.getSize_pixel_y(); y++){
            for(int cl = 0; cl < this->data.size(); cl++){
                tmp[cl] = 0;
                for(int cha = 0; cha < this->image.getBytes_per_pixel(); cha++){
                    tmp[cl] += pow(this->data[cl].mean[cha] - this->image[x][y][cha], 2);      
                }
            }
            this->data[std::min_element(tmp.begin(), tmp.end()) - tmp.begin()].addValue(x, y);
        }
    }
}

Image KMean::apply(std::vector<Cluster> & clusters){
    Image result = std::move(image);
    for(int i = 0; i < clusters.size(); i++) clusters[i].apply(result);
    return result;
}

bool operator ==(const Cluster & cluster_1, const Cluster & cluster_2){
    for(int i = 0; i < cluster_1.mean.size(); i++) if(std::abs(cluster_1.mean[i]-cluster_2.mean[i]) > 2) return false;
    return true;
}

