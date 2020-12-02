#ifndef SEGMENTATION_H
#define SEGMENTATION_H
#include "Image.h"
#include <vector>

template<typename T>
class Segmentation{
    public:
        Segmentation(Image & image, const unsigned int k = 2) : image(image){
            this->setK(k);
        }
        void setK(const unsigned int k){
            this->data = std::vector<T>(k, this->image.getBytes_per_pixel());
        }
        virtual std::vector<T> & compute() = 0;
        virtual Image apply(std::vector<T> &) = 0;
    protected:
        Image image;
        std::vector<T> data;
};
#endif