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
		math::Matrix4 _projectionMatrix;
		Renderer* _renderer;
		Shader _shader;
	public:
		Layer(
			math::Matrix4 projectionMatrix = math::Matrix4(1.0),
			Shader shader = Shader("Source/Graphics/Shaders/Basic.vert", "Source/Graphics/Shaders/Basic.frag"),
			Renderer* renderer = new Renderer()
			);
		virtual ~Layer();
		void add(Sprite* sprite);
		void add(Label* label);
		void add(const Group& group);
		void render();
	};
	
}