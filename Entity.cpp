#include "Entity.h"
#include "Game.h"
#include "Components/Component.h"

Entity::Entity(): Entity(sf::Vector2f(0,0)) {}

Entity::Entity(sf::Vector2f position)
{
	transform = new Transform(this);
	components.push_back(transform);

	transform->set_position(position);
} 

void Entity::define_game_parent(Game* game) {
	this->game = game;
}

Entity::~Entity() 
{ 
	for (int i = 0; i < components.size(); i++) {
		delete components[i];
	}
}

void Entity::set_render_layer(int layer) {
	render_layer = layer;
	game->determine_required_render_layers();
}
int Entity::get_render_layer() {
	return render_layer;
}

void Entity::remove_in(float time) {
	if (remove_started) {
		if (time < remove_time_tracked) {
			remove_time_tracked = time;
		}
	}
	else {
		remove_started = true;
		remove_time_tracked = time;
	}
}

void Entity::track_remove_time() {
	if (remove_started) {

		if (remove_time_tracked > 0) {
			remove_time_tracked -= game->delta_time / 10;
		}
		else {
			game->remove_entity(this);
		}
	}
}