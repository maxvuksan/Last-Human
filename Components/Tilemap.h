#pragma once
#include "../Entity.h"
#include "Tile.h"
#include <vector>
#include "BoxCollider.h"

//struct TilemapLayerProperties {
//	bool collisions = false;
//	bool is_trigger = false;
//	int collision_layer = 0; 
//	int render_layer = 0; 
//	std::vector<BoxCollider*> colliders;
//};

class Tilemap : public Component {

	private:
		std::vector<BoxCollider*> colliders; //layer, list of colliders for said layer
		std::vector<sf::RenderTexture*> buffers;
		sf::Vector2f dimensions;

		int focus_layer = -1; //is a specific layer focused (generally for editor view)

		int cellsize = 32; //how big is each indivudal tile?
		int layer_count = 1; //default number of layers is 1
		std::vector<std::vector<std::vector<short>>> tiles;
		


	public:
		std::map<int, int> layer_to_local; //maps stagemanager layer -> a local layer

		~Tilemap();
		Tilemap(Entity*);

		void regenerate();
		void regenerate(sf::Vector2f dimensions);
		void construct_buffer();
		void construct_colliders();

		void draw_buffer(int layer, sf::RenderTexture& surface);

		short get_tile(sf::Vector2i position, int layer = 0);
		std::vector<std::vector<std::vector<short>>> copy_tiles();
		void set_tile(int index, sf::Vector2i position, int layer = 0);
		void set_all_tiles(int index, int layer = 0);
		void remove_tile(sf::Vector2i position, int layer = 0);
		int layer_by_local(int layer);

		int get_cellsize();
		void set_cellsize(int);
		int get_layer_count();
		void add_layer(int layer); //referring to the stage manager layer

		void set_focus_layer(int);
		void delete_buffer_memory();
};