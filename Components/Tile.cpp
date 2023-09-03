#include "Tilemap.h"
#include "Tile.h"

Tile::Tile(Tilemap* tilemap, int tile_index) {
	this->tilemap = tilemap;
	this->tile_index = tile_index;
}


void Tile::draw(sf::RenderTexture& surface, sf::Vector2f position) {

	sprite.setPosition(position);
	surface.draw(sprite);
	surface.draw(test_shape);
}
void Tile::set_texture(sf::Texture* texture) {
	sprite.setTexture(*texture);
}
int Tile::get_tile_index() {
	return tile_index;
}

