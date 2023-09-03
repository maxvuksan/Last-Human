#include "../Game.h"
#include "SceneManager.h"
#include "../Managers/StageManager.h"
#include "../Prefabs/Player.h"

void SceneManager::reset_room(Room* room) {
	current_room = room;
	std::vector<Entity*>& e = current_room->get_entites();
	for (int i = 0; i < e.size(); i++) {

		auto falling_block = dynamic_cast<FallingBlock*>(e[i]);
		if (falling_block != NULL) {
			falling_block->reset();
		}
	}
}

void SceneManager::player_death_delay(Room* room) {
	if (!resetting) {
		current_room = room;
		resetting = true;
		resetting_time_tracked = resetting_time;
	}
}

void SceneManager::update() {
	if (resetting) {
		if (resetting_time_tracked <= 0) {

			reset_room(current_room);
			resetting = false;

			Entity* new_player = new Player;
			game->add_entity(new_player);

			game->shaders["ColourGrading"]->setUniform("u_grey_scale_amount", 0.0f);
			screen_transition_in_tracked = screen_transition_in;
		}
		else {
			for (int i = 0; i < game->get_stage_manager()->music_layers.size(); i++) {
				float pitch = resetting_time_tracked / resetting_time;
				if (pitch < 0.1f){
					pitch = 0.1f;
				}
				game->get_stage_manager()->music_layers[i].setPitch(pitch);
			}

			game->shaders["ScreenTransition"]->setUniform("u_transition_amount", (10*(resetting_time - resetting_time_tracked)/ resetting_time) - 7.0f);
			game->shaders["ColourGrading"]->setUniform("u_grey_scale_amount", (3*(resetting_time - resetting_time_tracked) / resetting_time));

			resetting_time_tracked -= game->delta_time;
		}
	}
	else if (screen_transition_in_tracked > 0) {
		screen_transition_in_tracked -= game->delta_time;
		game->shaders["ScreenTransition"]->setUniform("u_transition_amount", screen_transition_in_tracked / screen_transition_in);

		for (int i = 0; i < game->get_stage_manager()->music_layers.size(); i++) {
			float pitch = 1 - screen_transition_in_tracked / screen_transition_in;
			if (pitch < 0.2f) {
				pitch = 0.2f;
			}
			game->get_stage_manager()->music_layers[i].setPitch(pitch);
		}
	}
	else {
		for (int i = 0; i < game->get_stage_manager()->music_layers.size(); i++) {
			game->get_stage_manager()->music_layers[i].setPitch(1);
		}
	}
}