#pragma once
#include "Core/EC/Components/BehaviourScript.h"
#include "Core/EC/GameObject.hpp"
#include "Core/EC/Components/Rigidbody.hpp"
#include "Core/EC/Components/Collider.hpp"
#include "Physic/PhysicScene.hpp"

#include "Enemy.hpp"

#include <cereal/types/base_class.hpp>
#include <cereal/types/polymorphic.hpp>

class GroundPatrol : public BehaviourScript
{
private:
	float m_stoppingDistance = 25.0f;
	float m_speed = 50.0f;
	float MaxDelay = 2.0f;

	//runtime tmp
	float delay_timer;
	//-----------------------

	Transform* thisTransform;
	Rigidbody* rb;
	Enemy* enemySelf;
	Physic::PhysicScene* ps;
public:
	GroundPatrol();
	~GroundPatrol();
	void SetStopDis(float val);
	void SetSpeed(float val);
	void SetMaxDelay(float val);

	//return true if there is ground infront
	bool CheckGroundPath();
	void Patrol(float dt);

	virtual void OnAwake();

	//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<BehaviourScript>(this),
			m_stoppingDistance,
			m_speed,
			MaxDelay
			);
	}
};

CEREAL_REGISTER_TYPE(GroundPatrol);

