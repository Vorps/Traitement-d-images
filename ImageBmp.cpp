#include "ImageBmp.h"
#include <iostream>
#include <cstring>


ImageBmp::ImageBmp(Image &image) : ImageFile(image){
    header.header_struct.size_file.value = 0;
	header.header_struct.reserved_1.value = 0; //Non modifié
	header.header_struct.reserved_2.value = 0; //Non modifié
	header.header_struct.offset.value = 0;
	header.header_struct.size_header.value = 40; //Non modifié
	header.header_struct.width.value = 0;
	header.header_struct.height.value = 0;
	header.header_struct.color_planes.value = 1; //Non modifié
	header.header_struct.bits_per_pixel.value = 0;
	header.header_struct.compression.value = 0; //Non modifié
	header.header_struct.size_image.value = 0;
	header.header_struct.pixel_per_meter_x.value = 4724; //Non modifié
	header.header_struct.pixel_per_meter_y.value = 4724; //Non modifié 
	header.header_struct.number_color.value = 0;
	header.header_struct.important_color.value = 0; //Non modifié
}

ImageBmp::ImageBmp(std::ifstream &file_image) : ImageFile(){
    file_image.read((char *)this->header.header, sizeof(this->header));
    
    int size_pixel_x =  this->header.header_struct.width.value;
    int size_pixel_y =  this->header.header_struct.height.value;
  
    this->image = new Image(this->header.header_struct.bits_per_pixel.value,  size_pixel_x * size_pixel_y, size_pixel_x, size_pixel_y);
    for(int x = 0; x < this->image->getSize_pixel_x(); x++)
        for(int y = 0; y < this->image->getSize_pixel_y(); y++)
            file_image.read((char *) (*this->image)[x][y].data(), this->image->getBytes_per_pixel());
  
}

void ImageBmp::save(const std::string image_path){
    std::ofstream file_image(image_path, std::ios::out|std::ios::binary); 
    if(file_image.is_open()){
    	/*----EDIT HEADER----*/
        unsigned char id[2] = {0x42, 0x4d};
    	file_image.write((char *)id, 2); // Ecriture du nombre magic correspondant au format BMP

	    this->header.header_struct.width.value = this->image->getSize_pixel_x();
	    this->header.header_struct.height.value = this->image->getSize_pixel_y();

        this->header.header_struct.bits_per_pixel.value = this->image->getBits_per_pixel();
        this->header.header_struct.size_image.value = this->image->getSize()*this->image->getBytes_per_pixel(); // Set la taille de l'image
        this->header.header_struct.offset.value = LONGUEUR_ENTETE+2;
        
       
        switch(this->image->getBits_per_pixel()){
            case 1:
                this->header.header_struct.number_color.value = 2;
                break;
            case 8:
                this->header.header_struct.number_color.value = 256;
                break;
            default:
                this->header.header_struct.number_color.value = 0;
                break;
        }
        this->header.header_struct.offset.value += this->header.header_struct.number_color.value*4;// Set le début de valeur de pixels
        this->header.header_struct.size_file.value = this->header.header_struct.offset.value+this->header.header_struct.size_image.value; //Set la taille du fichier
        file_image.write((char *)this->header.header, LONGUEUR_ENTETE);

        /*----COLOR TABLE----*/
        std::vector<unsigned char> color_table;
        switch(this->image->getBits_per_pixel()){
            case 1:
                color_table = {0,0,0,0,255,255,255,0};
                file_image.write((char *) color_table.data(), color_table.size());
                break;
            case 8:
                for(unsigned int i = 0; i < 256; i++)
                    color_table.insert(color_table.end(), {(unsigned char)i, (unsigned char)i, (unsigned char)i, 0});
                
                file_image.write((char *) color_table.data(), color_table.size());
                break;
        }
        /*----WRITE PIXEL----*/
        std::vector<unsigned char> pixels_data;
        switch(this->image->getBits_per_pixel()){
            case 1:
                pixels_data.push_back(0);
                int u;
                u = 0;
                for(int x = 0; x < this->image->getSize_pixel_x(); x++){
                     for(int y = 0; y < this->image->getSize_pixel_y(); y++){
                        if(++u == 8){
                            pixels_data.push_back(0);
                            u = 0;
                        }
                        pixels_data.back() = (pixels_data.back() << 1) | ((*this->image)[x][y][0] & 1);
                    }
                }
                file_image.write((char *) pixels_data.data(), pixels_data.size());
                break;
            default:
                for(int x = 0; x < this->image->getSize_pixel_x(); x++)
                     for(int y = 0; y < this->image->getSize_pixel_y(); y++)
                        file_image.write((char *) (*this->image)[x][y].data(), (*this->image).getBytes_per_pixel());
                break;
                
        }
        file_image.close();
    } else throw std::string("ERREUR : Le fichier ne peut pas être ouvert !!!");
}