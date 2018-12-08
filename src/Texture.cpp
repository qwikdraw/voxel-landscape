#include "Texture.hpp"

std::map<std::string, Texture::TextureInfo> Texture::_cache;

Texture::Texture(std::string filepath)
{
	if (_cache.count(filepath) != 0)
	{
		TextureInfo info = _cache[filepath];
		_width = info.width;
		_height = info.height;
		_imageData = info.data;
	}
	else
	{
		_imageData = new std::vector<uint8_t>;
		unsigned error = lodepng::decode(*_imageData, _width, _height, filepath.c_str());

		if (error)
			std::cerr << "Error loading texture `" << filepath
				<< "`: " << lodepng_error_text(error) << std::endl;

		std::vector<uint8_t> temp(_width * 4);
	
		for (size_t i = 0; i < _height / 2; i++)
		{
			std::memmove(&temp[0],
				&(*_imageData)[i * _width * 4],
				_width * 4);
			std::memmove(&(*_imageData)[i * _width * 4],
				&(*_imageData)[(_height - i - 1) * _width * 4],
				_width * 4);
			std::memmove(&(*_imageData)[(_height - i - 1) * _width * 4],
				&temp[0],
				_width * 4);
		}
		TextureInfo info = {_imageData, _width, _height};
		_cache[filepath] = info;
	}
}

unsigned Texture::Width(void)
{
	return _width;
}

unsigned Texture::Height(void)
{
	return _height;
}

uint8_t* Texture::Data(void)
{
	return _imageData->data();
}
