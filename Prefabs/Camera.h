#pragma once
#include <SFML/Graphics.hpp>
#include "../Entity.h"


class Camera: public Entity {
	private:


		float zoom;

		//for smooth transition between 2 points
		float time_to_travel_tracked = 0;
		float time_to_travel = 0;
		sf::Vector2f travel_intial_position;
		sf::Vector2f travel_target_position;

	public:
		Camera();
		void update();

		void set_zoom(float);
		void add_zoom(float);
		float get_zoom();

		void translate(sf::Vector2f movement);
		void set_position(sf::Vector2f position);
		void travel_to_point(sf::Vector2f position, float time_to_travel);
		sf::Vector2f get_position();
		sf::Vector2f get_position_raw();

};