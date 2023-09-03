#include "Lamp.h"

Lamp::Lamp() {
	tag = "lamp";
	texture.loadFromFile("Images/Tiles/lamp.png");
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(16, 0, 8, 8));
}

void Lamp::awake() {
	auto source = add_component<LightSource>(true);
	source->offset = sf::Vector2f(4, 4);
}

void Lamp::draw(sf::RenderTexture& surface) {
	sprite.setPosition(transform->get_draw_position());
	surface.draw(sprite);
}