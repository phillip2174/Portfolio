#pragma once

#include "Core/EC/Components/BehaviourScript.h"
#include "Core/EC/Components/Transform.hpp"
#include "Core/EC/Components/Rigidbody.hpp"
#include "Core/EC/GameObject.hpp"
#include "Utility/ObjectPool.h"
#include "Physic/PhysicScene.hpp"
#include "GameController.hpp"


#include "EnemyBehaviours.h"

#include "Graphic/Camera.hpp"
#include "Graphic/Window.hpp"

#include <cereal/types/base_class.hpp>
#include <cereal/types/polymorphic.hpp>


struct EnemyPreset;
struct EnemyAmplifier;

enum SPAWN_MODE {
	EDGE = 0,
	PLATFORM,
	RANGE
};

class EnemySpawner : public BehaviourScript 
{
protected:
	float SpawnRate = 2;
	//spawn area
	int x1, x2, y1, y2;
	//enemy type
	int SpawnType;
	int SpawningMode = 0;

	bool Spawning = false;

	float SpawnRateCount = 0;
	ObjectPool* EnemyPool;

	Physic::Colliders Platforms;
	Graphic::CameraObject* cam;

public:
	GameObject* EnemyTarget = nullptr;

	EnemyPreset* SpawnPreset = nullptr;
	EnemyAmplifier* SpawnAmplifier = nullptr;

	GameObject* SpawnEnemy();
	GameObject* SpawnEnemy(float posX, float posY);

	void SetSpawnRate(float value);
	void SetSpawnRange(float x1, float y1, float x2, float y2);
	void SetSpawnType(int type);
	void SetSpawning(bool spawn) { Spawning = spawn; }
	void SetSpawnMode(int mode) { SpawningMode = mode; }

	glm::vec2 GetRandomPos_Edge();
	glm::vec2 GetRandomPos_Platform();
	glm::vec2 GetRandomPos_Range(float rx1, float ry1, float rx2, float ry2);

	int GetType() { return this->SpawnType; }
	bool isSpawning() { return Spawning; }

	void updateSpawner();

	EnemySpawner() {}
	~EnemySpawner() {}

	virtual void OnUpdate(float dt) override;
	virtual void OnAwake() override;

	//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<BehaviourScript>(this),
			SpawnRate,
			SpawningMode,
			x1, x2, y1, y2,
			SpawnType
			);
	}
};

CEREAL_REGISTER_TYPE(EnemySpawner);
