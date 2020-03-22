#include "TextureManager.h"

namespace graphics {

	std::vector<TextureManager::activeTexture> TextureManager::_textures;


	void TextureManager::add(Texture* texture, const std::string& name)
	{
		_textures.push_back(activeTexture(texture,name));
	}

	Texture* TextureManager::get(const std::string& name)
	{
		for (activeTexture texture : _textures)
		{
			if (texture._name == name)
				return texture._texture;
		}
		return nullptr;
	}

	void TextureManager::clean()
	{
		
		int n = _textures.size();
		GLuint* array = new GLuint[n];

		int i = 0;
		for (activeTexture texture : _textures)
		{
			array[i] = texture._texture->getId();
			++i;
		}

		glDeleteTextures(_textures.size(), array);
		for (activeTexture texture : _textures)
			delete texture._texture;

		_textures.clear();
	}

}