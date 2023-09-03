#include "Component.h"
#include "../Entity.h"
#include "../Game.h"

Component::Component(Entity* entity) {
	this->entity = entity;
	this->render_layer = entity->get_render_layer(); 
	active = true;
}


int Component::get_render_layer() {
	return render_layer;
}
void Component::set_render_layer(int new_layer) {
	render_layer = new_layer;
	entity->game->determine_required_render_layers();
}