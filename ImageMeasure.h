#include "Image.h"



std::vector<std::vector<std::vector<int>>> jointHistogram(const Image &, const Image &);
std::vector<std::vector<std::vector<double>>> densityJointHistogram(const Image &, const Image &);

std::vector<unsigned int> difference(const Image &, const Image &);
std::vector<double> SSD(const Image &, const Image &);
std::vector<double> entropieConjointe(const Image &, const Image &);
std::vector<double> informationMutuelle(const Image &, const Image &);
std::vector<double> crossCorrelation(const Image &, const Image &);
