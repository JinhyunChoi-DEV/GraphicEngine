/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2023> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Input.cpp
Purpose: Making of implementing for Input systems including keyboard and mouse for window.
Language: C++
Platform: Windows 11
Project: CS350_jinhyun.choi_1
Author: Jinhyun Choi / jinhyun.choi / 0055642
Creation date: 2/2/2023
End Header --------------------------------------------------------*/

#include "Input.h"

Input* INPUT = nullptr;
Input::Input()
{
	if (INPUT == nullptr)
		INPUT = this;

	clear();
}

void Input::Initialize()
{
	clear();
}

void Input::Update()
{
	keyTriggered.reset();
	mouseButtonTriggered.reset();
}

void Input::Terminate()
{
	clear();
}

void Input::SetKeyboard(int key, int action)
{
	KeyActionState state = convert(action);

	switch (state)
	{
	case KeyActionState::Release:
		keyPressed.reset(key);
		keyTriggered.reset(key);
		break;

	case KeyActionState::Press:
		keyPressed.set(key);
		keyTriggered.set(key);
		break;

	case KeyActionState::Repeat:
		keyPressed.set(key);
		keyTriggered.reset(key);
		break;

	case KeyActionState::None:
		break;
	}
}

void Input::SetMouse(double x, double y)
{
	mousePosition.x = (float)x;
	mousePosition.y = (float)y;
}

void Input::SetMouseButton(int key, int action)
{
	KeyActionState state = convert(action);

	switch (state)
	{
	case KeyActionState::Release:
		mouseButtonPressed.reset(key);
		mouseButtonTriggered.reset(key);
		break;

	case KeyActionState::Press:
		mouseButtonPressed.set(key);
		mouseButtonTriggered.set(key);
		break;

	case KeyActionState::Repeat:
		mouseButtonPressed.set(key);
		mouseButtonTriggered.reset(key);
		break;

	case KeyActionState::None:
		break;
	}
}

bool Input::KeyPressed(int key)
{
	return keyPressed[key];
}

bool Input::KeyReleased(int key)
{
	return !keyPressed[key];
}

bool Input::KeyTriggered(int key)
{
	return keyTriggered[key];
}

bool Input::MouseButtonPressed(int key)
{
	return mouseButtonPressed[key];
}

bool Input::MouseButtonReleased(int key)
{
	return !mouseButtonPressed[key];
}

bool Input::MouseButtonTriggered(int key)
{
	return mouseButtonTriggered[key];
}

void Input::clear()
{
	keyPressed.reset();
	keyTriggered.reset();
	mouseButtonPressed.reset();
	mouseButtonTriggered.reset();
}

KeyActionState Input::convert(int key)
{
	if (key == GLFW_RELEASE)
		return KeyActionState::Release;

	if (key == GLFW_PRESS)
		return KeyActionState::Press;

	if (key == GLFW_REPEAT)
		return KeyActionState::Repeat;

	return KeyActionState::None;
}
