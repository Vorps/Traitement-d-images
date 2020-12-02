#ifndef OTSU_H
#define OTSU_H
#include "Image.h"
#include "Segmentation.h"

class Otsu : public Segmentation<unsigned char>{
public:
    Otsu(Image &);
    std::vector<unsigned char> & compute();
    Image apply(std::vector<unsigned char> &);
};
#endif