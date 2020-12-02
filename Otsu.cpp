#include "Otsu.h"
#include "ImageAnalyse.h"
#include <algorithm>
#include "ImageTreatment.h"

Otsu::Otsu(Image & image) : Segmentation(image, image.getBytes_per_pixel()){
}

std::vector<unsigned char> & Otsu::compute(){
    std::vector<std::vector<double>> densityHistogram = ::densityHistogram(image);
    std::vector<double> var_intra_classe;
    auto w = [densityHistogram](const int & i, const int & min, const int & max){
        double w = 0;
        for(int x = min; x < max; x++) w+=densityHistogram[i][x];
        return w;
    };
    auto mean = [densityHistogram, w](const int & i, const int & min, const int & max){
         double mean = 0;
         for(int x = min; x < max; x++, mean+=x*densityHistogram[i][x]);
         return mean/w(i, min, max);
    };
    auto var = [densityHistogram, mean](const int & i, const int & min, const int & max){
        double var = 0;
        for(int x = min; x < max; x++) var+=pow(x-mean(i, min, max), 2)*densityHistogram[i][x];
        return var;
    };
    for(int i = 0; i < image.getBytes_per_pixel(); i++){
        var_intra_classe = std::vector<double>(256,0);
        for(int k = 0; k < 256; k++) var_intra_classe[k] = var(i, 0, k)+var(i, k, 256);
        this->data[i] = std::min_element(var_intra_classe.begin(), var_intra_classe.end()) - var_intra_classe.begin();
    }
    return this->data;
}

Image Otsu::apply(std::vector<unsigned char> & threshold){
    return  ImageTreatment(this->image).threshold(threshold, std::vector<unsigned char>(this->image.getBytes_per_pixel(), 255)).get();
}