#include "Component.h"
#include "Transform.h"
#include "../Entity.h"
#include "../CALC.h"
#include "../Game.h"

Transform::Transform(Entity* entity) : Component(entity)
{
	position = sf::Vector2f(0, 0);
	rotation = 0;
	parent = nullptr;
}

sf::Vector2f Transform::get_local_position() {
	return local_position;
}
sf::Vector2f Transform::get_position() 
{ 
	if (parent == nullptr) {
		return position;
	}

	if (rotation != 0) {
		double angle_between = atan(local_position.y / local_position.x); //in radians

		double final_angle = parent->get_rotation() + angle_between * (180 / M_PI);

		int x_multi = 1;
		if (local_position.x < 0) {
			x_multi = -1;
		}

		int y_multi = 1;
		if (local_position.y > 0) {
			y_multi = -1;
		}


		sf::Vector2f dir = CALC::angle_to_vector(final_angle);
		double distance = CALC::distance(local_position, parent->position);

		return parent->get_position() + sf::Vector2f(dir.x * distance * x_multi, dir.y * distance);
	}
	return local_position + parent->get_position();
	
}

sf::Vector2f Transform::get_draw_position() //the current position + camera offset
{
	return get_position() - get_entity()->game->get_camera()->get_position();
}

void Transform::set_position(sf::Vector2f new_position) 
{ 
	if (parent != nullptr) {
		this->position = new_position + parent->position;
	}
	else {
		this->position = new_position;
	}
}
void Transform::set_local_position(sf::Vector2f new_position) {
	this->local_position = new_position;
	set_position(new_position);
}
void Transform::set_position_x(float x) {
	position.x = x;
}
void Transform::set_position_y(float y) {
	position.y = y;
}

void Transform::translate(sf::Vector2f movement) {
	
	if (parent == nullptr) {
		this->position += movement;
	}
	else {
		this->local_position += movement;
	}
}

void Transform::set_active(bool state) 
{ 
	std::cout << "component 'Transform' cannot have a state change" << std::endl; 
}
void Transform::set_parent(Transform* parent) 
{ 
	this->parent = parent;
	parent->add_child(this);

	this->local_position = position - parent->get_position();
}

void Transform::add_child(Transform* child) {
	
	bool child_already_exists = false;
	for (int i = 0; i < children.size(); i++) {

		if (child->get_entity() == children[i]->get_entity()) {
			child_already_exists = true;
		}
	}

	if (!child_already_exists) {
		children.push_back(child);
	}
}


double Transform::get_rotation() {
	if (parent == nullptr) {
		return rotation;
	}
	return rotation + parent->get_rotation();
}
void Transform::set_rotation(double rotation) {
	this->rotation = rotation;
}
void Transform::rotate(double rotation) {
	this->rotation += rotation;
}

Transform::~Transform() {
	for (int i = 0; i < children.size(); i++) {
		get_entity()->game->remove_entity(children[i]->get_entity());
	}
}