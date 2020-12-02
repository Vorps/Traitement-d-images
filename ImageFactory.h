#ifndef IMAGE_FACTORY_H
#define IMAGE_FACTORY_H

#include "Image.h"
#include <string>
#include <iostream>
class ImageFactory {
public:
    ImageFactory();
    Image & open(const std::string);
    ~ImageFactory();

private:
    std::vector<Image *> images;
};

#endif