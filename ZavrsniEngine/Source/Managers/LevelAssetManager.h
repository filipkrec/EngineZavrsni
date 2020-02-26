#pragma once
#include <vector>
#include "../Graphics/Label.h"
#include "../Graphics/Layer.h"
#include "../Physics/Objects/Player.h"
#include "../Physics/Objects/NPC.h"
#include "../Graphics/Line.h"

namespace lam {
	class LevelAssetManager
	{
	public:
		struct activeObject {
			void* _object;
			std::string _name;

			activeObject(void* object, const std::string& name)
			{
				_object = object;
				_name = name;
			}

			activeObject(const activeObject& object)
			{
				_object = object._object;
				_name = object._name;
			}
		};
	private:
		static graphics::Layer* _layer;
		static engine::Timer* _timer;

		static std::vector<activeObject> _sprites;
		static std::vector<activeObject> _gameObjects;
		static std::vector<activeObject> _NPCs;
		static std::vector<activeObject> _labels;
		static std::vector<activeObject> _pickups;
		static std::vector<graphics::Line*> _shots;
		static objects::Player* _player;

		static math::Vector2 _calculatePath(const math::Vector2& nextStep, const math::Vector2& goal, unsigned int step);
	public:
		static void init(objects::Player* player, graphics::Layer* layer);
		static void processBegin(engine::Window& window);
		static void processEnd(engine::Window& window);

		static void add(graphics::Sprite* sprite, const std::string& name);
		static void cleanSprites();
		static graphics::Sprite* getSprite(const std::string& name);


		static void add(graphics::Label* label, const std::string& name);
		static void cleanLabels();
		static graphics::Label* getLabel(const std::string& name);

		static void add(objects::GameObject* gameeObject, const std::string& name);
		static void cleanGameObjects();
		static objects::GameObject* getGameObject(const std::string& name);

		static void add(objects::NPC* NPC, const std::string& name);
		static void cleanNPCs();
		static objects::NPC* getNPC(const std::string& name);

		static void add(objects::Pickup* pickup, const std::string& name);
		static void cleanPickups();
		static objects::Pickup* getPickup(const std::string& name);

		static void addToLayer(graphics::Layer* layer);
		static void clean();

		static objects::Player* getPlayer();
	};
}