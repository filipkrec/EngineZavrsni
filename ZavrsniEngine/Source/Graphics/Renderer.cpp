#include "Renderer.h"

namespace graphics {
	Renderer::Renderer()
		:_count(0)
	{
		init();
	}

	Renderer::~Renderer()
	{
		glDeleteBuffers(1, &_IBO); //brisanje buffer objecta sa zadanim indexom
		glDeleteBuffers(1, &_VBO); //brisanje buffer objecta sa zadanim indexom
	}

	void Renderer::init()
	{
		glGenVertexArrays(1,&_VAO); //globalni VertexArrayObject bindan za potrebe novijih verzija OpenGL-a
		glBindVertexArray(_VAO); 

		glGenBuffers(1, &_VBO); //generiranje vertex buffera, stavlanje ID-a u VBO
		glBindBuffer(GL_ARRAY_BUFFER, _VBO);
		glGenBuffers(1, &_IBO); //generiranje index buffera, stavljnaje ID-a u IBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_IBO);

		glBufferData(GL_ARRAY_BUFFER, MAX_VERTEX_BUFFER_SIZE, NULL, GL_DYNAMIC_DRAW); //Odredivanje podataka u bufferu A velicine B, nullptr za data jer je mapirana kasnije, dynamic draw za brzi drawcall
		glVertexAttribPointer(SHADER_VERTEX_INDEX, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const GLvoid*)0); //1. atribut na indeksu 0, 3 floata, ne treba normalizacija, svaki vertex velicine vertexdata, lokacija na 0. mjestu svake vertexdate
		glVertexAttribPointer(SHADER_COLOR_INDEX, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(VertexData), (const GLvoid*)offsetof(VertexData, VertexData::Color)); //1. atribut na indeksu 1, 4 unsigned bytea (4 bytea = 1 int), treba normalizacija, svaki vertex velicine vertexdata, lokacija na mjestu offseta boje svake vertexdate
		glEnableVertexAttribArray(SHADER_VERTEX_INDEX);//enable
		glEnableVertexAttribArray(SHADER_COLOR_INDEX);//enable


		glBufferData(GL_ELEMENT_ARRAY_BUFFER, MAX_INDEX_BUFFER_SIZE, NULL, GL_DYNAMIC_DRAW); //Odredivanje podataka u bufferu A velicine B, nullptr za data jer je mapirana kasnije, dynamic draw za brzi drawcall
		glBindVertexArray(0);
	}

	void Renderer::begin()
	{
		glBindVertexArray(_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, _VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_IBO);
		_vertexBufferData = (VertexData*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY); //mapiranje vertexbufferdata kao tocke unosa vertex podataka;
		_indices = (unsigned int*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY); //mapiranje indices kao tocke unosa index buffer podataka;
	}

	void Renderer::submit(const Sprite* sprite)
	{
		const math::Vector3& position = sprite->getPosition();
		const math::Vector2& size = sprite->getSize();
		const unsigned int color = sprite->getColor();
		VertexData temp;

		temp.Position = position;
		temp.Color = color;
		const unsigned int indexA = setIndex(temp); //postavljanje vrha tocke A kvadrata (ljevo dolje)

		temp.Position = math::Vector3(position.x, position.y + size.y, position.z);
		temp.Color = color;
		const unsigned int indexB = setIndex(temp); //postavljanje vrha tocke B kvadrata (ljevo gore)

		temp.Position = math::Vector3(position.x + size.x, position.y + size.y, position.z);
		temp.Color = color;
		const unsigned int indexC = setIndex(temp); //postavljanje vrha tocke C kvadrata (desno gore)

		temp.Position = math::Vector3(position.x + size.x, position.y, position.z);
		temp.Color = color;
		const unsigned int indexD = setIndex(temp); //postavljanje vrha tocke D kvadrata (desno dolje)

		//Rasporedivanje odgovarajucih indexa u 2 polovice(trokuta) za crtanje
		*_indices = indexA;
		_indices++;
		*_indices = indexB;
		_indices++;
		*_indices = indexC;
		_indices++;
		*_indices = indexC;
		_indices++;
		*_indices = indexD;
		_indices++;
		*_indices = indexA;
		_indices++;

		_indexCount += 6;
	}

	void Renderer::end()
	{
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
		//ciscenje da se moze crtat
	}

	void Renderer::flush()
	{
		glBindVertexArray(_VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IBO);

		glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, (void*)0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		_indexCount = 0;
		_count = 0;
		//crtanje
	}

	//PRIVATES

	const unsigned int& Renderer::setIndex(VertexData& current)
	{
		if (_count == 0)
		{
			_vertexBufferDataBegin = _vertexBufferData;
			*_vertexBufferData = current;
			_vertexBufferData++;
			return _count++;
		}
		else
		{
			VertexData* _vertexBufferTemp = _vertexBufferDataBegin;
			for (unsigned int i = 0; i < _count; ++i)
			{
				if (*_vertexBufferTemp == current)
				{
					return i;
				}
				_vertexBufferTemp++;
			}
			*_vertexBufferData = current;
			_vertexBufferData++;
			return _count++;
		}
	}
}