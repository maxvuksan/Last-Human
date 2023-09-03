#include "LightSource.h"
#include "../Entity.h"
#include "../Game.h"

LightSource::LightSource(Entity* entity): Component(entity)
{
	entity->game->add_light_source(this);
}

LightSource::~LightSource() {
	entity->game->remove_light_source(this);
}