
#include "EM.h"
#include <algorithm>
Parameter::Parameter(const int & channel){
    this->pi = 1;
    this->mu = std::vector<double>(channel, 0);
    this->sigma = std::vector<std::vector<double>>(channel, std::vector<double>(channel, 0));
}

void Parameter::setSigma(Matrix sigma){
    Parameter::sigma = sigma;    
    Parameter::sigma_inverse = invert_3_3(sigma);
    Parameter::norm = pow(2.50663, -3)*pow(determinant_3_3(sigma), -0.5);
}

double Parameter::density_loi_normal(Pixel x){
    std::vector<double> tmp(x.size(), 0); 
    for(int i = 0; i < tmp.size(); i++)
        tmp[i] = x[i]-mu[i];
    Matrix m = convert(tmp);
    Matrix quadform = transpose(m) * sigma_inverse * m;
    return norm * exp(-0.5*quadform[0][0]);
}

double Parameter::L(Pixel x){
    return Parameter::pi*density_loi_normal(x);
}


EM::EM(Image & image, const unsigned int k = 2) : Segmentation(image, k), gamma(std::vector<std::vector<double>>(this->image.getSize(), std::vector<double>(this->data.size(), 0))){
    std::default_random_engine generator;
    generator.seed(time(nullptr));
    std::uniform_int_distribution<int> distribution(0, 255);
    Matrix mean(k, std::vector<double>(this->image.getBytes_per_pixel(), 0));
    for(int i = 0; i < k; i++)
        for(int j = 0; j < this->image.getBytes_per_pixel(); j++)
            mean[i][j] = distribution(generator);
        
    this->setMean(mean);
}

EM::EM(Image &image, Matrix mean) : EM(image, mean.size()){
    this->setMean(mean);
}

void EM::setMean(Matrix mean){
    if(mean.size() == this->data.size())
        for(int i = 0; i < this->data.size(); i++) this->data[i].mu = mean[i];    
}

void EM::expectation(){
    auto gammaNormalise = [&](const Pixel x, const unsigned int k){
        double num = this->data[k].L(x);
        double denum = 0;
        for(int i = 0; i < this->data.size(); i++) denum += this->data[i].L(x);
        return num/denum;
    };
    for(int k = 0; k < this->data.size(); k++){
        for(int x = 0; x < this->image.getSize_pixel_x(); x++){
            for(int y = 0; y < this->image.getSize_pixel_y(); y++){
                 this->gamma[x*this->image.getSize_pixel_y()+y][k] = gammaNormalise(this->image[x][y], k);
            }
        }
        std::cout << k << std::endl;
    }
}


void EM::maximization(){
    double Nk;
    std::vector<double> mu_k;
    Matrix sigma_k;
    double pi_k;
    auto NK = [&](const int k) {
        double nK = 0;
        for(int n = 0; n < this->gamma.size(); n++) nK+=this->gamma[n][k];
        return nK;
    };
    for(int k = 0; k < this->data.size(); k++){
        Nk = NK(k);
        mu_k = std::vector<double>(this->data[k].mu.size(), 0);
        for(int x = 0; x < this->image.getSize_pixel_x(); x++){
            for(int y = 0; y < this->image.getSize_pixel_y(); y++){
                mu_k =mu_k+gamma[x*this->image.getSize_pixel_y()+y][k]*this->image[x][y];
            }
        }
        mu_k = 1/Nk*mu_k;
        sigma_k = Matrix(this->image.getBytes_per_pixel(), std::vector<double>(this->image.getBytes_per_pixel(),0));
        for(int x = 0; x < this->image.getSize_pixel_x(); x++){
            for(int y = 0; y < this->image.getSize_pixel_y(); y++){
                Matrix m = convert(this->image[x][y] - mu_k);
                for(int i = 0; i < 3; i++){
                    for(int j = 0; j < 3; j++){
                        sigma_k[i][j]=sigma_k[i][j]+gamma[x*this->image.getSize_pixel_y()+y][k]*(m[i][0]*m[j][0]);
                    }
                }
               
            }
        }
        sigma_k = 1/Nk*sigma_k;
        pi_k = Nk/(double)image.getSize();
        this->data[k].mu = mu_k;
        this->data[k].pi = pi_k;
        this->data[k].setSigma(sigma_k);
    }
    
}


std::vector<Parameter> & EM::compute(){
    for(int i = 0; i < this->data.size(); i++){
        this->data[i].setSigma(100*identity(image.getBytes_per_pixel()));
        this->data[i].pi = 1/(double) this->data.size();
    }
    for(int i = 0; i < 2; i++){
        expectation();
        maximization();
    }
    
    expectation();
    return data;
}

Image EM::apply(std::vector<Parameter> & parameters){
    Image result = std::move(this->image);
    for(int x = 0; x < this->image.getSize_pixel_x(); x++){
        for(int y = 0; y < this->image.getSize_pixel_y(); y++){
            std::vector<double> mean = this->data[std::max_element(this->gamma[x*this->image.getSize_pixel_y()+y].begin(), this->gamma[x*this->image.getSize_pixel_y()+y].end())-this->gamma[x*this->image.getSize_pixel_y()+y].begin()].mu;
            result[x][y] =  Pixel(mean.begin(), mean.end());
        }
    }
    return result;
}