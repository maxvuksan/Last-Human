#include "PhysicsBody.h"
#include "BoxCollider.h"
#include "../Game.h"

PhysicsBody::PhysicsBody(Entity* entity):Component(entity){}

void PhysicsBody::update() {

	if (!active) {
		return;
	}

	sf::Vector2f acceleration;

	if (gravity > 0) {
		if (gravity_tracked < gravity_max * gravity) {
			gravity_tracked += gravity_increment * gravity * entity->game->delta_time;
		}
		else {
			gravity_tracked = gravity_max * gravity;
		}
		acceleration.y += gravity_tracked * entity->game->delta_time * 40;

	}

	velocity += acceleration * (float)entity->game->delta_time;
	apply_counter_force(sf::Vector2f(drag,drag)); //applying drag

	if (abs(velocity.y) > max_velocity) {
		velocity.y = max_velocity * velocity.y / abs(velocity.y);
	}
	if (abs(velocity.x) > max_velocity) {
		velocity.x = max_velocity * velocity.x / abs(velocity.x);
	}

	if (constraint_x) {
		velocity.x = 0;
	}
	if (constraint_y) {
		velocity.y = 0;
	}
	//check if the entity has a collider, if so we move via the collider
	BoxCollider* collider = entity->get_component<BoxCollider>();

	//adding acceleration in relation to https://www.youtube.com/watch?v=hG9SzQxaCm8&t=124s
	sf::Vector2f movement = velocity * (float)entity->game->delta_time + (0.5f * acceleration * (float)(entity->game->delta_time * entity->game->delta_time));
	if (collider != nullptr) {
		collider->move(movement, constraint_x, constraint_y);
	}
	else { //a collider does not exist, so we can simply translate the entity
		get_entity()->transform->translate(movement);
	}
}

void PhysicsBody::apply_counter_force(sf::Vector2f force) {
	if (force.x != 0 || force.y != 0) {

		sf::Vector2f before_drag_velocity = velocity;
		float sign = abs(before_drag_velocity.x) / before_drag_velocity.x;
		//was the velocity positive or negative originally

		float applied_drag_x = 0.1 * force.x * entity->game->delta_time;
		float applied_drag_y = 0.1 * force.y * entity->game->delta_time;

		if (abs(velocity.x) > 0) {
			velocity.x -= velocity.x * applied_drag_x;
		}
		if (abs(velocity.y) > 0) {
			velocity.y -= velocity.y * applied_drag_y;
		}

		//the velocity has flipped signs (gone from positive to negative, therefore must be halted
		if (abs(before_drag_velocity.x - velocity.x) < 0.0001 * applied_drag_x) {
			velocity.x = 0;
		}
		if (abs(before_drag_velocity.y - velocity.y) < 0.0001 * applied_drag_y) {
			velocity.y = 0;
		}
	}
}


void PhysicsBody::restart_gravity() {
	gravity_tracked = 0;
}
void PhysicsBody::set_gravity(float gravity) {
	this->gravity = gravity;
}
float PhysicsBody::get_gravity() {
	return gravity;
}

void PhysicsBody::add_force(sf::Vector2f force) {
	velocity += force * (float)entity->game->delta_time;
}

void PhysicsBody::set_velocity(sf::Vector2f velocity) {
	this->velocity = velocity;
}
void PhysicsBody::set_velocity_x(float x) {
	velocity.x = x;
}
void PhysicsBody::set_velocity_y(float y) {
	velocity.y = y;
}
sf::Vector2f PhysicsBody::get_velocity() const {
	return velocity;
}

void PhysicsBody::set_constraint_x(bool x) {
	constraint_x = x;
}
void PhysicsBody::set_constraints(bool x, bool y){
	constraint_x = x;
	constraint_y = y;
}
void PhysicsBody::set_constraint_y(bool y) {
	constraint_y = y;
}
