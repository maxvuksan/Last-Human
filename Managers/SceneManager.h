#pragma once
#include "../Entity.h";

class Game;
class SceneManager : public Entity{
	private:


		Room* current_room = nullptr;
		bool resetting = false;
		float resetting_time_tracked;
		float resetting_time = 16; //how long until room reset;

		float screen_transition_in = 2.0f;
		float screen_transition_in_tracked;

	public:
		void update() override;
		void player_death_delay(Room* room);
		void reset_room(Room* room);

};