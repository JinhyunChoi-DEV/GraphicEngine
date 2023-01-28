#pragma once
#include <string>

class Texture
{
public:
	Texture() = default;

	void Load(std::string path);

	int Width = 0;
	int Height = 0;
	int Channel = 0;
	unsigned int BufferObject = -1;
	unsigned char* Buffer = nullptr;
};
