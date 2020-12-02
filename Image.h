#ifndef IMAGE_H
#define IMAGE_H
#include <string>
#include <vector>

#include <iostream>
#define EXCEPTIONS
typedef std::vector<unsigned char> Pixel;
typedef Pixel **ImageMatrix;
typedef Pixel *ImageLineMatrix;

class Image{

public:
	Image() = default;
	Image(const Image &);
	Image(const Image &&);
	Image(unsigned int, unsigned int, unsigned int, unsigned int);

	Image & operator +=(const Image &);
	Image & operator -=(const Image &);
	
	ImageLineMatrix operator [](const int);
	const ImageLineMatrix operator [](const int) const;
	
	friend bool operator ==(const Image &, const Image &);
	friend Image operator +(const Image &, const Image &);
	friend Image operator -(const Image &, const Image &);
	friend Image operator &(const Image &, const Image &);

	friend void dimTestImageException(const Image &, const Image &);
	void dimTestPixelException(const Pixel &);
	const unsigned int getBits_per_pixel() const;
	const unsigned int getBytes_per_pixel() const;
	const unsigned int getSize() const;
	const unsigned int getSize_pixel_x() const;
	const unsigned int getSize_pixel_y() const;

	void set_pixel_format(const unsigned int);
	virtual ~Image();
private:
	ImageMatrix pixels;
	unsigned int bits_per_pixel;
	unsigned int bytes_per_pixel;
	unsigned int size;
	unsigned int size_pixel_x;
	unsigned int size_pixel_y;
	void allocation();

};
#endif