#include "ImageMeasure.h"
#include "ImageAnalyse.h"
#include <cmath>
#include "Vector.h"

std::vector<std::vector<std::vector<int>>> jointHistogram(const Image & image_1, const Image & image_2){
    dimTestImageException(image_1, image_2);
    std::vector<std::vector<std::vector<int>>> result = std::vector<std::vector<std::vector<int>>>(image_1.getBytes_per_pixel(), std::vector<std::vector<int>>(256, std::vector<int>(256,0)));
    for(int x = 0; x < image_1.getSize_pixel_x(); x++)
        for(int y = 0; y < image_1.getSize_pixel_y(); y++)
            for(int i = 0; i < image_1.getBytes_per_pixel(); i++)
                result[i][image_1[x][y][i]][image_2[x][y][i]]+=1;
    return result;
}

std::vector<std::vector<std::vector<double>>> densityJointHistogram(const Image & image_1, const Image & image_2){
    dimTestImageException(image_1, image_2);
    std::vector<std::vector<std::vector<int>>> jointHistogram = ::jointHistogram(image_1, image_2);
    std::vector<std::vector<std::vector<double>>> densityJointHistogram = std::vector<std::vector<std::vector<double>>>(image_1.getBytes_per_pixel(), std::vector<std::vector<double>>(256, std::vector<double>(256, 0)));
    for(int x = 0; x < 256; x++)
        for(int y = 0; y < 256; y++)
            for(int i = 0; i < image_1.getBytes_per_pixel(); i++)
                densityJointHistogram[i][x][y] = jointHistogram[i][x][y]/(double)(image_1.getSize_pixel_x()*image_1.getSize_pixel_y());
    return densityJointHistogram;
}

std::vector<unsigned int> difference(const Image & image_1, const Image & image_2){
    dimTestImageException(image_1, image_2);
    std::vector<unsigned int> result = std::vector<unsigned int>(image_1.getBytes_per_pixel(), 0);
    for(int x = 0; x < image_1.getSize_pixel_x(); x++)
        for(int y = 0; y < image_1.getSize_pixel_y(); y++)
            for(int i = 0; i < image_1.getBytes_per_pixel(); i++)
                result[i] += !(image_1[x][y][i] == image_2[x][y][i]);
    return result;
}

std::vector<double> SSD(const Image & image_1, const Image & image_2){
    dimTestImageException(image_1, image_2);
    std::vector<double> result = std::vector<double>(image_1.getBytes_per_pixel(), 0);
    for(int x = 0; x < image_1.getSize_pixel_x(); x++)
        for(int y = 0; y < image_1.getSize_pixel_y(); y++)
            for(int i = 0; i < image_1.getBytes_per_pixel(); i++)
                result[i]+= pow(image_1[x][y][i] - image_2[x][y][i], 2);
    return result;
}


std::vector<double> entropieConjointe(const Image & image_1, const Image & image_2){
    dimTestImageException(image_1, image_2);
    std::vector<std::vector<std::vector<double>>> densityJointHistogram = ::densityJointHistogram(image_1, image_2);
    std::vector<double> result = std::vector<double>(image_1.getBytes_per_pixel(), 0);
    for(int x = 0; x < 256; x++)
        for(int y = 0; y < 256; y++)
             for(int i = 0; i < image_1.getBytes_per_pixel(); i++)
                if(densityJointHistogram[i][x][y] > 0)
                    result[i]-=densityJointHistogram[i][x][y]*log2(densityJointHistogram[i][x][y]);
    return result;
}

std::vector<double> informationMutuelle(const Image & image_1, const Image & image_2){
    dimTestImageException(image_1, image_2);
    return entropie(image_1)+entropie(image_2)-entropieConjointe(image_1, image_2);
}

std::vector<double> crossCorrelation(const Image & image_1, const Image & image_2){
    dimTestImageException(image_1, image_2);
    std::vector<double> mean_1 = ::mean(image_1);
    std::vector<double> mean_2 = ::mean(image_2);
    std::vector<double> var_1 = ::var(image_1);
    std::vector<double> var_2 = ::var(image_2);

    std::vector<double> result = std::vector<double>(image_1.getBytes_per_pixel(), 0);
    for(int i = 0; i < image_1.getBytes_per_pixel(); i++){
        for(int x = 0; x < 256; x++)
            for(int y = 0; y < 256; y++)
                result[i] += (image_1[x][y][i]-mean_1[i]) * (image_2[x][y][i]-mean_2[i]); //Covariance
        result[i]/=(image_1.getSize_pixel_x()*image_1.getSize_pixel_y()); //Mean
        result[i]/=(sqrt(var_1[i])*sqrt(var_2[i])); //Produit des écartypes  
    }  
    return result;
}
