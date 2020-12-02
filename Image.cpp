#include "Image.h"
#include <cmath>
#include <functional>
#include <algorithm>
#include <numeric>
#include <algorithm>
#include "Vector.h"

Image::~Image(){
    if(this->pixels != nullptr){
        for(int x = 0; x < this->size_pixel_x; x++)
           if(this->pixels[x] != nullptr)
                delete [] this->pixels[x];
        delete [] this->pixels;
        this->pixels=nullptr;
    }
}

Image::Image(unsigned int bits_per_pixel, unsigned int size, unsigned int size_pixel_x, unsigned int size_pixel_y){
    this->set_pixel_format(bits_per_pixel);
	this->size = size;
	this->size_pixel_x = size_pixel_x;
	this->size_pixel_y = size_pixel_y;
    this->allocation();
    for(int x = 0; x < size_pixel_x; x++) 
        for(int y = 0; y < size_pixel_y; y++)
            this->pixels[x][y] = Pixel(this->bytes_per_pixel, 0);
}

Image::Image(const Image &image){
    this->set_pixel_format(image.bits_per_pixel);
	this->size = image.size;
	this->size_pixel_x = image.size_pixel_x;
	this->size_pixel_y = image.size_pixel_y;
    this->allocation();
    for(int x = 0; x < size_pixel_x; x++) 
        for(int y = 0; y < size_pixel_y; y++)
            this->pixels[x][y] = image.pixels[x][y];
}

Image::Image(const Image && image){
    this->set_pixel_format(image.bits_per_pixel);
	this->size = image.size;
	this->size_pixel_x = image.size_pixel_x;
	this->size_pixel_y = image.size_pixel_y;
    this->allocation();
     for(int x = 0; x < size_pixel_x; x++) 
        for(int y = 0; y < size_pixel_y; y++)
            this->pixels[x][y] = Pixel(this->bytes_per_pixel, 0);
}

void Image::allocation(){
    this->pixels = new Pixel*[this->size_pixel_x];
    for(int x = 0; x < this->size_pixel_x; x++)
        this->pixels[x] = new Pixel[this->size_pixel_y];
    
}

void dimTestImageException(const Image &image_1, const Image &image_2){
     #ifdef EXCEPTIONS
        if(image_1.size_pixel_x != image_2.size_pixel_x || image_1.size_pixel_y != image_2.size_pixel_y || image_1.bits_per_pixel != image_2.bits_per_pixel)
            throw std::string("Erreur : La dimention des images ne correspond pas !!!");
     #endif
}

void Image::dimTestPixelException(const Pixel &pixel_1){
    #ifdef EXCEPTIONS
        if(pixel_1.size() != this->bytes_per_pixel)
            throw std::string("Erreur : La dimention des vecteurs ne correspond pas !!!");
    #endif
}

const unsigned int Image::getBits_per_pixel() const {return this->bits_per_pixel;}
const unsigned int Image::getBytes_per_pixel() const {return this->bytes_per_pixel;}
const unsigned int Image::getSize() const {return this->size;}
const unsigned int Image::getSize_pixel_x() const {return this->size_pixel_x;}
const unsigned int Image::getSize_pixel_y() const {return this->size_pixel_y;}

void Image::set_pixel_format(const unsigned int bits_per_pixel){
    this->bits_per_pixel = bits_per_pixel;
    this->bytes_per_pixel = ceil(bits_per_pixel/8.0);
}

/*----OPERATEURS----*/
Image operator +(const Image &image_1, const Image &image_2){
    dimTestImageException(image_1, image_2);
    Image imageResult = image_1;
    return imageResult+=image_2;
}

Image operator -(const Image &image_1, const Image &image_2){
    dimTestImageException(image_1, image_2);
    Image imageResult = image_1;
    return imageResult-=image_2;
}

Image & Image::operator +=(const Image &image){
    dimTestImageException(*this, image);
    for(int x = 0; x < image.size_pixel_x; x++) 
        for(int y = 0; y < image.size_pixel_y; y++)   
            (*this)[x][y] +=image.pixels[x][y];
    return *this;
}

Image & Image::operator -=(const Image &image){
    dimTestImageException(*this, image);
    for(int x = 0; x < image.size_pixel_x; x++) 
        for(int y = 0; y < image.size_pixel_y; y++)     
            (*this)[x][y] -= image.pixels[x][y];
     return *this;
}


Image operator &(const Image & image, const Image &mask){
    dimTestImageException(image, mask);
    Image imageResult(image);
    for(int x = 0; x < image.size_pixel_x; x++) 
        for(int y = 0; y < image.size_pixel_y; y++)      
            imageResult.pixels[x][y] &= mask.pixels[x][y];
    return imageResult;
}


bool operator ==(const Image &image_1, const Image &image_2){
    if(image_1.size_pixel_x != image_2.size_pixel_x || image_1.size_pixel_y != image_2.size_pixel_y || image_1.bits_per_pixel != image_2.bits_per_pixel)
        return false;
    for(int i = 0; i < image_1.size_pixel_x; i++) 
        for(int j = 0; j < image_1.size_pixel_y; j++)
            if(!(image_1[i][j] == image_2[i][j])) return false;
    return true;
}

ImageLineMatrix Image::operator [](const int i){
    return this->pixels[i];
}

const ImageLineMatrix Image::operator [](const int i) const{
    return this->pixels[i];
}


