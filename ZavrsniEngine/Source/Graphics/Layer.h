#pragma once

#include "Renderer.h"
#include "Sprite.h" 
#include "Shaders/Shader.h"

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
		virtual void add(Sprite* sprite);
		void render();
	};
	
}