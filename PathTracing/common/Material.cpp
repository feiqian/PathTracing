#include <iostream>
#include "Material.h"

Texture::Texture(string filename)
{
	FREE_IMAGE_FORMAT imageFormat = FreeImage_GetFIFFromFilename(filename.c_str());
	if(imageFormat==-1)
	{
		cout<<"unsupported texture type : "+filename<<endl;
		image = NULL;
		return;
	}

	image = FreeImage_Load(imageFormat,filename.c_str());
	width = FreeImage_GetWidth(image);
	height = FreeImage_GetHeight(image);
}

Color3 Texture::getColor(const Vec2& uvCoord)
{
	if(image==NULL) return Color3::WHITE;

	unsigned int i = ((unsigned int)(uvCoord.x * width)) % width;
	unsigned int j = ((unsigned int)(uvCoord.y * height)) % height;

	RGBQUAD color;
	FreeImage_GetPixelColor(image, i, j, &color);
	return Color3(color.rgbRed / 255.0, color.rgbGreen / 255.0, color.rgbBlue / 255.0);
}