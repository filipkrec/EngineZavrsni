#pragma once

#include "Renderer.h"
#include "Sprite.h" 
#include "Shaders/Shader.h"
#include "Group.h"

namespace graphics {
	class Layer
	{
	protected:
		std::vector<Sprite*> _sprites;
		std::vector<Label*> _labels;
		std::vector<Sprite*> _labelSprites;
		std::vector<Sprite*> _renderingSprites;
		Renderer* _renderer;
		Shader _shader;
	public:
		Layer(
			Shader shader = Shader("../ZavrsniEngine/Source/Graphics/Shaders/Basic.vert", "../ZavrsniEngine/Source/Graphics/Shaders/Basic.frag"),
			Renderer* renderer = new Renderer()
			);
		virtual ~Layer();
		void add(Sprite* sprite);
		void add(Label* label);
		void labelToSprite(Label* label);
		void add(const Group& group);
		void clear();
		void render();
	};
	
}