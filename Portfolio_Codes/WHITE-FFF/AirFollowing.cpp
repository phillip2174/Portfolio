#include "AirFollowing.hpp"
#include "Core/EC/GameObject.hpp"
#include "Core/Logger.hpp"

#include "Utility/WhiteMath.h"

AirFollowing::AirFollowing()
{

}

void AirFollowing::OnAwake() {
	t = GetGameObject()->m_transform.get();
	rb = GetGameObject()->GetComponent<Rigidbody>();
	e = GetGameObject()->GetComponent<Character>();

	velocityOrientation = t->GetRotation();
}

void AirFollowing::SetPlayer(Transform* player) {
	m_target = player;
}

void AirFollowing::SetFlySpeed(float value) {
	m_speed = WhiteMath::Rand(value - 10, value + 10);
}

void AirFollowing::SetRotRate(float value) {
	rotRate = WhiteMath::Rand(value - 10, value + 10);
}

void AirFollowing::FollowPlayer(float dt) {
	glm::vec3 myVel = rb->GetVelocity();
	glm::vec3 direction = glm::normalize(m_target->GetPosition() - t->GetPosition());
	float rotDir = glm::sign(glm::cross(glm::normalize(myVel), direction).z);
	velocityOrientation += (rotDir * rotRate * dt);

	t->SetRotation(glm::degrees(glm::atan(direction.y, direction.x)));

	float vorad = glm::radians(velocityOrientation);
	rb->SetVelocity(glm::vec3(glm::cos(vorad), glm::sin(vorad), 0) * m_speed);
	
	//if (direction.x > 0 != e->facingRight) {
	//	e->flip();
	//}
	float sign = 1.0f;
	if (direction.x < 0.0f) {
		sign = -1.0f;
	}
	m_gameObject->m_transform->SetScale(glm::vec3(m_gameObject->m_transform->GetScale().x, glm::abs(m_gameObject->m_transform->GetScale().y) * sign, m_gameObject->m_transform->GetScale().z));
}
