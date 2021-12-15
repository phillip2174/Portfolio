#include "GroundPatrol.hpp"
#include "Graphic/GLRenderer.h"

GroundPatrol::GroundPatrol()
{
}


GroundPatrol::~GroundPatrol()
{
}

void GroundPatrol::OnAwake() {
	MaxDelay = delay_timer;
	thisTransform = m_gameObject->m_transform.get();
	rb = m_gameObject->GetComponent<Rigidbody>();
	enemySelf = m_gameObject->GetComponent<Enemy>();
	ps = Physic::PhysicScene::GetInstance();
}

void GroundPatrol::SetStopDis(float val) {
	this->m_stoppingDistance = val;
}

void GroundPatrol::SetSpeed(float val) {
	this->m_speed = val;
}

void GroundPatrol::SetMaxDelay(float val) {
	this->delay_timer = val;
	this->MaxDelay = delay_timer;
}

bool GroundPatrol::CheckGroundPath() {
	Physic::RayHit* Ground = nullptr;

	float posX = thisTransform->GetPosition().x;
	float posY = thisTransform->GetPosition().y;

	Ground = &(ps->Raycast(Physic::Ray(posX, posY, (posX + (glm::sign(thisTransform->GetTrueScale().x) * m_stoppingDistance)), (posY - (thisTransform->GetTrueScale().y / 2.0f))), ps->GetLayerFromString("Platform")));
	GLRenderer::GetInstance()->DrawDebug_Line(posX, posY, (posX + (glm::sign(thisTransform->GetTrueScale().x) * m_stoppingDistance)), (posY - (thisTransform->GetTrueScale().y / 2.0f)), 1.0f, 0.0f, 0.0f);

	return Ground->hit;
}

void GroundPatrol::Patrol(float dt) {
	float posX, posY;
	posX = thisTransform->GetPosition().x;
	posY = thisTransform->GetPosition().y;
	float sign = glm::sign(thisTransform->GetScale().x);

	rb->SetVelocity(glm::vec3(m_speed * glm::sign(thisTransform->GetTrueScale().x), rb->GetVelocity().y, 0));

	if (!CheckGroundPath()) {
		if (enemySelf != nullptr) {
			enemySelf->flip();
		}
	}

}