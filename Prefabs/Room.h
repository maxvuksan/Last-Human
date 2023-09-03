#pragma once
#include "../COMPONENTS.h"

class Room; //forward reference for the struct
struct Adjacent_Room {
	sf::Vector2f offset;
	Room* room;
};

class Room: public Entity {
	private:
		int cellsize;
		sf::RectangleShape rect;
		sf::Vector2f dimensions;
		sf::Vector2f spawn_point;
		sf::Vector2f camera_position;

		std::vector<Entity*> entites;

		Tilemap* tilemap;

		BoxCollider* bounds_trigger;
		BoxCollider* camera_trigger;

		float camera_buffer = 20;
		bool selected;
		bool in_room = false;

	public:
		bool music_layers[4] = { true, false, false, false };
		std::vector<Adjacent_Room> adjacent_rooms;

		Room(int cellsize);
		Room(int cellsize, sf::Vector2f dimensions);
		Room(int cellsize, sf::Vector2f dimensions, sf::Vector2f spawn_point);
		~Room();
		void set_select(bool);
		void set_spawnpoint(sf::Vector2f spawnpoint);

		void remove_entity(Entity* entity);
		void add_entity(Entity* entity);

		void remove_all_wires();

		sf::Vector2f get_spawnpoint();
		sf::Vector2f get_spawnpoint_world_position();
		sf::Vector2f get_camera_position();
		void set_camera_position(sf::Vector2f);
		void awake() override;
		void draw(sf::RenderTexture&) override;
		void update() override;
		sf::Vector2f get_dimensions();
		BoxCollider* get_bounds_trigger();

		std::vector<Entity*>& get_entites();

		void resize_room(int anchor_index, sf::Vector2f difference);

};