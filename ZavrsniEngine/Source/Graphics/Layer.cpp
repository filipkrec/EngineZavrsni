#include "Layer.h"
#include <algorithm>
namespace graphics {
	Layer::Layer(math::Matrix4 projectionMatrix, Shader shader, Renderer* renderer):
		_shader(shader),_renderer(renderer),_projectionMatrix(projectionMatrix)
	{
		_shader.enable();
		int textureIds[] =
		{
			 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
			10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
			20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
			30, 31
		}; 
		_shader.setUniform1iv("textures",32, textureIds); //textureId's popunjeni za sampler sa maximalnim brojam sampleova (32 za openGL)
		_shader.setUniformMat4("projection_matrix", _projectionMatrix);
	}
	Layer::~Layer() {
		for (int i = 0; i < _sprites.size(); ++i)
		{
			delete _sprites[i];
		}
	}

	void Layer::add(Sprite* renderable) {
		_sprites.push_back(renderable);
	}

	void Layer::add(const Group& group)
	{
		for (Sprite* sprite : group._sprites)
		{
			_sprites.push_back(sprite);
		}
	}

	bool sortSprite(Sprite* first, Sprite* second)
	{
		return (first->getZindex() < second->getZindex());
	}

	void Layer::render() {
		_shader.enable();
		_renderer->begin();
		std::sort(_sprites.begin(), _sprites.end(), sortSprite);
		for (const Sprite* sprite : _sprites) {
			_renderer->submit(sprite);
		}
		_renderer->end();
		_renderer->flush();
	}


	
}