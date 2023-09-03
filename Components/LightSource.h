#pragma once
#include "Component.h"

class LightSource : public Component {
	public:
		sf::Color colour = sf::Color(255, 150, 100);
		float intensity = 0.1;
		float feather = 0.12; //from drop off edge, to no light
		float drop_off = 0.13; //how far the light colour stays constant

		sf::Vector2f offset;

		LightSource(Entity* entity);
		~LightSource();
};