#pragma once
#include "Component.h"

struct Frame {
	int index; //the frame index
	int duration;
};

class AnimationClip { //takes in spritesheet

	public:
		bool loop;
		std::vector<Frame> frames;
		std::string file_name;
		sf::Vector2f frame_dimensions;

		AnimationClip();
		AnimationClip(std::string filename, sf::Vector2f frame_dimensions, int start, int end, bool loop=true);
		void set_duration(int);
		void set_duration(std::vector<int> durations);
};