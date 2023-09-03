#pragma once
#include "Shader.h"

class ScreenTransition : public Shader {
	
	sf::Color colour = sf::Color(37, 35, 40);

	public:
		ScreenTransition(std::string name);
		void update() override;
};