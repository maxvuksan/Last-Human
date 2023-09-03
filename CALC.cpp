#include "CALC.h"

namespace CALC {

	sf::Vector2f angle_to_vector(float angle) {
		float dy = sin(angle * (M_PI / 180));
		float dx = cos(angle * (M_PI / 180));

		return sf::Vector2f(dx, dy);
	}

	float lerp(float a, float b, float t) {
		return (a * (1 - t)) + (b * t);
	}

	sf::Vector2f lerp(sf::Vector2f a, sf::Vector2f b, float t) {
		float sqt = t * t;
		float new_t = sqt / (2.0f * (sqt - t) + 1.0f);
		return (a * (1 - new_t)) + (b * new_t);
	}

	float distance(sf::Vector2f a, sf::Vector2f b) {
		return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
	}
}