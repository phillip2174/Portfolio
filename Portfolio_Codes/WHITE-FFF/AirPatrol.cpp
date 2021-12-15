#include "AirPatrol.hpp"
#include "Core/EC/GameObject.hpp"
#include "Core/Logger.hpp"

AirPatrol::AirPatrol()
{

}

void AirPatrol::OnAwake() {
	queen = GetGameObject()->m_transform.get();
	rb = GetGameObject()->GetComponent<Rigidbody>();
}

void AirPatrol::Patrol() {

	rb->SetVelocity(glm::vec3(-m_speed, 0, 0));

	if (queen->GetPosition().x <= m_pointAX) {
		rb->SetVelocity(glm::vec3(m_speed, 0, 0));
	}
	else if (queen->GetPosition().x >= m_pointBX) {
		rb->SetVelocity(glm::vec3(-m_speed, 0, 0));
	}

	if (rb->GetVelocity().x < 0) {
		if (m_gameObject->GetComponent<Character>()->facingRight) 
		{
			m_gameObject->GetComponent<Character>()->flip();
		}
	}
	else {
		if (!m_gameObject->GetComponent<Character>()->facingRight)
		{
			m_gameObject->GetComponent<Character>()->flip();
		}
	}
}

void AirPatrol::SetPoint(float aX, float bX) {
	m_pointAX = aX;
	m_pointBX = bX;
}

void AirPatrol::SetSpeed(float speed) {
	this->m_speed = speed;
}
