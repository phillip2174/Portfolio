#pragma once
#include "Core/EC/Components/BehaviourScript.h"
#include "Core/EC/GameObject.hpp"
#include "Core/EC/Components/Rigidbody.hpp"
#include "Core/EC/Components/Collider.hpp"
#include "Physic/PhysicScene.hpp"
#include "HPsystem.hpp"
#include <string>
#include <set>

#include <cereal/types/string.hpp>
#include <cereal/types/set.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/polymorphic.hpp>

class Explosion : public BehaviourScript
{
private:
	Transform* thisTransform = nullptr;
	Physic::PhysicScene* ps;

	Physic::Layer targetlayer;

public:
	float m_damage = 10.0f;
	float m_radius = 200.0f;
	std::set<std::string> TargetLayers;

	Explosion();
	~Explosion();

	void Explode();
	void SetLayer(std::string layer);
	void SetDamage(float val);
	void SetRadius(float val);

	virtual void OnAwake() override;
	virtual void OnUpdate(float) override;

//serialization
public:
	template <class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<BehaviourScript>(this),
			m_damage,
			m_radius,
			TargetLayers
			);
	}
};

CEREAL_REGISTER_TYPE(Explosion);
