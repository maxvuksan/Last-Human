#pragma once
#include "Component.h"


class BezierCurve : public Component {

	private:
		sf::Vector2f start;
		sf::Vector2f anchor;
		sf::Vector2f anchor_offset; //to animate said, curve, you can adjust the anchor offset, (e.g. through a sin wave)
		sf::Vector2f end;
		float resolution; //between 0 - 1
		sf::Color colour;


	public:
		BezierCurve(Entity*);

		void define_curve(sf::Vector2f start, sf::Vector2f end);
		void define_curve(sf::Vector2f start, sf::Vector2f end, sf::Vector2f anchor);
		void define_curve(sf::Vector2f start, sf::Vector2f end, sf::Vector2f anchor, float resolution, sf::Color colour);

		sf::Vector2f calculate_point(float t); //t represents 0-1 the position along the curve

		void draw(sf::RenderTexture& surface) override;

		void set_start(sf::Vector2f);
		void set_anchor(sf::Vector2f);
		void set_anchor_offset(sf::Vector2f);
		void set_end(sf::Vector2f);
		void set_resolution(float);
		void set_colour(sf::Color);

		sf::Vector2f get_start();
		sf::Vector2f get_end();
		sf::Vector2f get_anchor();
};