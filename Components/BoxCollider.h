#pragma once
#include "Collider.h"


class Entity;
class BoxCollider: public Collider{

	protected:

		sf::Vector2f dimensions = sf::Vector2f(30, 50);
		sf::RectangleShape debug_shape;
		float bounce = 0;
		bool center;

	public:
		BoxCollider(Entity* entity);
		void draw(sf::RenderTexture& surface);
		void update() override;
		void move(sf::Vector2f movement, bool constraint_x=false, bool constraint_y=false);

		sf::Vector2f const get_dimensions();
		void set_dimensions(sf::Vector2f dimensions);
		void is_centered(bool);
		bool is_centered();

		std::vector<Collider*> check_for_collisions();

		bool point_in_collider(sf::Vector2f);
		bool point_in_collider(sf::Vector2i);
		bool overlapping(Collider& other);
		float left();
		float right();
		float top();
		float bottom();


};