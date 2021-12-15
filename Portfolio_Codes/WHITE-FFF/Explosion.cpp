#include "Explosion.hpp"

#include "Graphic/GLRenderer.h"

Explosion::Explosion()
{

}

void Explosion::OnAwake() {
	thisTransform = m_gameObject->m_transform.get();
	ps = Physic::PhysicScene::GetInstance();
}

void Explosion::OnUpdate(float dt) {
	GLRenderer::GetInstance()->DrawDebug_Circle(m_gameObject->m_transform->GetPosition().x, m_gameObject->m_transform->GetPosition().y, m_radius, 1.0f, 0.0f, 0.0f);
}

void Explosion::Explode() {
	//get all collider in target layers
	Physic::Colliders targetColliders;
	for (std::string target : TargetLayers) {
		Physic::Colliders layerColliders = ps->GetColliderLayer(target);
		targetColliders.insert(targetColliders.end(), layerColliders.begin(), layerColliders.end());
	}

	for (Collider* c : targetColliders) {
		if (c->GetGameObject()->Active()) {
			float distance = glm::length(c->GetGameObject()->m_transform->GetPosition() - thisTransform->GetPosition());
			if (distance <= m_radius) {
				HPsystem* hp = c->GetGameObject()->GetComponent<HPsystem>();
				if (hp != nullptr) {
					hp->TakeDamage(m_damage);
				}
			}
		}
	}
	
}

void Explosion::SetDamage(float val) {
	this->m_damage = val;
}

void Explosion::SetRadius(float val) {
	this->m_radius = val;
}

Explosion::~Explosion()
{
}