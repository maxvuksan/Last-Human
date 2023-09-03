#include "WeatherManager.h"
#include "../Game.h"
#include "StageManager.h"

WeatherManager::WeatherManager() {}


void WeatherManager::update() {

	for (int r = 0; r < game->get_stage_manager()->rooms.size(); r++) {
		Room* room = game->get_stage_manager()->rooms[r];
		for (int i = 0; i < room->get_entites().size(); i++) {
			if (room->get_entites()[i]->tag == "wire") {

				Wire* wire = dynamic_cast<Wire*>(room->get_entites()[i]);
				float x = sin(cycle_tracked) * wind_strength;
				float y = sin(cycle_tracked * 1.5f) * wind_strength / 4;
				wire->curve->set_anchor_offset(sf::Vector2f(x, y));
			}
		}
	}
	cycle_tracked += wind_speed * game->delta_time;
	if (cycle_tracked > 10 * M_PI) {
		cycle_tracked -= 10 * M_PI;
	}

}