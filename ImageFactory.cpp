#include "ImageFactory.h"
#include "ImageBmp.h"
#include <iostream>
#include <fstream>

ImageFactory::ImageFactory(){}

ImageFactory::~ImageFactory(){
    for(Image * image : this->images)
        delete image;
}

Image & ImageFactory::open(const std::string image_path){
    std::ifstream file_image(image_path, std::ios::in|std::ios::binary);
    if(file_image.is_open()){
        unsigned char id[2];
        file_image.read((char *) id, 2);
        if(id[0] == 0x42 && id[1] == 0x4d){
            this->images.push_back(ImageBmp(file_image).image);
            file_image.close();
        } else  throw std::string("ERREUR : Le format de l'image est inconue !!!");
    } else throw std::string("ERREUR : Le fichier ne peut pas être ouvert !!!");
    return *this->images.back();
}

