#pragma once
#include "Game.h"
#include "Prefabs/Room.h"

struct KeyboardInputsEditor {
	short left = 0;
	short right = 0;
	short up = 0;
	short down = 0;
	bool left_mouse = false;
	bool right_mouse = false;
	bool middle_mouse = false;
	bool shift = false;
};

enum EditorState {
	Default,
	Drawing_Curves,
	Placing_Entity,
	Drawing_Room,
	Drawing_FallingBlock,
	Resizing_Room,
	Set_Spawnpoint,
	Set_Camera_Position,
};

enum RoomMode { 
	DrawMode,
	EditMode,
};

class Editor: public Game {

	//Room structure
	//	Room class
	//		- tilemap to store block data
	//		- entity list to store all entities in scene (allowing entites to be added to rooms)
	//		- 

	private:
		int selected_tile;
		int starting_room_index = 0;
		EditorState state = Default;
		RoomMode room_mode = EditMode;
		bool grabbing = false;

		int camera_move_speed = 15;
		KeyboardInputsEditor inputs;

		Entity* overlapping_entity;
		Entity* selected_entity;
		//std::string tile_filenames; //the file names for each tile
		Room* selected_room = nullptr;
		Room* overlapped_room = nullptr;
		int selected_room_working_layer = 0; //the current layer we are working on 
		std::string entity_to_place;

		sf::Vector2f rect_dimensions;

		sf::Vector2i mouse_pos;
		sf::Vector2i mouse_pos_worldposition;
		sf::Vector2f mouse_position_locked; //locked to the cellsize

		sf::Vector2f inital_mouse_position;
		sf::Vector2f inital_position;

		sf::Vector2i tilemap_pos;

		sf::RectangleShape cursor;
		sf::RectangleShape starting_outline; // showing where the starting room is
		sf::RectangleShape selected_entity_outline;
		sf::CircleShape grab_point_shape; //visual for resizing rooms

		int room_resize_grab_index = -1;
		int curve_point_index = 0; //the current point index for a bezier curve, start : end : anchor
		Wire* working_curve;

		sf::Texture editor_icon_texture;
		sf::Sprite camera_icon;
		sf::Sprite spawn_point_icon;

	public:

		Editor();

		void awake();
		void general_update();
		int get_cellsize();
		void general_manage_events(sf::Event);

		void select();
		void drawing_rect();
		void create_rect();

		void render_room_settings();
		void render_room_layer_settings();
		void render_tile_palette();
		void render_entity_palette(); 

		void set_spawnpoint();

		bool can_draw();

		void drawing_curve();

		void manage_room_resize_anchors();

};