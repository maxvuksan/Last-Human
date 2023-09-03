#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <list>
#include <iterator>
#include <filesystem>

#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"

#include <json/json.h>

#include "COMPONENTS.h"
#include "PREFABS.h"
#include "SHADERS.h"

//namespaces
#include "CALC.h"
#include "VFX.h"

#include "Prefabs/Camera.h"
#include "Prefabs/Room.h"
#include "Managers/WeatherManager.h"
#include "Managers/SceneManager.h"

class StageManager;

class Game {
	protected:
		int display_width = 300;//when zoom = 1
		int display_height = 200;

		sf::Vector2f zoom_display_size;
		int zoom_edge_buffer = 4; //pixel edge buffer for when the screen is zooming

		int window_width = 900;
		int window_height = 600;

		sf::RenderTexture* display;
		sf::RenderWindow* window;

		sf::Color background_colour = sf::Color(137, 151, 135);
		sf::Clock clock;
		sf::SoundBuffer sound_buffer;

		//for the link between sf::Sound and sf::SoundBuffer, sounds must not be resized besides on init
		std::vector<sf::Sound> sounds;
		std::vector<sf::SoundBuffer> sound_buffers;
		int current_sound_index;
		int max_sound_size = 30; //max number of sounds that can be played in unison

		Camera* camera;

		unsigned int entity_id_counter = 0;
		std::vector<Entity*> entites;
		std::vector<Entity*> remove_queue; //removes these entites after update
		std::vector<Collider*> colliders;
		std::vector<LightSource*> light_sources;

		Entity* player = nullptr;
		SceneManager* scene_manager;
		StageManager* stage_manager;
		WeatherManager* weather_manager;

		//add this to the editor
		int collision_matrix[4][4] =
		{
			{0, 1, 2, -1}, //player
			{0, 1, 2, 3}, //player triggers
			{0, 2, -1, -1}, //spikes
			{1, -1, -1, -1} //oil
		}; 

		std::vector<int> required_render_layers;
	public:

		sf::Font font;

		std::map<std::string, Shader*> shaders;
		int cellsize = 8;

		bool debug_mode;

		long double time; //time since game started in seconds
		 double delta_time;

		Game();
		~Game();

		void run();
		void shader_update();
		void update();
		void manage_events();

		virtual void awake();
		virtual void general_update();
		virtual void general_manage_events(sf::Event);

		void determine_required_render_layers();

		void clear_remove_queue();
		void add_entity(Entity* entity);
		void remove_entity(Entity* entity);

		void add_light_source(LightSource* source);
		void remove_light_source(LightSource* source);
		void add_collider(Collider* collider);
		void remove_collider(Collider* collider);
		std::vector<Entity*>& get_entities();
		std::vector<Collider*>& get_colliders();
		bool collision_matrix_check(int layer1, int layer2);

		Entity* get_player();
		Camera* get_camera();
		StageManager* get_stage_manager();
		SceneManager* get_scene_manager();

		sf::RenderTexture& const get_display();
		sf::RenderWindow*& const get_window();
		int get_zoom_edge_buffer();
		sf::Vector2f get_zoom_display_size();
		sf::Vector2f get_display_size();
		sf::Vector2f get_window_size();
		void set_window_size(int x, int y);

		Entity* find_entity_by_tag(std::string tag);

		void play_sound(std::string filename, float volume=1);

};
