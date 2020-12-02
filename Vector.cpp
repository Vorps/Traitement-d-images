#include "Vector.h"

//Addition de pixels terme à terme
std::vector<unsigned char> & operator +=(std::vector<unsigned char> &pixel_1, const std::vector<unsigned char> &pixel_2){
    dimTestPixelException(pixel_1, pixel_2);
    double result;
    for(int i = 0; i < pixel_1.size(); i++){
        result = pixel_1[i]+pixel_2[i];
        if(result > 255) result = 255;
        else if(result < 0) result = 0;
        pixel_1[i] = result;
    } 
    return pixel_1;
}

//Soustraction de pixels terme à terme
std::vector<unsigned char> & operator -=(std::vector<unsigned char> &pixel_1, const std::vector<unsigned char> &pixel_2){
    dimTestPixelException(pixel_1, pixel_2);
    int result;
    for(int i = 0; i < pixel_1.size(); i++) {
        result = pixel_1[i]-pixel_2[i];
        if(result < 0) result = 0;
        else if(result > 255) result = 255;
        pixel_1[i] = result;;
    }
    return pixel_1;
}

std::vector<unsigned char> operator &=(std::vector<unsigned char> &pixel_1, const std::vector<unsigned char> mask){
    dimTestPixelException(pixel_1, mask);
    for(int i = 0; i < pixel_1.size(); i++) 
        if(!mask[i]) pixel_1[i] = 0;
    return pixel_1;
}