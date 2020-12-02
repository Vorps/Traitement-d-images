#ifndef EM_H
#define EM_H
#include <vector>
#include "Image.h"
#include "Matrix.h"
#include "Vector.h"
#include <random>
#include "Segmentation.h"

struct Parameter{
    double pi;
    std::vector<double> mu;
    Matrix sigma;
    Matrix sigma_inverse;
    double norm;
    Parameter(const int &);

    void setSigma(Matrix);
    double density_loi_normal(Pixel);
    double L(Pixel x);
};

class EM : public Segmentation<Parameter>{
public:
    EM(Image &, const unsigned int);
    EM(Image &, Matrix);
    void setMean(Matrix);
    std::vector<Parameter> & compute();
    Image apply(std::vector<Parameter> &);
private:
    void expectation();
    void maximization();
    Matrix gamma;
};

#endif