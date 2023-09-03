#include "BezierCurve.h"
#include "../CALC.h"
#include "../Entity.h"

BezierCurve::BezierCurve(Entity* entity): Component(entity)
{
	this->resolution = 0.03;
	this->colour = sf::Color(255, 255, 255);
	anchor_offset = sf::Vector2f(0, 0);
}


void BezierCurve::define_curve(sf::Vector2f start, sf::Vector2f end) 
{
	define_curve(start, end, ((start + end) / (float)2) + sf::Vector2f(0,20), resolution, colour);
}
void BezierCurve::define_curve(sf::Vector2f start, sf::Vector2f end, sf::Vector2f anchor)
{
	define_curve(start, end, anchor, resolution, colour);
}
void BezierCurve::define_curve(sf::Vector2f start, sf::Vector2f end, sf::Vector2f anchor, float resolution, sf::Color colour)
{
	this->start = start;
	this->end = end;
	this->anchor = anchor;
	this->resolution = resolution;
	this->colour = colour;
}

sf::Vector2f BezierCurve::calculate_point(float t) 
{
	sf::Vector2f a = CALC::lerp(start, anchor + anchor_offset, t);
	sf::Vector2f b = CALC::lerp(anchor + anchor_offset, end, t);

	sf::Vector2f final = CALC::lerp(a, b, t);

	return final;
}

void BezierCurve::draw(sf::RenderTexture& surface) 
{
	sf::VertexArray curve(sf::LinesStrip, 1);

	int i = 0;
	for (float t = 0; t < 1; t += resolution) {
		sf::Vertex v(calculate_point(t) + entity->transform->get_draw_position());
		v.color = this->colour;

		if (i == 0) {
			curve[0] = v;
		}
		else {
			curve.append(v);
		}
		i++;
	}
	sf::Vertex v(calculate_point(1) + entity->transform->get_draw_position());
	v.color = this->colour;
	curve.append(v);

	surface.draw(curve);

}

void BezierCurve::set_start(sf::Vector2f pos) {
	start = pos;
}
void BezierCurve::set_end(sf::Vector2f pos) {
	end = pos;
}
void BezierCurve::set_anchor(sf::Vector2f pos) {
	anchor = pos;
}
void BezierCurve::set_anchor_offset(sf::Vector2f off) {
	anchor_offset = off;
}
void BezierCurve::set_resolution(float res) {
	resolution = res;
}
void BezierCurve::set_colour(sf::Color col) {
	colour = col;
}

sf::Vector2f BezierCurve::get_start() {
	return start;
}
sf::Vector2f BezierCurve::get_end() {
	return end;
}
sf::Vector2f BezierCurve::get_anchor() {
	return anchor;
}