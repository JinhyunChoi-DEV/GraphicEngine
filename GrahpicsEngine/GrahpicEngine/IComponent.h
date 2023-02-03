/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: IComponent.h
Purpose: Making of header file for Component interface
Language: C++
Platform: Windows 11
Project: CS350_jinhyun.choi_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 2/2/2023
End Header --------------------------------------------------------*/

#pragma once

class IComponent
{
public:
	virtual ~IComponent() = default;
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Delete() = 0;
};