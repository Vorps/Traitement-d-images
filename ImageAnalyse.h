#ifndef IMAGE_ANALYSE_H
#define IMAGE_ANALYSE_H
#include <vector>
#include "Image.h"
#include <random>


std::vector<std::vector<unsigned int>> histogram(const Image &);
std::vector<std::vector<double>> densityHistogram(const Image &);
std::vector<double> entropie(const Image &);

std::vector<double> mean(const Image &);
std::vector<double> var(const Image &);

Image otsu(const Image &);

#endif