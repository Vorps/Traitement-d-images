#include "ImageAnalyse.h"
#include <algorithm> 
#include  <ctime>
#include <iostream>
#include <cmath>
#include "ImageTreatment.h"
#include "Vector.h"
#include "Matrix.h"

std::vector<std::vector<unsigned int>>  histogram(const Image & image){
    std::vector<std::vector<unsigned int>>  histogram = std::vector<std::vector<unsigned int>>(image.getBytes_per_pixel(), std::vector<unsigned int>(256, 0));
    for(int x = 0; x < image.getSize_pixel_x(); x++)
        for(int y = 0; y < image.getSize_pixel_y(); y++)
            for(int i = 0; i < image.getBytes_per_pixel(); i++)
                histogram[i][image[x][y][i]]+=1;
    return histogram;
}

std::vector<std::vector<double>> densityHistogram(const Image & image){
    std::vector<std::vector<unsigned int>> histogram = ::histogram(image);
    std::vector<std::vector<double>> densityHistogram = std::vector<std::vector<double>>(image.getBytes_per_pixel(), std::vector<double>(256, 0));
    for(int i = 0; i < image.getBytes_per_pixel(); i++)
        for(int x = 0; x < 256; x++)
            densityHistogram[i][x] = histogram[i][x]/(double)(image.getSize_pixel_x()*image.getSize_pixel_y());
    return densityHistogram;
}

std::vector<double> entropie(const Image & image){
    std::vector<std::vector<double>> densityHistogram = ::densityHistogram(image);
    std::vector<double> result = std::vector<double>(image.getBytes_per_pixel(), 0);
    for(int i = 0; i < image.getBytes_per_pixel(); i++)
        for(int x = 0; x < 256; x++)
            if(densityHistogram[i][x] > 0)
                result[i] -= densityHistogram[i][x]*log2(densityHistogram[i][x]);
    return result;
}

std::vector<double> mean(const Image & image){
    std::vector<double> mean = std::vector<double>(image.getBytes_per_pixel(), 0);
    for(int x = 0; x < image.getSize_pixel_x(); x++)
        for(int y = 0; y < image.getSize_pixel_y(); y++)
            for(int i = 0; i < image.getBytes_per_pixel(); i++)
                mean[i]+= image[x][y][i];
    for(int i = 0; i < image.getBytes_per_pixel(); i++)
        mean[i]/=(image.getSize_pixel_x()*image.getSize_pixel_y());
    return mean;
}

std::vector<double> var(const Image & image){
    std::vector<double> var = std::vector<double>(image.getBytes_per_pixel(), 0);
    std::vector<double> mean = ::mean(image);
    for(int x = 0; x < image.getSize_pixel_x(); x++)
        for(int y = 0; y < image.getSize_pixel_y(); y++)
            for(int i = 0; i < image.getBytes_per_pixel(); i++)
                var[i]+=pow(image[x][y][i]-mean[i], 2);
    for(int i = 0; i < image.getBytes_per_pixel(); i++)
        var[i]/=(image.getSize_pixel_x()*image.getSize_pixel_y());
    return var;
}




