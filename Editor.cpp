#include "Editor.h"
#include "Prefabs/Room.h"
#include "Managers/StageManager.h"

Editor::Editor(){}

void Editor::awake() {
	std::cout << "editor starting..." << std::endl;
	debug_mode = true;

	editor_icon_texture.loadFromFile("Images/Tiles/editor_icons.png");
	camera_icon.setTexture(editor_icon_texture);
	spawn_point_icon.setTexture(editor_icon_texture);

	spawn_point_icon.setOrigin(sf::Vector2f(4, 4));
	camera_icon.setOrigin(sf::Vector2f(4, 4));

	spawn_point_icon.setTextureRect(sf::IntRect(8, 0, 8, 8));
	camera_icon.setTextureRect(sf::IntRect(0, 0, 8, 8));
}

void Editor::render_room_settings() {

	if (selected_room != nullptr) {

		ImGui::Separator();
		ImGui::Text("Room");

		ImGui::Separator();
		ImGui::Text("Tilemap");

		//for (int i = 0; i < selected_room->get_component<Tilemap>()->get_layer_count(); i++) {
		//	std::string layer_name = "Layer " + std::to_string(i);
		//	if (ImGui::Button(layer_name.c_str())) { //selecting layer 'i' of the current room
		//		selected_room_working_layer = i;
		//	}
		//	if (selected_room_working_layer == i) {
		//		ImGui::SameLine();
		//		ImGui::Text("Selected");
		//	}
		//}

		//if (ImGui::Button("+")) {
		//	selected_room->get_component<Tilemap>()->add_layer();
		//}


		ImGui::Text("Music");

		ImGui::Checkbox("Layer 1", &selected_room->music_layers[0]);
		ImGui::Checkbox("Layer 2", &selected_room->music_layers[1]);
		ImGui::Checkbox("Layer 3", &selected_room->music_layers[2]);
		ImGui::Checkbox("Layer 4", &selected_room->music_layers[3]);

		ImGui::Separator();

		if (ImGui::BeginMenu("Set")) {
			if (ImGui::MenuItem("Spawnpoint")) {
				state = Set_Spawnpoint;
			}
			if (ImGui::MenuItem("Camera")) {
				state = Set_Camera_Position;
			}
			ImGui::EndMenu();
		}
	}
}

void Editor::render_tile_palette() {

	ImGui::Begin("Tile Palette");
	ImGui::SetWindowPos(ImVec2(120, 4));
	ImGui::SetWindowSize(ImVec2(180, 140));

	sf::Sprite tile_sprite;
	tile_sprite.setScale(3, 3);

	ImGui::BeginChild("Scrolling");

	int row_tracker = 0;
	for (int i = 0; i < stage_manager->tile_data.size(); i++) {

		tile_sprite.setTexture(*stage_manager->tile_data[i].texture);
		tile_sprite.setTextureRect(sf::IntRect(stage_manager->tile_data[i].texture_offset.x * cellsize, stage_manager->tile_data[i].texture_offset.y * cellsize, cellsize, cellsize));

		if (ImGui::ImageButton(tile_sprite, 0)) {
			selected_tile = i;
		}
		if (row_tracker < 4) {
			ImGui::SameLine();
		}
		else {
			row_tracker = 0;
		}
		row_tracker++;
	}

	ImGui::EndChild();
	ImGui::End();
}
void Editor::render_entity_palette() {
	ImGui::Begin("Entity Palette");
	ImGui::SetWindowPos(ImVec2(120, 4));
	ImGui::SetWindowSize(ImVec2(180, 140));

	ImGui::BeginChild("Scrolling");

	if (ImGui::Button("Add Wire")) {
		state = Drawing_Curves;
	}
	ImGui::SameLine();
	if (ImGui::Button("Remove All")) {
		selected_room->remove_all_wires();
	}
	ImGui::Separator();

	if(ImGui::Button("Falling Block"))
	{
		state = Drawing_FallingBlock;
		room_mode = EditMode;
	}
	if (ImGui::Button("Lamp"))
	{
		state = Placing_Entity;
		room_mode = EditMode;
		entity_to_place = "lamp";
	}
	if (ImGui::Button("Spring"))
	{
		state = Placing_Entity;
		room_mode = EditMode;
		entity_to_place = "spring";
	}
	if (ImGui::Button("Npc"))
	{
		state = Placing_Entity;
		room_mode = EditMode;
		entity_to_place = "npc";
	}

	ImGui::EndChild();
	ImGui::End();
}

void Editor::general_update() {

	if (state != Resizing_Room) {
		room_resize_grab_index = -1;
	}
	mouse_pos = sf::Mouse::getPosition(*window);
	mouse_pos.x /= window_width / display_width;
	mouse_pos.y /= window_height / display_height;
	mouse_pos_worldposition = mouse_pos + sf::Vector2i(camera->get_position().x, camera->get_position().y);
	mouse_position_locked = sf::Vector2f(round(mouse_pos_worldposition.x / cellsize) * cellsize, round(mouse_pos_worldposition.y / cellsize) * cellsize);
	
	if (selected_room != nullptr) {
		tilemap_pos = sf::Vector2i((mouse_position_locked.x - selected_room->transform->get_position().x) / cellsize, (mouse_position_locked.y - selected_room->transform->get_position().y) / cellsize);
	}

	cursor.setSize(sf::Vector2f(cellsize, cellsize));
	cursor.setFillColor(sf::Color(0, 0, 0, 0));
	cursor.setOutlineThickness(1);
	cursor.setPosition(mouse_position_locked - camera->get_position());

	switch (state) {
		case Placing_Entity:
		case Drawing_FallingBlock:
		case Drawing_Room:
			drawing_rect();
			break;

		case Drawing_Curves:
			drawing_curve();
			break;

		case Resizing_Room:
			manage_room_resize_anchors();
			break;

		case Default:
			ImGui::Begin("Editor");
			ImGui::SetWindowPos(ImVec2(4, 4));
			ImGui::SetWindowSize(ImVec2(0, 0));
			ImGui::Separator();
			if(ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("Save")) {
					stage_manager->save(stage_manager->save_file);
				}
				if (ImGui::MenuItem("Load")) {
					selected_room = nullptr;
					stage_manager->load(stage_manager->save_file, true);
					room_mode = EditMode;
				}
				ImGui::EndMenu();
			}
			ImGui::Separator();

			if (ImGui::Button("Draw")) {
				room_mode = DrawMode;
			}
			ImGui::SameLine();
			if (ImGui::Button("Edit")) {
				room_mode = EditMode;
			}
			if (room_mode == EditMode) {
				if (ImGui::Button("Add Room")) {
					state = Drawing_Room;
				}
			}
			render_room_settings();
			ImGui::End();
			//if (selected_room != nullptr) {
			//	render_room_layer_settings();
			//}

			this->overlapping_entity = nullptr;
			for (int i = 0; i < colliders.size(); i++) {

				if (colliders[i]->get_entity()->tag != "room") {

					if (colliders[i]->point_in_collider(mouse_pos_worldposition)) {
						overlapping_entity = colliders[i]->get_entity();
					}
				}
			}
			if (overlapping_entity == nullptr) { //we aren't hovering an entity so look for room
				this->overlapped_room = nullptr; //determining which room is currently being hovered over
				for (int i = 0; i < entites.size(); i++) {

					if (entites[i]->tag == "room") {

						Room* room = dynamic_cast<Room*>(entites[i]);
						if (room != NULL) {
							if (room->get_bounds_trigger()->point_in_collider(mouse_pos_worldposition)) {
								overlapped_room = room; //converting Entity* return type to Room*
								break;
							}
						}
					}
				}
			}

			if (room_mode == EditMode) { 
				if (selected_entity != nullptr) {

					if (inputs.left_mouse) {
						sf::Vector2f offset = mouse_position_locked - inital_mouse_position;
						selected_entity->transform->set_position(inital_position + offset);
					}
					selected_entity_outline.setOutlineColor(sf::Color(255, 0, 255));
					selected_entity_outline.setOutlineThickness(2);
					selected_entity_outline.setFillColor(sf::Color(0, 0, 0, 0));
					selected_entity_outline.setSize(selected_entity->get_component<BoxCollider>()->get_dimensions());
					selected_entity_outline.setPosition(selected_entity->transform->get_draw_position());

					if (selected_entity->get_component<BoxCollider>()->is_centered()) {
						selected_entity_outline.setOrigin(selected_entity->get_component<BoxCollider>()->get_dimensions() / (float)2 - selected_entity->get_component<BoxCollider>()->offset);
					}

					//OUTLINING SELECTED ENTITY
					//selected_entity_outline.setSize(selected_entity->get_component<Collider>())
					display->draw(selected_entity_outline);
				}
				else {
					if (selected_room != nullptr) {
						render_entity_palette();
						manage_room_resize_anchors();

						if (inputs.left_mouse && room_resize_grab_index == -1) {
							sf::Vector2f offset = mouse_position_locked - inital_mouse_position;
							selected_room->transform->set_position(inital_position + offset);
						}
					}
				}
			}
			else { //room_mode = DrawMode // DRAWING TILES
				if (selected_room != nullptr) {
					render_tile_palette();
					if (inputs.middle_mouse) {
						drawing_rect();

						if (inputs.left_mouse || inputs.right_mouse) {
							create_rect();
						}
					}
					else if (can_draw()) {
						if (inputs.left_mouse) {
							selected_room->get_component<Tilemap>()->set_tile(selected_tile, tilemap_pos, stage_manager->tile_data[selected_tile].tile_layer);
						}
						else if (inputs.right_mouse) {
							selected_room->get_component<Tilemap>()->remove_tile(tilemap_pos, stage_manager->tile_data[selected_tile].tile_layer);
						}
					}
				}
			}
			break;

		case Set_Camera_Position:
		case Set_Spawnpoint:
			if (can_draw()) {
				if (inputs.left_mouse) {
					if (state == Set_Spawnpoint) {
						selected_room->set_spawnpoint(sf::Vector2f(tilemap_pos.x + 0.5, tilemap_pos.y + 0.5));
					}
					if (state == Set_Camera_Position) {
						selected_room->set_camera_position(sf::Vector2f(tilemap_pos.x + 0.5, tilemap_pos.y + 0.5));
					}
					inputs.left_mouse = false;
					state = Default;
				}
			}
			break;
	}

	//determining which room should be highlighted, and drawing each rooms spawnpoint
	for (int i = 0; i < stage_manager->rooms.size(); i++) {
		if (stage_manager->rooms[i] == selected_room) {
			stage_manager->rooms[i]->set_select(true);
			stage_manager->rooms[i]->get_component<Tilemap>()->set_focus_layer(stage_manager->tile_data[selected_tile].tile_layer);
			stage_manager->rooms[i]->get_component<Tilemap>()->construct_buffer();

			sf::Vector2f spawn_point_wrldpos = sf::Vector2f(stage_manager->rooms[i]->get_spawnpoint().x * cellsize, stage_manager->rooms[i]->get_spawnpoint().y * cellsize);
			spawn_point_icon.setPosition(spawn_point_wrldpos + stage_manager->rooms[i]->transform->get_draw_position());
			display->draw(spawn_point_icon);

			sf::Vector2f camera_wrldpos = sf::Vector2f(stage_manager->rooms[i]->get_camera_position().x * cellsize, stage_manager->rooms[i]->get_camera_position().y * cellsize);
			camera_icon.setPosition(camera_wrldpos + stage_manager->rooms[i]->transform->get_draw_position());
			display->draw(camera_icon);
		}
		else {
			stage_manager->rooms[i]->set_select(false);
		}

		if (starting_room_index == i) {
			starting_outline.setSize(sf::Vector2f(stage_manager->rooms[i]->get_dimensions().x * cellsize, stage_manager->rooms[i]->get_dimensions().y * cellsize));
			starting_outline.setPosition(stage_manager->rooms[i]->transform->get_draw_position());
			starting_outline.setFillColor(sf::Color(0, 0, 0, 0));
			starting_outline.setOutlineThickness(1);
			starting_outline.setOutlineColor(sf::Color(255, 0, 255));

			display->draw(starting_outline);
		}
	}

	determine_required_render_layers();

	float speed = camera_move_speed;
	if (inputs.shift) {
		speed *= 3;
	}
	camera->translate(sf::Vector2f(inputs.left + inputs.right, inputs.up + inputs.down)* (float)(speed* delta_time) * camera->get_zoom());
}

void Editor::manage_room_resize_anchors(){
	sf::Vector2f top_left = sf::Vector2f(selected_room->get_bounds_trigger()->left(), selected_room->get_bounds_trigger()->top());
	sf::Vector2f bottom_left = sf::Vector2f(selected_room->get_bounds_trigger()->left(), selected_room->get_bounds_trigger()->bottom());
	sf::Vector2f top_right = sf::Vector2f(selected_room->get_bounds_trigger()->right(), selected_room->get_bounds_trigger()->top());
	sf::Vector2f bottom_right = sf::Vector2f(selected_room->get_bounds_trigger()->right(), selected_room->get_bounds_trigger()->bottom());

	std::vector<sf::Vector2f> grab_points = { top_left, bottom_left, top_right, bottom_right };

	grab_point_shape.setRadius(4);
	grab_point_shape.setOrigin(sf::Vector2f(2, 2));

	for (int i = 0; i < 4; i++) {
		grab_point_shape.setFillColor(sf::Color(255, 255, 0));

		grab_point_shape.setPosition(grab_points[i] - camera->get_position());
		float distance = sqrt(pow(mouse_pos_worldposition.x - grab_points[i].x, 2) + pow(mouse_pos_worldposition.y - grab_points[i].y, 2));

		if (state == Resizing_Room && i == room_resize_grab_index) {
			grab_point_shape.setPosition(mouse_position_locked - camera->get_position());
			distance = 0;
		}

		if (distance < 6) {
			grab_point_shape.setFillColor(sf::Color(255, 0, 255));
			room_resize_grab_index = i;
		}

		display->draw(grab_point_shape);
	}
}

void Editor::general_manage_events(sf::Event event) {

	if (event.type == sf::Event::MouseButtonPressed && !ImGui::GetIO().WantCaptureMouse) {
		switch (event.mouseButton.button)
		{
		case 0: //left mouse button
			inputs.left_mouse = true;
			if (room_mode == EditMode) {
				if (state == Default) {
					select();
					if (selected_room != nullptr) {
						if (room_resize_grab_index != -1) {
							state = Resizing_Room;
						}

						if (selected_entity != nullptr) { //focusing on entities
							inital_position = selected_entity->transform->get_position();
						}
						else { //focusing on room
							inital_position = selected_room->transform->get_position();
						}
						inital_mouse_position = mouse_position_locked;
					}
				}
				else if (state == Drawing_Room || state == Drawing_FallingBlock) {
					inital_mouse_position = mouse_position_locked;
				}
				else if (state == Drawing_Curves) {
					curve_point_index += 1;
					if (curve_point_index == 1) {
						Wire* e = new Wire;
						selected_room->add_entity(e);
						e->curve->define_curve(mouse_position_locked, mouse_position_locked);
						working_curve = e;
					}
					else if (curve_point_index == 3) {
						working_curve = nullptr;
						curve_point_index = 0;
					}
				}
				else if (state == Placing_Entity) {
					Entity* e = nullptr;
					if (entity_to_place == "lamp"){
						e = new Lamp;
 					}
					else if (entity_to_place == "spring") {
						e = new Spring;
					}
					else if (entity_to_place == "npc") {
						e = new Npc;
					}

					if (e != nullptr) {
						e->transform->set_position(sf::Vector2f(mouse_pos_worldposition.x , mouse_pos_worldposition.y));
						selected_room->add_entity(e);

						if (e->get_component<BoxCollider>() == nullptr) {
							e->add_component<BoxCollider>();
							e->get_component<BoxCollider>()->is_centered(false);
							e->get_component<BoxCollider>()->set_dimensions(sf::Vector2f(8, 8));
						}

						selected_entity = e;
						state = Default;
					}
				}
			}

			break;

		case 1:
			inputs.right_mouse = true;
			break;
		case 2:
			inputs.middle_mouse = true;
			if (selected_room != nullptr && room_mode == DrawMode) {
				inital_mouse_position = mouse_position_locked;
			}
			break;

		}
	}
	if (event.type == sf::Event::MouseButtonReleased) {
		switch (event.mouseButton.button)
		{

		case 0: //left mouse button
			inputs.left_mouse = false;

			if (state == Resizing_Room) {
				selected_room->resize_room(room_resize_grab_index, (mouse_position_locked - inital_mouse_position)/(float)cellsize);
				state = Default;
			}

			if (state == Drawing_Room || state == Drawing_FallingBlock) {
				if (!ImGui::GetIO().WantCaptureMouse) { //not hovering ui element
					create_rect();
				}
			}
			break;
		case 1:
			inputs.right_mouse = false;
			break;
		case 2:
			inputs.middle_mouse = false;
			break;
		}
	}

	if (event.type == sf::Event::KeyPressed)
	{
		switch (event.key.code)
		{
		case sf::Keyboard::LShift:
			inputs.shift = true;
			break;
		case sf::Keyboard::BackSpace:
			if (selected_entity != nullptr && selected_room != nullptr) {
				selected_room->remove_entity(selected_entity);
				selected_entity = nullptr;
			}
			else {
				stage_manager->remove_room(selected_room);
				stage_manager->determine_surrounding_rooms(); 
				selected_room = nullptr;
			}

		case sf::Keyboard::Escape:
			state = Default;

			if (working_curve != nullptr) {
				selected_room->remove_entity(working_curve);
			}
			curve_point_index = 0;
			working_curve = nullptr;

			break;
		case sf::Keyboard::V:
			if (selected_room != nullptr) {
				camera->set_position(selected_room->get_camera_position() * (float)cellsize + selected_room->transform->get_position());
				camera->set_zoom(1);
			}
			break;

		case sf::Keyboard::W:
			inputs.up = -1;
			break;
		case sf::Keyboard::A:
			inputs.left = -1;
			break;
		case sf::Keyboard::S:
			inputs.down = 1;
			break;
		case sf::Keyboard::D:
			inputs.right = 1;
			break;
		}
	}
	if (event.type == sf::Event::KeyReleased)
	{
		switch (event.key.code)
		{
		case sf::Keyboard::LShift:
			inputs.shift = false;
		case sf::Keyboard::W:
			inputs.up = 0;
			break;
		case sf::Keyboard::A:
			inputs.left = 0;
			break;
		case sf::Keyboard::S:
			inputs.down = 0;
			break;
		case sf::Keyboard::D:
			inputs.right = 0;
			break;
		}
	}

	if (event.type == sf::Event::MouseWheelMoved) {
		float amount = 0.045f;
		if (inputs.shift) {
			amount *= 3.0f;
		}
		camera->add_zoom(-event.mouseWheel.delta * amount);
	}
}

bool Editor::can_draw() {
	if (selected_room != nullptr) {
		if (selected_room->get_bounds_trigger()->point_in_collider(mouse_position_locked)) { //hovering over the selected room
			cursor.setOutlineColor(sf::Color(255, 255, 255));
			display->draw(cursor);
			return true;
		}
		else {
			cursor.setOutlineColor(sf::Color(255, 0, 0, 100));
			display->draw(cursor);
		}
	}
	return false;
}

void Editor::drawing_curve() {
	cursor.setOutlineColor(sf::Color(255, 255, 255));
	cursor.setSize(sf::Vector2f(1, 1));

	if (curve_point_index == 1) {
		working_curve->curve->set_end(mouse_position_locked);
	}
	if (curve_point_index == 2) {
		working_curve->curve->set_anchor(mouse_position_locked);
	}

	display->draw(cursor);
	
}

void Editor::drawing_rect() {
	rect_dimensions = sf::Vector2f(cellsize, cellsize);
	sf::Vector2f position = mouse_position_locked;

	if (state != Placing_Entity) {
		if (room_mode == EditMode) {
			if (inputs.left_mouse) {
				rect_dimensions = mouse_position_locked - inital_mouse_position;
				position = inital_mouse_position;
			}
		}
		else { //DrawMode
			if (inputs.middle_mouse) {
				rect_dimensions = mouse_position_locked - inital_mouse_position;
				position = inital_mouse_position;
			}
		}
	}

	sf::RectangleShape cursor(rect_dimensions);
	cursor.setFillColor(sf::Color(0, 0, 0, 0));
	cursor.setOutlineColor(sf::Color(255, 255, 255));
	cursor.setOutlineThickness(1);
	cursor.setPosition(position - camera->get_position());
	display->draw(cursor);
}
void Editor::create_rect() {
	rect_dimensions = sf::Vector2f(abs(rect_dimensions.x), abs(rect_dimensions.y));
	//is the room of a valid size? 
	if(rect_dimensions.x > 0 && rect_dimensions.y > 0){

		sf::Vector2f final_point = inital_mouse_position;
		if (inital_mouse_position.y > mouse_position_locked.y) {
			final_point.y = mouse_position_locked.y;
		}
		if (inital_mouse_position.x > mouse_position_locked.x) {
			final_point.x = mouse_position_locked.x;
		}

		if (state == Drawing_Room) {
			Room* new_room = new Room(cellsize, sf::Vector2f(rect_dimensions.x / cellsize, rect_dimensions.y / cellsize));
			add_entity(new_room);
			stage_manager->rooms.push_back(new_room);
			new_room->get_component<Tilemap>()->set_cellsize(cellsize);
			new_room->transform->set_position(final_point);
			selected_room = new_room;
		}
		else if (state == Drawing_FallingBlock) {
			FallingBlock* block = new FallingBlock(final_point, sf::Vector2f(rect_dimensions.x / cellsize, rect_dimensions.y / cellsize));
			selected_room->add_entity(block);
		}
		//rect fill tool for tiles
		else if (room_mode == DrawMode && inputs.middle_mouse) {

			sf::Vector2f offset = final_point - selected_room->transform->get_position();

			if (offset.x < 0) {
				rect_dimensions.x += offset.x;
				offset.x = 0;
			}
			if (offset.y < 0) {
				rect_dimensions.y += offset.y;
				offset.y = 0;
			}

			rect_dimensions /= (float)cellsize;
			offset /= (float)cellsize;
			
			if (inputs.left_mouse) { //place
				for (int x = 0; x < rect_dimensions.x; x++) {
					for (int y = 0; y < rect_dimensions.y; y++) {
						selected_room->get_component<Tilemap>()->set_tile(selected_tile, sf::Vector2i(x + offset.x, y + offset.y), stage_manager->tile_data[selected_tile].tile_layer);
					}
				}
				inputs.left_mouse = false;
			}

			if (inputs.right_mouse) { //remove

				for (int x = 0; x < rect_dimensions.x; x++) {
					for (int y = 0; y < rect_dimensions.y; y++) {
						selected_room->get_component<Tilemap>()->remove_tile(sf::Vector2i(x + offset.x, y + offset.y), stage_manager->tile_data[selected_tile].tile_layer);
					}
				}
				inputs.right_mouse = false;
			}
		}
	}

	state = Default;
}

void Editor::select() {
	room_mode = EditMode;
	selected_entity = nullptr;

	if (overlapping_entity) { //an entity is being hovered
		selected_entity = overlapping_entity;
	}
	else if (overlapped_room != nullptr) {
		selected_room_working_layer = 0;
		selected_room = overlapped_room;
	}
	else {
		if (room_resize_grab_index == -1 && selected_room != nullptr && selected_entity == nullptr) {
			selected_room_working_layer = 0;
			selected_room = nullptr;
		}
	}
	for (int i = 0; i < stage_manager->rooms.size(); i++) {
		if (stage_manager->rooms[i] != selected_room) {
			stage_manager->rooms[i]->get_component<Tilemap>()->set_focus_layer(-1);
			stage_manager->rooms[i]->get_component<Tilemap>()->construct_buffer();
		}
	}

} 

int Editor::get_cellsize() {
	return cellsize;
}
