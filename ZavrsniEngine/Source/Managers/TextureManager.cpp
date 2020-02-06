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
		for (activeTexture texture : _textures)
			delete texture._texture;

		_textures.clear();
	}

}