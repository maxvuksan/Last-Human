#include "ParticleSource.h"
#include "Particle.h"
#include "../Game.h"
#include "../Entity.h"
#include "../CALC.h"

Particle::Particle(ParticleSource* source, double speed, double angle) 
{
    this->source = source;
    this->inital_speed = speed;
    this->angle = angle;
    this->position = source->get_entity()->transform->get_draw_position();
    this->inital_position = position;
    this->lifetime_tracked = source->lifetime;

    shape = sf::CircleShape();
    shape.setFillColor(source->colour);

    outline_shape = sf::CircleShape();
    outline_shape.setFillColor(source->outline_colour);
}

void Particle::update(sf::RenderTexture& surface) {
    
    float lifetime_progress = lifetime_tracked/source->lifetime;

    current_radius = source->size;
    if (source->size_overtime) {
        current_radius*=lifetime_progress;
    }

    float current_speed = inital_speed;
    if (source->speed_overtime) {
        current_speed*=lifetime_progress;
    }

    sf::Vector2f offset(0, 0);
    if (source->follow_parent) {
        offset = source->get_entity()->transform->get_draw_position() - inital_position;
    }

    position += CALC::angle_to_vector(angle) * (float)(current_speed * source->get_entity()->game->delta_time);
    final_position = position + offset;

    angle += source->angle_overtime;
    lifetime_tracked -= source->get_entity()->game->delta_time;

    if (lifetime_tracked <= 0) {
        //destroy particle
        source->remove_particle(this);
        return;
    }
}

void Particle::draw(sf::RenderTexture& surface) {
    shape.setRadius(current_radius);
    shape.setPosition(final_position);
    shape.setOrigin(sf::Vector2f(current_radius / 2, current_radius / 2));
    surface.draw(shape);
}

void Particle::draw_outline(sf::RenderTexture& surface){
    
    float outline_radius = current_radius + source->outline_thickness;

    outline_shape.setRadius(outline_radius);
    outline_shape.setPosition(final_position);
    outline_shape.setOrigin(sf::Vector2f(outline_radius / 2, outline_radius / 2));
    surface.draw(outline_shape);
}

