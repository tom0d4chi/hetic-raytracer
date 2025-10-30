#include <iostream>
#include <cmath>
#include "Image.hpp"
#include "../lodepng/lodepng.h"


Image:: Image(unsigned int w, unsigned int h) : width(w), height(h)
{  
  buffer.resize(width * height);
  for (int i = 0; i < buffer.size(); ++i) {
    buffer[i] = Color();
  }
}

Image:: Image(unsigned int w, unsigned int h, Color c) : width(w), height(h)
{  
  buffer.resize(width * height);
  for (int i = 0; i < buffer.size(); ++i) {
    buffer[i] = c;
  }
}

Image::~ Image()
{
}

void Image::SetPixel(unsigned int x, unsigned int y, Color color) {
  unsigned int index = (y * width) + x;

  if (index >= buffer.size()) { throw std::invalid_argument("Image: Invalid index"); }
  buffer[index] = color;
}

Color Image::GetPixel(unsigned int x, unsigned int y) {
  unsigned int index = (y * width) + x;

  if (index >= buffer.size()) { throw std::invalid_argument("Image: Invalid index"); }
  return buffer[index];
}


void Image::WriteFile(const char * filename) {
  std::vector<unsigned char> image;
  image.resize(width * height * 4);
  for(unsigned index = 0; index < buffer.size(); index++) {
    Color pixel = buffer[index];
    int offset = index * 4;

    image[offset] = (unsigned int)floor(pixel.R() * 255); 
    image[offset + 1] = (unsigned int)floor(pixel.G() * 255); 
    image[offset + 2] = (unsigned int)floor(pixel.B() * 255); 
    image[offset + 3] = 255;      // Alpha
  }

  //Encode the image
  unsigned error = lodepng::encode(filename, image, width, height);

  //if there's an error, display it
  if(error) std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;
}