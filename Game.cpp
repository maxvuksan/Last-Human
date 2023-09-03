#include "Game.h"
#include "Prefabs/Player.h"
#include "Managers/StageManager.h"

Game::Game() {
	window = new sf::RenderWindow(sf::VideoMode(window_width, window_height), "Platformer");
	window->setKeyRepeatEnabled(false);

	required_render_layers.push_back(0);

	display = new sf::RenderTexture();
	display->create(display_width, display_height);

	window->setFramerateLimit(140);
	ImGui::SFML::Init(*window);

	sounds.resize(max_sound_size);
	sound_buffers.resize(max_sound_size);
	current_sound_index = 0;

	camera = new Camera();
	add_entity(camera);
	stage_manager = new StageManager(cellsize);
	add_entity(stage_manager);
	weather_manager = new WeatherManager;
	add_entity(weather_manager);
	scene_manager = new SceneManager;
	add_entity(scene_manager);

	shaders.insert(std::make_pair("ColourGrading", new ColourGrading("ColourGrading")));
	shaders.insert(std::make_pair("ScreenTransition", new ScreenTransition("ScreenTransition")));
	shaders.insert(std::make_pair("LightingLayer", new LightingLayer("LightingLayer")));

	for (auto const& shader : shaders)
	{
		shader.second->define_game_parent(this);
	}

	font.loadFromFile("Images/quinque.ttf");
}
void Game::awake() {
	stage_manager->load(stage_manager->save_file);
	player = new Player();
	add_entity(player);
}

void Game::general_update() {
}
void Game::general_manage_events(sf::Event event) {
}

void Game::shader_update() {
	//for (auto const& shader : shaders)
	//{
	//	shader.second->update();
	//}
	shaders["ColourGrading"]->update();
}

void Game::run() {
	awake();
	while (window->isOpen()) {

		window->clear();

		delta_time = clock.getElapsedTime().asSeconds();//clock.getElapsedTime().asSeconds() * 10;
		time += delta_time;
		delta_time *= 6;

		ImGui::SFML::Update(*window, clock.restart());

		zoom_display_size = sf::Vector2f(display_width * camera->get_zoom(), display_height * camera->get_zoom());
		if (zoom_display_size.x > 1 && zoom_display_size.y > 1) {
			display->create(zoom_display_size.x + zoom_edge_buffer, zoom_display_size.y + zoom_edge_buffer);
		}
		display->clear(background_colour);

		shader_update();
		update();
		general_update();
		manage_events();

		display->display();

		sf::Sprite display_sprite(display->getTexture());
		display_sprite.setScale(sf::Vector2f(window_width / (float)zoom_display_size.x, window_height / (float)zoom_display_size.y));
		display_sprite.setPosition(sf::Vector2f(-zoom_edge_buffer / 2.0f, -zoom_edge_buffer / 2.0f));

		// you can only add one shader per ->draw call
		sf::RenderTexture shader_tex;
		shader_tex.create(window_width, window_height);
		shader_tex.draw(display_sprite, shaders["ColourGrading"]);
		shader_tex.display();

		sf::Sprite light_layer;
		auto lighting_layer = dynamic_cast<LightingLayer*>(shaders["LightingLayer"]);
		for (int i = 0; i < light_sources.size(); i++) {
			if (light_sources[i]->get_active()) {
				lighting_layer->set_focused_light(light_sources[i]);
				light_layer.setTexture(shader_tex.getTexture());
				shader_tex.draw(light_layer, lighting_layer);
			}
		}
		shader_tex.display();

		window->draw(sf::Sprite(shader_tex.getTexture()), shaders["ScreenTransition"]);//, &colour_grade);
		ImGui::SFML::Render(*window);
		window->display();
	}
	ImGui::SFML::Shutdown();
}

void Game::determine_required_render_layers() {

	required_render_layers.clear();
	required_render_layers.push_back(0);

	for (int i = 0; i < TILEMAP_LAYER_SIZE; i++) {
		required_render_layers.push_back(stage_manager->tile_layer_properties[i].render_layer);
	}

	for (int i = 0; i < entites.size(); i++) {
		for (int c = 0; c < entites[i]->components.size(); c++) {

			std::vector<int> next_layers;

			Animator* anim = dynamic_cast<Animator*>(entites[i]->components[c]);
			if (anim != NULL) {
				next_layers.push_back(anim->get_render_layer());
			}

			ParticleSource* part = dynamic_cast<ParticleSource*>(entites[i]->components[c]);
			if (part != NULL) {
				next_layers.push_back(part->get_render_layer());
			}

			Collider* col = dynamic_cast<Collider*>(entites[i]->components[c]);
			if (col != NULL) {
				next_layers.push_back(col->get_render_layer());
			}

			next_layers.push_back(entites[i]->get_render_layer());


			for (int next_layer = 0; next_layer < next_layers.size(); next_layer++) {
				bool found_match = false;
				for (int exisiting = 0; exisiting < required_render_layers.size(); exisiting++) {
					if (next_layers[next_layer] == required_render_layers[exisiting]) {
						found_match = true;
						break;
					}
				}

				if (!found_match) {
					required_render_layers.push_back(next_layers[next_layer]);
				}
			}

		}
	}

	std::sort(required_render_layers.begin(), required_render_layers.end());
}

void Game::clear_remove_queue() {
	for (int i = 0; i < remove_queue.size(); i++) {
		delete remove_queue[i];
	}
	remove_queue.clear();
}

void Game::update() {
	//gemeral updates
	for (int i = 0; i < entites.size(); i++) {

		for (int c = 0; c < entites[i]->components.size(); c++) {
			entites[i]->components[c]->update(); //no rendering
		}
	}
	for (int i = 0; i < entites.size(); i++) {
		
		entites[i]->update();
		entites[i]->track_remove_time();
	}

	//drawing VIA rendering layers (ordered rendering)
	for (int layer_index = 0; layer_index < required_render_layers.size(); layer_index++) 
	{
		int render_layer = required_render_layers[layer_index];
		for (int i = 0; i < entites.size(); i++) {

			for (int c = 0; c < entites[i]->components.size(); c++) {

				Tilemap* tmap = dynamic_cast<Tilemap*>(entites[i]->components[c]);
				if (tmap != NULL) {
					for (int layer = 0; layer < tmap->layer_to_local.size(); layer++) {
						if (stage_manager->tile_layer_properties[tmap->layer_by_local(layer)].render_layer == render_layer) {
							tmap->draw_buffer(layer, *display);
						}
					}
				}
				else {
					if (entites[i]->components[c]->get_render_layer() == render_layer) {
						entites[i]->components[c]->draw(*display);
					}
				}
			}

			if (entites[i]->get_render_layer() == render_layer) {
				entites[i]->draw(*display);
			}
		}
	}
	clear_remove_queue();
}

void Game::manage_events() {
	sf::Event event;
	while (window->pollEvent(event)) {

		ImGui::SFML::ProcessEvent(event);
		if (event.type == sf::Event::Closed) {
			window->close();
		}
		if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::Q) {
				debug_mode = !debug_mode;
			}
		}

		for (int i = 0; i < entites.size(); i++) {
			entites[i]->manage_events(event);
		}

		general_manage_events(event);
	}
}

void Game::add_entity(Entity* entity) {
	entites.push_back(entity);
	entity->define_game_parent(this);
	entity->awake();
}
void Game::remove_entity(Entity* entity) {
	for (int i = 0; i < entites.size(); i++) {
		if (entity == entites[i]) {
			entites.erase(entites.begin() + i);
			entity->on_remove();
			remove_queue.push_back(entity);
			break;
		}
	}
}

void Game::add_light_source(LightSource* source) {
	light_sources.push_back(source);
}
void Game::remove_light_source(LightSource* source) {
	for (int i = 0; i < light_sources.size(); i++) {
		if (light_sources[i] == source) {
			light_sources.erase(light_sources.begin() + i);
		}
	}
}

void Game::add_collider(Collider* collider) {
	colliders.push_back(collider);
}
void Game::remove_collider(Collider* collider) {
	for (int i = 0; i < colliders.size(); i++) {
		if (colliders[i] == collider) {
			colliders.erase(colliders.begin() + i);
		}
	}
}

Entity* Game::find_entity_by_tag(std::string tag) {
	for (int i = 0; i < entites.size(); i++) {
		if (entites[i]->tag == tag) {
			return entites[i];
		}
	}
	return nullptr;
}
std::vector<Entity*>& Game::get_entities() {
	return entites;
}
std::vector<Collider*>& Game::get_colliders() {
	return colliders;
}
bool Game::collision_matrix_check(int layer1, int layer2) {

	for (int i = 0; i < 4; i++) {
		if (collision_matrix[layer1][i] == layer2) {
			return true;
		}
	}
	return false;
}

SceneManager* Game::get_scene_manager() {
	return scene_manager;
}
StageManager* Game::get_stage_manager() {
	return stage_manager;
}
Camera* Game::get_camera() {
	return camera;
}

Entity* Game::get_player() {
	return player;
}

sf::RenderWindow*& Game::get_window() {
	return window;
}
sf::RenderTexture& Game::get_display() {
	return *display;
}
int Game::get_zoom_edge_buffer() {
	return zoom_edge_buffer;
}
sf::Vector2f Game::get_zoom_display_size() {
	return zoom_display_size;
}
sf::Vector2f Game::get_display_size() {
	return sf::Vector2f(display_width, display_height);
}
sf::Vector2f Game::get_window_size() {
	return sf::Vector2f(window_width, window_height);
}
void Game::set_window_size(int x, int y) {
	window_width = x;
	window_height = y;

	window->setSize(sf::Vector2u(x, y));
}

void Game::play_sound(std::string filename, float volume) {

	current_sound_index++;
	current_sound_index %= max_sound_size; //making sure sound is less than max size

	if (!sound_buffers[current_sound_index].loadFromFile("Sounds/" + filename)) {
		std::cout << "sound: Sounds/" << filename << " could not open" << std::endl;
		return;
	}
	
	sounds[current_sound_index].setVolume(10 * volume);
	sounds[current_sound_index].setBuffer(sound_buffers[current_sound_index]);
	sounds[current_sound_index].play();
}

Game::~Game() {

	delete window;
	delete display;

	//deleting entity heap memory
	for (int i = 0; i < entites.size(); i++) {
		delete entites[i];
	}

	//deleting shader heap memory
	std::map<std::string, Shader*>::iterator it;
	for (it = shaders.begin(); it != shaders.end(); it++) {
		delete it->second;
	}
}

