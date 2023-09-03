#pragma once
#include <SFML/Graphics.hpp>
class Tilemap;

class Tile {

	Tilemap* tilemap;
	sf::RectangleShape test_shape;
	sf::Sprite sprite;
	int tile_index;

	public:
		Tile(Tilemap* tilemap, int tile_index);
		void draw(sf::RenderTexture& surface, sf::Vector2f position);
		void set_texture(sf::Texture* texture);
		int get_tile_index();

};