#include "ParticleSource.h"
#include "Particle.h"
#include "../Game.h"
#include <stdlib.h> 
#include <math.h>

ParticleSource::ParticleSource(Entity* entity) :Component(entity) {}

void ParticleSource::draw(sf::RenderTexture& surface) {
    
    if (!burst || (burst && !burst_tracked)) {
        if (delay_tracked >= delay) {

            for (int c = 0; c < count; c++) {
                double spawn_angle = angle_min + rand() % ((int)angle_max - (int)angle_min) + 180;
                //because -1 is upwards, flip angle by adding 180

                Particle* particle_ptr = new Particle(this, speed * 2, spawn_angle);
                add_particle(particle_ptr);
            }
            delay_tracked = 0;
            burst_tracked = true;
        }
    }

    delay_tracked += get_entity()->game->delta_time * 100;

    for (int i = 0; i < particles.size(); i++) {
        particles[i]->update(surface);
    }
    if (outline_thickness > 0) {
        for (int i = 0; i < particles.size(); i++) {
            particles[i]->draw_outline(surface);
        }
    }
    for (int i = 0; i < particles.size(); i++) {
        particles[i]->draw(surface);
    }
}

void ParticleSource::add_particle(Particle* particle) {
    
    particles.push_back(particle);
}

void ParticleSource::remove_particle(Particle* particle) {

    for (int i = 0; i < particles.size(); i++) {

        if (particle == particles[i]) {
            delete particles[i];
            particles.erase(particles.begin() + i);
            break;
        }
    }
}

ParticleSource::~ParticleSource() {
    for (int i = 0; i < particles.size(); i++) {
        delete particles[i];
    }
}