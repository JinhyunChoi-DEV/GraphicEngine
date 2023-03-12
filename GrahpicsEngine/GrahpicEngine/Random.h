/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Random.h
Purpose: Making of header file for making random generator.
Language: C++
Platform: Windows 11
Project: CS350_jinhyun.choi_2
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 3/12/2023
End Header --------------------------------------------------------*/

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