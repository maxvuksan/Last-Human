#pragma once

#include "Component.h"
#include "../Entity.h"

class PhysicsBody : public Component {

	protected:
		sf::Vector2f velocity;

		//all values are positive
		float gravity = 2; //the strength of the gravity, applies to gravity max aswell
		float gravity_increment = 4; //the amount gravity force increases per frame
		 //the current effect gravity has have on the velocity (because the value won't be constant)
		float gravity_max = 16; 
		float max_velocity = 100;
		float drag = 13;//13;
		bool constraint_x = false;
		bool constraint_y = false;

	public:
		float gravity_tracked = 0;
		PhysicsBody(Entity*);

		void update() override;

		void apply_counter_force(sf::Vector2f force);
		void apply_friction(sf::Vector2f friction);

		void add_force(sf::Vector2f force);

		void set_velocity(sf::Vector2f velocity);
		void set_velocity_x(float x);
		void set_velocity_y(float y);
		sf::Vector2f get_velocity() const;

		void restart_gravity(); //resets the gravity tracked value
		float get_gravity();
		void set_gravity(float gravity); 

		void set_constraint_x(bool);
		void set_constraints(bool, bool);
		void set_constraint_y(bool);

};