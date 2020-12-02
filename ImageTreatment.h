#ifndef TREATMENT_H
#define TREATMENT_H

#include "Vector.h"
#include "Matrix.h"
#include "Image.h"
#include <cmath>
#include <functional>
#include <iostream>


enum Format {BINARY = 1, GRAY_SCALE = 8, RGB = 24, RGBA = 32};


static auto translate = [](double x, double y = 0) {
    return Matrix{{1,0,0},{0,1,0}, {x,-y,0}};
};

static auto rotate = [](double alpha_degre) {
    double alpha_radian = M_PI/180.0*alpha_degre;
    return Matrix{{cos(alpha_radian),sin(alpha_radian),0}, {-sin(alpha_radian),cos(alpha_radian),0}, {0,0,0}};
};

static auto zoom = [](double x, double y = 1) {
    return Matrix{{1/x,0, 0}, {0, 1/y,0}, {0,0,0}};
};



class ImageTreatment{
    public:
        ImageTreatment(Image &);

        ImageTreatment & invert();
	    ImageTreatment & exchange(const std::vector<unsigned char> &);
	    ImageTreatment & mask(const std::vector<unsigned char> &);
        ImageTreatment & threshold(std::vector<unsigned char>, std::vector<unsigned char>);
        ImageTreatment & adjustContrast(std::vector<unsigned char>, std::vector<unsigned char>);
        ImageTreatment & applyFunction(std::function<void(Pixel &)>);
        ImageTreatment & transform(Matrix);
        ImageTreatment & filter(Matrix &);
        ImageTreatment & convert_bits_per_pixel(const Format);
        ImageTreatment & salt_and_pepper_noise(const int = 100, const int = 100);
        ImageTreatment & gaussian_noise(const double, double);
        ImageTreatment & median_filter(const unsigned int);
        Image & get();
    private:
        Image image;
        void dimTestException(const unsigned int) const;
        void applySegments(const int, const int, std::function<void(Image &, Pixel &)>);


};
#endif