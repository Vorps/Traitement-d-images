#ifndef IMAGE_FILE_H
#define IMAGE_FILE_H

#include <string>
#include <vector>
#include "Image.h"

class ImageFile{

public:
    ImageFile(){}
    ImageFile(Image &image) : image{&image}{}
	virtual void save(const std::string) = 0;

    Image *image;
};
#endif