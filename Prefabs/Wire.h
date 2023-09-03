#pragma once
#include "../Entity.h"
#include "../COMPONENTS.h"

class Wire : public Entity{
	public:
		BezierCurve* curve;

		Wire();
		Wire(sf::Vector2f start, sf::Vector2f end, sf::Vector2f anchor);

};