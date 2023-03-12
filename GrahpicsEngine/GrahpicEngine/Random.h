#pragma once
#include <glm/glm.hpp>
#include <random>

inline std::random_device device;
inline std::mt19937_64 RNGen(device());
inline std::uniform_real_distribution<float> myrandom(0.0f, 1.0f);

inline float GetRandom()
{
	return myrandom(RNGen);
}

inline glm::vec3 GetRandomColor()
{
	float r = GetRandom();
	float g = GetRandom();
	float b = GetRandom();

	return { r,g,b };
}

inline int GetRandomIndex(int size)
{
	std::uniform_int_distribution<int> distribution(0, size - 1);
	return distribution(RNGen);
}