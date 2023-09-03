#pragma once
#include "Component.h"

class Transform: public Component {

	protected:
		sf::Vector2f position; 
		sf::Vector2f local_position;
		double rotation;

		Transform* parent = nullptr; //implement a parent child hierachay system
		std::vector<Transform*> children;

	public:
		~Transform();
		Transform(Entity*);

		sf::Vector2f get_position();
		sf::Vector2f get_local_position();
		sf::Vector2f get_draw_position();
		void set_position(sf::Vector2f position);
		void set_position_x(float x);
		void set_position_y(float y);
		void set_local_position(sf::Vector2f position);
		void translate(sf::Vector2f movement);

		double get_rotation();
		void set_rotation(double rotation);
		void rotate(double rotation);

		void set_active(bool state);
		void set_parent(Transform* parent);
		void add_child(Transform* child);
		std::vector<Transform*>& get_children();
		void remove_child(Transform* child);
};