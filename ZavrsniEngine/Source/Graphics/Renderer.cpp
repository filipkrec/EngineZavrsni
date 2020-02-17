#include "Renderer.h"

namespace graphics {
	Renderer::Renderer()
		:_count(0), _indexCount(0), _camera(Camera::getInstance())
	{
		init();
	}

	Renderer::~Renderer()
	{
		delete _indices;
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
		glVertexAttribPointer(SHADER_VERTEX_INDEX, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const GLvoid*)0); //1. atribut na indeksu 0, 3 floata, ne treba normalizacija, svaki vertex velicine vertexdata, lokacija na 0. mjestu svake vertexdate
		glVertexAttribPointer(SHADER_TEXTURE_SLOT_INDEX, 1, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const GLvoid*)(offsetof(VertexData, VertexData::TextureSlot)));
		glVertexAttribPointer(SHADER_TEXTURE_CORDS_INDEX, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const GLvoid*)(offsetof(VertexData, VertexData::TextureCoordinate)));
		glVertexAttribPointer(SHADER_COLOR_INDEX, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(VertexData), (const GLvoid*)offsetof(VertexData, VertexData::Color)); //1. atribut na indeksu 1, 4 unsigned bytea (4 bytea = 1 int), treba normalizacija, svaki vertex velicine vertexdata, lokacija na mjestu offseta boje svake vertexdate
		glEnableVertexAttribArray(SHADER_VERTEX_INDEX);//enable
		glEnableVertexAttribArray(SHADER_COLOR_INDEX);//enable
		glEnableVertexAttribArray(SHADER_TEXTURE_SLOT_INDEX);//enable
		glEnableVertexAttribArray(SHADER_TEXTURE_CORDS_INDEX);//enable


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

	void Renderer::submit(const math::Vector2 lineBegin, const math::Vector2 lineEnd, unsigned int color)
	{
		VertexData temp;

		temp.Color = color;
		temp.TextureSlot = -1;
		math::Vector2 offset(0.01f, 0.01f);

		temp.Position = _camera->getViewMatrix() * lineBegin;
		temp.TextureCoordinate = math::Vector2(0.0f,0.0f);
		const unsigned int indexA = setIndex(temp); //postavljanje vrha tocke A kvadrata (ljevo dolje)

		temp.Position = _camera->getViewMatrix() * (lineBegin + offset);
		temp.TextureCoordinate = math::Vector2(0.0f, 1.0f);
		const unsigned int indexB = setIndex(temp); //postavljanje vrha tocke B kvadrata (ljevo gore)

		temp.Position = _camera->getViewMatrix() * lineEnd;
		temp.TextureCoordinate = math::Vector2(1.0f, 1.0f);
		const unsigned int indexC = setIndex(temp); //postavljanje vrha tocke C kvadrata (desno gore)

		temp.Position = _camera->getViewMatrix() * (lineEnd + offset);
		temp.TextureCoordinate = math::Vector2(1.0f, 0.0f);
		const unsigned int indexD = setIndex(temp); //postavljanje vrha tocke D kvadrata (desno dolje)

		//Rasporedivanje odgovarajucih indexa u 2 polovice(trokuta) za crtanje
		*_indices = indexA;
		_indices++;
		*_indices = indexB;
		_indices++;
		*_indices = indexC;
		_indices++;
		*_indices = indexA;
		_indices++;
		*_indices = indexC;
		_indices++;
		*_indices = indexD;
		_indices++;

		_indexCount += 6;
	}

	void Renderer::submit(const Sprite* sprite)
	{
		math::Vector4 lineElements = sprite->getLineElements();
		if (lineElements != math::Vector4(0,0,0,0))
		{
			submit(math::Vector2(lineElements.x,lineElements.y), math::Vector2(lineElements.z, lineElements.w), sprite->getColor());
			return;
		}

		const math::Vector2& position = sprite->getPosition();
		const math::Vector2& offset = sprite->getOffset();
		const math::Vector2& size = sprite->getSize();
		const unsigned int color = sprite->getColor();
		const math::Vector2* textureCoordinates = sprite->getTextureCoordinates();
		const Texture* texture = sprite->getTexture();
		float textureSlot;
		const math::Matrix4& modelMatrix = sprite->getModelMatrix();

		if (texture != nullptr)
		{
			textureSlot = getTextureSlot(texture->getId()); //ukoliko je tekstura vec ucitana, uzima se postojeci texture slot, inace uzima sljedeci
		}
		else
		{

			textureSlot = -1;
		}


		VertexData temp;

		temp.Color = color;
		temp.TextureSlot = textureSlot;

		temp.Position = _camera->getViewMatrix() * modelMatrix * (math::Vector2(-size.x / 2, -size.y / 2) + offset);
		temp.TextureCoordinate = textureCoordinates[0];
		const unsigned int indexA = setIndex(temp); //postavljanje vrha tocke A kvadrata (ljevo dolje)

		temp.Position = _camera->getViewMatrix() * modelMatrix * (math::Vector2(-size.x / 2, size.y / 2) + offset);
		temp.TextureCoordinate = textureCoordinates[1];
		const unsigned int indexB = setIndex(temp); //postavljanje vrha tocke B kvadrata (ljevo gore)

		temp.Position = _camera->getViewMatrix() * modelMatrix * (math::Vector2(size.x / 2, size.y / 2) + offset);
		temp.TextureCoordinate = textureCoordinates[2];
		const unsigned int indexC = setIndex(temp); //postavljanje vrha tocke C kvadrata (desno gore)

		temp.Position = _camera->getViewMatrix() * modelMatrix * (math::Vector2(size.x / 2, -size.y / 2) + offset);
		temp.TextureCoordinate = textureCoordinates[3];
		const unsigned int indexD = setIndex(temp); //postavljanje vrha tocke D kvadrata (desno dolje)

		//Rasporedivanje odgovarajucih indexa u 2 polovice(trokuta) za crtanje
		*_indices = indexA;
		_indices++;
		*_indices = indexB;
		_indices++;
		*_indices = indexC;
		_indices++;
		*_indices = indexA;
		_indices++;
		*_indices = indexC;
		_indices++;
		*_indices = indexD;
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
		for (int i = 0; i < _textures.size(); ++i)
		{
			glActiveTexture(GL_TEXTURE0 + i); //aktiviranje teksture slotova (0-i (max32))
			glBindTexture(GL_TEXTURE_2D, _textures[i]); //bindanje svih aktivnih tekstura 
		}

		glBindVertexArray(_VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IBO);

		glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, (void*)0);  //crtanje

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		_indexCount = 0;
		_count = 0;
		//crtanje
	}

	//PRIVATES

	const unsigned int& Renderer::setIndex(VertexData& current)
	{
			*_vertexBufferData = current;
			_vertexBufferData++;
			return _count++;
	}


	unsigned int Renderer::getTextureSlot(unsigned int textureId)
	{
		if (textureId == 0)
			return 0;

		for (int i = 0; i < _textures.size(); ++i)
		{
			if (_textures[i] == textureId)
			{
				return i;
			}
		}

		if (_textures.size() >= 32) //ako su texture slotovi puni, prazni ih i poziva se dodatan draw call
		{
			end();
			flush();
			begin();
		}
		_textures.push_back(textureId);
		return _textures.size() - 1;
	}
}