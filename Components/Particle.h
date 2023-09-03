#pragma once
#include <SFML/Graphics.hpp>
class ParticleSource;

class Particle {

protected:
	double inital_speed;
	double angle;
	double lifetime_tracked;

	float current_radius;

	ParticleSource* source;
	sf::Vector2f position;
	sf::Vector2f final_position;
	sf::Vector2f inital_position;
	sf::CircleShape outline_shape;
	sf::CircleShape shape;

public:
	Particle(ParticleSource* source, double speed, double angle);
	void update(sf::RenderTexture& surface);
	sf::Vector2f movement_direction(float current_speed);
	void draw_outline(sf::RenderTexture& window);
	void draw(sf::RenderTexture& window);
};