#include "RangeInteraction.h"
#include "../Game.h"

RangeInteraction::RangeInteraction(Entity* entity):Component(entity){}




void RangeInteraction::set_target(Entity* entity) {
	target = entity;
}

void RangeInteraction::update() {



	in_range = false;
	if (target != nullptr) {

		if (CALC::distance(entity->transform->get_position(), target->transform->get_position()) <= distance) {
			if (time_threshhold_tracked < time_threshhold) {
				time_threshhold_tracked += entity->game->delta_time;
			}
			else {
				in_range = true;
			}
		}
		else {
			time_threshhold_tracked = 0;
		}
	}
}