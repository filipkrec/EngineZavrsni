#include "Layer.h"
namespace graphics {
	Layer::Layer(math::Matrix4 projectionMatrix, Shader* shader, Renderer* renderer)
	{
		if(shader == nullptr) //ako nije unesen shader defaultni shader 
		_shader =
#if !AT_JOB
			new Shader("Source/Graphics/Shaders/Basic.vert", "Source/Graphics/Shaders/Basic.frag");
#else
			new Shader("C:/Users/fkrec/source/repos/EngineZavrsni/ZavrsniEngine/Source/Graphics/Shaders/Basic.vert", "C:/Users/fkrec/source/repos/EngineZavrsni/ZavrsniEngine/Source/Graphics/Shaders/Basic.frag");
#endif
		if (renderer == nullptr) //ako nije unesen renderer defaultni renderer
			_renderer = new Renderer();
		
		if (projectionMatrix.isEmpty()) //ako nije unesena matrica jedinicna matrica
			_projectionMatrix = math::Matrix4(1.0);
		else
			_projectionMatrix = projectionMatrix;

		_shader->enable();
		_shader->setUniformMat4("projection_matrix", _projectionMatrix);
		_shader->disable();
	}
	Layer::~Layer() {
		delete _shader;
		delete _renderer;
		for (int i = 0; i < _sprites.size(); ++i)
		{
			delete _sprites[i];
		}
	}

	void Layer::add(Sprite* renderable) {
		_sprites.push_back(renderable);
	}

	void Layer::render() {
		_shader->enable();
		_renderer->begin();
		for (const Sprite* sprite : _sprites) {
			_renderer->submit(sprite);
		}
		_renderer->end();
		_renderer->flush();
	}
}