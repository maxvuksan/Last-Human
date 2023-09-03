#pragma once
#include "Shader.h"
#include "../Components/LightSource.h"

class LightingLayer : public Shader {

	public:
		LightingLayer(std::string name);
		void set_focused_light(LightSource* source);
};