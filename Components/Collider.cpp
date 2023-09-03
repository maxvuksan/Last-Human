#include "Collider.h"
#include "../Game.h"

Collider::Collider(Entity* entity) : Component(entity)
{
	collision_layer = entity->collision_layer;
	entity->game->add_collider(this);

	render_layer = 99; //drawing ui ontop of everything
	entity->game->determine_required_render_layers();
}


Collider::~Collider() {
	entity->game->remove_collider(this);
}

sf::Vector2f Collider::get_friction() {
	return sf::Vector2f(friction_x, friction_y);
}

void Collider::set_friction(float val) {
	friction_x = val;
	friction_y = val;
}
void Collider::set_friction(float x, float y) {
	friction_x = x;
	friction_y = y;
}
void Collider::set_friction_x(float val) {
	friction_x = val;
}
void Collider::set_friction_y(float val) {
	friction_y = val;
}