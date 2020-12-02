#ifndef VECTOR_H
#define VECTOR_H
#include <vector>
#include <iostream>

template<typename T>
class Vector{
    
    private:
        std::vector<T> vector;
};
template<typename T1, typename T2>
void dimTestPixelException(const std::vector<T1> &op1, const std::vector<T2> &op2){
    #ifdef EXCEPTIONS
        if(op1.size() != op2.size())
            throw std::string("Erreur : La dimention des vecteurs ne correspond pas !!!");
    #endif
}


std::vector<unsigned char> & operator +=(std::vector<unsigned char> &, const std::vector<unsigned char> &);
std::vector<unsigned char> & operator -=(std::vector<unsigned char> &, const std::vector<unsigned char> &);


//Multiplication terme à terme
template<typename T1, typename T2>
std::vector<T1> operator*(std::vector<T1> op1, const std::vector<T2> op2){
    for(int i = 0; i < op1.size(); i++)
        op1[i]*=op2[i];
    return op1;
}

//Division terme à terme
template<typename T>
std::vector<T> operator/(std::vector<T> op1, const std::vector<T> op2){
    for(int i = 0; i < op1.size(); i++)
        op1[i]/=op2[i];
    return op1;
}


//Addition terme à terme
template<typename T>
std::vector<T> operator +(std::vector<T> op1, std::vector<T> op2){
    dimTestPixelException(op1, op2);
    std::vector<T> result = op1;
    return result+=op2;
}
template<typename T>
std::vector<T> operator +=(std::vector<T> op1, std::vector<T> op2){
    dimTestPixelException(op1, op2);
    for(int i = 0; i < op1.size(); i++) op1[i] += op2[i];
    return op1;
}

//Soustraction terme à terme
template<typename T1, typename T2>
std::vector<T1> operator -(std::vector<T1> op1, std::vector<T2> op2){
    dimTestPixelException(op1, op2);
    std::vector<T1> result = op1;
    return result-=op2;
}

template<typename T1, typename T2>
std::vector<T1> operator -=(std::vector<T1> op1, std::vector<T2> op2){
    dimTestPixelException(op1, op2);
    for(int i = 0; i < op1.size(); i++) op1[i] -= op2[i];
    return op1;
}

//Produit par un scalaire
template<typename T>
std::vector<double> operator *(const double coef, const std::vector<T> op){
    std::vector<double> result(op.size(), 0);
    for(int i = 0; i < op.size(); i++) result[i] = coef*op[i];
    return result;
}

//Masque des pixels
std::vector<unsigned char> operator &=(std::vector<unsigned char>&, const std::vector<unsigned char>);

template<typename T>
bool operator <=(std::vector<T> op1, std::vector<T> op2){
    dimTestPixelException(op1, op2);
    int tmp = 0;
    for(int i = 0; i < op1.size(); i++) tmp += op1[i] <= op2[i];
    return tmp == op1.size();
}


template<typename T>
bool operator >=(std::vector<T> op1, std::vector<T> op2){
    dimTestPixelException(op1, op2);
    int tmp = 0;
    for(int i = 0; i < op1.size(); i++) tmp += op1[i] >= op2[i];
    return tmp == op1.size();
}

#endif