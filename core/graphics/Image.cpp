#include "Image.h"
#include <iostream>
#include <fstream>

Image::Image(const char *name){

  std::ifstream TextureStream(name, std::ios::in);

  if(TextureStream.is_open()){
      TextureStream.close();
      FREE_IMAGE_FORMAT format = FreeImage_GetFileType(name,0);
      img = FreeImage_Load(format,name);
      bPP =  FreeImage_GetBPP(img);
      if( bPP == 32 ){
        img32 = img;
      }else{
        img32 = FreeImage_ConvertTo32Bits(img);
      }
      w = FreeImage_GetWidth(img32);
      h = FreeImage_GetHeight(img32);
      tex = FreeImage_GetBits(img32);

  }else{
    std::cout << "Impossible to open " << name << std::endl;
    exit(0);
  }
}

Image::~Image(){
  FreeImage_Unload(img32);
  if( bPP != 32 ) FreeImage_Unload(img);
}
