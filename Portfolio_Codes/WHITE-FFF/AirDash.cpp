#include "AirDash.hpp"
#include "Core/EC/GameObject.hpp"
#include "Core/Logger.hpp"
#include "PlayerController.hpp"

AirDash::AirDash()
{

}

void AirDash::OnAwake() {
	this->m_target = glm::vec3(0);
	self = m_gameObject->m_transform.get();
	rb = m_gameObject->GetComponent<Rigidbody>();
	sp = m_gameObject->GetComponent<SoundPlayer>();
	//sp->SetSoundType(SOUND_SFX);

	Reset();
}

void AirDash::SetDashSpeed(float value) {
	this->m_dashSpeed = value;
}

void AirDash::SetAimTime(float value) {
	this->m_aimTime = value;
}

void AirDash::SetAimSpeed(float value) {
	this->m_aimSpeed = value;
}

void AirDash::Dash(float dt) {
	
	sp->SetSound(SoundPath("SFX_Bomber_Dash"));
	glm::vec3 dir = m_target - self->GetPosition();
	float distance = glm::length(dir);
	m_angle = glm::atan(dir.y, dir.x);
	dashEnd = false;

	if (!dashState) {
		timer -= dt;
		if (timer > 0) {
			rb->SetVelocity(glm::vec3(0, 0, 0));
			self->SetRotation(glm::degrees(m_angle));
		}
		else {
			sp->PlaySound();
			dashState = true;
			
		}
	}else if (dashState) {
		if (distance <= m_dashSpeed * dt * 2) {
			Reset();
		}
		else 
		{
			rb->SetVelocity(glm::vec3(glm::cos(m_angle), glm::sin(m_angle), 0) * m_dashSpeed);
		}
	}

}

bool AirDash::DashEnd() {
	return dashEnd;
}

void AirDash::TargetLock(glm::vec3 pos) {
	if (!targetLocked) {
		this->m_target = pos;
		targetLocked = true;
		sp->SetSound(SoundPath("SFX_Bomber_Alert"));
		sp->PlaySound();
	}
}

void AirDash::Reset() {
	timer = m_aimTime;
	dashState = false;
	dashEnd = true;
	targetLocked = false;
}

void AirDash::OnTriggerEnter(const Physic::Collision collision) {
	GameObject* obj = collision.m_otherCollider->GetGameObject();
	PlayerController* player = obj->GetComponent<PlayerController>();

	///damage player
	if (player != nullptr) {
		obj->GetComponent<HPsystem>()->TakeDamage(dashDamage);
	}
}