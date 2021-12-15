#pragma once
#include "Core/EC/Components/Transform.hpp"
#include "Core/EC/Components/BehaviourScript.h"
#include "Core/EC/GameObject.hpp"
#include "Core/EC/Components/Rigidbody.hpp"
#include "Core/EC/Components/SoundPlayer.hpp"
#include "Character.hpp"

#include <cereal/types/base_class.hpp>
#include <cereal/types/polymorphic.hpp>

class AirDash : public BehaviourScript
{
private:
	//runtime tmp var
	float timer;
	bool dashing;

	bool dashState = false;
	bool dashEnd = false;
	bool targetLocked = false;
	//---------------------------
protected:
	glm::vec3 m_target;
	Transform* self;
	Rigidbody* rb;
	SoundPlayer* sp;

	float m_dashSpeed = 700.f;
	float m_aimTime = 1.0f;
	float m_aimSpeed = 50.0f;

	//runtime tmp
	float m_angle = 0.0f;
public:
	float dashDamage = 1.0f;

	AirDash();

	~AirDash() {}
	void SetDashSpeed(float value);
	void SetAimTime(float value);
	void SetAimSpeed(float value);
	void Dash(float dt);
	void TargetLock(glm::vec3 pos);
	bool DashEnd();
	bool DashState() { return dashState; }
	void Reset();

	virtual void OnAwake();
	virtual void OnTriggerEnter(const Physic::Collision collision);

	//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<BehaviourScript>(this),
			m_dashSpeed,
			dashDamage,
			m_aimTime,
			m_aimSpeed
			);
	}
};

CEREAL_REGISTER_TYPE(AirDash);
