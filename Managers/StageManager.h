#pragma once
#include "../Prefabs/Room.h"
#include "../Game.h"

enum TilemapLayers {
	Ground,
	Hazard,
	Background,
	Foreground,
	Oil,
	//
	TILEMAP_LAYER_SIZE //always last layer
};

struct TilemapLayerProperties {
	bool collisions = false;
	bool is_trigger = false;
	int collision_layer = 0;
	int render_layer = 0;
};

struct Tiledata {
	std::string name;
	std::string filename;

	TilemapLayers tile_layer;

	int cellsize;
	sf::Vector2f texture_offset;// = sf::Vector2f(0, 0);
	sf::Texture* texture;

	CollisionMode collision_mode;
	sf::Vector2f collision_shape;
	sf::Vector2f collision_offset;
};

struct BackgroundLayer {
	std::string filename;
	float parallax_speed = 0.3;
	float scroll_speed = 0;
	sf::Vector2f position;
	sf::Texture texture;
	float offsets[3] = { -1, 0, 1 };
};


class StageManager : public Entity{

	public:
		std::string save_file = "test_level";

		int cellsize;
		Room* starting_room; 
		Room* current_room;

		sf::Sprite background_wrapper;
		std::vector<BackgroundLayer> backgrounds;

		std::vector<TilemapLayerProperties> tile_layer_properties;
		std::vector<Tiledata> tile_data; //the data of each tile type
		std::vector<Room*> rooms;

		std::vector<sf::Sound> music_layers;
		std::vector<sf::SoundBuffer> music_buffers;
		int music_track_count = 8;

		StageManager(int cellsize);
		~StageManager();

		void awake() override;
		void tile_layers_init();
		void tile_init();
		void music_init();

		void draw(sf::RenderTexture&) override;
		void save(std::string save_file);
		void load(std::string save_file, bool editor_mode=false);

		void save_json(); //testing 

		void remove_room(Room* room);

		void determine_surrounding_rooms();

		sf::Sprite get_autotile_sprite(int layer, int index, sf::Vector2i position, std::string tile_type, Tilemap* tilemap);
};