#pragma once
#include "Camera.h"
#include "../Game.h"
#include "../CALC.h"

Camera::Camera() {
	zoom = 1;
}

void Camera::translate(sf::Vector2f movement) {
	transform->translate(movement);
}
void Camera::set_position(sf::Vector2f position) {
	transform->set_position(position);
}
sf::Vector2f Camera::get_position() {
	return transform->get_position() - (game->get_zoom_display_size() / (float)2);
}
sf::Vector2f Camera::get_position_raw() {
	return transform->get_position();
}

void Camera::set_zoom(float amount) {
	zoom = amount;
}
void Camera::add_zoom(float amount) {
	zoom += amount;
	if (zoom < 0.01f) {
		zoom = 0.01f;
	}
}

float Camera::get_zoom() {
	return zoom;
}

void Camera::travel_to_point(sf::Vector2f position, float time_to_travel) {

	this->travel_intial_position = get_position_raw();
	this->time_to_travel_tracked = time_to_travel;
	this->travel_target_position = position;
	this->time_to_travel = time_to_travel;
}

void Camera::update() {
	//translate(sf::Vector2f(inputs.left + inputs.right, inputs.up + inputs.down) * move_speed * game->get_dt());
	if (time_to_travel_tracked > 0) {
		float t = 1 - time_to_travel_tracked / time_to_travel;

		set_position(CALC::lerp(travel_intial_position, travel_target_position, t));

		time_to_travel_tracked -= game->delta_time;
	}
}