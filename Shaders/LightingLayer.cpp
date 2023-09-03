#include "LightingLayer.h"
#include "../Entity.h"
#include "../Game.h"

LightingLayer::LightingLayer(std::string name): Shader(name){}

void LightingLayer::set_focused_light(LightSource* source) {

	setUniform("u_intensity", source->intensity);
	setUniform("u_drop_off", source->drop_off);
	setUniform("u_feather", source->feather);
	setUniform("u_colour", sf::Glsl::Vec4(source->colour));

	sf::Vector2f position = source->get_entity()->transform->get_draw_position() + source->offset;
	position.x /= game->get_display_size().x;
	position.y /= game->get_display_size().y;
	position.y = 1.0 - position.y;
	setUniform("u_light_position", position);
}