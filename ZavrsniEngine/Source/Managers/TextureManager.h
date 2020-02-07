#pragma once
#include <vector>
#include "../Graphics/Texture.h"

namespace graphics
{
	class TextureManager
	{
	public:
		struct activeTexture {
			Texture* _texture;
			std::string _name;

			activeTexture(Texture* texture, const std::string& name)
			{
				_texture = texture;
				_name = name;
			}

			activeTexture(const activeTexture& texture)
			{
				_texture = texture._texture;
				_name = texture._name;
			}
		};
	private:
		static std::vector<activeTexture> _textures;
	public:
		static void add(Texture* texture, const std::string& name);
		static void clean();
		static Texture* get(const std::string& name);
	};

}