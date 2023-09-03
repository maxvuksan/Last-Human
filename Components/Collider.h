#pragma once

#include "Component.h"

enum CollisionMode {
	AllSided, 
	TopSided, //only collides with top side, for platforms
};

class Entity;
class Collider: public Component {

	protected:
		std::vector<Collider*> colliders_in_trigger; //if the collider is a trigger
		float friction_x = 2;
		float friction_y = 0;

	public:
		CollisionMode collision_mode = AllSided;
		bool is_trigger = false;
		bool triggered = false; //when the collider is either overlapping (for triggers) 
		int collision_layer; 
		sf::Vector2f offset;

		Collider(Entity*);
		virtual ~Collider();

		virtual void move(sf::Vector2f movement){}
		virtual std::vector<Collider*> check_for_collisions() { return std::vector<Collider*>(); } //creates a list of all the collisions which DID overlap with this collider
		std::vector<Collider*>& const get_colliders_in_trigger() { return colliders_in_trigger; }
		virtual bool point_in_collider(sf::Vector2f) { return false; }
		virtual bool point_in_collider(sf::Vector2i) { return false; }
		
		virtual float left() { return 0; }
		virtual float right(){ return 0; }
		virtual float top(){ return 0; }
		virtual float bottom(){ return 0; }

		sf::Vector2f get_friction();
		void set_friction(float);
		void set_friction(float x, float y);
		void set_friction_x(float);
		void set_friction_y(float);
};
