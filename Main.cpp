#include "ImageFactory.h"
#include <iostream>
#include <string>
#include "ImageTreatment.h"
#include "ImageBmp.h"
#include "ImageAnalyse.h"
#include <assert.h>
#include "ImageMeasure.h"
#include "EM.h"
#include "KMean.h"
#include "Otsu.h"

int main(int argc, char **argv){
    
    ImageFactory imageFactory;
    try {
        /*KMean optimisation Hyper-paramètre*/
        /*Image &image = imageFactory.open("Images/linescolor.bmp");
        KMean kMean(image);
        std::vector<std::vector<double>> distances;
        for(int k = 1; k < 8; k++){
            kMean.setK(k);
            auto cluster = kMean.compute();
            Image image_segmentation = kMean.apply(cluster);
            distances.push_back(SSD(image_segmentation, image));
        }
        std::cout << "[";
        for(std::vector<double> distance : distances){
             std::cout << "[";
             for(double value : distance){
                 std::cout << value << ",";
             }
             std::cout << "]," << std::endl;
        }
        std::cout << "]" << std::endl;
        */
        /*KMean Segmentation*/
        /*
        Image &image = imageFactory.open("Images/linescolor.bmp");
        KMean kMean(image, 20);
        auto cluster = kMean.compute();
        Image image_segmentation = kMean.apply(cluster);
        ImageBmp(image_segmentation).save("Results/Segmentation/linescolorKMean.bmp");
        Image imageAsoluteDifference(image.getBits_per_pixel(), image.getSize(), image.getSize_pixel_x(), image.getSize_pixel_y());
        for(int x = 0; x < image.getSize_pixel_x(); x++)
            for(int y = 0; y < image.getSize_pixel_y(); y++)
                for(int i = 0; i < image.getBytes_per_pixel(); i++)
                    imageAsoluteDifference[x][y][i] = abs(image[x][y][i]-image_segmentation[x][y][i]);
        ImageBmp(imageAsoluteDifference).save("Results/Segmentation/linescolorDiffKMean.bmp");
        ImageBmp(ImageTreatment(imageAsoluteDifference).convert_bits_per_pixel(GRAY_SCALE).threshold({20}, {255}).get()).save("Results/Segmentation/linescolortDiffKMeanBinary.bmp");
        */
        /*Segmentation KMean*/
        Image &image = imageFactory.open("Images/linescolor.bmp");
        KMean kMean(image, 8);
        auto cluster = kMean.compute();
        Matrix mean(cluster.size(), std::vector<double>(image.getBytes_per_pixel()));
        for(int i = 0; i < cluster.size(); i++){
            for(int j = 0; j < image.getBytes_per_pixel(); j++){
                mean[i][j] = cluster[i].mean[j];
            }
        }
        EM em = EM(image, mean);
        auto parameter = em.compute();
        Image image_segmentationEM = em.apply(parameter);
        ImageBmp(image_segmentationEM).save("Results/Segmentation/linescolorEM.bmp");

        /*Segmentation Otsu*/
        /*Image &image = imageFactory.open("Images/lena24.bmp");
        Otsu otsu = Otsu(ImageTreatment(image).convert_bits_per_pixel(GRAY_SCALE).get());
        auto threshold = otsu.compute();
        Image imageOtsu = otsu.apply(threshold);
        ImageBmp(ImageTreatment(imageOtsu).convert_bits_per_pixel(BINARY).get()).save("Results/Segmentation/lenaOtsu.bmp");*/

        /*Test mesure de similarité*/
        /*std::default_random_engine generator;
        generator.seed(time(nullptr));
        std::uniform_int_distribution<int> distributionX(0, image.getSize_pixel_x()-1);
        std::uniform_int_distribution<int> distributionY(0, image.getSize_pixel_y()-1);
        Image imageTranslate = ImageTreatment(image).transform(::translate(distributionX(generator),distributionY(generator))).get();
        
        auto differenceMinimal = ::difference(image, image);
        auto difference1 = ::difference(imageTranslate, image);
        auto difference2 = ::difference(image, imageTranslate);
        //Tests de propriétés Mesure de dissimilarité
        for(int i = 0; i < image.getBytes_per_pixel(); i++) assert(difference1[i] == difference2[i] && difference2[i] >= differenceMinimal[i] && differenceMinimal[i] >= 0);
        //Pour les tests de propriétés Mesure de similarité(Information Mutuel, CCm) : assert(difference1[i] == difference2[i] && difference2[i] <= differenceMinimal[i] && difference1[i] >= 0);
        */


        /*auto entropieConjointe = ::entropieConjointe(image, image);
        for(int i = 0; i < image.getBytes_per_pixel(); i++) std::cout << entropieConjointe[i] << std::endl;
        auto informationMutuelle = ::informationMutuelle(image, image);
        for(int i = 0; i < image.getBytes_per_pixel(); i++) std::cout << informationMutuelle[i] << std::endl;
        auto crossCorrelation = ::crossCorrelation(image, image);
        for(int i = 0; i < image.getBytes_per_pixel(); i++) std::cout << crossCorrelation[i] << std::endl;*/

        /*Test opérateurs*/
        //Image blackImage = std::move(image); //Copy temporaire -> Image 0
        //Image whiteImage = ImageTreatment(blackImage).invert().get(); // Inverse -> Image 255 
        
        //assert(whiteImage == image+ImageTreatment(image).invert().get());  // Mask + !mask = 255 
        //assert(blackImage == image-image);

       
        /*Traitement Pixel*/        
        //std::string name;
        //ImageTreatment imageTreatment(image);

        //name = "lena_invert";
        //imageTreatment.invert();

        //name = "lena_RGB_to_GB";
        //imageTreatment.mask({1,1,0});

        //name = "lena_RGB_to_BGR";
        //imageTreatment.exchange({2,1,0});

        //name = "lena_binary";
        //imageTreatment.convert_bits_per_pixel(GRAY_SCALE).threshold({128}, {255}).convert_bits_per_pixel(BINARY);
        
        //name = "lena_adjust_contrast";
        //imageTreatment.convert_bits_per_pixel(GRAY_SCALE).adjustContrast({60}, {200});
        //Image image_mask = (image & imageFactory.open("Images/mask.bmp"));
        //ImageBmp(image_mask).save("Results/lena_mask.bmp");


        //name = "lena_translate_100_100";
        //imageTreatment.transform(translate(100,100));
        
        //name = "lena_rotate_45";
        //imageTreatment.transform(rotate(45));
        
        //name = "lena_zoom_2";
        //imageTreatment.transform(zoom(2,2));
        
        //name = "lena_salt_and_pepper_noise";
        //imageTreatment.salt_and_pepper_noise(10000,10000);

        //name = "lena_gaussian_noise";
        //imageTreatment.gaussian_noise(0,50);

        //imageTreatment.gaussian_noise(0,50);
        //imageTreatment.salt_and_pepper_noise(10000,10000);
        //name = "lena_gaussian_noise_mean_filter";
        //name = "lena_salt_and_pepper_noise_mean_filter";
        //Matrix mean = {{1/9.0,1/9.0,1/9.0},{1/9.0,1/9.0,1/9.0},{1/9.0,1/9.0,1/9.0}};
        //imageTreatment.filter(mean);
        //name = "lena_gaussian_noise_median_filter";
        //name = "lena_salt_and_pepper_noise_median_filter";
        //imageTreatment.median_filter(3);
        //name = "lena_high_pass_filter";
        //Matrix high = {{-1/9.0,-1/9.0,-1/9.0},{-1/9.0,8/9.0,-1/9.0},{-1/9.0,-1/9.0,-1/9.0}};
        //imageTreatment.convert_bits_per_pixel(GRAY_SCALE).filter(high).threshold({10},{255}).convert_bits_per_pixel(BINARY);
        //ImageBmp(imageTreatment.get()).save("Results/"+name+".bmp");

        
    } catch(std::string const& error){
        std::cout << error << std::endl;
    }
}
