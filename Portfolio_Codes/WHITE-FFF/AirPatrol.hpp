#pragma once
#include "Core/EC/Components/Transform.hpp"
#include "Core/EC/Components/BehaviourScript.h"
#include "Core/EC/GameObject.hpp"
#include "Core/EC/Components/Rigidbody.hpp"
#include "Character.hpp"

#include <cereal/types/polymorphic.hpp>

class AirPatrol : public BehaviourScript
{
private:
	float m_pointAX = -1000.0f;
	float m_pointBX = 1000.0f;
	float m_speed = 30.0f;
public:
	Transform* queen;
	Rigidbody* rb;
	AirPatrol();
	~AirPatrol() {}
	void Patrol();
	void SetPoint(float aX, float bX);
	void SetSpeed(float speed);

	virtual void OnAwake();

	//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<BehaviourScript>(this),
			m_pointAX,
			m_pointBX,
			m_speed
		);
	}
};

CEREAL_REGISTER_TYPE(AirPatrol);
