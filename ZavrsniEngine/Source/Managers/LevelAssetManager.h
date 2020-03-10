#pragma once
#include <vector>
#include "../Graphics/Label.h"
#include "../Graphics/Layer.h"
#include "../Physics/Objects/Player.h"
#include "../Physics/Objects/NPC.h"
#include "../Graphics/Line.h"
#include <algorithm>

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
		static std::vector<activeObject> _allObjects;
		static std::vector<activeObject> _allActors;
		static objects::Player* _player;

		static math::Vector2 calculatePath(const math::Vector2& goal, const objects::NPC* npc);
		static bool pathBlocked(const math::Vector2& positionTo, const math::Vector2& current, const objects::NPC* npc);

		static void fillObjects(); //fill allObjects allActors
		static void processPathfinding(); //process patroling, calculate next move for NPCs FORCE CALCULATION
		static void processSight(); //add to allActors sighted
		static void processPlayer(engine::Window& window); //add to player.pickupable, player.process() FORCE CALCULATION
		static void processNPCs(engine::Window& window); //NPC.process
		static void processCollision(); //process collision between 2 objects FORCE CALCULATION
		static void processHitDetection(); //process shots and consequences FORCE CALCULATION
		static void processMovement(); //move in final FORCE direction
	public:
		static void init(graphics::Layer* layer);

		static void setPlayer(objects::Player* player); //do not use midlevel

		//Base game loop
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