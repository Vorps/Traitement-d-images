#ifndef MATRIX_H
#define MATRIX_H
#include <vector>

typedef std::vector<std::vector<double>> Matrix;

Matrix operator +(Matrix, Matrix);
Matrix operator -(Matrix, Matrix);
Matrix operator*(Matrix , Matrix);
Matrix operator*(double, Matrix);
Matrix transpose(Matrix& );
Matrix invert_3_3(Matrix &);
Matrix identity(const int);

template<typename T>
Matrix convert(const std::vector<T> op){
    Matrix result(op.size(), std::vector<double>(1,0));
    for(int x = 0; x < op.size(); x++){
        result[x][0] =op[x];
    }
    return result;
}
double determinant_3_3(Matrix &);

#endif