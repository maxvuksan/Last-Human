#pragma once

#include <cmath>
#include <corecrt_math_defines.h>
#include "SFML/Graphics.hpp"

namespace CALC {
	//converts an angle (in degrees) to a vector
	sf::Vector2f angle_to_vector(float angle);

	float lerp(float a, float b, float t);
	sf::Vector2f lerp(sf::Vector2f a, sf::Vector2f b, float t); 

	float distance(sf::Vector2f a, sf::Vector2f b); 	

}