#include "Tile.h"
#include "Tilemap.h"
#include "../Game.h"
#include "../Managers/StageManager.h"

Tilemap::Tilemap(Entity* entity) : Component(entity)
{
	dimensions = sf::Vector2f(1, 1);
	buffers.push_back(new sf::RenderTexture);
	construct_buffer();
}

void Tilemap::draw_buffer(int layer, sf::RenderTexture& surface) {

	if (buffers.size() > layer) {
		sf::Sprite spri(buffers[layer]->getTexture());
		spri.setPosition(get_entity()->transform->get_draw_position());
		surface.draw(spri);
	}
}

int Tilemap::get_cellsize() {
	return cellsize;
}
void Tilemap::set_cellsize(int cellsize) {
	this->cellsize = cellsize;
}
int Tilemap::get_layer_count() {
	return layer_count;
}
void Tilemap::add_layer(int layer) {
	tiles.push_back(std::vector<std::vector<short>>(dimensions.x, std::vector<short>(dimensions.y, -1)));
	buffers.push_back(new sf::RenderTexture);
	layer_to_local.insert(std::make_pair(layer, layer_count - 1));

	layer_count++;
}

void Tilemap::regenerate(sf::Vector2f dimensions) {
	this->dimensions = dimensions;
	regenerate();
}
void Tilemap::regenerate() {

	tiles.clear();
			// number of layers,                           //number of rows                 //number of columns
	tiles.resize(layer_count, std::vector<std::vector<short>>(dimensions.x, std::vector<short>(dimensions.y, -1)));
	
}
void Tilemap::construct_buffer() {

	for (int i = 0; i < layer_count; i++) {

		buffers[i]->create(dimensions.x * cellsize, dimensions.y * cellsize);
		buffers[i]->clear(sf::Color(0, 0, 0, 0));
	}

	sf::Sprite sprite;
	for (int layer = 0; layer < tiles.size(); layer++) {
		for (int r = 0; r < dimensions.x; r++) {
			for (int c = 0; c < dimensions.y; c++)
			{
				if (tiles[layer][r][c] >= 0) { //the tile exists at this position

					sf::Vector2f position = sf::Vector2f(r * cellsize, c * cellsize);
					int index = tiles[layer][r][c];

					Tiledata& data = entity->game->get_stage_manager()->tile_data[index];
					sf::Texture texture;
					//is an auto tile, must break down texture
					std::string filename = data.filename.substr(0, data.filename.find("_"));
					if (filename == "autotile") {
						sprite = entity->game->get_stage_manager()->get_autotile_sprite(layer_by_local(layer), index, sf::Vector2i(r, c), filename, this);
					}
					else if (filename == "simpleautotile") {
						sprite = entity->game->get_stage_manager()->get_autotile_sprite(layer_by_local(layer), index, sf::Vector2i(r, c), filename, this);
					}
					else {
						sprite.setTextureRect(sf::IntRect(data.texture_offset.x * data.cellsize, data.texture_offset.y * data.cellsize, data.cellsize, data.cellsize));
						sprite.setTexture(*data.texture);
					}

					//if in the editor, allows for highlighting on the working layer
					if (focus_layer != -1) {
						if (layer == focus_layer) {
							sprite.setColor(sf::Color(255, 255, 255, 255));
						}
						else {
							sprite.setColor(sf::Color(255, 255, 255, 100));
						}
					}

					sprite.setPosition(position);
					buffers[layer]->draw(sprite);
				}
			}
		}
		buffers[layer]->display(); //we are finished drawing to the buffer
	}
}

int Tilemap::layer_by_local(int layer) {
	for (auto& i : layer_to_local) {
		if (i.second == layer) {
			return i.first;
		}
	}
}

void Tilemap::construct_colliders() {

	auto sm = entity->game->get_stage_manager();
	std::vector<TilemapLayerProperties>& layer_properties = sm->tile_layer_properties;

	struct RectShape {
		sf::Vector2f position;
		sf::Vector2f dimensions;
		CollisionMode mode;
		int layer = 0;
	};

	for (int layer = 0; layer < layer_count; layer++) {

		std::vector<RectShape> stored_rects;
		RectShape rect; //temporary rect container

		bool no_collisions = false;
		for (int y = 0; y < dimensions.y; y++) {
			bool working_rect = false;

			for (int x = 0; x < dimensions.x; x++)
			{
				if (tiles[layer][x][y] >= 0) { //the tile exists at this position
					
					if (!sm->tile_layer_properties[sm->tile_data[tiles[layer][x][y]].tile_layer].collisions) {
						no_collisions = true;
						break;
					}


					Tiledata& data = sm->tile_data[tiles[layer][x][y]];

					//not a default collider, will not obey collider simplication

					if (working_rect) {
						if (data.collision_shape.y != rect.dimensions.y || data.collision_mode != rect.mode || rect.layer != data.tile_layer) {
							stored_rects.push_back(rect);

							rect = { sf::Vector2f(x * cellsize + data.collision_offset.x ,y * cellsize + data.collision_offset.y), sf::Vector2f(data.collision_shape.x,data.collision_shape.y), data.collision_mode };
							rect.layer = data.tile_layer;
						}
						else {
							rect.dimensions.x += data.collision_shape.x;
						}
					}
					else {
						working_rect = true;
						rect = { sf::Vector2f(x * cellsize + data.collision_offset.x ,y * cellsize + data.collision_offset.y), sf::Vector2f(data.collision_shape.x,data.collision_shape.y), data.collision_mode };
						rect.layer = data.tile_layer;
					}

				}
				else {
					if (working_rect) {
						working_rect = false;
						stored_rects.push_back(rect);
					}
				}

			}
			if (no_collisions) {
				break;
			}
			if (working_rect) {
				stored_rects.push_back(rect);
			}
		
		}
		//merging row rects
		for (int y = 0; y < dimensions.y; y++) {
			for (int c = 0; c < stored_rects.size(); c++) {
				for (int i = 0; i < stored_rects.size(); i++) {

					RectShape& a = stored_rects[c];
					RectShape& b = stored_rects[i];
					if (a.position.x == b.position.x && a.mode == b.mode && a.layer == b.layer) { //same x position
						if (a.dimensions.x == b.dimensions.x) { //same width

							if (b.position.y == a.position.y + a.dimensions.y) {

								a.dimensions.y += b.dimensions.y;
								stored_rects.erase(stored_rects.begin() + i);
								break;
							}

						}
					}
				}
			}
		}

		//adding the colliders
		for (int c = 0; c < stored_rects.size(); c++) {
			BoxCollider* col = entity->add_component<BoxCollider>(true);
			col->collision_layer = sm->tile_layer_properties[stored_rects[c].layer].collision_layer;
			col->collision_mode = stored_rects[c].mode;

			col->is_centered(false);
			col->is_trigger = sm->tile_layer_properties[stored_rects[c].layer].is_trigger;
			col->offset = stored_rects[c].position;
			col->set_dimensions(stored_rects[c].dimensions);

			colliders.push_back(col);
		}
	}
}

void Tilemap::set_tile(int index, sf::Vector2i position, int global_layer) {
	if (layer_to_local.find(global_layer) == layer_to_local.end()) { //layer does not exist in local layers
		add_layer(global_layer);
	}

	if (position.x >= dimensions.x || position.y >= dimensions.y) {
		return; // the position is out of the tilemap bounds
	}
	if (position.x < 0 || position.y < 0) {
		return; // the position is out of the tilemap bounds
	}
	if (index != tiles[layer_to_local[global_layer]][position.x][position.y]) {
		tiles[layer_to_local[global_layer]][position.x][position.y] = index;
		construct_buffer();
	}
}
void Tilemap::set_all_tiles(int index, int layer) {
	if (layer_to_local.count(layer) == 0) { //layer does not exist in local layers
		add_layer(layer);
	}

	for (int x = 0; x < dimensions.x; x++) {
		for (int y = 0; y < dimensions.y; y++) {
			tiles[layer][x][y] = index;
		}
	}
	construct_buffer();
}
void Tilemap::remove_tile(sf::Vector2i position, int layer) {
	if (position.x >= dimensions.x || position.y >= dimensions.y) {
		return; // the position is out of the tilemap bounds
	}
	if (position.x < 0 || position.y < 0) {
		return; // the position is out of the tilemap bounds
	}
	if (tiles[layer_to_local[layer]][position.x][position.y] != -1) {
		tiles[layer_to_local[layer]][position.x][position.y] = -1;
		construct_buffer();
	}
}
short Tilemap::get_tile(sf::Vector2i position, int layer) {
	if (position.x >= dimensions.x || position.y >= dimensions.y) {
		return -1; // the position is out of the tilemap bounds
	}
	if (position.x < 0 || position.y < 0) {
		return -1; // the position is out of the tilemap bounds
	}
	return tiles[layer_to_local[layer]][position.x][position.y];
}
std::vector<std::vector<std::vector<short>>> Tilemap::copy_tiles() {
	return tiles;
}

void Tilemap::set_focus_layer(int layer) {
	if (layer_to_local.count(layer) == 0) {
		focus_layer = -1;
	}
	else {
		focus_layer = layer_to_local[layer];
	}
}
//void Tilemap::set_render_layer(int layer, int render_layer_value) {
//	layer_properties[layer].render_layer = render_layer_value;
//	entity->game->determine_required_render_layers();
//}
//int Tilemap::get_render_layer(int layer) {
//	return layer_properties[layer].render_layer;
//}

void Tilemap::delete_buffer_memory() {
	for (int i = 0; i < buffers.size(); i++) {
		delete buffers[i];
	}
}

Tilemap::~Tilemap() 
{
	delete_buffer_memory();
}
