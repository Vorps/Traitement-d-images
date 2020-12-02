#include "Matrix.h"
#include "Vector.h"
#include <string>

Matrix transpose(Matrix & matrix){
    if(matrix.size() == 0)  throw std::string("Erreur Transpose : La dimention de la matrice est nulle !!!");
    Matrix result(matrix[0].size(), std::vector<double>(matrix.size(), 0));
    for(int x = 0; x < matrix.size(); x++){
        for(int y = 0; y < matrix[y].size(); y++){
            result[y][x] = matrix[x][y];
        }
    }
    return result;
}

Matrix identity(const int size){
    Matrix result(size, std::vector<double>(size, 0));
    for(int i = 0; i < size; i++)
        result[i][i] = 1;
    
    return result;
}

double determinant_3_3(Matrix &matrix){
    double determinant = 0;
	for(int i = 0; i < 3; i++)
		determinant = determinant + (matrix[0][i] * (matrix[1][(i+1)%3] * matrix[2][(i+2)%3] - matrix[1][(i+2)%3] * matrix[2][(i+1)%3]));
    return determinant;
}
Matrix invert_3_3(Matrix &matrix){
    if(matrix.size() != 3 || matrix[0].size() != 3)  throw std::string("Erreur Invert : La matrice n'est pas carré !!!");
    Matrix result(3,std::vector<double>(3,0));
	double determinant = determinant_3_3(matrix);
	for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++)
            result[i][j] = ((matrix[(j+1)%3][(i+1)%3] * matrix[(j+2)%3][(i+2)%3]) - (matrix[(j+1)%3][(i+2)%3] * matrix[(j+2)%3][(i+1)%3]))/ determinant;
    return result;
}

Matrix operator +(Matrix op1, Matrix op2){
    Matrix result(op1.size(), std::vector<double>(op1[0].size()));
    for(int i = 0; i < op1.size(); i++) result[i] = op1[i]+op2[i];
    return result;
}
Matrix operator -(Matrix op1, Matrix op2){
    Matrix result(op1.size(), std::vector<double>(op1[0].size()));
    for(int i = 0; i < op1.size(); i++) result[i] = op1[i]-op2[i];
    return result;
}


Matrix operator*(Matrix op1, Matrix op2){
    Matrix result; 
    if(op1.size() > 0 && op1[0].size() == op2.size()){
        result = Matrix(op1.size(), std::vector<double>(op2[0].size(), 0));
        for(int x = 0; x < op1.size(); x++)
            for(int y = 0; y < op2[0].size(); y++)
                for(int i = 0; i < op2.size(); i++){
                    result[x][y]+= op1[x][i]*op2[i][y];
                }
    } else throw std::string("Erreur : La dimention des matrice ne sont pas correct !!!");
    return result;
}

Matrix operator*(double value, Matrix matrix){
    for(int x = 0; x < matrix.size(); x++)
        for(int y = 0; y < matrix[x].size(); y++)
            matrix[x][y]*=value;
    return matrix;
}