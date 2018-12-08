#pragma once

#include <cstring>
#include <vector>
#include <fstream>
#include <iostream>
#include <map>
#include <stdint.h>
#include "lodepng.h"

class	Texture
{
private:

	struct TextureInfo
	{
		std::vector<uint8_t>* data;
		unsigned width;
		unsigned height;
	};

	static std::map<std::string, TextureInfo> _cache;
	std::vector<uint8_t> *_imageData;
	unsigned _width;
	unsigned _height;
	
public:

	//! Only supports png currently
	Texture(std::string filepath);

	unsigned Width(void);
	unsigned Height(void);
	uint8_t* Data(void);
};
