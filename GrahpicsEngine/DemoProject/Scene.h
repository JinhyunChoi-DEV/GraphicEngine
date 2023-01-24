#pragma once
#include "Object.h"

class Scene
{
public:
	void Initialize();
	void Update();
	void Close();

private:
	Object* mainObject;
};
