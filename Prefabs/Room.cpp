#include "Room.h"
#include "../Editor.h"
#include "../Managers/StageManager.h"

Room::Room(int cellsize) :Room(cellsize, sf::Vector2f(0, 0)){}

Room::Room(int cellsize, sf::Vector2f dimensions) {
	this->cellsize = cellsize;
	this->dimensions = dimensions;
	this->tag = "room";

	spawn_point = sf::Vector2f(round(dimensions.x / 2), round(dimensions.y / 2) + 1);
	camera_position = sf::Vector2f(round(dimensions.x / 2), round(dimensions.y / 2));
}

void Room::remove_all_wires() {
	std::vector<Entity*> wires;
	for (int i = 0; i < entites.size(); i++) {
		if (entites[i]->tag == "wire") {
			wires.push_back(entites[i]);
		}
	}

	for (int i = 0; i < wires.size(); i++) {
		remove_entity(wires[i]);
	}
}

void Room::awake() {

	add_component<Tilemap>();
	tilemap = get_component<Tilemap>();
	tilemap->set_cellsize(cellsize);
	tilemap->regenerate(dimensions);

	sf::Vector2f scaled_dim = sf::Vector2f(dimensions.x * cellsize, dimensions.y * cellsize);
	rect = sf::RectangleShape(scaled_dim); 
	set_select(false);

	bounds_trigger = add_component<BoxCollider>(true);
	bounds_trigger->is_centered(false);
	bounds_trigger->is_trigger = true;
	bounds_trigger->set_dimensions(scaled_dim);

	sf::Vector2f camera_buffer_vector = sf::Vector2f(camera_buffer, camera_buffer);
	camera_trigger = add_component<BoxCollider>(true);
	camera_trigger->is_centered(false);
	camera_trigger->is_trigger = true;
	camera_trigger->set_dimensions(scaled_dim - camera_buffer_vector);
	camera_trigger->offset = camera_buffer_vector / (float)2;
}

void Room::set_spawnpoint(sf::Vector2f spawnpoint) {
	this->spawn_point = spawnpoint;
}

void Room::set_camera_position(sf::Vector2f camera_position) {
	this->camera_position = camera_position;
}

BoxCollider* Room::get_bounds_trigger() {
	return bounds_trigger;
}

void Room::set_select(bool select) {
	this->selected = select;
	if (selected) {
		rect.setFillColor(sf::Color(255, 255, 255, 35));
	}
	else {
		rect.setFillColor(sf::Color(255, 255, 255, 15));
	}
}

void Room::update() {
	if (camera_trigger->triggered) {

		bool found = false;
		std::vector<Collider*> cols = camera_trigger->check_for_collisions();
		for (int i = 0; i < cols.size(); i++) {
			if (cols[i]->get_entity()->tag == "player") {
				if (!in_room) {
					game->get_stage_manager()->current_room = this;
					game->get_scene_manager()->reset_room(this);
					in_room = true;
					game->get_camera()->travel_to_point(transform->get_position() + sf::Vector2f(camera_position.x * cellsize, camera_position.y * cellsize), 3);
				}
				found = true;
			}
		}
		if (!found) {
			in_room = false;
		}
	}
	else {
		in_room = false;
	}
}

void Room::draw(sf::RenderTexture& surface) {
	if (game->debug_mode) {
		rect.setPosition(transform->get_draw_position());
		surface.draw(rect);
	}
}

void Room::add_entity(Entity* entity) {
	entites.push_back(entity);
	game->add_entity(entity);
}
void Room::remove_entity(Entity* entity) {
	for (int i = 0; i < entites.size(); i++) {
		if (entity == entites[i]) {
			entites.erase(entites.begin() + i);
		}
	}
	game->remove_entity(entity);
}

sf::Vector2f Room::get_dimensions() {
	return dimensions;
}
sf::Vector2f Room::get_spawnpoint() {
	return spawn_point;
}
sf::Vector2f Room::get_spawnpoint_world_position() {
	return sf::Vector2f(spawn_point.x * cellsize, spawn_point.y * cellsize) + transform->get_position();
}
sf::Vector2f Room::get_camera_position() {
	return camera_position;
}
std::vector<Entity*>& Room::get_entites() {
	return entites;
}

void Room::resize_room(int anchor_index, sf::Vector2f difference) {
	//0,1,2,3
	//topleft, bottomleft, topright, bottomright

	std::vector<std::vector<std::vector<short>>> tiles_copy = tilemap->copy_tiles();

	sf::Vector2f translation = sf::Vector2f(0, 0);
	switch(anchor_index){

		case 0:
			dimensions = dimensions + sf::Vector2f(-difference.x, -difference.y);
			translation = difference;
			break;
		case 1:
			dimensions = dimensions + sf::Vector2f(-difference.x, difference.y);
			translation = sf::Vector2f(difference.x, 0);
			break;
		case 2:
			dimensions = dimensions + sf::Vector2f(difference.x, -difference.y);
			translation = sf::Vector2f(0, difference.y);
			break;
		case 3:
			dimensions = dimensions + sf::Vector2f(difference.x, difference.y);
			break;
	}
	transform->translate(translation * (float)cellsize);

	for (int i = 0; i < entites.size(); i++) {
		entites[i]->transform->translate(-translation * (float)cellsize);
	}

	rect.setSize(dimensions * float(cellsize));
	bounds_trigger->set_dimensions(dimensions * (float)cellsize);
	camera_trigger->set_dimensions(dimensions * (float)cellsize - sf::Vector2f(camera_buffer, camera_buffer));

	spawn_point -= translation;
	camera_position -= translation;

	tilemap->regenerate(dimensions);

	for (int layer = 0; layer < tilemap->get_layer_count() - 1; layer++) {
		for (int x = 0; x < tiles_copy[layer].size(); x++) {
			for (int y = 0; y < tiles_copy[layer][x].size(); y++)
			{
				tilemap->set_tile(tiles_copy[layer][x][y], sf::Vector2i(x - translation.x, y - translation.y), tilemap->layer_by_local(layer));
			}
		}
	}

	tilemap->construct_buffer();
}

Room::~Room() {
	for (int i = 0; i < entites.size(); i++) {
		std::cout << entites[i]->tag << std::endl;
		game->remove_entity(entites[i]);
	}
}
