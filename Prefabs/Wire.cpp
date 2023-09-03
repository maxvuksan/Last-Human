#include "Wire.h"

Wire::Wire(): Wire(sf::Vector2f(0,0), sf::Vector2f(0, 0), sf::Vector2f(0, 0)) {}

Wire::Wire(sf::Vector2f start, sf::Vector2f end, sf::Vector2f anchor) {
	tag = "wire";
	curve = add_component<BezierCurve>(true);
	curve->define_curve(start, end, anchor);
	curve->set_colour(sf::Color(85, 102, 84, 255));
}