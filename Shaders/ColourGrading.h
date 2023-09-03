#pragma once
#include <SFML/Graphics.hpp>
#include "Shader.h"

class ColourGrading : public Shader {

	private:
		float strength = 4.0f;

	public:
		ColourGrading(std::string name);
		void update() override;
};