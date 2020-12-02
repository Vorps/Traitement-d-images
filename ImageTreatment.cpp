#include "ImageTreatment.h"

#include <iostream>
#include <algorithm>
#include <numeric>
#include <random>
#include <ctime>

ImageTreatment::ImageTreatment(Image & image) : image(image){}

Image & ImageTreatment::get(){
    return this->image;
}

ImageTreatment & ImageTreatment::invert(){
    applyFunction([](Pixel & pixel) {pixel = Pixel(pixel.size(), 255)-pixel;});
    return (*this); 
}

ImageTreatment & ImageTreatment::exchange(const std::vector<unsigned char> &index){
    this->image.dimTestPixelException(index);
    applyFunction([index](Pixel & pixel) {
        Pixel value_copy = pixel;
        for(int i = 0; i < value_copy.size(); i++) pixel[i] = value_copy[index[i]];
    });
    return (*this); 
}

ImageTreatment & ImageTreatment::threshold(std::vector<unsigned char> threshold_1, std::vector<unsigned char> threshold_2){
    this->image.dimTestPixelException(threshold_1); this->image.dimTestPixelException(threshold_2);
    applyFunction([threshold_1, threshold_2](Pixel & pixel) mutable {pixel = Pixel(pixel.size(), threshold_1 <= threshold_2 && pixel >= threshold_1 & pixel <= threshold_2 ? 255 : 0);});
    return (*this);
}

ImageTreatment & ImageTreatment::adjustContrast(std::vector<unsigned char> threshold_1, std::vector<unsigned char> threshold_2){
    this->image.dimTestPixelException(threshold_1); this->image.dimTestPixelException(threshold_2);
    this->applyFunction([threshold_1, threshold_2](Pixel & pixel) mutable {pixel = (pixel - threshold_1)* (Pixel(pixel.size(), 255)/(threshold_2-threshold_1));});
    return (*this);
}

ImageTreatment & ImageTreatment::mask(const std::vector<unsigned char> &mask){
   this->image.dimTestPixelException(mask);
    applyFunction([mask](Pixel & pixel) {pixel&=mask;});
    return (*this);
}

ImageTreatment & ImageTreatment::salt_and_pepper_noise(const int nb_0, const int nb_255){
    std::default_random_engine generator;
    generator.seed(time(nullptr));
    std::uniform_int_distribution<int> distributionX(0,this->image.getSize_pixel_x()-1);
    std::uniform_int_distribution<int> distributionY(0,this->image.getSize_pixel_y()-1);
    for(int i = 0; i < nb_0; i++)
        this->image[distributionX(generator)][distributionY(generator)] = Pixel(this->image.getBytes_per_pixel(), 0);
    for(int i = 0; i < nb_255; i++)
        this->image[distributionX(generator)][distributionY(generator)] = Pixel(this->image.getBytes_per_pixel(), 255);
    
    return (*this);
}

ImageTreatment & ImageTreatment::gaussian_noise(const double mean, double sigma){
    std::default_random_engine generator;
    generator.seed(time(nullptr));
    std::normal_distribution<double> distribution(mean,sigma);
    applyFunction([&distribution, &generator](Pixel & pixel) {
        std::vector<double> noise(pixel.size(), 0);
        std::generate(noise.begin(), noise.end(), [&distribution, &generator](){return distribution(generator);});
        pixel=pixel+noise;
    });
    return (*this);
}


ImageTreatment & ImageTreatment::applyFunction(std::function<void(Pixel &)> func){
    for(int x = 0; x < this->image.getSize_pixel_x(); x++)
        for(int y = 0; y < this->image.getSize_pixel_y(); y++)
            func(this->image[x][y]);        
    return (*this);
}



ImageTreatment & ImageTreatment::transform(Matrix matrix){
    if(matrix.size() != 3 || matrix[0].size() !=3)  throw std::string("Erreur : Matrice de transformation incorecte !!!");
    Image image = this->image;
    Matrix init = {{this->image.getSize_pixel_x()/2.0, this->image.getSize_pixel_y()/2.0, 0}};
    Matrix result;
    Matrix X;
    std::vector<double> min(3,0);
    std::vector<double> max = std::vector<double>({this->image.getSize_pixel_x()-1.0, this->image.getSize_pixel_y()-1.0, 0});
    for(int x = 0; x < this->image.getSize_pixel_x(); x++){
        for(int y = 0; y < this->image.getSize_pixel_y(); y++){
            X = {{(double)x, (double)y, 1}};
            result = (X-init)*matrix+init;
            this->image[x][y] = result[0] >= min && result[0] <= max ? image[(int)result[0][0]][(int)result[0][1]] : Pixel(this->image.getBytes_per_pixel(), 0);;
        }
    }
    return (*this);
}



ImageTreatment & ImageTreatment::median_filter(const unsigned int size){
    this->applySegments(size, size, [size](Image & image_segment, Pixel & pixel) {
        //Split canals
        std::vector<std::vector<unsigned char>> canaux =  std::vector<std::vector<unsigned char>>(pixel.size(), std::vector<unsigned char>()); 
        for(int i = 0; i < size; i++)
            for(int j = 0; j < size; j++)
                for(int u = 0; u < pixel.size(); u++)
                    canaux[u].push_back(image_segment[i][j][u]);
             
        for(int u = 0; u < pixel.size(); u++){
            std::sort(canaux[u].begin(), canaux[u].end());
            pixel[u] = canaux[u][(size*size)/2];
        }
    });
    return (*this);
}

ImageTreatment & ImageTreatment::filter(Matrix & matrix){
    int size_matrix_x = matrix.size();
    int size_matrix_y = 0;
    if(size_matrix_x) size_matrix_y = matrix[0].size();
    this->applySegments(size_matrix_x, size_matrix_y, [& matrix](Image & image_segment, Pixel & pixel) {
        std::vector<double> result(pixel.size(),0);
        for(int i = 0; i < matrix.size(); i++)
            for(int j = 0; j < matrix[i].size(); j++)
                result=result+(matrix[i][j]*image_segment[i][j]);
        
        for(int y = 0; y < pixel.size(); y++){
            if(result[y] < 0)result[y] = 0;
            if(result[y] > 255)result[y] = 255;
            pixel[y] = result[y]; 
        }
    });
    return (*this);
}

void ImageTreatment::applySegments(const int size_x, const int size_y, std::function<void(Image &, Pixel &)> func){
    int x_tmp, y_tmp;
    int middle_x = size_x/2.0;
    int middle_y = size_y/2.0;
    Image imageSrc(image);
    Image image_segment(this->image.getBits_per_pixel(), size_x*size_y, size_x, size_y);
    Pixel tmp;
    for(int x = 0; x < this->image.getSize_pixel_x(); x++){
        for(int y = 0; y < this->image.getSize_pixel_y(); y++){
            for(int kx = 0; kx < size_x; kx++){
                for(int ky = 0; ky < size_y; ky++){
                    x_tmp = x+kx-middle_x;
                    y_tmp = y+ky-middle_y;
                    image_segment[kx][ky] = x_tmp >= 0 && x_tmp < this->image.getSize_pixel_x() && y_tmp >= 0 && y_tmp < this->image.getSize_pixel_y() ? imageSrc[x_tmp][y_tmp] : Pixel(this->image.getBytes_per_pixel(), 0);
                }
            }
            func(image_segment, this->image[x][y]);
        }
    }
}

ImageTreatment & ImageTreatment::convert_bits_per_pixel(const Format bits_per_pixel){
    if(this->image.getBits_per_pixel() != bits_per_pixel){
        switch(bits_per_pixel){
            case BINARY:
                this->convert_bits_per_pixel(GRAY_SCALE); //Vérifie que l'on est bien en gray_scale
                for(int x = 0; x < this->image.getSize_pixel_x(); x++)
                    for(int y = 0; y < this->image.getSize_pixel_y(); y++)
                        this->image[x][y][0] = this->image[x][y][0] > 128 ? 1 : 0;
                break;
            case GRAY_SCALE:
                int mean;
                for(int x = 0; x < this->image.getSize_pixel_x(); x++)
                    for(int y = 0; y < this->image.getSize_pixel_y(); y++){
                        mean = std::accumulate(this->image[x][y].begin(), this->image.getBits_per_pixel() < 4 ? this->image[x][y].end() : this->image[x][y].begin()+3, 0.0) / this->image[x][y].size();
                        this->image[x][y].clear();
                        this->image[x][y].push_back(mean);
                    }
                break;
            default:
                break;
        }
        this->image.set_pixel_format(bits_per_pixel);
    }
    return (*this);
}
