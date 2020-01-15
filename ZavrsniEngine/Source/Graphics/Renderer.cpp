#include "Renderer.h"

namespace graphics {
	Renderer::Renderer()
	{
		init();
	}

	GLenum err;

	Renderer::~Renderer()
	{
		glDeleteBuffers(1, &_VBO); //brisanje buffer objecta sa zadanim indexom
	}

	void Renderer::init()
	{
		glGenBuffers(1, &_VBO); //generiranje vertex buffera, stavlanje ID-a u VBO
		glBindBuffer(GL_ARRAY_BUFFER, _VBO); 


		glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(VertexData), NULL, GL_DYNAMIC_DRAW); //Odredivanje podataka u bufferu A velicine B, nullptr za data jer je mapirana kasnije, dynamic draw za brzi drawcall
		glVertexAttribPointer(SHADER_VERTEX_INDEX, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const GLvoid*)0); //1. atribut na indeksu 0, 3 floata, ne treba normalizacija, svaki vertex velicine vertexdata, lokacija na 0. mjestu svake vertexdate
		glEnableVertexAttribArray(SHADER_VERTEX_INDEX);//enable
		glBindBuffer(GL_ARRAY_BUFFER, 0); 
	}

	void Renderer::begin()
	{
		glBindBuffer(GL_ARRAY_BUFFER, _VBO); 
		_vertexBufferData = (VertexData*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY); //mapiranje vertexbufferdata kao tocke unosa vertex podataka;
	}

	void Renderer::submit(const Sprite* sprite)
	{
		const math::Vector3& position = sprite->getPosition();
		const math::Vector2& size = sprite->getSize();
		const unsigned int color = sprite->getColor();

		//crtanje trokuta
		_vertexBufferData->Position = position;
		_vertexBufferData++;

		_vertexBufferData->Position = math::Vector3(position.x, position.y + size.y, position.z);
		_vertexBufferData++;

		_vertexBufferData->Position = math::Vector3(position.x + size.x, position.y + size.y, position.z);
		_vertexBufferData++;

		_count += 3;
	}

	void Renderer::end()
	{
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		//ciscenje da se moze crtat
	}

	void Renderer::flush()
	{
		//crtanje
		glDrawArrays(GL_TRIANGLES, 0, _count);
		_count = 0;
	}
}