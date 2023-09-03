#include "StageManager.h"
#include "../COMPONENTS.h"
#include <sstream>
#include <string>
#include <fstream>
#include <json/json.h>


StageManager::StageManager(int cellsize)
{	
	this->cellsize = cellsize;
	tile_layers_init();
	tile_init();
}

void StageManager::music_init() {

	music_layers.resize(music_track_count);
	music_buffers.resize(music_track_count);

	//for ambience
	for (int i = 0; i < 6; i++) {
		std::string file_specifer = std::to_string(i) + ".wav";// +".wav";
		if (!music_buffers[i].loadFromFile("Sounds/Music/layer" + file_specifer)) {
			std::cout << "sound: Sounds/Music/layer" << file_specifer << " could not open" << std::endl;
			return;
		}

		music_layers[i].setVolume(25);
		music_layers[i].setBuffer(music_buffers[i]);
		music_layers[i].setLoop(true);
	}

	for (int i = 0; i < music_track_count; i++) {
		music_layers[i].play();
	}
}

void StageManager::tile_layers_init() {
	//bool collisions = false;
	//bool is_trigger = false;
	//int collision_layer = 0;
	//int render_layer = 0;
	tile_layer_properties.resize(TILEMAP_LAYER_SIZE);

	tile_layer_properties[Ground] = { true, false, 0, 1 };
	tile_layer_properties[Hazard] = { true, true, 2, 1 };
	tile_layer_properties[Background] = { false, false, 0, 0 };
	tile_layer_properties[Foreground] = { false, false, 0, 2 };
	tile_layer_properties[Oil] = { true, false, 3, 1 };
}

void StageManager::tile_init() {

	backgrounds.push_back({ "Images/Tiles/background_example.png", 0.1});
	backgrounds.push_back({ "Images/Tiles/tree_line.png", 0.3 });

	for(int i = 0; i < backgrounds.size(); i++)
	{
		backgrounds[i].texture = sf::Texture();
		backgrounds[i].texture.loadFromFile(backgrounds[i].filename);
	}

	tile_data.push_back({ "block", "tile.png", Ground, cellsize });
	tile_data.push_back({ "bricks", "autotile_bricks.png", Ground, cellsize });
	tile_data.push_back({ "rocks", "autotile_rocks.png", Ground, cellsize });

	tile_data.push_back({ "spike up", "tileset_spikes.png", Hazard, cellsize,sf::Vector2f(0,0) });
	tile_data.push_back({ "spike down", "tileset_spikes.png", Hazard, cellsize, sf::Vector2f(1,0) });
	tile_data.push_back({ "spike right", "tileset_spikes.png", Hazard, cellsize, sf::Vector2f(2,0) });
	tile_data.push_back({ "spike left", "tileset_spikes.png", Hazard, cellsize,sf::Vector2f(3,0) });

	tile_data.push_back({ "background rocks", "autotile_backgroundrocks.png", Background, cellsize });
	tile_data.push_back({ "filler", "filler.png", Foreground, cellsize });
	tile_data.push_back({ "pink block", "pink_block.png", Ground, cellsize });

	tile_data.push_back({ "oil right", "tileset_oil.png", Oil, cellsize, sf::Vector2f(0,0) });
	tile_data.push_back({ "oil left", "tileset_oil.png", Oil, cellsize,sf::Vector2f(1,0) });

	tile_data.push_back({ "platform left", "tileset_platform.png", Ground, cellsize, sf::Vector2f(0,0) });
	tile_data.push_back({ "platform middle", "tileset_platform.png", Ground, cellsize, sf::Vector2f(1,0) });
	tile_data.push_back({ "platform right", "tileset_platform.png", Ground, cellsize, sf::Vector2f(2,0) });

	tile_data.push_back({ "car", "car.png", Background, 80 });

	tile_data.push_back({ "chain", "lamp.png", Background, cellsize, sf::Vector2f(0,0) });
	tile_data.push_back({ "chain vine", "lamp.png", Background, cellsize,sf::Vector2f(1,0) });

	tile_data.push_back({ "background bricks", "simpleautotile_backgroundbricks.png", Background, cellsize });

	tile_data.push_back({ "vines", "vines.png", Foreground, 16 });
	tile_data.push_back({ "sign", "sign.png", Foreground, 40 });

	for (int i = 0; i < tile_data.size(); i++) {
		tile_data[i].texture = new sf::Texture;
		tile_data[i].collision_shape = sf::Vector2f(cellsize, cellsize);
		tile_data[i].collision_mode = AllSided;

		//loading texture 
		if (!tile_data[i].texture->loadFromFile("Images/Tiles/" + tile_data[i].filename)) {
			std::cout << "error loading tile " << i << std::endl;
		}


		//SPIKES
		if (tile_data[i].name == "spike up") {
			tile_data[i].collision_shape = sf::Vector2f(8, 3);
			tile_data[i].collision_offset = sf::Vector2f(0, 5);
		}
		else if (tile_data[i].name == "spike down") {
			tile_data[i].collision_shape = sf::Vector2f(8, 3);
		}
		else if (tile_data[i].name == "spike right") {
			tile_data[i].collision_shape = sf::Vector2f(3, 8);
		}
		else if (tile_data[i].name == "spike left") {
			tile_data[i].collision_shape = sf::Vector2f(3, 8);
			tile_data[i].collision_offset = sf::Vector2f(5, 0);
		}
		//OIL
		else if (tile_data[i].name == "oil right") {
			tile_data[i].collision_shape = sf::Vector2f(3, 8);
		}
		else if (tile_data[i].name == "oil left") {
			tile_data[i].collision_shape = sf::Vector2f(3, 8);
			tile_data[i].collision_offset = sf::Vector2f(5, 0);
		}
		//PLATFORM
		else if (tile_data[i].name == "platform left" || tile_data[i].name == "platform middle" || tile_data[i].name == "platform right") {
			tile_data[i].collision_shape = sf::Vector2f(8, 4);
			tile_data[i].collision_mode = TopSided;
		}
	}
}

void StageManager::awake() {
	set_render_layer(-99); //to draw backgrounds last
	save_json();
	background_wrapper.setPosition(game->get_display_size().x / 2, game->get_display_size().y / 2.0f); //setting position in relation to paralax
}

void StageManager::save_json() {

	//std::ifstream file("Saves/Levels/test.json");
	//Json::Value obj;
	//Json::StyledWriter w; //file writer

	//w.write("test");
}

void StageManager::save(std::string save_file) {
	std::ofstream file; //
	file.open("Saves/Levels/" + save_file + ".txt");

	for (int room = 0; room < this->rooms.size(); room++) {
		Room* r = rooms[room];

		//write room position -> 10,20| 
		//followed by '|', what i will use as a seperator between data values
		file << r->transform->get_position().x << "," << r->transform->get_position().y << "|";
		file << r->get_dimensions().x << "," << r->get_dimensions().y << "|";
		file << r->get_spawnpoint().x << "," << r->get_spawnpoint().y << "|";
		file << r->get_camera_position().x << "," << r->get_camera_position().y << "|";


		auto tilemap = rooms[room]->get_component<Tilemap>();

		for (int i = 0; i < r->get_entites().size(); i++) {
			
			Entity* e = r->get_entites()[i];

			if (e->tag == "wire") {
				Wire* wire = dynamic_cast<Wire*>(e);
				file << e->tag + "." <<
				wire->curve->get_start().x << "," << wire->curve->get_start().y << "." <<
				wire->curve->get_end().x << "," << wire->curve->get_end().y << "." <<
				wire->curve->get_anchor().x << "," << wire->curve->get_anchor().y << ".";
			}
			else if(e->tag == "falling_block") {
				FallingBlock* block = dynamic_cast<FallingBlock*>(e);
				file << e->tag + "." <<
				block->dimensions.x << "," << block->dimensions.y << "." <<
				e->transform->get_position().x << "," << e->transform->get_position().y << ".";
			}
			else if (e->tag == "lamp" || e->tag == "spring" || e->tag == "npc") {
				file << e->tag + "." <<
				e->transform->get_position().x << "," << e->transform->get_position().y << ".";
			}

			file << "~";
		}
		file << "|";

		for (int layer = 0; layer < tilemap->get_layer_count(); layer++) {
			//for some reason, defining the loop iterater variables (x,y) would not work inside of the for statement

			for (int x = 0; x < r->get_dimensions().x; x++) {
				for (int y = 0; y < r->get_dimensions().y; y++) {

					if (tilemap->get_tile(sf::Vector2i(x, y), layer) >= 0) {
						file << tilemap->get_tile(sf::Vector2i(x, y), layer);
					} 
					else {
						file << "."; //empty tile
					}
					file << ","; //tile seperator
				}
			}
			file << "~"; // '~' end of layer signifer
		}
		file << "*" << std::endl; // '**' end of room
	}

	file.close();
	std::cout << "saved!" << std::endl;
}
void StageManager::load(std::string save_file, bool editor_mode) {

	std::cout << "loading: Saves/Levels/" << save_file << std::endl;

	for (int i = 0; i < rooms.size(); i++) {
		game->remove_entity(rooms[i]);
	}
	rooms.clear();

	std::ifstream file;
	file.open("Saves/Levels/" + save_file + ".txt");
	if (file) { //file successfully opened

		std::stringstream buffer;
		buffer << file.rdbuf(); //read all contents to buffer
		std::string s = buffer.str(); //convert buffer to string

		while (s.length() > 1) {

			std::string curr_room = s.substr(0, s.find("*"));
			s.erase(0, s.find("*") + 1);

			sf::Vector2f room_position;
			sf::Vector2f room_dimensions;
			sf::Vector2f room_spawn_point;
			sf::Vector2f room_camera_position;
			for (int i = 0; i < 4; i++) {

				std::string data = curr_room.substr(0, curr_room.find("|"));
				curr_room.erase(0, curr_room.find("|") + 1);
				int _x = stoi(data.substr(0, data.find(","))); //extracting x value
				data.erase(0, data.find(",") + 1); // removing x value and ,
				int _y = stoi(data); //extracting remaining y value

				switch (i) {
					case 0:
						room_position = sf::Vector2f(_x, _y);
					case 1:
						room_dimensions = sf::Vector2f(_x, _y);
					case 2:
						room_spawn_point = sf::Vector2f(_x, _y);
					case 3:
						room_camera_position = sf::Vector2f(_x, _y);

				}
			}

			Room* new_room = new Room(cellsize, room_dimensions);
			rooms.push_back(new_room);
			this->game->add_entity(new_room);
			new_room->transform->set_position(room_position);
			new_room->set_spawnpoint(room_spawn_point);
			new_room->set_camera_position(room_camera_position);


			std::string layer_details = curr_room.substr(0, curr_room.find("/"));
			curr_room.erase(0, curr_room.find("/") + 1);


			//interpreting entites VIA the stored prefabs
			std::string e = curr_room.substr(0, curr_room.find("|"));
			curr_room.erase(0, curr_room.find("|") + 1);
			while (e.length() > 0) {
				std::string entity_string = e.substr(0, e.find("~"));
				e.erase(0, e.find("~") + 1);
				std::string tag = entity_string.substr(0, entity_string.find("."));
				entity_string.erase(0, entity_string.find(".") + 1);

				if (tag == "wire") {
					Wire* wire = new Wire;
					new_room->add_entity(wire);
					wire->transform->set_parent(new_room->transform);

					for (int i = 0; i < 3; i++) {
						std::string vect = entity_string.substr(0, entity_string.find("."));
						entity_string.erase(0, entity_string.find(".") + 1);

						int x = stoi(vect.substr(0, vect.find(",")));
						vect.erase(0, vect.find(",") + 1);
						int y = stoi(vect);

						switch (i) {
							case 0:
								wire->curve->set_start(sf::Vector2f(x, y));
							case 1:
								wire->curve->set_end(sf::Vector2f(x, y));
							case 2:
								wire->curve->set_anchor(sf::Vector2f(x, y));
						}
					}
				}
				else if (tag == "falling_block") {
					
					sf::Vector2f dimensions;
					sf::Vector2f position;
					for (int i = 0; i < 2; i++) {
						std::string vect = entity_string.substr(0, entity_string.find("."));
						entity_string.erase(0, entity_string.find(".") + 1);

						int x = stoi(vect.substr(0, vect.find(",")));
						vect.erase(0, vect.find(",") + 1);
						int y = stoi(vect);

						switch (i) {
							case 0:
								dimensions = sf::Vector2f(x, y);
							case 1:
								position = sf::Vector2f(x, y);
						}
					}
					auto block = new FallingBlock(position, dimensions);
					new_room->add_entity(block);
					//block->transform->set_parent(new_room->transform);
				}
				else if (tag == "lamp" || tag == "spring" || tag == "npc") {
					std::string vect = entity_string.substr(0, entity_string.find("."));
					entity_string.erase(0, entity_string.find(".") + 1);

					int x = stoi(vect.substr(0, vect.find(",")));
					vect.erase(0, vect.find(",") + 1);
					int y = stoi(vect);

					Entity* entity = nullptr;
					if (tag == "lamp") {
						entity = new Lamp;
					}
					else if (tag == "spring") {
						entity = new Spring;
					}
					else if (tag == "npc") {
						entity = new Npc;
					}

					if (entity != nullptr) {
						new_room->add_entity(entity);
						entity->transform->set_position(sf::Vector2f(x, y));

						if (editor_mode) {
							if (entity->get_component<BoxCollider>() == nullptr) {
								entity->add_component<BoxCollider>();
								entity->get_component<BoxCollider>()->is_centered(false);
								entity->get_component<BoxCollider>()->set_dimensions(sf::Vector2f(8, 8));
							}
						}
					}
				}

			}

			std::string grid_string = curr_room.substr(0, curr_room.find("*"));

			int layer = 0;
			while (grid_string.length() > 0) {

				std::string layer_string = grid_string.substr(0, grid_string.find("~"));
				grid_string.erase(0, grid_string.find("~") + 1);

				for (int x = 0; x < room_dimensions.x; x++) {
					for (int y = 0; y < room_dimensions.y; y++) {

						std::string til = layer_string.substr(0, layer_string.find(","));
						layer_string.erase(0, layer_string.find(",") + 1);

						if (til != ".") //tile is valid
						{ 
							int global_layer = tile_data[stoi(til)].tile_layer;
							new_room->get_component<Tilemap>()->set_tile(stoi(til), sf::Vector2i(x, y), global_layer);
						}
						//using ascii values we can calculate the correct index	
					}
				}

				if (!editor_mode) {
					new_room->get_component<Tilemap>()->construct_colliders();
				}
				layer++;
			}
		}


		for (int i = 0; i < rooms.size(); i++) {
			std::cout << "______________" << std::endl;
			for (int l = 0; l < rooms[i]->get_component<Tilemap>()->layer_to_local.size(); l++) {
				std::cout << "room " << i << " :" << rooms[i]->get_component<Tilemap>()->layer_by_local(l) << std::endl;
			}
		}


		starting_room = rooms[0];
		current_room = starting_room;
		//moving camera
		//game->get_camera()->set_position(starting_room->get_camera_position() * (float)cellsize + starting_room->transform->get_position());
		game->determine_required_render_layers();

		std::cout << "sucessfully loaded!" << std::endl;
	}
	else { //open file failed
		std::cerr << "File could not be opened!" << std::endl;
	}

	determine_surrounding_rooms();
	//redrawing tiles becuase we must consider adjacent rooms
	for (int i = 0; i < rooms.size(); i++) {
		rooms[i]->get_component<Tilemap>()->construct_buffer();
	}

	music_init();
}

void StageManager::remove_room(Room* room) {
	for (int i = 0; i < rooms.size(); i++) {
		if (rooms[i] == room) {
			rooms.erase(rooms.begin() + i);

		}
	}
	game->remove_entity(room);
}

void StageManager::draw(sf::RenderTexture& display)
{

	for (int i = 0; i < backgrounds.size(); i++) {


		background_wrapper.setTextureRect(sf::IntRect(0, 0, backgrounds[i].texture.getSize().x, backgrounds[i].texture.getSize().y));
		background_wrapper.setTexture(backgrounds[i].texture); //assigning correct texture
		background_wrapper.setOrigin(backgrounds[i].texture.getSize().x / 2, backgrounds[i].texture.getSize().y / 2);

		display.draw(background_wrapper);
		backgrounds[i].position.x += backgrounds[i].scroll_speed;
	}
}

void StageManager::determine_surrounding_rooms() {
	for (int a = 0; a < rooms.size(); a++) {
		rooms[a]->adjacent_rooms.clear();

		for (int b = 0; b < rooms.size(); b++) {

			if (rooms[a] != rooms[b]) {
				sf::Vector2f a_pos = rooms[a]->transform->get_position();
				sf::Vector2f b_pos = rooms[b]->transform->get_position();

				sf::Vector2f a_dim = rooms[a]->get_dimensions() * (float)cellsize;
				sf::Vector2f b_dim = rooms[b]->get_dimensions() * (float)cellsize;

				bool adjacent_x = false;
				bool adjacent_y = false;

				if (a_pos.x + a_dim.x == b_pos.x) { // right
					adjacent_x = true;
				}
				else if (a_pos.x - b_dim.x == b_pos.x) { // left
					adjacent_x = true;
				}

				if (a_pos.y + a_dim.y == b_pos.y){ // below
					adjacent_y = true;
				}
				else if (a_pos.y - b_dim.y == b_pos.y) { // above
					adjacent_y = true;
				}


				sf::Vector2f difference = a_pos - b_pos;
				if (adjacent_x) {
					if (a_pos.y < b_pos.y + b_dim.y && a_pos.y + a_dim.y > b_pos.y)
					{
						rooms[a]->adjacent_rooms.push_back({ difference, rooms[b] });
					}
				}
				else if (adjacent_y) {
					if (a_pos.x < b_pos.x + b_dim.x && a_pos.x + a_dim.x > b_pos.x)
					{
						rooms[a]->adjacent_rooms.push_back({ difference, rooms[b] });
					}
				}
			}
		}
	}
}

sf::Sprite StageManager::get_autotile_sprite(int layer, int index, sf::Vector2i position, std::string tile_type, Tilemap* ref) { //uses surrounding grid to calculate 

	sf::Sprite s;
	s.setTexture(*tile_data[index].texture);

	sf::Vector2i offsets[8] = { sf::Vector2i(-1,-1), sf::Vector2i(0,-1), sf::Vector2i(1,-1), sf::Vector2i(-1,0), sf::Vector2i(1,0), sf::Vector2i(-1,1), sf::Vector2i(0,1), sf::Vector2i(1,1) };

	bool STATES[8] = {false};

	auto room = dynamic_cast<Room*>(ref->get_entity());

	for (int i = 0; i < 8; i++) {
		if (ref->get_tile(position + offsets[i], layer) != -1) {
			STATES[i] = true;
			continue;
		}
		if (room != NULL) { //the tilemap IS  attached to a room, (not e.g. a falling block)
			for (int r = 0; r < room->adjacent_rooms.size(); r++) {
				auto adja_tilemap = room->adjacent_rooms[r].room->get_component<Tilemap>();
				sf::Vector2i adja_offset(round(room->adjacent_rooms[r].offset.x / cellsize), round(room->adjacent_rooms[r].offset.y / cellsize));

				//tile found
				if (adja_tilemap->get_tile(position + offsets[i] + adja_offset, layer) != -1) {
					STATES[i] = true;
					break;
				}
			}
		}
	}

	bool TL, T, TR, L, R, BL, B, BR;
	TL = STATES[0]; 
	T = STATES[1]; 
	TR = STATES[2]; 
	L = STATES[3]; 
	R = STATES[4]; 
	BL = STATES[5];
	B = STATES[6]; 
	BR = STATES[7];

	int x = 0;
	int y = 0;
	if (TL && T && TR && L && R && BL && B && BR) { //ALL
		x = 1;
		y = 1;
	}
	else if (!T && !L && R && B && BR) { //TL
		x = 0;
		y = 0;
	}
	else if (!T && L && R && B && BR && BL) { //T
		x = 1;
		y = 0;
	}
	else if (!T && L && !R && B && BL) { //TR
		x = 2;
		y = 0;
	}
	else if (!L && T && B && R && TR && BR){ //L
		x = 0;
		y = 1;
	}
	else if (!R && T && B && L && TL && BL) { //R
		x = 2;
		y = 1;
	}
	else if (!B && !L && R && T && TR) { //BL
		x = 0;
		y = 2;
	}
	else if (!B && L && R && T && TR && TL) { //B
		x = 1;
		y = 2;
	}
	else if (!B && L && !R && T && TL) { //BR
		x = 2;
		y = 2;
	} 
	else if (L && T && B && R && !BR && BL && TL && TR) { //bottom right corner
		x = 0;
		y = 3;
	}
	else if (L && T && B && R && !BL && BR && TL && TR) { //bottom left corner
		x = 1;
		y = 3;
	}
	else if (L && T && B && R && !TR && TL && BR && BL) { //top right corner
		x = 0;
		y = 4;
	}
	else if (L && T && B && R && !TL && TR && BR && BL) { //top left corner
		x = 1;
		y = 4;
	}
	//if not simpleautotile
	else if (tile_type == "autotile") {
		if (L && T && B && R && !TL && !BL && BR && TR) { 
			x = 2;
			y = 3;
		}
		else if (L && T && B && R && !TR && !BR && TL && BL) {
			x = 2;
			y = 4;
		}
		else if (L && T && B && R && !TL && !TR && BR && BL) {
			x = 0;
			y = 5;
		}
		else if (L && T && B && R && !BR && !BL && TL && TR) {
			x = 1;
			y = 5;
		}
		else if (L && T && B && R && !TR && !BL && TL && BR) {
			x = 2;
			y = 5;
		}
		else if (L && T && B && R && !TL && !BR && TR && BL) {
			x = 0;
			y = 6;
		} 
		else if (L && T && B && R && !TL && !BL && !BR && TR) {
			x = 1;
			y = 6;
		}
		else if (L && T && B && R && !TR && !BL && !BR && TL) {
			x = 2;
			y = 6;
		} 
		else if (L && T && B && R && !TR && BL && !BR && !TL) {
			x = 0;
			y = 7;
		}
		else if (L && T && B && R && !TR && !BL && BR && !TL) {
			x = 1;
			y = 7;
		}
		else if (L && T && B && R && !TR && !BL && !BR && !TL) {
			x = 2;
			y = 7;
		}
		else if (!T && !L && R && B && !BR) {
			x = 0;
			y = 8;
		}
		else if (!T && L && !R && B && !BL) {
			x = 1;
			y = 8;
		}
		else if (!L && T && B && R && TR && !BR) {
			x = 2;
			y = 8;
		}
		else if (T && !L && R && !B && !TR) {
			x = 0;
			y = 9;
		}
		else if (T && L && !R && !B && !TL) {
			x = 1;
			y = 9;
		}
		else if (!L && T && B && R && BR && !TR) {
			x = 2;
			y = 9;
		}
		else if (!R && T && B && L && BL && !TL) {
			x = 0;
			y = 10;
		}
		else if (!R && T && B && L && !BL && TL) {
			x = 1;
			y = 10;
		}
		else if (!L && T && B && R && !TR && !BR) {
			x = 2;
			y = 10;
		}      
		else if (!L && T && B && R && !TL && !BL) {
			x = 0;
			y = 11;
		}
		else if (!T && B && L && R && BL && !BR){
			x = 1;
			y = 11;
		}
		else if (!T && B && L && R && BR && !BL){
			x = 2;
			y = 11;
		}
		else if (!T && L && R && B && !BR && !BL) {
			x = 0;
			y = 12;
		}
		else if (T && L && R && !B && !TL && TR) {
			x = 1;
			y = 12;
		}
		else if (T && L && R && !B && !TR && TL) {
			x = 2;
			y = 12;
		}
		else if (T && L && R && !B && !TR && !TL) {
			x = 0;
			y = 13;
		}
		else if (T && B && !L && !R){
			x = 1;
			y = 13;
		}
		else if (R && L && !T && !B) {
			x = 2;
			y = 13;
		}
		else if (!T && !R && !L && B) {
			x = 0;
			y = 14;
		}
		else if (T && !R && !L && !B) {
			x = 1;
			y = 14;
		}
		else if (!T && !R && L && !B) {
			x = 2;
			y = 14;
		}
		else if (!L && !T && !B && R) {
			x = 0;
			y = 15;
		}
		else {
			x = 1;
			y = 15;
		}
	}

	s.setTextureRect(sf::IntRect((x + tile_data[index].texture_offset.x)* cellsize, (y+tile_data[index].texture_offset.y)* cellsize, cellsize, cellsize));
	return s;
}

StageManager::~StageManager() {
	for (int i = 0; i < tile_data.size(); i++) {
		delete tile_data[i].texture;
	}
}