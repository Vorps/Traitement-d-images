#ifndef IMAGE_BMP_H 
#define IMAGE_BMP_H
    
#include "ImageFile.h"
#include <string>
#include <fstream>


#define LONGUEUR_ENTETE 52

union Value_2{
	unsigned char data[2];
	unsigned short value;
};

union Value_4{
	unsigned char data[4];
	unsigned int value;
};


struct Header_struct{
	Value_4 size_file;
	Value_2 reserved_1;
	Value_2 reserved_2;
	Value_4 offset;
	Value_4 size_header;
	Value_4 width;
	Value_4 height;
	Value_2 color_planes;
	Value_2 bits_per_pixel;
	Value_4 compression;
	Value_4 size_image;
	Value_4 pixel_per_meter_x;
	Value_4 pixel_per_meter_y;
	Value_4 number_color;
	Value_4 important_color;
};

union Header_union{
	unsigned char header[LONGUEUR_ENTETE];
	Header_struct header_struct;
};

class ImageBmp : public ImageFile{

public:
	ImageBmp(std::ifstream &);
	ImageBmp(Image &);
    void save(const std::string);
protected:
    Header_union header;
};


#endif